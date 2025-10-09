#include "../predator_i.h"
#include "../helpers/predator_view_helpers.h"
#include "../helpers/predator_subghz.h"
#include "../helpers/predator_ui_elements.h"
#include "../helpers/predator_compliance.h"
#include "../helpers/predator_ui_status.h"
#include "../helpers/predator_logging.h"

// Remove or comment out unused functions to avoid build errors
/*
static void predator_scene_car_key_bruteforce_popup_callback(void* context) {
    // Implementation commented out to prevent build issues
}
*/

/*
static void car_key_bruteforce_update_popup(PredatorApp* app) {
    // Implementation commented out to prevent build issues
}
*/

void predator_scene_car_key_bruteforce_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    if(!app) {
        FURI_LOG_E("CarKeyBruteforce", "App context is NULL on enter");
        return;
    }
    
    // Validate board type before any hardware initialization
    if(app->board_type == 0) {
        FURI_LOG_W("CarKeyBruteforce", "Board type is Unknown, defaulting to Original");
        app->board_type = 0; // Keep as Original
    }
    
    // Ensure scene_manager and view_dispatcher are valid to prevent crashes
    if(!app->scene_manager) {
        FURI_LOG_E("CarKeyBruteforce", "Scene manager is NULL, cannot proceed");
        return;
    }
    
    if(!app->view_dispatcher) {
        FURI_LOG_E("CarKeyBruteforce", "View dispatcher is NULL, cannot switch view");
        return;
    }
    
    // Configure popup content to avoid blank screen
    if(!app->popup) {
        FURI_LOG_E("CarKeyBruteforce", "Popup is NULL, cannot initialize UI");
        return;
    }
    popup_reset(app->popup);
    popup_set_header(app->popup, "Car Key Bruteforce", 64, 10, AlignCenter, AlignTop);
    // Prefer selected model frequency, else region-aware default
    PredatorRegion r = app->region; PredatorFeature feat = PredatorFeatureSubGhz433Tx; uint32_t freq = 433920000;
    if(app->selected_model_freq) {
        freq = app->selected_model_freq;
        // Choose feature based on freq
        feat = (freq < 400000000) ? PredatorFeatureSubGhz315Tx : PredatorFeatureSubGhz433Tx;
    } else if(r == PredatorRegionUS || r == PredatorRegionJP) {
        feat = PredatorFeatureSubGhz315Tx; freq = 315000000;
    }
    bool live_allowed = predator_compliance_is_feature_allowed(app, feat, app->authorized);
    // Always show initial status including frequency
    {
        char detail[48]; snprintf(detail, sizeof(detail), "Rolling @ %lu Hz", freq);
        char status[64]; predator_ui_build_status(app, detail, status, sizeof(status));
        popup_set_text(app->popup, status, 64, 28, AlignCenter, AlignTop);
    }
    if(live_allowed) {
        predator_subghz_init(app);
        if(predator_subghz_start_rolling_code_attack(app, freq)) {
            char logline[80]; snprintf(logline, sizeof(logline), "CarRolling START freq=%lu", freq);
            predator_log_append(app, logline);
        } else {
            predator_log_append(app, "CarRolling FALLBACK_SIM");
            FURI_LOG_W("CarKeyBruteforce", "RF start failed; continuing with simulated progress");
        }
    } else {
        predator_log_append(app, "CarRolling DEMO_SIM");
    }
    popup_set_context(app->popup, app);
    popup_set_timeout(app->popup, 0);
    popup_enable_timeout(app->popup);

    // Start simulated rolling code transmission
    app->attack_running = true;
    app->packets_sent = 0;
    FURI_LOG_I("CarKeyBruteforce", "Starting simulated rolling code transmission");

    // Switch to popup view
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
    FURI_LOG_I("CarKeyBruteforce", "Car Key Bruteforce scene entered with simulation mode");
}

bool predator_scene_car_key_bruteforce_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(!app) {
        FURI_LOG_E("CarKeyBruteforce", "App context is NULL in event handler");
        return false;
    }
    
    if(event.type == SceneManagerEventTypeBack) {
        FURI_LOG_I("CarKeyBruteforce", "Back event received, navigating to previous scene");
        app->attack_running = false;
        // Stop live rolling code if active
        PredatorRegion r = app->region; PredatorFeature feat = (r == PredatorRegionUS || r == PredatorRegionJP) ? PredatorFeatureSubGhz315Tx : PredatorFeatureSubGhz433Tx;
        if(predator_compliance_is_feature_allowed(app, feat, app->authorized)) {
            predator_subghz_stop_rolling_code_attack(app);
        }
        scene_manager_previous_scene(app->scene_manager);
        consumed = true;
    } else if(event.type == SceneManagerEventTypeTick) {
        if(app->attack_running) {
            app->packets_sent += 10; // Faster visible progress
            if(app->packets_sent % 10 == 0) {
                // Update popup text to show progress
                char progress_text[64];
                snprintf(progress_text, sizeof(progress_text), "Codes attempted: %lu\nPress Back to stop", app->packets_sent);
                if(app->popup) {
                    popup_set_text(app->popup, progress_text, 64, 28, AlignCenter, AlignTop);
                    FURI_LOG_I("CarKeyBruteforce", "Updated popup with codes attempted: %lu", app->packets_sent);
                } else {
                    FURI_LOG_W("CarKeyBruteforce", "Popup is NULL, cannot update text");
                }
            }
            consumed = true;
        }
    }
    
    return consumed;
}

void predator_scene_car_key_bruteforce_new_on_exit(void* context) {
    PredatorApp* app = context;
    
    if(!app) {
        FURI_LOG_E("CarKeyBruteforce", "App context is NULL on exit");
        return;
    }
    
    app->attack_running = false;
    // Stop live rolling code if active
    PredatorRegion r = app->region; PredatorFeature feat = (r == PredatorRegionUS || r == PredatorRegionJP) ? PredatorFeatureSubGhz315Tx : PredatorFeatureSubGhz433Tx;
    if(predator_compliance_is_feature_allowed(app, feat, app->authorized)) {
        predator_subghz_stop_rolling_code_attack(app);
    }
    FURI_LOG_I("CarKeyBruteforce", "Exited Car Key Bruteforce scene");
}
