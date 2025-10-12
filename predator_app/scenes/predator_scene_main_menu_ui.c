#include "../predator_i.h"
#include "predator_scene.h"
#include "predator_submenu_index.h"

// Main Menu - Professional UI
// Clean submenu implementation for Tesla demo

static void main_menu_submenu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    if(!app || !app->view_dispatcher) return;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void predator_scene_main_menu_ui_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app || !app->submenu) return;
    
    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "🔧 PREDATOR Security Testing");
    
    // MINIMAL WORKING MENU - Only available scenes
    submenu_add_item(app->submenu, "🚗 Tesla Security", 1, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "📡 WiFi Scan", 2, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "📡 WiFi Deauth", 3, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "📊 Module Status", 4, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "🔧 Board Selection", 5, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "ℹ️ About", 6, main_menu_submenu_callback, app);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

bool predator_scene_main_menu_ui_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(event.type == SceneManagerEventTypeCustom) {
        consumed = true;
        switch(event.event) {
        case 1: // Tesla Security
            scene_manager_next_scene(app->scene_manager, PredatorSceneCarTeslaUI);
            return true;
        case 2: // WiFi Scan
            scene_manager_next_scene(app->scene_manager, PredatorSceneWifiScanUI);
            return true;
        case 3: // WiFi Deauth
            scene_manager_next_scene(app->scene_manager, PredatorSceneWifiDeauthUI);
            return true;
        case 4: // Module Status
            scene_manager_next_scene(app->scene_manager, PredatorSceneModuleStatusUI);
            return true;
        case 5: // Board Selection
            scene_manager_next_scene(app->scene_manager, PredatorSceneBoardSelectionUI);
            return true;
        case 6: // About
            scene_manager_next_scene(app->scene_manager, PredatorSceneAboutUI);
            return true;
        default:
            consumed = false;
            break;
        }
    }
    
    return consumed;
}

void predator_scene_main_menu_ui_on_exit(void* context) {
    PredatorApp* app = context;
    if(app && app->submenu) {
        submenu_reset(app->submenu);
    }
}
