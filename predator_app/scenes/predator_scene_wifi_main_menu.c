#include "../predator_i.h"
#include "predator_scene.h"
#include "predator_submenu_index.h"
#include "../helpers/predator_workflow_validator.h"

// WIFI SPECIALIST MAIN MENU - ONLY WIFI FEATURES
// Professional UI for WiFi security testing

static void wifi_main_menu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    if(!app || !app->view_dispatcher) return;
    
    // Professional workflow validation
    if(!predator_workflow_validate_submenu_callback(app, index)) {
        return;
    }
    
    predator_workflow_add_breadcrumb(app, "WiFi Menu Selection");
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void predator_scene_wifi_main_menu_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app || !app->submenu) return;
    
    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "📶 WIFI SPECIALIST");
    
    // 📶 WIFI ATTACK TOOLS
    submenu_add_item(app->submenu, "📶 WiFi Attacks", SubmenuIndexWiFiAttacks, wifi_main_menu_callback, app);
    submenu_add_item(app->submenu, "🔍 WiFi Scanner", SubmenuIndexWiFiScan, wifi_main_menu_callback, app);
    submenu_add_item(app->submenu, "💥 WiFi Deauth", SubmenuIndexWiFiDeauth, wifi_main_menu_callback, app);
    submenu_add_item(app->submenu, "🎭 WiFi Evil Twin", SubmenuIndexWiFiEvilTwin, wifi_main_menu_callback, app);
    
    // 🔧 WIFI SYSTEM TOOLS
    submenu_add_item(app->submenu, "📊 Module Status", SubmenuIndexModuleStatus, wifi_main_menu_callback, app);
    submenu_add_item(app->submenu, "🔧 Board Selection", SubmenuIndexBoardSelection, wifi_main_menu_callback, app);
    submenu_add_item(app->submenu, "⚙️ Settings", SubmenuIndexSettings, wifi_main_menu_callback, app);
    submenu_add_item(app->submenu, "📖 About", SubmenuIndexAbout, wifi_main_menu_callback, app);
    
    // 🔙 RETURN TO MASTER
    submenu_add_item(app->submenu, "🔙 Return to Master", SubmenuIndexReturnToMaster, wifi_main_menu_callback, app);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

bool predator_scene_wifi_main_menu_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(event.type == SceneManagerEventTypeCustom) {
        switch(event.event) {
            case SubmenuIndexWiFiAttacks:
                scene_manager_next_scene(app->scene_manager, PredatorSceneWiFiAttacksUI);
                consumed = true;
                break;
            case SubmenuIndexWiFiScan:
                scene_manager_next_scene(app->scene_manager, PredatorSceneWiFiScanUI);
                consumed = true;
                break;
            case SubmenuIndexWiFiDeauth:
                scene_manager_next_scene(app->scene_manager, PredatorSceneWiFiDeauthUI);
                consumed = true;
                break;
            case SubmenuIndexWiFiEvilTwin:
                scene_manager_next_scene(app->scene_manager, PredatorSceneWiFiEvilTwinUI);
                consumed = true;
                break;
            case SubmenuIndexModuleStatus:
                scene_manager_next_scene(app->scene_manager, PredatorSceneModuleStatusUI);
                consumed = true;
                break;
            case SubmenuIndexBoardSelection:
                scene_manager_next_scene(app->scene_manager, PredatorSceneBoardSelectionUI);
                consumed = true;
                break;
            case SubmenuIndexSettings:
                scene_manager_next_scene(app->scene_manager, PredatorSceneSettingsUI);
                consumed = true;
                break;
            case SubmenuIndexAbout:
                scene_manager_next_scene(app->scene_manager, PredatorSceneAboutUI);
                consumed = true;
                break;
            case SubmenuIndexReturnToMaster:
                // TODO: Implement IPC call to return to master orchestrator
                consumed = true;
                break;
        }
    }
    
    return consumed;
}

void predator_scene_wifi_main_menu_on_exit(void* context) {
    PredatorApp* app = context;
    if(!app || !app->submenu) return;
    submenu_reset(app->submenu);
}
