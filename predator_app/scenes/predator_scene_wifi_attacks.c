#include "../predator_i.h"
#include "predator_scene.h"

enum SubmenuIndex {
    SubmenuIndexWifiScan,
    SubmenuIndexWifiDeauth,
    SubmenuIndexWifiEvilTwin,
    SubmenuIndexWifiHandshakeCapture,
    SubmenuIndexWifiPwnagotchi,
};

void predator_scene_wifi_attacks_submenu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void predator_scene_wifi_attacks_on_enter(void* context) {
    PredatorApp* app = context;
    Submenu* submenu = app->submenu;

    // DEMO MODE: Display only options that won't crash
    submenu_add_item(
        submenu, "📡 WiFi Scanner (Demo)", SubmenuIndexWifiScan, predator_scene_wifi_attacks_submenu_callback, app);
    submenu_add_item(
        submenu, "💥 Deauth Attack (Demo)", SubmenuIndexWifiDeauth, predator_scene_wifi_attacks_submenu_callback, app);
    submenu_add_item(
        submenu, "👥 Evil Twin AP (Demo)", SubmenuIndexWifiEvilTwin, predator_scene_wifi_attacks_submenu_callback, app);
    
    // Set default selection to first item
    submenu_set_selected_item(submenu, 0);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

bool predator_scene_wifi_attacks_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        scene_manager_set_scene_state(app->scene_manager, PredatorSceneWifiAttacks, event.event);
        consumed = true;
        switch(event.event) {
        case SubmenuIndexWifiScan:
            scene_manager_next_scene(app->scene_manager, PredatorSceneWifiScan);
            break;
        case SubmenuIndexWifiDeauth:
            scene_manager_next_scene(app->scene_manager, PredatorSceneWifiDeauth);
            break;
        case SubmenuIndexWifiEvilTwin:
            scene_manager_next_scene(app->scene_manager, PredatorSceneWifiEvilTwin);
            break;
        default:
            break;
        }
    }

    return consumed;
}

void predator_scene_wifi_attacks_on_exit(void* context) {
    PredatorApp* app = context;
    submenu_reset(app->submenu);
}
