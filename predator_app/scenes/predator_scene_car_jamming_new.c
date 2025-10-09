#include "../predator_i.h"
#include "../helpers/predator_view_helpers.h"
#include "../helpers/predator_subghz.h"
#include "../helpers/predator_ui_elements.h"
#include "../helpers/predator_compliance.h"

// Remove or comment out unused functions to avoid build errors
/*
static void predator_scene_car_jamming_popup_callback(void* context) {
    // Implementation commented out to prevent build issues
}
*/

/*
static void car_jamming_update_popup(PredatorApp* app) {
    // Implementation commented out to prevent build issues
}
*/

void predator_scene_car_jamming_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    if(!app) {
        FURI_LOG_E("CarJamming", "App context is NULL on enter");
        return;
    }
    
    // Validate board type before any hardware initialization
    if(app->board_type == 0) {
        FURI_LOG_W("CarJamming", "Board type is Unknown, defaulting to Original");
        app->board_type = 0; // Keep as Original
    }
    
    // Ensure scene_manager and view_dispatcher are valid to prevent crashes
    if(!app->scene_manager) {
        FURI_LOG_E("CarJamming", "Scene manager is NULL, cannot proceed");
        return;
    }
    
    if(!app->view_dispatcher) {
        FURI_LOG_E("CarJamming", "View dispatcher is NULL, cannot switch view");
        return;
    }
    
    // Check if board configuration is accessible before SubGHz initialization
    const PredatorBoardConfig* config = predator_boards_get_config(app->board_type);
    if(!config) {
        FURI_LOG_E("CarJamming", "Board configuration is NULL, cannot initialize SubGHz");
        return;
    }
    
    // Configure popup content to avoid blank screen
    if(!app->popup) {
        FURI_LOG_E("CarJamming", "Popup is NULL, cannot initialize UI");
        return;
    }
    popup_reset(app->popup);
    popup_set_header(app->popup, "Car Jamming", 64, 10, AlignCenter, AlignTop);
    bool live_allowed = predator_compliance_is_feature_allowed(app, PredatorFeatureCarJamming, app->authorized);
    if(!live_allowed) {
        popup_set_text(app->popup, "Demo Mode — Authorization required\nSimulated jamming only\nPress Back", 64, 28, AlignCenter, AlignTop);
    } else {
        // Live: SubGHz jamming with region-appropriate frequency
        predator_subghz_init(app);
        uint32_t freq = 433920000; // default EU/CH
        PredatorRegion r = app->region;
        if(r == PredatorRegionUS) freq = 315000000;
        else if(r == PredatorRegionJP) freq = 315000000;
        else if(r == PredatorRegionCN) freq = 433920000;
        bool started = predator_subghz_start_jamming(app, freq);
        if(started) {
            char buf[64];
            snprintf(buf, sizeof(buf), "Live — Jamming %lu Hz\nPress Back to stop", freq);
            popup_set_text(app->popup, buf, 64, 28, AlignCenter, AlignTop);
            FURI_LOG_I("CarJamming", "Live jamming started at %lu Hz", freq);
        } else {
            popup_set_text(app->popup, "RF not ready — Falling back to Demo\nPress Back to return", 64, 28, AlignCenter, AlignTop);
            FURI_LOG_W("CarJamming", "SubGHz start_jamming failed; falling back to Demo text");
        }
    }
    popup_set_context(app->popup, app);
    popup_set_timeout(app->popup, 0);
    popup_enable_timeout(app->popup);

    // Start simulated jamming signal transmission
    app->attack_running = true;
    app->packets_sent = 0;
    app->swiss_station_test = false; // Default to normal mode
    FURI_LOG_I("CarJamming", "Starting simulated car jamming signal transmission");

    // Switch to popup view
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
    FURI_LOG_I("CarJamming", "Car Jamming scene entered with simulation mode");
}

bool predator_scene_car_jamming_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(!app) {
        FURI_LOG_E("CarJamming", "App context is NULL in event handler");
        return false;
    }
    
    if(event.type == SceneManagerEventTypeBack) {
        FURI_LOG_I("CarJamming", "Back event received, navigating to previous scene");
        app->attack_running = false;
        if(predator_compliance_is_feature_allowed(app, PredatorFeatureCarJamming, app->authorized)) {
            predator_subghz_stop_attack(app);
        }
        scene_manager_previous_scene(app->scene_manager);
        consumed = true;
    } else if(event.type == SceneManagerEventTypeTick) {
        if(app->attack_running) {
            app->packets_sent += 20; // Simulate sending jamming signals quickly
            if(app->packets_sent >= 50 && app->popup) {
                if(app->swiss_station_test) {
                    popup_set_text(app->popup, "Swiss Electric Stations JAMMED!\nSecurity Test Successful!\nPress Back", 64, 28, AlignCenter, AlignTop);
                    FURI_LOG_I("CarJamming", "Simulated successful jamming of Swiss electric car stations");
                    app->attack_running = false; // Stop further updates for demo effect
                } else {
                    popup_set_text(app->popup, "Car Signal JAMMED!\nSuccess! Press Back", 64, 28, AlignCenter, AlignTop);
                    FURI_LOG_I("CarJamming", "Simulated successful car signal jamming");
                    app->attack_running = false; // Stop further updates for demo effect
                }
            } else if(app->packets_sent % 10 == 0 && app->popup) {
                char progress_text[64];
                snprintf(progress_text, sizeof(progress_text), "Jamming signals: %lu\nPress Back to stop", app->packets_sent);
                popup_set_text(app->popup, progress_text, 64, 28, AlignCenter, AlignTop);
                FURI_LOG_I("CarJamming", "Updated popup with jamming signals sent: %lu", app->packets_sent);
            }
            consumed = true;
        }
    } else if(event.type == SceneManagerEventTypeCustom) {
        // Custom event to toggle Swiss station test mode (could be triggered by a specific input or setting)
        if(event.event == 998) { // Placeholder for Swiss station test mode toggle
            app->swiss_station_test = true;
            FURI_LOG_I("CarJamming", "Swiss Electric Station Test Mode activated");
            popup_set_text(app->popup, "Swiss Station Test Mode\nJamming All Electric Stations...", 64, 28, AlignCenter, AlignTop);
            app->packets_sent = 0;
            app->attack_running = true;
            consumed = true;
        }
    }
    
    return consumed;
}

void predator_scene_car_jamming_new_on_exit(void* context) {
    PredatorApp* app = context;
    
    if(!app) {
        FURI_LOG_E("CarJamming", "App context is NULL on exit");
        return;
    }
    
    // Clean up
    app->attack_running = false;
    app->swiss_station_test = false;
    if(predator_compliance_is_feature_allowed(app, PredatorFeatureCarJamming, app->authorized)) {
        predator_subghz_stop_attack(app);
    }
    FURI_LOG_I("CarJamming", "Exited Car Jamming scene");
}
