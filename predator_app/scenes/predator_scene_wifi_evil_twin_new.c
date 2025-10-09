#include "../predator_i.h"
#include "../helpers/predator_view_helpers.h"
#include "../helpers/predator_esp32.h"
#include "../helpers/predator_ui_elements.h"
#include "../helpers/predator_compliance.h"

void predator_scene_wifi_evil_twin_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    if(!app) {
        FURI_LOG_E("WiFiEvilTwin", "App context is NULL on enter");
        return;
    }
    
    // Validate board type before any hardware initialization
    if(app->board_type == 0) {
        FURI_LOG_W("WiFiEvilTwin", "Board type is Unknown, defaulting to Original");
        app->board_type = 0; // Keep as Original
    }
    
    // Ensure scene_manager and view_dispatcher are valid to prevent crashes
    if(!app->scene_manager) {
        FURI_LOG_E("WiFiEvilTwin", "Scene manager is NULL, cannot proceed");
        return;
    }
    
    if(!app->view_dispatcher) {
        FURI_LOG_E("WiFiEvilTwin", "View dispatcher is NULL, cannot switch view");
        return;
    }
    
    // Ensure popup is valid
    if(!app->popup) {
        FURI_LOG_E("WiFiEvilTwin", "Popup is NULL, cannot initialize UI");
        return;
    }
    
    // Configure popup content to avoid blank screen
    popup_reset(app->popup);
    popup_set_header(app->popup, "Evil Twin", 64, 10, AlignCenter, AlignTop);
    bool live_allowed = predator_compliance_is_feature_allowed(app, PredatorFeatureWifiEvilTwin, app->authorized);
    if(!live_allowed) {
        popup_set_text(app->popup, "Demo Mode — Authorization required\nPress Back to return", 64, 28, AlignCenter, AlignTop);
    } else {
        // Live path
        predator_esp32_init(app);
        bool started = predator_esp32_wifi_evil_twin(app);
        if(started) {
            popup_set_text(app->popup, "Live — Evil Twin active\nPress Back to stop", 64, 28, AlignCenter, AlignTop);
            FURI_LOG_I("WiFiEvilTwin", "Live Evil Twin started");
        } else {
            popup_set_text(app->popup, "ESP32 not ready — Falling back to Demo\nPress Back to return", 64, 28, AlignCenter, AlignTop);
            FURI_LOG_W("WiFiEvilTwin", "ESP32 start failed; falling back to Demo text");
        }
    }
    popup_set_context(app->popup, app);
    popup_set_timeout(app->popup, 0);
    popup_enable_timeout(app->popup);
    
    // Start simulated Evil Twin attack (for UI animation/progress)
    app->attack_running = true;
    app->targets_found = 0;
    app->packets_sent = 0;
    FURI_LOG_I("WiFiEvilTwin", "Starting simulated Evil Twin attack");
    
    // Switch to popup view
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
    FURI_LOG_I("WiFiEvilTwin", "WiFi Evil Twin scene entered");
}

bool predator_scene_wifi_evil_twin_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(!app) {
        FURI_LOG_E("WiFiEvilTwin", "App context is NULL in event handler");
        return false;
    }
    
    if(event.type == SceneManagerEventTypeBack) {
        // Handle back event to stop the attack
        app->attack_running = false;
        FURI_LOG_I("WiFiEvilTwin", "Back event received, stopping attack and navigating back");
        if(predator_compliance_is_feature_allowed(app, PredatorFeatureWifiEvilTwin, app->authorized)) {
            predator_esp32_stop_attack(app);
        }
        scene_manager_previous_scene(app->scene_manager);
        consumed = true;
    } else if(event.type == SceneManagerEventTypeTick) {
        if(app->attack_running) {
            // Simulate connections to Evil Twin AP using a simple counter
            app->packets_sent += 1;
            if(app->packets_sent >= 20 && app->popup) {
                app->packets_sent = 0;
                app->targets_found += 1;
                // Update popup text to show progress
                char progress_text[64];
                snprintf(progress_text, sizeof(progress_text), "Devices connected: %lu\nPress Back to stop", app->targets_found);
                popup_set_text(app->popup, progress_text, 64, 28, AlignCenter, AlignTop);
                FURI_LOG_I("WiFiEvilTwin", "Updated devices connected: %lu", app->targets_found);
            }
            consumed = true;
        }
    }
    
    return consumed;
}

void predator_scene_wifi_evil_twin_new_on_exit(void* context) {
    PredatorApp* app = context;
    
    if(!app) {
        FURI_LOG_E("WiFiEvilTwin", "App context is NULL on exit");
        return;
    }
    
    // Stop any running attack
    app->attack_running = false;
    if(predator_compliance_is_feature_allowed(app, PredatorFeatureWifiEvilTwin, app->authorized)) {
        predator_esp32_stop_attack(app);
    }
    FURI_LOG_I("WiFiEvilTwin", "Exited WiFi Evil Twin scene");
}
