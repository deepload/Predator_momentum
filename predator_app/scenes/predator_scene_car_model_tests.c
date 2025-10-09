#include "../predator_i.h"
#include "predator_scene.h"
#include "predator_submenu_index.h"
#include "../helpers/predator_ui_status.h"
#include "../helpers/predator_logging.h"
#include "../helpers/predator_compliance.h"
#include "../helpers/predator_subghz.h"

// Submenu callback
static void car_model_tests_submenu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    if(!app || !app->view_dispatcher) return;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void predator_scene_car_model_tests_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app) return;

    if(!app->scene_manager || !app->view_dispatcher || !app->submenu) return;

    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "Model Tests");

    // Show selected model at top as a non-selectable info line via a fake index
    char hdr[64];
    snprintf(hdr, sizeof(hdr), "%.15s %.23s", app->selected_model_make, app->selected_model_name);
    submenu_add_item(app->submenu, hdr, 0, car_model_tests_submenu_callback, app);

    // Add functional tests
    submenu_add_item(app->submenu, "Tesla Charge Port", 1, car_model_tests_submenu_callback, app);
    submenu_add_item(app->submenu, "Rolling Code Attack", 2, car_model_tests_submenu_callback, app);
    submenu_add_item(app->submenu, "RF Jamming", 3, car_model_tests_submenu_callback, app);
    submenu_add_item(app->submenu, "Passive Key (TBD)", 4, car_model_tests_submenu_callback, app);
    submenu_add_item(app->submenu, "Live Monitor (logs)", 99, car_model_tests_submenu_callback, app);

    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

bool predator_scene_car_model_tests_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    if(!app) return false;

    if(event.type == SceneManagerEventTypeBack) {
        scene_manager_previous_scene(app->scene_manager);
        return true;
    }

    if(event.type == SceneManagerEventTypeCustom) {
        switch(event.event) {
        case 0:
            // Non-selectable header; ignore
            return true;
        case 1: // Tesla Charge Port
            scene_manager_next_scene(app->scene_manager, PredatorSceneCarTesla);
            return true;
        case 2: { // Rolling Code Attack
            // Region-aware frequency; fallback to model freq if present
            uint32_t freq = app->selected_model_freq ? app->selected_model_freq : 433920000;
            if(app->region == PredatorRegionUS || app->region == PredatorRegionJP) freq = 315000000;
            app->selected_model_freq = freq; // stash for downstream scenes
            predator_log_append(app, "ModelTests RollingCode");
            FURI_LOG_I("ModelTests", "RollingCode using freq %u Hz", (unsigned)freq);
            scene_manager_next_scene(app->scene_manager, PredatorSceneCarKeyBruteforce);
            return true;
        }
        case 3: { // RF Jamming
            predator_log_append(app, "ModelTests Jamming");
            scene_manager_next_scene(app->scene_manager, PredatorSceneCarJamming);
            return true;
        }
        case 4: // Passive Key (TBD)
            if(app->popup) {
                popup_reset(app->popup);
                char status[64]; predator_ui_build_status(app, "Passive Key TBD", status, sizeof(status));
                popup_set_header(app->popup, "Passive Key", 64, 10, AlignCenter, AlignTop);
                popup_set_text(app->popup, status, 64, 28, AlignCenter, AlignTop);
                popup_set_context(app->popup, app);
                popup_set_timeout(app->popup, 0);
                popup_enable_timeout(app->popup);
                view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
            }
            return true;
        case 99: // Live Monitor
            scene_manager_next_scene(app->scene_manager, PredatorSceneLiveMonitor);
            return true;
        default:
            return false;
        }
    }

    return false;
}

void predator_scene_car_model_tests_on_exit(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    if(app->submenu) submenu_reset(app->submenu);
}
