#include "../predator_i.h"
#include "predator_scene.h"
#include "predator_submenu_index.h"
#include "../helpers/predator_full_detection.h"

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
    
    submenu_add_item(app->submenu, "📡 WiFi Scan", 1, wifi_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "💥 WiFi Deauth", 2, wifi_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "👹 Evil Twin", 3, wifi_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "🤝 Handshake", 4, wifi_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "🔑 PMKID", 5, wifi_attacks_submenu_callback, app);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

bool predator_scene_wifi_attacks_ui_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    // Handle back button - return to main menu
    if(event.type == SceneManagerEventTypeBack) {
        // Return false to let scene manager navigate back
        return false;
    }
    
    if(event.type == SceneManagerEventTypeCustom) {
        consumed = true;
        switch(event.event) {
        case 1: // WiFi Scan
            // PROFESSIONAL: Check hardware requirements before proceeding
            if(predator_full_detection_show_requirements_popup(app, PredatorAttackWiFi)) {
                scene_manager_next_scene(app->scene_manager, PredatorSceneWifiScanUI);
            }
            return true;
        case 2: // WiFi Deauth
            if(predator_full_detection_show_requirements_popup(app, PredatorAttackWiFi)) {
                scene_manager_next_scene(app->scene_manager, PredatorSceneWifiDeauthUI);
            }
            return true;
        // case 3: // Evil Twin - REMOVED: Memory optimization
        //     if(predator_full_detection_show_requirements_popup(app, PredatorAttackWiFi)) {
        //         scene_manager_next_scene(app->scene_manager, PredatorSceneWifiEvilTwinUI);
        //     }
        //     return true;
        // case 4: // Handshake - REMOVED: Memory optimization
        //     if(predator_full_detection_show_requirements_popup(app, PredatorAttackWiFi)) {
        //         scene_manager_next_scene(app->scene_manager, PredatorSceneWifiHandshakeUI);
        //     }
        //     return true;
        // case 5: // PMKID - REMOVED: Memory optimization
        //     if(predator_full_detection_show_requirements_popup(app, PredatorAttackWiFi)) {
        //         scene_manager_next_scene(app->scene_manager, PredatorSceneWifiPmkidUI);
        //     }
        //     return true;
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
