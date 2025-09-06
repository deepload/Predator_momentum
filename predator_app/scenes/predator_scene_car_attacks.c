#include "../predator_i.h"
#include "predator_scene.h"

enum SubmenuIndex {
    SubmenuIndexCarModels,
    SubmenuIndexCarPassiveOpener,
    SubmenuIndexCarKeyBruteforce,
    SubmenuIndexCarJamming,
    SubmenuIndexCarTesla,
    SubmenuIndexCarRollingCode,
    SubmenuIndexCarTireMonitor,
};

void predator_scene_car_attacks_submenu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void predator_scene_car_attacks_on_enter(void* context) {
    PredatorApp* app = context;
    Submenu* submenu = app->submenu;

    submenu_add_item(
        submenu, "🚙 All Car Models", SubmenuIndexCarModels, predator_scene_car_attacks_submenu_callback, app);
    submenu_add_item(
        submenu, "👜 Passive Bag Mode", SubmenuIndexCarPassiveOpener, predator_scene_car_attacks_submenu_callback, app);
    submenu_add_item(
        submenu, "🔑 Key Bruteforce", SubmenuIndexCarKeyBruteforce, predator_scene_car_attacks_submenu_callback, app);
    submenu_add_item(
        submenu, "📡 Signal Jamming", SubmenuIndexCarJamming, predator_scene_car_attacks_submenu_callback, app);
    submenu_add_item(
        submenu, "🚗 Tesla Charge Port", SubmenuIndexCarTesla, predator_scene_car_attacks_submenu_callback, app);
    submenu_add_item(
        submenu, "🔄 Rolling Code Attack", SubmenuIndexCarRollingCode, predator_scene_car_attacks_submenu_callback, app);
    submenu_add_item(
        submenu, "🛞 Tire Monitor Spoof", SubmenuIndexCarTireMonitor, predator_scene_car_attacks_submenu_callback, app);

    submenu_set_selected_item(
        submenu, scene_manager_get_scene_state(app->scene_manager, PredatorSceneCarAttacks));

    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

bool predator_scene_car_attacks_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        scene_manager_set_scene_state(app->scene_manager, PredatorSceneCarAttacks, event.event);
        consumed = true;
        switch(event.event) {
        case SubmenuIndexCarModels:
            scene_manager_next_scene(app->scene_manager, PredatorSceneCarModels);
            break;
        case SubmenuIndexCarPassiveOpener:
            scene_manager_next_scene(app->scene_manager, PredatorSceneCarPassiveOpener);
            break;
        case SubmenuIndexCarKeyBruteforce:
            scene_manager_next_scene(app->scene_manager, PredatorSceneCarKeyBruteforce);
            break;
        case SubmenuIndexCarJamming:
            scene_manager_next_scene(app->scene_manager, PredatorSceneCarJamming);
            break;
        case SubmenuIndexCarTesla:
            scene_manager_next_scene(app->scene_manager, PredatorSceneCarTesla);
            break;
        default:
            break;
        }
    }

    return consumed;
}

void predator_scene_car_attacks_on_exit(void* context) {
    PredatorApp* app = context;
    submenu_reset(app->submenu);
}
