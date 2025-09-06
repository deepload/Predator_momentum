#include "../predator_i.h"
#include "predator_scene.h"

enum SubmenuIndex {
    SubmenuIndexWifiAttacks,
    SubmenuIndexBluetoothAttacks,
    SubmenuIndexSubghzAttacks,
    SubmenuIndexCarAttacks,
    SubmenuIndexRfidAttacks,
    SubmenuIndexGpsTracker,
    SubmenuIndexWardriving,
    SubmenuIndexSocialEngineering,
    SubmenuIndexSettings,
    SubmenuIndexAbout,
};

void predator_scene_start_submenu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void predator_scene_start_on_enter(void* context) {
    PredatorApp* app = context;
    Submenu* submenu = app->submenu;

    submenu_add_item(
        submenu, "📡 WiFi Attacks", SubmenuIndexWifiAttacks, predator_scene_start_submenu_callback, app);
    submenu_add_item(
        submenu, "📱 Bluetooth Attacks", SubmenuIndexBluetoothAttacks, predator_scene_start_submenu_callback, app);
    submenu_add_item(
        submenu, "📻 SubGHz/RF Attacks", SubmenuIndexSubghzAttacks, predator_scene_start_submenu_callback, app);
    submenu_add_item(
        submenu, "🚗 Car Attacks", SubmenuIndexCarAttacks, predator_scene_start_submenu_callback, app);
    submenu_add_item(
        submenu, "💳 RFID/NFC Attacks", SubmenuIndexRfidAttacks, predator_scene_start_submenu_callback, app);
    submenu_add_item(
        submenu, "🛰️ GPS Tracker", SubmenuIndexGpsTracker, predator_scene_start_submenu_callback, app);
    submenu_add_item(
        submenu, "🗺️ Wardriving", SubmenuIndexWardriving, predator_scene_start_submenu_callback, app);
    submenu_add_item(
        submenu, "🎭 Social Engineering", SubmenuIndexSocialEngineering, predator_scene_start_submenu_callback, app);
    submenu_add_item(
        submenu, "⚙️ Settings", SubmenuIndexSettings, predator_scene_start_submenu_callback, app);
    submenu_add_item(
        submenu, "ℹ️ About", SubmenuIndexAbout, predator_scene_start_submenu_callback, app);

    submenu_set_selected_item(
        submenu, scene_manager_get_scene_state(app->scene_manager, PredatorSceneStart));

    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

bool predator_scene_start_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        scene_manager_set_scene_state(app->scene_manager, PredatorSceneStart, event.event);
        consumed = true;
        switch(event.event) {
        case SubmenuIndexWifiAttacks:
            scene_manager_next_scene(app->scene_manager, PredatorSceneWifiAttacks);
            break;
        case SubmenuIndexBluetoothAttacks:
            scene_manager_next_scene(app->scene_manager, PredatorSceneBluetoothAttacks);
            break;
        case SubmenuIndexSubghzAttacks:
            scene_manager_next_scene(app->scene_manager, PredatorSceneSubghzAttacks);
            break;
        case SubmenuIndexCarAttacks:
            scene_manager_next_scene(app->scene_manager, PredatorSceneCarAttacks);
            break;
        case SubmenuIndexRfidAttacks:
            scene_manager_next_scene(app->scene_manager, PredatorSceneRfidAttacks);
            break;
        case SubmenuIndexGpsTracker:
            scene_manager_next_scene(app->scene_manager, PredatorSceneGpsTracker);
            break;
        case SubmenuIndexWardriving:
            scene_manager_next_scene(app->scene_manager, PredatorSceneWardriving);
            break;
        case SubmenuIndexSocialEngineering:
            scene_manager_next_scene(app->scene_manager, PredatorSceneSocialEngineering);
            break;
        case SubmenuIndexSettings:
            scene_manager_next_scene(app->scene_manager, PredatorSceneSettings);
            break;
        case SubmenuIndexAbout:
            scene_manager_next_scene(app->scene_manager, PredatorSceneAbout);
            break;
        default:
            break;
        }
    }

    return consumed;
}

void predator_scene_start_on_exit(void* context) {
    PredatorApp* app = context;
    submenu_reset(app->submenu);
}
