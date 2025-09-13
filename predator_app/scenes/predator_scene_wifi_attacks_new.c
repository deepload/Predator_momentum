#include "../predator_i.h"
#include "predator_scene.h"
#include "predator_submenu_index.h"
#include "../helpers/predator_ui_elements.h"
#include <furi.h>

// Submenu callback for navigation
static void wifi_attacks_submenu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    if(!app || !app->view_dispatcher) return;
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
    
    // Validate board type before any hardware initialization
    if(app->board_type == PredatorBoardTypeUnknown) {
        FURI_LOG_W("WifiAttacks", "Board type is Unknown, defaulting to Original");
        app->board_type = PredatorBoardTypeOriginal;
    }
    
    // Set up submenu for WiFi Attacks
    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "WiFi Attacks");

    // Add submenu items for WiFi attack scenes
    submenu_add_item(app->submenu, "WiFi Scan", 1, wifi_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "WiFi Deauth", 2, wifi_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "Evil Twin", 3, wifi_attacks_submenu_callback, app);
    submenu_set_selected_item(app->submenu, 0);
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

bool predator_scene_wifi_attacks_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(!app) {
        FURI_LOG_E("WifiAttacks", "App context is NULL in event handler");
        return false;
    }
    
    if(event.type == SceneManagerEventTypeBack) {
        scene_manager_previous_scene(app->scene_manager);
        consumed = true;
    } else if(event.type == SceneManagerEventTypeCustom) {
        consumed = true;
        switch(event.event) {
        case 1: // WiFi Scan
            scene_manager_next_scene(app->scene_manager, PredatorSceneWifiScan);
            break;
        case 2: // WiFi Deauth
            scene_manager_next_scene(app->scene_manager, PredatorSceneWifiDeauth);
            break;
        case 3: // Evil Twin
            scene_manager_next_scene(app->scene_manager, PredatorSceneWifiEvilTwin);
            break;
        default:
            consumed = false;
            break;
        }
    }
    
    return consumed;
}

void predator_scene_wifi_attacks_new_on_exit(void* context) {
    PredatorApp* app = context;
    
    // Clean up submenu
    submenu_reset(app->submenu);
}
