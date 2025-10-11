#include "../predator_i.h"
#include "predator_scene.h"
#include "predator_submenu_index.h"
#include "../helpers/predator_workflow_validator.h"

// MASTER ORCHESTRATOR MAIN MENU - ONLY ORCHESTRATION FEATURES
// Professional UI for controlling specialist apps

static void master_main_menu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    if(!app || !app->view_dispatcher) return;
    
    // Professional workflow validation
    if(!predator_workflow_validate_submenu_callback(app, index)) {
        return;
    }
    
    predator_workflow_add_breadcrumb(app, "Master Menu Selection");
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void predator_scene_master_main_menu_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app || !app->submenu) return;
    
    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "🏆 MASTER ORCHESTRATOR");
    
    // 🎛️ ORCHESTRATION CONTROLS
    submenu_add_item(app->submenu, "🎛️ Master Control", SubmenuIndexMasterOrchestrator, master_main_menu_callback, app);
    submenu_add_item(app->submenu, "📱 App Launcher", SubmenuIndexAppLauncher, master_main_menu_callback, app);
    submenu_add_item(app->submenu, "📊 System Monitor", SubmenuIndexSystemMonitor, master_main_menu_callback, app);
    submenu_add_item(app->submenu, "📊 App Status", SubmenuIndexAppStatus, master_main_menu_callback, app);
    
    // 🚀 SPECIALIST APP LAUNCHERS
    submenu_add_item(app->submenu, "📶 Launch WiFi Specialist", SubmenuIndexLaunchWiFi, master_main_menu_callback, app);
    submenu_add_item(app->submenu, "🚗 Launch Car Specialist", SubmenuIndexLaunchCar, master_main_menu_callback, app);
    submenu_add_item(app->submenu, "⚡ Launch Tesla VIP", SubmenuIndexLaunchTesla, master_main_menu_callback, app);
    submenu_add_item(app->submenu, "🔵 Launch Bluetooth", SubmenuIndexLaunchBluetooth, master_main_menu_callback, app);
    
    // ⚙️ SYSTEM MANAGEMENT
    submenu_add_item(app->submenu, "⚙️ Module Status", SubmenuIndexModuleStatus, master_main_menu_callback, app);
    submenu_add_item(app->submenu, "🔧 Board Selection", SubmenuIndexBoardSelection, master_main_menu_callback, app);
    submenu_add_item(app->submenu, "⚙️ Settings", SubmenuIndexSettings, master_main_menu_callback, app);
    submenu_add_item(app->submenu, "📖 About", SubmenuIndexAbout, master_main_menu_callback, app);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

bool predator_scene_master_main_menu_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(event.type == SceneManagerEventTypeCustom) {
        switch(event.event) {
            case SubmenuIndexMasterOrchestrator:
                scene_manager_next_scene(app->scene_manager, PredatorSceneMasterOrchestratorSimple);
                consumed = true;
                break;
            case SubmenuIndexAppLauncher:
                scene_manager_next_scene(app->scene_manager, PredatorSceneAppLauncherUI);
                consumed = true;
                break;
            case SubmenuIndexSystemMonitor:
                scene_manager_next_scene(app->scene_manager, PredatorSceneLiveMonitorUI);
                consumed = true;
                break;
            case SubmenuIndexAppStatus:
                scene_manager_next_scene(app->scene_manager, PredatorSceneAppStatus);
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
            // APP LAUNCHERS - IPC calls to launch specialist apps
            case SubmenuIndexLaunchWiFi:
                // TODO: Implement IPC call to launch WiFi specialist
                consumed = true;
                break;
            case SubmenuIndexLaunchCar:
                // TODO: Implement IPC call to launch Car specialist
                consumed = true;
                break;
            case SubmenuIndexLaunchTesla:
                // TODO: Implement IPC call to launch Tesla VIP
                consumed = true;
                break;
            case SubmenuIndexLaunchBluetooth:
                // TODO: Implement IPC call to launch Bluetooth specialist
                consumed = true;
                break;
        }
    }
    
    return consumed;
}

void predator_scene_master_main_menu_on_exit(void* context) {
    PredatorApp* app = context;
    if(!app || !app->submenu) return;
    submenu_reset(app->submenu);
}
