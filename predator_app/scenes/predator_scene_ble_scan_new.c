#include "../predator_i.h"
#include "../helpers/predator_view_helpers.h"
#include "../helpers/predator_esp32.h"
#include "../helpers/predator_ui_elements.h"

typedef struct {
    View* view;
    uint32_t devices_found;
    bool scanning;
    uint8_t animation_frame;
    uint8_t signal_strength;
    char device_name[32];
    char device_mac[18];
} BleScanView;

static void ble_scan_view_draw_callback(Canvas* canvas, void* context) {
    PredatorApp* app = context;
    
    if(!app) return;
    
    // Get view state
    BleScanView* state = PREDATOR_GET_MODEL(app->view_dispatcher, BleScanView);
    if(!state) return;
    
    // Update animation frame
    uint8_t animation_frame = (furi_get_tick() / 200) % 4;
    state->animation_frame = animation_frame;
    
    // Update state from app
    state->scanning = app->attack_running;
    state->devices_found = app->targets_found;
    
    // Update signal strength (cycles 1-3 for animation)
    state->signal_strength = 1 + (animation_frame % 3);
    
    // Update device info with latest found device
    if(state->devices_found > 0) {
        // Simulate different devices being found
        uint32_t device_num = state->devices_found % 5;
        switch(device_num) {
        case 0:
            strncpy(state->device_name, "iPhone 14", sizeof(state->device_name));
            strncpy(state->device_mac, "A4:5E:60:12:EF:19", sizeof(state->device_mac));
            break;
        case 1:
            strncpy(state->device_name, "Galaxy Watch", sizeof(state->device_name));
            strncpy(state->device_mac, "E8:33:B2:C7:89:1D", sizeof(state->device_mac));
            break;
        case 2:
            strncpy(state->device_name, "Bluetooth Speaker", sizeof(state->device_name));
            strncpy(state->device_mac, "00:1A:7D:DA:71:11", sizeof(state->device_mac));
            break;
        case 3:
            strncpy(state->device_name, "Car Audio", sizeof(state->device_name));
            strncpy(state->device_mac, "F4:65:A6:BF:25:90", sizeof(state->device_mac));
            break;
        case 4:
        default:
            strncpy(state->device_name, "Fitness Tracker", sizeof(state->device_name));
            strncpy(state->device_mac, "D8:9E:3F:12:45:A2", sizeof(state->device_mac));
            break;
        }
    } else {
        strncpy(state->device_name, "No devices found", sizeof(state->device_name));
        strncpy(state->device_mac, "--:--:--:--:--:--", sizeof(state->device_mac));
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
    if(state->scanning) {
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
    snprintf(device_text, sizeof(device_text), "Devices found: %lu", state->devices_found);
    canvas_draw_str(canvas, 50, 32, device_text);
    
    // Status text
    canvas_draw_str(canvas, 50, 42, "Status:");
    if(state->scanning) {
        // Show animated scanning status
        char scan_text[16] = "Scanning";
        for(uint8_t i = 0; i < (animation_frame % 4); i++) {
            strcat(scan_text, ".");
        }
        canvas_draw_str(canvas, 90, 42, scan_text);
    } else {
        canvas_draw_str(canvas, 90, 42, "Ready");
    }
    
    // Device information section
    predator_ui_draw_status_box(canvas, "Device Info", 10, 56, 108, 28);
    
    if(state->devices_found > 0) {
        // Draw device name and MAC
        canvas_draw_str(canvas, 16, 66, state->device_name);
        canvas_draw_str(canvas, 16, 76, state->device_mac);
        
        // Draw signal strength indicator
        canvas_draw_str(canvas, 90, 66, "RSSI:");
        
        for(uint8_t i = 0; i < 3; i++) {
            uint8_t bar_height = 2 + (i * 2);
            uint8_t bar_x = 90 + (i * 4);
            uint8_t bar_y = 76;
            
            if(i < state->signal_strength) {
                canvas_draw_box(canvas, bar_x, bar_y - bar_height, 2, bar_height);
            } else {
                canvas_draw_frame(canvas, bar_x, bar_y - bar_height, 2, bar_height);
            }
        }
    } else {
        // No devices found
        canvas_draw_str_aligned(canvas, 64, 70, AlignCenter, AlignCenter, 
            state->scanning ? "Searching..." : "Press OK to start");
    }
    
    // Draw controls
    canvas_set_font(canvas, FontSecondary);
    elements_button_left(canvas, "Back");
    
    if(state->scanning) {
        elements_button_center(canvas, "Stop");
    } else {
        elements_button_center(canvas, "Scan");
    }
}

static bool ble_scan_view_input_callback(InputEvent* event, void* context) {
    PredatorApp* app = context;
    bool consumed = false;
    
    // Get view state
    BleScanView* state = PREDATOR_GET_MODEL(app->view_dispatcher, BleScanView);
    if(!state) return consumed;
    
    if(event->type == InputTypeShort) {
        switch(event->key) {
        case InputKeyBack:
        case InputKeyLeft:
            scene_manager_previous_scene(app->scene_manager);
            consumed = true;
            break;
            
        case InputKeyOk:
        case InputKeyCenter:
            // Toggle scanning
            if(state->scanning) {
                // Stop scanning
                app->attack_running = false;
                state->scanning = false;
            } else {
                // Start scanning
                app->attack_running = true;
                state->scanning = true;
                app->targets_found = 0; // Reset device counter
            }
            consumed = true;
            break;
            
        default:
            break;
        }
    }
    
    return consumed;
}

static View* ble_scan_view_alloc(PredatorApp* app) {
    View* view = view_alloc();
    view_set_context(view, app);
    view_set_draw_callback(view, ble_scan_view_draw_callback);
    view_set_input_callback(view, ble_scan_view_input_callback);
    
    // Initialize model
    BleScanView* state = malloc(sizeof(BleScanView));
    state->devices_found = 0;
    state->scanning = false;
    state->animation_frame = 0;
    state->signal_strength = 0;
    strncpy(state->device_name, "No devices found", sizeof(state->device_name));
    strncpy(state->device_mac, "--:--:--:--:--:--", sizeof(state->device_mac));
    
    predator_view_set_model(view, state);
    predator_view_set_model_free_callback(view, free);
    
    return view;
}

static void ble_scan_view_free(View* view) {
    view_free(view);
}

void predator_scene_ble_scan_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    // Initialize ESP32 hardware
    predator_esp32_init(app);
    
    // Create custom view
    View* view = ble_scan_view_alloc(app);
    
    // Replace popup view with custom view
    view_dispatcher_remove_view(app->view_dispatcher, PredatorViewPopup);
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewPopup, view);
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
}

bool predator_scene_ble_scan_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(event.type == SceneManagerEventTypeBack) {
        consumed = true;
        app->attack_running = false;
        scene_manager_previous_scene(app->scene_manager);
    } else if(event.type == SceneManagerEventTypeTick) {
        if(app->attack_running) {
            // Every ~20 ticks, "find" a new device
            if(app->packets_sent % 20 == 0) {
                app->targets_found++;
            }
            
            // Increment packet counter
            app->packets_sent++;
            
            // Force view refresh for animations
            view_dispatcher_send_custom_event(app->view_dispatcher, 0xFF);
            consumed = true;
        }
    }
    
    return consumed;
}

void predator_scene_ble_scan_new_on_exit(void* context) {
    PredatorApp* app = context;
    app->attack_running = false;
    
    // Remove and free custom view
    view_dispatcher_remove_view(app->view_dispatcher, PredatorViewPopup);
    View* view = predator_view_dispatcher_get_current_view(app->view_dispatcher);
    if(view) {
        ble_scan_view_free(view);
    }
    
    // Restore standard popup view
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewPopup, popup_get_view(app->popup));
}



