#include "../predator_i.h"
#include "predator_scene.h"
#include "predator_submenu_index.h"
#include "../helpers/predator_ui_elements.h"
#include <furi.h>

// Submenu callback for navigation
static void wifi_attacks_submenu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void predator_scene_wifi_attacks_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    if(!app) {
        FURI_LOG_E("WifiAttacks", "App context is NULL on enter");
        return;
    }
    
    // Ensure scene_manager and view_dispatcher are valid to prevent crashes
    if(!app->scene_manager) {
        FURI_LOG_E("WifiAttacks", "Scene manager is NULL, cannot proceed");
        return;
    }
    
    if(!app->view_dispatcher) {
        FURI_LOG_E("WifiAttacks", "View dispatcher is NULL, cannot switch view");
        return;
    }
    
    // Set up submenu for WiFi Attacks
    submenu_reset(app->submenu);
    submenu_add_item(app->submenu, "📡 WiFi Scanner", 0, wifi_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "💥 Deauth Attack", 1, wifi_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "👥 Evil Twin AP", 2, wifi_attacks_submenu_callback, app);
    submenu_set_header(app->submenu, "WiFi Attacks");
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

bool predator_scene_wifi_attacks_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(!app) {
        FURI_LOG_E("WifiAttacks", "App context is NULL in event handler");
        return false;
    }
    
    if(event.type == SceneManagerEventTypeCustom) {
        consumed = true;
        switch(event.event) {
        case 0: 
            scene_manager_next_scene(app->scene_manager, PredatorSceneWifiScan);
            break;
        case 1: 
            scene_manager_next_scene(app->scene_manager, PredatorSceneWifiDeauth);
            break;
        case 2: 
            scene_manager_next_scene(app->scene_manager, PredatorSceneWifiEvilTwin);
            break;
        default:
            consumed = false;
            break;
        }
    } else if(event.type == SceneManagerEventTypeBack) {
        scene_manager_previous_scene(app->scene_manager);
        consumed = true;
    }
    
    return consumed;
}

void predator_scene_wifi_attacks_new_on_exit(void* context) {
    PredatorApp* app = context;
    
    // Clean up submenu
    submenu_reset(app->submenu);
}
