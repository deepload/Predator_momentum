#include "../predator_i.h"
#include "../helpers/predator_view_helpers.h"
#include "../helpers/predator_esp32.h"
#include "../helpers/predator_ui_elements.h"

// Popup callback for evil twin
static void predator_scene_wifi_evil_twin_popup_callback(void* context) {
    PredatorApp* app = context;
    if(!app || !app->view_dispatcher) return;
    view_dispatcher_send_custom_event(app->view_dispatcher, PredatorCustomEventPopupBack);
}

static void evil_twin_update_popup(PredatorApp* app) {
    if(!app || !app->popup) return;
    
    // Update popup text based on attack progress
    char popup_text[128];
    uint32_t clients_connected = app->packets_sent / 50;
    
    if(!app->attack_running) {
        snprintf(popup_text, sizeof(popup_text), "📶 Starting Evil Twin AP\nSSID: FreeWiFi\nPress Back to stop");
    } else if(clients_connected == 0) {
        snprintf(popup_text, sizeof(popup_text), "📶 Evil Twin AP Active\nSSID: FreeWiFi\nWaiting for clients...");
    } else {
        snprintf(popup_text, sizeof(popup_text), "📶 Evil Twin AP Active\nSSID: FreeWiFi\nClients: %lu", clients_connected);
    }
    
    popup_set_text(app->popup, popup_text, 64, 32, AlignCenter, AlignTop);
}

void predator_scene_wifi_evil_twin_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    if(!app) {
        FURI_LOG_E("WifiEvilTwin", "App context is NULL on enter");
        return;
    }
    
    // Validate board type before any hardware initialization
    if(app->board_type == 0) { // Assuming 0 represents Unknown or default
        FURI_LOG_W("WifiEvilTwin", "Board type is Unknown, defaulting to Original");
        app->board_type = 0; // Keep as Original
    }
    
    // Ensure scene_manager and view_dispatcher are valid to prevent crashes
    if(!app->scene_manager) {
        FURI_LOG_E("WifiEvilTwin", "Scene manager is NULL, cannot proceed");
        return;
    }
    
    if(!app->view_dispatcher) {
        FURI_LOG_E("WifiEvilTwin", "View dispatcher is NULL, cannot switch view");
        return;
    }
    
    // Initialize ESP32 hardware
    predator_esp32_init(app);
    
    // Use popup with callback for safe event-driven navigation
    popup_set_header(app->popup, "Evil Twin AP", 64, 10, AlignCenter, AlignTop);
    
    if(!app->esp32_uart) {
        popup_set_text(app->popup, "❌ Hardware Error\nESP32 not available\nCheck hardware", 64, 32, AlignCenter, AlignTop);
    } else {
        popup_set_text(app->popup, "📶 Starting Evil Twin AP\nSSID: FreeWiFi\nPress Back to stop", 64, 32, AlignCenter, AlignTop);
        predator_esp32_wifi_evil_twin(app);
        app->attack_running = true;
        app->packets_sent = 0;
    }
    
    popup_set_context(app->popup, app);
    popup_set_callback(app->popup, predator_scene_wifi_evil_twin_popup_callback);
    popup_set_timeout(app->popup, 0);
    popup_enable_timeout(app->popup);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
}

bool predator_scene_wifi_evil_twin_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == PredatorCustomEventPopupBack) {
            app->attack_running = false;
            predator_esp32_deinit(app);
            scene_manager_previous_scene(app->scene_manager);
            consumed = true;
        }
    } else if(event.type == SceneManagerEventTypeBack) {
        app->attack_running = false;
        predator_esp32_deinit(app);
        scene_manager_previous_scene(app->scene_manager);
        consumed = true;
    } else if(event.type == SceneManagerEventTypeTick) {
        if(app->attack_running) {
            app->packets_sent++;
            
            // Update popup text every 10 ticks
            if(app->packets_sent % 10 == 0) {
                evil_twin_update_popup(app);
            }
            
            consumed = true;
        }
    }
    
    return consumed;
}

void predator_scene_wifi_evil_twin_new_on_exit(void* context) {
    PredatorApp* app = context;
    
    // Clean up
    app->attack_running = false;
    predator_esp32_deinit(app);
}
