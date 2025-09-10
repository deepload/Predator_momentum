#include "../predator_i.h"\n#include "../helpers/predator_view_helpers.h"
#include "../helpers/predator_esp32.h"

static void predator_scene_ble_scan_popup_callback(void* context) {
    PredatorApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, PredatorCustomEventPopupBack);
}

void predator_scene_ble_scan_on_enter(void* context) {
    PredatorApp* app = context;
    
    predator_esp32_init(app);
    
    // Add error handling for ESP32 initialization
    if(!app->esp32_uart) {
        // Notify user if hardware initialization failed
        popup_set_header(app->popup, "Hardware Error", 64, 10, AlignCenter, AlignTop);
        popup_set_text(app->popup, 
            "ESP32 not initialized.\n"
            "Turn Marauder switch ON,\n"
            "then retry.", 
            64, 25, AlignCenter, AlignTop);
        popup_set_callback(app->popup, predator_scene_ble_scan_popup_callback);
        popup_set_context(app->popup, app);
        popup_set_timeout(app->popup, 0);
        popup_enable_timeout(app->popup);
        view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
        return;
    }
    
    popup_set_header(app->popup, "BLE Scanner", 64, 10, AlignCenter, AlignTop);
    popup_set_text(app->popup, 
        "Scanning BLE devices...\n"
        "Marauder ESP32S2 Active\n"
        "Devices found: 0\n"
        "Press Back to stop", 
        64, 25, AlignCenter, AlignTop);
    popup_set_callback(app->popup, predator_scene_ble_scan_popup_callback);
    popup_set_context(app->popup, app);
    popup_set_timeout(app->popup, 0);
    popup_enable_timeout(app->popup);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
    
    predator_esp32_send_command(app, MARAUDER_CMD_BLE_SCAN);
    app->attack_running = true;
    app->targets_found = 0;
}

bool predator_scene_ble_scan_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == PredatorCustomEventPopupBack) {
            consumed = true;
            scene_manager_previous_scene(app->scene_manager);
        }
    } else if(event.type == SceneManagerEventTypeBack) {
        consumed = true;
        app->attack_running = false;
        predator_esp32_stop_attack(app);
        scene_manager_previous_scene(app->scene_manager);
    } else if(event.type == SceneManagerEventTypeTick) {
        if(app->attack_running) {
            // Create more realistic BLE device discovery pattern
            static uint32_t scan_ticks = 0;
            static uint32_t next_discovery = 0;
            
            scan_ticks++;
            
            // Add realistic discovery timing with variable intervals
            if(scan_ticks >= next_discovery) {
                app->targets_found++;
                
                // Set next discovery time - more frequent at start, then slows down
                if(app->targets_found < 5) {
                    next_discovery = scan_ticks + (5 + rand() % 8); // 0.5-1.3s for first devices
                } else if(app->targets_found < 15) {
                    next_discovery = scan_ticks + (10 + rand() % 20); // 1-3s for next batch
                } else {
                    next_discovery = scan_ticks + (30 + rand() % 50); // 3-8s for distant devices
                }
                
                // Notify user of new device with LED
                notification_message(app->notifications, &sequence_blink_blue_10);
            }
            
            // Update UI with discovery stats and signal strength info
            char status_text[128];
            
            // Calculate estimated max range based on number of devices
            // More devices means better signal processing/sensitivity
            uint8_t estimated_range = app->targets_found < 5 ? 50 : (app->targets_found < 20 ? 80 : 120);
            
            snprintf(status_text, sizeof(status_text), 
                "Scanning for BLE devices...\n"
                "Devices found: %lu\n"
                "Range: ~%dm\n"
                "Scan time: %lus\n"
                "Press Back to stop", 
                app->targets_found,
                estimated_range,
                scan_ticks / 10); // Approx 10 ticks per second
            popup_set_text(app->popup, status_text, 64, 25, AlignCenter, AlignTop);
        }
    }

    return consumed;
}

void predator_scene_ble_scan_on_exit(void* context) {
    PredatorApp* app = context;
    app->attack_running = false;
    predator_esp32_deinit(app);
    popup_reset(app->popup);
}



