#include "../predator_i.h"
#include "predator_scene.h"
#include "predator_submenu_index.h"
#include "../helpers/predator_workflow_validator.h"

// CAR SPECIALIST MAIN MENU - ONLY CAR FEATURES
// Professional UI for automotive security testing

static void car_main_menu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    if(!app || !app->view_dispatcher) return;
    
    // Professional workflow validation
    if(!predator_workflow_validate_submenu_callback(app, index)) {
        return;
    }
    
    predator_workflow_add_breadcrumb(app, "Car Menu Selection");
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void predator_scene_car_main_menu_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app || !app->submenu) return;
    
    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "🚗 CAR SPECIALIST");
    
    // 🚗 CAR ATTACK TOOLS
    submenu_add_item(app->submenu, "🚗 Car Attacks", SubmenuIndexCarAttacks, car_main_menu_callback, app);
    submenu_add_item(app->submenu, "⚡ Tesla Security", SubmenuIndexCarTesla, car_main_menu_callback, app);
    submenu_add_item(app->submenu, "🔑 Key Bruteforce", SubmenuIndexCarKeyBruteforce, car_main_menu_callback, app);
    submenu_add_item(app->submenu, "📡 Car Jamming", SubmenuIndexCarJamming, car_main_menu_callback, app);
    submenu_add_item(app->submenu, "🚪 Passive Opener", SubmenuIndexCarPassiveOpener, car_main_menu_callback, app);
    submenu_add_item(app->submenu, "🚗 Model Selector", SubmenuIndexCarModelSelector, car_main_menu_callback, app);
    
    // 🔧 CAR SYSTEM TOOLS
    submenu_add_item(app->submenu, "📊 Module Status", SubmenuIndexModuleStatus, car_main_menu_callback, app);
    submenu_add_item(app->submenu, "🔧 Board Selection", SubmenuIndexBoardSelection, car_main_menu_callback, app);
    submenu_add_item(app->submenu, "⚙️ Settings", SubmenuIndexSettings, car_main_menu_callback, app);
    submenu_add_item(app->submenu, "📖 About", SubmenuIndexAbout, car_main_menu_callback, app);
    
    // 🔙 RETURN TO MASTER
    submenu_add_item(app->submenu, "🔙 Return to Master", SubmenuIndexReturnToMaster, car_main_menu_callback, app);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

bool predator_scene_car_main_menu_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(event.type == SceneManagerEventTypeCustom) {
        switch(event.event) {
            case SubmenuIndexCarAttacks:
                scene_manager_next_scene(app->scene_manager, PredatorSceneCarAttacksUI);
                consumed = true;
                break;
            case SubmenuIndexCarTesla:
                scene_manager_next_scene(app->scene_manager, PredatorSceneCarTeslaUI);
                consumed = true;
                break;
            case SubmenuIndexCarKeyBruteforce:
                scene_manager_next_scene(app->scene_manager, PredatorSceneCarKeyBruteforceUI);
                consumed = true;
                break;
            case SubmenuIndexCarJamming:
                scene_manager_next_scene(app->scene_manager, PredatorSceneCarJammingUI);
                consumed = true;
                break;
            case SubmenuIndexCarPassiveOpener:
                scene_manager_next_scene(app->scene_manager, PredatorSceneCarPassiveOpenerUI);
                consumed = true;
                break;
            case SubmenuIndexCarModelSelector:
                scene_manager_next_scene(app->scene_manager, PredatorSceneCarModelSelector);
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

void predator_scene_car_main_menu_on_exit(void* context) {
    PredatorApp* app = context;
    if(!app || !app->submenu) return;
    submenu_reset(app->submenu);
}
