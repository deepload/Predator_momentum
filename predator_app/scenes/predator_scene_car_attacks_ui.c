#include "../predator_i.h"
#include "predator_scene.h"
#include "predator_submenu_index.h"
#include "../helpers/predator_real_attack_engine.h"
#include "../helpers/predator_logging.h"

// Car Attacks Submenu - Professional UI for Tesla Testing
static void car_attacks_submenu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    if(!app || !app->view_dispatcher) return;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void predator_scene_car_attacks_ui_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app || !app->submenu) return;
    
    // Initialize Real Attack Engine (critical for Elon's requirements)
    if(!predator_real_attack_init(app)) {
        FURI_LOG_E("CarAttacks", "CRITICAL: Real Attack Engine initialization failed");
    }
    
    // Activate VIP mode for Tesla demonstrations
    app->region = PredatorRegionUnblock;
    app->vip_mode = true;
    app->authorized = true;
    
    predator_log_append(app, "CAR ATTACKS: Real attack engine activated");
    predator_log_append(app, "TESLA MODE: Ready for security demonstrations");
    
    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "🚗 Car Attacks - REAL POWER");
    
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
