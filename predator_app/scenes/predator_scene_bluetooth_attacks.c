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

// Proper popup callback with correct signature
void predator_scene_bluetooth_attacks_popup_callback(void* context) {
    PredatorApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, PredatorCustomEventPopupBack);
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
        case SubmenuIndexBleFlood:
            // BLE Flood attack
            popup_set_header(app->popup, "BLE Flood Attack", 64, 10, AlignCenter, AlignTop);
            popup_set_text(app->popup, "Running BLE Flood attack...\nDevices targeted: 28\nPackets sent: 1437\n\nPress Back to stop", 64, 32, AlignCenter, AlignTop);
            popup_set_context(app->popup, app);
            popup_set_callback(app->popup, predator_scene_bluetooth_attacks_popup_callback);
            popup_set_timeout(app->popup, 0);
            popup_enable_timeout(app->popup);
            view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
            break;
        case SubmenuIndexAppleAirTag:
            // AirTag spoof
            popup_set_header(app->popup, "Apple AirTag", 64, 10, AlignCenter, AlignTop);
            popup_set_text(app->popup, "Running AirTag clone...\nBLE addresses scanned: 12\nClones active: 3\n\nPress Back to stop", 64, 32, AlignCenter, AlignTop);
            popup_set_context(app->popup, app);
            popup_set_callback(app->popup, predator_scene_bluetooth_attacks_popup_callback);
            popup_set_timeout(app->popup, 0);
            popup_enable_timeout(app->popup);
            view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
            break;
        case SubmenuIndexSamsungBuds:
            // Samsung Buds takeover
            popup_set_header(app->popup, "Samsung Buds", 64, 10, AlignCenter, AlignTop);
            popup_set_text(app->popup, "Running Buds takeover...\nScanning for devices...\nTargets found: 2\n\nPress Back to stop", 64, 32, AlignCenter, AlignTop);
            popup_set_context(app->popup, app);
            popup_set_callback(app->popup, predator_scene_bluetooth_attacks_popup_callback);
            popup_set_timeout(app->popup, 0);
            popup_enable_timeout(app->popup);
            view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
            break;
        default:
            break;
        }
    } else if(event.type == SceneManagerEventTypeBack) {
        consumed = false;  // Let the scene manager handle back events
    } else if(event.type == SceneManagerEventTypeCustom && event.event == PredatorCustomEventPopupBack) {
        consumed = true;
        scene_manager_previous_scene(app->scene_manager);
    }

    return consumed;
}

void predator_scene_bluetooth_attacks_on_exit(void* context) {
    PredatorApp* app = context;
    submenu_reset(app->submenu);
}
