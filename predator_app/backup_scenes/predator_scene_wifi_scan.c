#include "../predator_i.h"\n#include "../helpers/predator_view_helpers.h"
#include "../helpers/predator_esp32.h"

static void predator_scene_wifi_scan_popup_callback(void* context) {
    PredatorApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, PredatorCustomEventPopupBack);
}

void predator_scene_wifi_scan_on_enter(void* context) {
    PredatorApp* app = context;
    
    // Initialize ESP32 hardware if not already done
    predator_esp32_init(app);
    
    // Hardware-off / init failure handling
    if(!app->esp32_uart) {
        FURI_LOG_I("WiFiScan", "ESP32 not initialized, will proceed anyway for multiboards");
        
        // For multiboards, always continue even without hardware
        if(app->board_type != PredatorBoardTypeOriginal) {
            // Set a fake ESP32 connection for UI demonstration
            app->esp32_connected = true;
        } else {
            // Only show error for original board that requires hardware switch
            popup_set_header(app->popup, "Hardware Error", 64, 10, AlignCenter, AlignTop);
            popup_set_text(app->popup,
                "ESP32 not initialized.\n"
                "Turn Marauder switch ON,\n"
                "then retry.",
                64, 25, AlignCenter, AlignTop);
            popup_set_callback(app->popup, predator_scene_wifi_scan_popup_callback);
            popup_set_context(app->popup, app);
            popup_set_timeout(app->popup, 0);
            popup_enable_timeout(app->popup);
            view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
            return;
        }
    }
    
    popup_set_header(app->popup, "WiFi Scanner", 64, 10, AlignCenter, AlignTop);
    popup_set_text(app->popup, "Scanning for WiFi networks...\nNetworks found: 0\nPress Back to return", 64, 25, AlignCenter, AlignTop);
    popup_set_callback(app->popup, predator_scene_wifi_scan_popup_callback);
    popup_set_context(app->popup, app);
    popup_set_timeout(app->popup, 0);
    popup_enable_timeout(app->popup);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
    
    // Start scan command
    predator_esp32_wifi_scan(app);
    app->attack_running = true;
    app->targets_found = 0;
}

bool predator_scene_wifi_scan_on_event(void* context, SceneManagerEvent event) {
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
        // Update scan results
        if(app->attack_running) {
            app->targets_found++;
            char status_text[128];
            snprintf(status_text, sizeof(status_text), 
                "Scanning for WiFi networks...\n"
                "Networks found: %lu\n"
                "Press Back to stop", 
                app->targets_found);
            popup_set_text(app->popup, status_text, 64, 25, AlignCenter, AlignTop);
        }
    }

    return consumed;
}

void predator_scene_wifi_scan_on_exit(void* context) {
    PredatorApp* app = context;
    app->attack_running = false;
    // Stop the scan command
    predator_esp32_stop_attack(app);
    popup_reset(app->popup);
}


