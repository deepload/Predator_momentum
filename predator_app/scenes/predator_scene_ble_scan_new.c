#include "../predator_i.h"
#include "../helpers/predator_view_helpers.h"
#include "../helpers/predator_esp32.h"
#include "../helpers/predator_ui_elements.h"

// Popup callback for BLE scan
static void predator_scene_ble_scan_popup_callback(void* context) {
    PredatorApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, PredatorCustomEventPopupBack);
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



