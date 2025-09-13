#include "../predator_i.h"
#include "../helpers/predator_view_helpers.h"
#include "../helpers/predator_esp32.h"
#include "../helpers/predator_ui_elements.h"

void predator_scene_wifi_scan_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    if(!app) {
        FURI_LOG_E("WiFiScan", "App context is NULL on enter");
        return;
    }
    
    // Validate board type before any hardware initialization
    if(app->board_type == 0) {
        FURI_LOG_W("WiFiScan", "Board type is Unknown, defaulting to Original");
        app->board_type = 0; // Keep as Original
    }
    
    // Ensure scene_manager and view_dispatcher are valid to prevent crashes
    if(!app->scene_manager) {
        FURI_LOG_E("WiFiScan", "Scene manager is NULL, cannot proceed");
        return;
    }
    
    if(!app->view_dispatcher) {
        FURI_LOG_E("WiFiScan", "View dispatcher is NULL, cannot switch view");
        return;
    }
    
    // Ensure popup is valid
    if(!app->popup) {
        FURI_LOG_E("WiFiScan", "Popup is NULL, cannot initialize UI");
        return;
    }
    
    // Configure popup content to avoid blank screen
    popup_reset(app->popup);
    popup_set_header(app->popup, "WiFi Scanner", 64, 10, AlignCenter, AlignTop);
    popup_set_text(app->popup, "Preparing WiFi scan...\nPress Back to return", 64, 28, AlignCenter, AlignTop);
    popup_set_context(app->popup, app);
    popup_set_timeout(app->popup, 0);
    popup_enable_timeout(app->popup);

    // Start simulated WiFi scanning
    app->attack_running = true;
    app->targets_found = 0;
    app->packets_sent = 0;
    FURI_LOG_I("WiFiScan", "Starting simulated WiFi scanning");

    // Switch to popup view
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
    FURI_LOG_I("WiFiScan", "WiFi Scan scene entered with simulation mode");
}

bool predator_scene_wifi_scan_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(!app) {
        FURI_LOG_E("WiFiScan", "App context is NULL in event handler");
        return false;
    }
    
    if(event.type == SceneManagerEventTypeBack) {
        // Stop scanning and return to previous scene
        app->attack_running = false;
        FURI_LOG_I("WiFiScan", "Back event received, stopping scan and navigating back");
        scene_manager_previous_scene(app->scene_manager);
        consumed = true;
    } else if(event.type == SceneManagerEventTypeTick) {
        if(app->attack_running) {
            // Every ~15 ticks, "find" a new network
            if(app->packets_sent % 15 == 0 && app->popup) {
                app->targets_found++;
                // Update popup text to show progress
                char progress_text[64];
                snprintf(progress_text, sizeof(progress_text), "Networks found: %lu\nPress Back to stop", app->targets_found);
                popup_set_text(app->popup, progress_text, 64, 28, AlignCenter, AlignTop);
                FURI_LOG_I("WiFiScan", "Updated networks found: %lu", app->targets_found);
            }
            // Increment packet counter
            app->packets_sent++;
            consumed = true;
        }
    }
    
    return consumed;
}

void predator_scene_wifi_scan_new_on_exit(void* context) {
    PredatorApp* app = context;
    
    if(!app) {
        FURI_LOG_E("WiFiScan", "App context is NULL on exit");
        return;
    }
    
    // Stop any running scan
    app->attack_running = false;
    FURI_LOG_I("WiFiScan", "Exited WiFi Scan scene");
}
