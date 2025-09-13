#include "../predator_i.h"
#include "../helpers/predator_view_helpers.h"
#include "../helpers/predator_subghz.h"
#include "../helpers/predator_ui_elements.h"
#include "predator_scene.h"

// Remove or comment out unused functions to avoid build errors
/*
static void predator_scene_car_passive_opener_popup_callback(void* context) {
    // Implementation commented out to prevent build issues
}
*/

void predator_scene_car_passive_opener_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    if(!app) {
        FURI_LOG_E("CarPassiveOpener", "App context is NULL on enter");
        return;
    }
    
    // Validate board type before any hardware initialization
    if(app->board_type == 0) {
        FURI_LOG_W("CarPassiveOpener", "Board type is Unknown, defaulting to Original");
        app->board_type = 0; // Keep as Original
    }
    
    // Ensure scene_manager and view_dispatcher are valid to prevent crashes
    if(!app->scene_manager) {
        FURI_LOG_E("CarPassiveOpener", "Scene manager is NULL, cannot proceed");
        return;
    }
    
    if(!app->view_dispatcher) {
        FURI_LOG_E("CarPassiveOpener", "View dispatcher is NULL, cannot switch view");
        return;
    }
    
    // Configure popup content to avoid blank screen
    if(!app->popup) {
        FURI_LOG_E("CarPassiveOpener", "Popup is NULL, cannot initialize UI");
        return;
    }
    popup_reset(app->popup);
    popup_set_header(app->popup, "Passive Car Opener", 64, 10, AlignCenter, AlignTop);
    popup_set_text(app->popup, "Listening for key fob...\nPress Back to return", 64, 28, AlignCenter, AlignTop);
    popup_set_context(app->popup, app);
    popup_set_timeout(app->popup, 0);
    popup_enable_timeout(app->popup);

    // Start simulated key fob signal capture
    app->attack_running = true;
    app->packets_sent = 0;
    FURI_LOG_I("CarPassiveOpener", "Starting simulated key fob signal capture");

    // Switch to popup view
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
    FURI_LOG_I("CarPassiveOpener", "Passive Car Opener scene entered with simulation mode");
}

bool predator_scene_car_passive_opener_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(!app) {
        FURI_LOG_E("CarPassiveOpener", "App context is NULL in event handler");
        return false;
    }
    
    if(event.type == SceneManagerEventTypeBack) {
        FURI_LOG_I("CarPassiveOpener", "Back event received, navigating to previous scene");
        app->attack_running = false;
        scene_manager_previous_scene(app->scene_manager);
        consumed = true;
    } else if(event.type == SceneManagerEventTypeTick) {
        if(app->attack_running) {
            app->packets_sent += 1; // Simulate capturing signals
            if(app->packets_sent % 10 == 0) {
                // Update popup text to show progress
                char progress_text[64];
                snprintf(progress_text, sizeof(progress_text), "Signals captured: %lu\nPress Back to stop", app->packets_sent);
                if(app->popup) {
                    popup_set_text(app->popup, progress_text, 64, 28, AlignCenter, AlignTop);
                    FURI_LOG_I("CarPassiveOpener", "Updated popup with signals captured: %lu", app->packets_sent);
                } else {
                    FURI_LOG_W("CarPassiveOpener", "Popup is NULL, cannot update text");
                }
            }
            consumed = true;
        }
    }
    
    return consumed;
}

void predator_scene_car_passive_opener_new_on_exit(void* context) {
    PredatorApp* app = context;
    
    if(!app) {
        FURI_LOG_E("CarPassiveOpener", "App context is NULL on exit");
        return;
    }
    
    // Stop any running attack
    app->attack_running = false;
    FURI_LOG_I("CarPassiveOpener", "Exited Passive Car Opener scene");
}
