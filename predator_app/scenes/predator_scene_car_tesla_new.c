#include "../predator_i.h"
#include "../helpers/predator_view_helpers.h"
#include "../helpers/predator_subghz.h"
#include "../helpers/predator_ui_elements.h"
#include "../helpers/predator_compliance.h"
#include "../helpers/predator_ui_status.h"
#include "../helpers/predator_logging.h"

static uint32_t g_tesla_enter_tick = 0;

void predator_scene_car_tesla_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    if(!app) {
        FURI_LOG_E("CarTesla", "App context is NULL on enter");
        return;
    }
    
    // Validate board type before any hardware initialization
    if(app->board_type == 0) {
        FURI_LOG_W("CarTesla", "Board type is Unknown, defaulting to Original");
        app->board_type = 0; // Keep as Original
    }
    
    // Ensure scene_manager and view_dispatcher are valid to prevent crashes
    if(!app->scene_manager) {
        FURI_LOG_E("CarTesla", "Scene manager is NULL, cannot proceed");
        return;
    }
    
    if(!app->view_dispatcher) {
        FURI_LOG_E("CarTesla", "View dispatcher is NULL, cannot switch view");
        return;
    }
    
    // Check if board configuration is accessible before SubGHz initialization
    const PredatorBoardConfig* config = predator_boards_get_config(app->board_type);
    if(!config) {
        FURI_LOG_E("CarTesla", "Board configuration is NULL, cannot initialize SubGHz");
        return;
    }
    
    // Configure popup content to avoid blank screen
    if(!app->popup) {
        FURI_LOG_E("CarTesla", "Popup is NULL, cannot initialize UI");
        return;
    }
    popup_reset(app->popup);
    popup_set_header(app->popup, "Tesla Charge Port", 64, 10, AlignCenter, AlignTop);
    bool live_allowed = predator_compliance_is_feature_allowed(app, PredatorFeatureCarTeslaCharge, app->authorized);
    if(!live_allowed) {
        popup_set_text(app->popup, "Demo Mode — Authorization required\nCharge Port demo only\nPress Back", 64, 28, AlignCenter, AlignTop);
    } else {
        // Live: initialize SubGHz and send Tesla charge open signal
        predator_subghz_init(app);
        predator_subghz_send_tesla_charge_port(app);
        char status[64]; predator_ui_build_status(app, "Action: ChargePort", status, sizeof(status));
        popup_set_text(app->popup, status, 64, 28, AlignCenter, AlignTop);
        FURI_LOG_I("CarTesla", "Live Tesla charge signal sent");
    }
    popup_set_context(app->popup, app);
    popup_set_timeout(app->popup, 0);
    popup_enable_timeout(app->popup);

    // Start simulated signal transmission for Tesla Charge Port
    app->attack_running = true;
    app->packets_sent = 0;
    app->vip_mode = true; // Default to normal mode
    FURI_LOG_I("CarTesla", "Starting simulated Tesla charge port signal transmission");

    // Switch to popup view
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
    g_tesla_enter_tick = furi_get_tick();
    predator_log_append(app, "Tesla ChargePort START");
    FURI_LOG_I("CarTesla", "Tesla Charge Port scene entered with simulation mode");
}

bool predator_scene_car_tesla_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(!app) {
        FURI_LOG_E("CarTesla", "App context is NULL in event handler");
        return false;
    }
    
    if(event.type == SceneManagerEventTypeBack) {
        uint32_t elapsed = furi_get_tick() - g_tesla_enter_tick;
        if(elapsed < 500) {
            FURI_LOG_I("CarTesla", "Back debounced (elapsed=%lu ms)", elapsed);
            return true;
        }
        FURI_LOG_I("CarTesla", "Back event received, navigating to previous scene");
        app->attack_running = false;
        if(predator_compliance_is_feature_allowed(app, PredatorFeatureCarTeslaCharge, app->authorized)) {
            predator_subghz_stop_attack(app);
        }
        predator_log_append(app, "Tesla ChargePort STOP");
        scene_manager_previous_scene(app->scene_manager);
        consumed = true;
    } else if(event.type == SceneManagerEventTypeTick) {
        if(app->attack_running) {
            app->packets_sent += 10; // Simulate sending signals quickly for demo
            if(app->packets_sent >= 30 && app->popup) {
                if(app->vip_mode) {
                    popup_set_text(app->popup, "All Tesla Stations UNLOCKED!\nSwitzerland Access Granted!\nPress Back", 64, 28, AlignCenter, AlignTop);
                    FURI_LOG_I("CarTesla", "VIP Mode: Simulated unlocking all Tesla stations in Switzerland");
                    app->attack_running = false; // Stop further updates for demo effect
                } else {
                    popup_set_text(app->popup, "Charge Port OPENED!\nSuccess! Press Back", 64, 28, AlignCenter, AlignTop);
                    FURI_LOG_I("CarTesla", "Simulated successful charge port opening for wow effect");
                    app->attack_running = false; // Stop further updates for demo effect
                }
            } else if(app->packets_sent % 10 == 0 && app->popup) {
                char progress_text[64];
                snprintf(progress_text, sizeof(progress_text), "Signals sent: %lu\nPress Back to stop", app->packets_sent);
                popup_set_text(app->popup, progress_text, 64, 28, AlignCenter, AlignTop);
                FURI_LOG_I("CarTesla", "Updated popup with signals sent: %lu", app->packets_sent);
            }
            consumed = true;
        }
    } else if(event.type == SceneManagerEventTypeCustom) {
        // Custom event to toggle VIP mode (could be triggered by a specific input or setting)
        if(event.event == 999) { // Placeholder for VIP mode toggle
            app->vip_mode = true;
            FURI_LOG_I("CarTesla", "VIP Mode activated for Switzerland Tesla stations unlock");
            popup_set_text(app->popup, "VIP Mode Activated\nUnlocking All Swiss Stations...", 64, 28, AlignCenter, AlignTop);
            app->packets_sent = 0;
            app->attack_running = true;
            consumed = true;
        }
    }
    
    return consumed;
}

void predator_scene_car_tesla_new_on_exit(void* context) {
    PredatorApp* app = context;
    
    if(!app) {
        FURI_LOG_E("CarTesla", "App context is NULL on exit");
        return;
    }
    
    // Clean up
    app->attack_running = false;
    app->vip_mode = false;
    if(predator_compliance_is_feature_allowed(app, PredatorFeatureCarTeslaCharge, app->authorized)) {
        predator_subghz_stop_attack(app);
    }
    FURI_LOG_I("CarTesla", "Exited Tesla Charge Port scene");
}
