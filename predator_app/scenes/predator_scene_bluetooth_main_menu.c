#include "../predator_i.h"
#include "predator_scene.h"
#include "predator_submenu_index.h"
#include "../helpers/predator_workflow_validator.h"

// BLUETOOTH SPECIALIST MAIN MENU - ONLY BLUETOOTH FEATURES
// Professional UI for Bluetooth security testing

static void bluetooth_main_menu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    if(!app || !app->view_dispatcher) return;
    
    predator_workflow_add_breadcrumb(app, "Bluetooth Menu Selection");
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void predator_scene_bluetooth_main_menu_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app || !app->submenu) return;
    
    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "🔵 BLUETOOTH SPECIALIST");
    
    // 🔵 BLUETOOTH ATTACK TOOLS
    submenu_add_item(app->submenu, "🔵 Bluetooth Attacks", SubmenuIndexBluetoothAttacks, bluetooth_main_menu_callback, app);
    submenu_add_item(app->submenu, "📡 BLE Scanner", SubmenuIndexBLEScan, bluetooth_main_menu_callback, app);
    submenu_add_item(app->submenu, "💥 BLE Spam", SubmenuIndexBLESpam, bluetooth_main_menu_callback, app);
    
    // 🔧 BLUETOOTH SYSTEM TOOLS
    submenu_add_item(app->submenu, "📊 Module Status", SubmenuIndexModuleStatus, bluetooth_main_menu_callback, app);
    submenu_add_item(app->submenu, "🔧 Board Selection", SubmenuIndexBoardSelection, bluetooth_main_menu_callback, app);
    submenu_add_item(app->submenu, "⚙️ Settings", SubmenuIndexSettings, bluetooth_main_menu_callback, app);
    submenu_add_item(app->submenu, "📖 About", SubmenuIndexAbout, bluetooth_main_menu_callback, app);
    
    // 🔙 RETURN TO MASTER
    submenu_add_item(app->submenu, "🔙 Return to Master", SubmenuIndexReturnToMaster, bluetooth_main_menu_callback, app);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

bool predator_scene_bluetooth_main_menu_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(event.type == SceneManagerEventTypeCustom) {
        switch(event.event) {
            case SubmenuIndexBluetoothAttacks:
                scene_manager_next_scene(app->scene_manager, PredatorSceneBluetoothAttacksUI);
                consumed = true;
                break;
            case SubmenuIndexBLEScan:
                scene_manager_next_scene(app->scene_manager, PredatorSceneBLEScanUI);
                consumed = true;
                break;
            case SubmenuIndexBLESpam:
                scene_manager_next_scene(app->scene_manager, PredatorSceneBLESpamUI);
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
                // IPC call to return to master orchestrator
                consumed = true;
                break;
        }
    }
    
    return consumed;
}

void predator_scene_bluetooth_main_menu_on_exit(void* context) {
    PredatorApp* app = context;
    if(!app || !app->submenu) return;
    submenu_reset(app->submenu);
}
