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
    
    submenu_add_item(app->submenu, "🚗 Tesla Security", 1, car_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "🚗 Universal Car Hacker", 2, car_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "🔑 Key Bruteforce", 3, car_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "📡 Car Jamming", 4, car_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "🔓 Passive Opener", 5, car_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "🚶 Walking Open", 6, car_attacks_submenu_callback, app);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

bool predator_scene_car_attacks_ui_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    // Handle back button - return to main menu
    if(event.type == SceneManagerEventTypeBack) {
        scene_manager_previous_scene(app->scene_manager);
        return true;
    }
    
    if(event.type == SceneManagerEventTypeCustom) {
        consumed = true;
        switch(event.event) {
        case 1: // Tesla Security
            scene_manager_next_scene(app->scene_manager, PredatorSceneCarTeslaUI);
            return true;
        case 2: // Universal Car Hacker
            scene_manager_next_scene(app->scene_manager, PredatorSceneUniversalCarHacker);
            return true;
        case 3: // Key Bruteforce
            scene_manager_next_scene(app->scene_manager, PredatorSceneCarKeyBruteforceUI);
            return true;
        case 4: // Car Jamming
            scene_manager_next_scene(app->scene_manager, PredatorSceneCarJammingUI);
            return true;
        case 5: // Passive Opener
            scene_manager_next_scene(app->scene_manager, PredatorSceneCarPassiveOpenerUI);
            return true;
        case 6: // Walking Open
            scene_manager_next_scene(app->scene_manager, PredatorSceneWalkingOpenUI);
            return true;
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
