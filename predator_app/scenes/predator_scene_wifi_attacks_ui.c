#include "../predator_i.h"
#include "predator_scene.h"
#include "predator_submenu_index.h"

// WiFi Attacks Submenu - Professional UI
static void wifi_attacks_submenu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    if(!app || !app->view_dispatcher) return;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void predator_scene_wifi_attacks_ui_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app || !app->submenu) return;
    
    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "📡 WiFi Attacks");
    
    submenu_add_item(app->submenu, "📡 WiFi Scan", SubmenuIndexWifiScan, wifi_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "💥 WiFi Deauth", SubmenuIndexWifiDeauth, wifi_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "👹 Evil Twin", SubmenuIndexWifiEvilTwin, wifi_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "🤝 Handshake", SubmenuIndexWifiHandshake, wifi_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "🔑 PMKID", SubmenuIndexWifiPmkid, wifi_attacks_submenu_callback, app);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

bool predator_scene_wifi_attacks_ui_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(event.type == SceneManagerEventTypeCustom) {
        consumed = true;
        switch(event.event) {
        case SubmenuIndexWifiScan:
            scene_manager_next_scene(app->scene_manager, PredatorSceneWifiScanUI);
            break;
        case SubmenuIndexWifiDeauth:
            scene_manager_next_scene(app->scene_manager, PredatorSceneWifiDeauthUI);
            break;
        case SubmenuIndexWifiEvilTwin:
            scene_manager_next_scene(app->scene_manager, PredatorSceneWifiEvilTwinUI);
            break;
        case SubmenuIndexWifiHandshake:
            scene_manager_next_scene(app->scene_manager, PredatorSceneWifiHandshakeUI);
            break;
        case SubmenuIndexWifiPmkid:
            scene_manager_next_scene(app->scene_manager, PredatorSceneWifiPmkidUI);
            break;
        default:
            consumed = false;
            break;
        }
    }
    
    return consumed;
}

void predator_scene_wifi_attacks_ui_on_exit(void* context) {
    PredatorApp* app = context;
    if(app && app->submenu) {
        submenu_reset(app->submenu);
    }
}
