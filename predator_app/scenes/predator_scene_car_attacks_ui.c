#include "../predator_i.h"
#include "predator_scene.h"
#include "predator_submenu_index.h"

// Car Attacks Submenu - Professional UI for Tesla Testing
static void car_attacks_submenu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    if(!app || !app->view_dispatcher) return;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void predator_scene_car_attacks_ui_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app || !app->submenu) return;
    
    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "🚗 Car Attacks");
    
    submenu_add_item(app->submenu, "🏆 ULTIMATE AUTO TEST", SubmenuIndexCarTestResults, car_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "🚶‍♂️ WALKING OPEN (ELON)", SubmenuIndexWalkingOpen, car_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "🚗 Tesla Charge Port", SubmenuIndexCarTesla, car_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "🚙 Car Models", SubmenuIndexCarModels, car_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "🔑 Key Bruteforce", SubmenuIndexCarKeyBruteforce, car_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "📡 Car Jamming", SubmenuIndexCarJamming, car_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "🔓 Passive Opener", SubmenuIndexCarPassiveOpener, car_attacks_submenu_callback, app);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

bool predator_scene_car_attacks_ui_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(event.type == SceneManagerEventTypeCustom) {
        consumed = true;
        switch(event.event) {
        case SubmenuIndexCarTestResults:
            scene_manager_next_scene(app->scene_manager, PredatorSceneCarTestResults);
            break;
        case SubmenuIndexWalkingOpen:
            scene_manager_next_scene(app->scene_manager, PredatorSceneWalkingOpenUI);
            break;
        case SubmenuIndexCarTesla:
            scene_manager_next_scene(app->scene_manager, PredatorSceneCarTeslaUI);
            break;
        case SubmenuIndexCarModels:
            scene_manager_next_scene(app->scene_manager, PredatorSceneCarModelSelector);
            break;
        case SubmenuIndexCarKeyBruteforce:
            scene_manager_next_scene(app->scene_manager, PredatorSceneCarKeyBruteforceUI);
            break;
        case SubmenuIndexCarJamming:
            scene_manager_next_scene(app->scene_manager, PredatorSceneCarJammingUI);
            break;
        case SubmenuIndexCarPassiveOpener:
            scene_manager_next_scene(app->scene_manager, PredatorSceneCarPassiveOpenerUI);
            break;
        default:
            consumed = false;
            break;
        }
    }
    
    return consumed;
}

void predator_scene_car_attacks_ui_on_exit(void* context) {
    PredatorApp* app = context;
    if(app && app->submenu) {
        submenu_reset(app->submenu);
    }
}
