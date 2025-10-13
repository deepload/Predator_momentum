#include "../predator_i.h"
#include "predator_scene.h"
#include "../helpers/predator_subghz.h"

// Attack methods for the model selected in app->selected_model_*
// Uses shared submenu only. Real hardware calls, no simulation.

static void model_attacks_cb(void* context, uint32_t index) {
    PredatorApp* app = context;
    if(!app || !app->view_dispatcher) return;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void predator_scene_car_model_attacks_ui_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app || !app->submenu) return;

    submenu_reset(app->submenu);

    char header[48];
    // Show Make Model and MHz (integer formatting, trimmed to fit safely)
    unsigned long mhz_i = (unsigned long)(app->selected_model_freq / 1000000U);
    unsigned long mhz_d = (unsigned long)((app->selected_model_freq % 1000000U) / 10000U); // 2 decimals
    snprintf(header, sizeof(header), "🚗 %.12s %.16s (%lu.%02luMHz)",
             app->selected_model_make,
             app->selected_model_name,
             mhz_i, mhz_d);
    submenu_set_header(app->submenu, header);

    // Offer all methods; user decides which to demo
    submenu_add_item(app->submenu, "🔑 Rolling Code Attack", 1, model_attacks_cb, app);
    submenu_add_item(app->submenu, "🔒 Fixed Code Bruteforce", 2, model_attacks_cb, app);
    submenu_add_item(app->submenu, "🔐 Smart Key / Proximity", 3, model_attacks_cb, app);
    submenu_add_item(app->submenu, "📈 Live Monitor", 4, model_attacks_cb, app);

    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

bool predator_scene_car_model_attacks_ui_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    if(!app) return false;

    if(event.type == SceneManagerEventTypeBack) {
        // Ensure any TX is stopped, then go back to model list
        predator_subghz_stop_attack(app);
        scene_manager_previous_scene(app->scene_manager);
        return true;
    }

    if(event.type == SceneManagerEventTypeCustom) {
        switch(event.event) {
            case 1: // Rolling Code
                predator_subghz_stop_attack(app);
                predator_subghz_start_rolling_code_attack(app, app->selected_model_freq);
                return true;
            case 2: // Fixed Code Bruteforce
                predator_subghz_stop_attack(app);
                predator_subghz_start_car_bruteforce(app, app->selected_model_freq);
                return true;
            case 3: // Smart Key / Proximity demo (use Tesla charge port as proof)
                predator_subghz_stop_attack(app);
                predator_subghz_send_tesla_charge_port(app);
                return true;
            case 4: // Live Monitor
                scene_manager_next_scene(app->scene_manager, PredatorSceneLiveMonitorUI);
                return true;
            default:
                break;
        }
    }

    return false;
}

void predator_scene_car_model_attacks_ui_on_exit(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    predator_subghz_stop_attack(app);
}
