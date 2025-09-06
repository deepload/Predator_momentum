#include "../predator_i.h"
#include "predator_scene.h"

enum SubmenuIndex {
    SubmenuIndexBleScan,
    SubmenuIndexBleSpam,
    SubmenuIndexBleFlood,
    SubmenuIndexAppleAirTag,
    SubmenuIndexSamsungBuds,
};

void predator_scene_bluetooth_attacks_submenu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void predator_scene_bluetooth_attacks_on_enter(void* context) {
    PredatorApp* app = context;
    Submenu* submenu = app->submenu;

    submenu_add_item(
        submenu, "📱 BLE Scanner", SubmenuIndexBleScan, predator_scene_bluetooth_attacks_submenu_callback, app);
    submenu_add_item(
        submenu, "💥 BLE Spam", SubmenuIndexBleSpam, predator_scene_bluetooth_attacks_submenu_callback, app);
    submenu_add_item(
        submenu, "🌊 BLE Flood", SubmenuIndexBleFlood, predator_scene_bluetooth_attacks_submenu_callback, app);
    submenu_add_item(
        submenu, "🏷️ Apple AirTag Spoof", SubmenuIndexAppleAirTag, predator_scene_bluetooth_attacks_submenu_callback, app);
    submenu_add_item(
        submenu, "🎧 Samsung Buds Takeover", SubmenuIndexSamsungBuds, predator_scene_bluetooth_attacks_submenu_callback, app);

    submenu_set_selected_item(
        submenu, scene_manager_get_scene_state(app->scene_manager, PredatorSceneBluetoothAttacks));

    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

bool predator_scene_bluetooth_attacks_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        scene_manager_set_scene_state(app->scene_manager, PredatorSceneBluetoothAttacks, event.event);
        consumed = true;
        switch(event.event) {
        case SubmenuIndexBleScan:
            scene_manager_next_scene(app->scene_manager, PredatorSceneBleScan);
            break;
        case SubmenuIndexBleSpam:
            scene_manager_next_scene(app->scene_manager, PredatorSceneBleSpam);
            break;
        default:
            break;
        }
    }

    return consumed;
}

void predator_scene_bluetooth_attacks_on_exit(void* context) {
    PredatorApp* app = context;
    submenu_reset(app->submenu);
}
