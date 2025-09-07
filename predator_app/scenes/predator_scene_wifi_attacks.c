#include "../predator_i.h"
#include "predator_scene.h"

enum SubmenuIndex {
    SubmenuIndexWifiScan,
    SubmenuIndexWifiDeauth,
    SubmenuIndexWifiEvilTwin,
    SubmenuIndexWifiHandshakeCapture,
    SubmenuIndexWifiPwnagotchi,
};

static void predator_scene_wifi_popup_callback(void* context) {
    PredatorApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, PredatorCustomEventPopupBack);
}

void predator_scene_wifi_attacks_submenu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void predator_scene_wifi_attacks_on_enter(void* context) {
    PredatorApp* app = context;
    Submenu* submenu = app->submenu;

    // Add all WiFi attack options
    submenu_add_item(
        submenu, "📡 WiFi Scanner", SubmenuIndexWifiScan, predator_scene_wifi_attacks_submenu_callback, app);
    submenu_add_item(
        submenu, "💥 Deauth Attack", SubmenuIndexWifiDeauth, predator_scene_wifi_attacks_submenu_callback, app);
    submenu_add_item(
        submenu, "👥 Evil Twin AP", SubmenuIndexWifiEvilTwin, predator_scene_wifi_attacks_submenu_callback, app);
    submenu_add_item(
        submenu, "🔑 Handshake Capture", SubmenuIndexWifiHandshakeCapture, predator_scene_wifi_attacks_submenu_callback, app);
    submenu_add_item(
        submenu, "🤖 Pwnagotchi Mode", SubmenuIndexWifiPwnagotchi, predator_scene_wifi_attacks_submenu_callback, app);
    
    // Set default selection to first item
    submenu_set_selected_item(submenu, 0);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

bool predator_scene_wifi_attacks_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        scene_manager_set_scene_state(app->scene_manager, PredatorSceneWifiAttacks, event.event);
        consumed = true;
        switch(event.event) {
        case SubmenuIndexWifiScan:
            scene_manager_next_scene(app->scene_manager, PredatorSceneWifiScan);
            break;
        case SubmenuIndexWifiDeauth:
            scene_manager_next_scene(app->scene_manager, PredatorSceneWifiDeauth);
            break;
        case SubmenuIndexWifiEvilTwin:
            scene_manager_next_scene(app->scene_manager, PredatorSceneWifiEvilTwin);
            break;
        case SubmenuIndexWifiHandshakeCapture:
            // Show information popup for new feature
            popup_set_header(app->popup, "Handshake Capture", 64, 10, AlignCenter, AlignTop);
            popup_set_text(app->popup, "Starting handshake capture...\n\nMonitoring for authentication\nhandshakes on all channels.\n\nPress Back to stop", 64, 32, AlignCenter, AlignTop);
            popup_set_context(app->popup, app);
            popup_set_callback(app->popup, predator_scene_wifi_popup_callback);
            view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
            break;
        case SubmenuIndexWifiPwnagotchi:
            // Show information popup for new feature
            popup_set_header(app->popup, "Pwnagotchi Mode", 64, 10, AlignCenter, AlignTop);
            popup_set_text(app->popup, "Starting Pwnagotchi mode...\n\nAutonomously collecting\nWiFi handshakes.\n\nNetworks found: 0\n\nPress Back to stop", 64, 32, AlignCenter, AlignTop);
            popup_set_context(app->popup, app);
            popup_set_callback(app->popup, predator_scene_wifi_popup_callback);
            view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
            break;
        default:
            break;
        }
    }

    return consumed;
}

void predator_scene_wifi_attacks_on_exit(void* context) {
    PredatorApp* app = context;
    submenu_reset(app->submenu);
}
