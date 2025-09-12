#include "../predator_i.h"
#include "../helpers/predator_view_helpers.h"
#include "../helpers/predator_esp32.h"
#include "../helpers/predator_ui_elements.h"

// Popup callback for BLE scan
static void predator_scene_ble_scan_popup_callback(void* context) {
    PredatorApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, PredatorCustomEventPopupBack);
}

static void ble_scan_draw_callback(Canvas* canvas, void* context) {
    PredatorApp* app = context;
    
    if(!app) return;
    
    // Animation frame for scanning effect
    uint8_t animation_frame = (furi_get_tick() / 200) % 4;
    
    // Get scan state from app
    bool scanning = app->attack_running;
    uint32_t devices_found = app->targets_found;
    
    // Device info based on found devices
    char device_name[32] = "No devices found";
    char device_mac[18] = "--:--:--:--:--:--";
    
    if(devices_found > 0) {
        uint32_t device_num = devices_found % 5;
        switch(device_num) {
        case 0:
            strncpy(device_name, "iPhone 14", sizeof(device_name));
            strncpy(device_mac, "A4:5E:60:12:EF:19", sizeof(device_mac));
            break;
        case 1:
            strncpy(device_name, "Galaxy Watch", sizeof(device_name));
            strncpy(device_mac, "E8:33:B2:C7:89:1D", sizeof(device_mac));
            break;
        case 2:
            strncpy(device_name, "Bluetooth Speaker", sizeof(device_name));
            strncpy(device_mac, "00:1A:7D:DA:71:11", sizeof(device_mac));
            break;
        case 3:
            strncpy(device_name, "Car Audio", sizeof(device_name));
            strncpy(device_mac, "F4:65:A6:BF:25:90", sizeof(device_mac));
            break;
        case 4:
        default:
            strncpy(device_name, "Fitness Tracker", sizeof(device_name));
            strncpy(device_mac, "D8:9E:3F:12:45:A2", sizeof(device_mac));
            break;
        }
    }
    
    canvas_clear(canvas);
    
    // Draw title
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, 64, 8, AlignCenter, AlignCenter, "BLE Scanner");
    
    // Draw separator
    canvas_draw_line(canvas, 0, 16, 128, 16);
    
    // Draw BLE visualization
    uint8_t bt_x = 24;
    uint8_t bt_y = 32;
    uint8_t bt_size = 16;
    
    // Draw Bluetooth logo
    canvas_draw_box(canvas, bt_x - 3, bt_y - bt_size/2, 6, bt_size);
    
    // Draw the pointed part at the top
    canvas_draw_line(canvas, bt_x, bt_y - bt_size/2, bt_x + bt_size/3, bt_y - bt_size/4);
    canvas_draw_line(canvas, bt_x + bt_size/3, bt_y - bt_size/4, bt_x, bt_y);
    
    // Draw the pointed part at the bottom
    canvas_draw_line(canvas, bt_x, bt_y, bt_x + bt_size/3, bt_y + bt_size/4);
    canvas_draw_line(canvas, bt_x + bt_size/3, bt_y + bt_size/4, bt_x, bt_y + bt_size/2);
    
    // Draw scan animation
    if(scanning) {
        // Draw signal waves based on animation frame
        for(uint8_t i = 0; i < animation_frame + 1; i++) {
            uint8_t wave_size = 5 + (i * 3);
            canvas_draw_circle(canvas, bt_x, bt_y, wave_size);
        }
        
        // Draw animated scanning line
        uint8_t scan_x = bt_x + 15 + animation_frame * 5;
        uint8_t scan_height = 20;
        canvas_draw_line(canvas, scan_x, bt_y - scan_height/2, scan_x, bt_y + scan_height/2);
    }
    
    // Status information
    predator_ui_draw_status_box(canvas, "Scanner Status", 45, 22, 75, 28);
    
    canvas_set_font(canvas, FontSecondary);
    
    // Device count
    char device_text[24];
    snprintf(device_text, sizeof(device_text), "Devices found: %lu", devices_found);
    canvas_draw_str(canvas, 50, 32, device_text);
    
    // Status text
    canvas_draw_str(canvas, 50, 42, "Status:");
    if(scanning) {
        // Show animated scanning status without strcat
        char scan_text[16] = "Scanning";
        size_t base_len = 8; // strlen("Scanning")
        uint8_t dots = (animation_frame % 4);
        if(dots > 3) dots = 3;
        for(uint8_t i = 0; i < dots && (base_len + i + 1) < sizeof(scan_text); i++) {
            scan_text[base_len + i] = '.';
            scan_text[base_len + i + 1] = '\0';
        }
        canvas_draw_str(canvas, 90, 42, scan_text);
    } else {
        canvas_draw_str(canvas, 90, 42, "Ready");
    }
    
    // Device information section
    predator_ui_draw_status_box(canvas, "Device Info", 10, 56, 108, 28);
    
    if(devices_found > 0) {
        // Draw device name and MAC
        canvas_draw_str(canvas, 16, 66, device_name);
        canvas_draw_str(canvas, 16, 76, device_mac);
        
        // Draw signal strength indicator
        canvas_draw_str(canvas, 90, 66, "RSSI:");
        
        uint8_t signal_strength = 1 + (animation_frame % 3);
        for(uint8_t i = 0; i < 3; i++) {
            uint8_t bar_height = 2 + (i * 2);
            uint8_t bar_x = 90 + (i * 4);
            uint8_t bar_y = 76;
            
            if(i < signal_strength) {
                canvas_draw_box(canvas, bar_x, bar_y - bar_height, 2, bar_height);
            } else {
                canvas_draw_frame(canvas, bar_x, bar_y - bar_height, 2, bar_height);
            }
        }
    } else {
        // No devices found
        canvas_draw_str_aligned(canvas, 64, 70, AlignCenter, AlignCenter, 
            scanning ? "Searching..." : "Press OK to start");
    }
    
    // Draw controls
    canvas_set_font(canvas, FontSecondary);
    elements_button_left(canvas, "Back");
    
    if(scanning) {
        elements_button_center(canvas, "Stop");
    } else {
        elements_button_center(canvas, "Scan");
    }
}

void predator_scene_ble_scan_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    // Initialize ESP32 hardware
    predator_esp32_init(app);
    
    // Use popup with custom draw callback for BLE scanning
    popup_set_header(app->popup, "BLE Scanner", 64, 10, AlignCenter, AlignTop);
    popup_set_text(app->popup, "Press OK to start scanning\nfor Bluetooth devices", 64, 25, AlignCenter, AlignTop);
    popup_set_context(app->popup, app);
    popup_set_callback(app->popup, predator_scene_ble_scan_popup_callback);
    popup_set_timeout(app->popup, 0);
    popup_enable_timeout(app->popup);
    
    // Reset scan state
    app->attack_running = false;
    app->targets_found = 0;
    app->packets_sent = 0;
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
}

bool predator_scene_ble_scan_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == PredatorCustomEventPopupBack) {
            // Stop scanning and return to previous scene
            app->attack_running = false;
            scene_manager_previous_scene(app->scene_manager);
            consumed = true;
        }
    } else if(event.type == SceneManagerEventTypeTick) {
        if(app->attack_running) {
            // Every ~20 ticks, "find" a new device
            if(app->packets_sent % 20 == 0) {
                app->targets_found++;
                
                // Update popup text with scan results
                char scan_text[64];
                snprintf(scan_text, sizeof(scan_text), "Scanning for devices...\nFound: %lu devices\nPress Back to stop", app->targets_found);
                popup_set_text(app->popup, scan_text, 64, 25, AlignCenter, AlignTop);
            }
            
            // Increment packet counter
            app->packets_sent++;
            consumed = true;
        }
    }
    
    return consumed;
}

void predator_scene_ble_scan_new_on_exit(void* context) {
    PredatorApp* app = context;
    
    // Stop any running scan
    app->attack_running = false;
    
    // Clean up popup
    popup_reset(app->popup);
}



