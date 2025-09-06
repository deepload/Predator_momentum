#include "../predator_i.h"
#include "predator_scene.h"

enum SubmenuIndex {
    SubmenuIndexCaptivePortal,
    SubmenuIndexPhishingAP,
    SubmenuIndexFakeUpdate,
    SubmenuIndexQRCodeGen,
    SubmenuIndexUSBRubberDucky,
};

void predator_scene_social_engineering_submenu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void predator_scene_social_engineering_on_enter(void* context) {
    PredatorApp* app = context;
    Submenu* submenu = app->submenu;

    submenu_add_item(
        submenu, "🌐 Captive Portal", SubmenuIndexCaptivePortal, predator_scene_social_engineering_submenu_callback, app);
    submenu_add_item(
        submenu, "📡 Phishing AP", SubmenuIndexPhishingAP, predator_scene_social_engineering_submenu_callback, app);
    submenu_add_item(
        submenu, "💻 Fake Update Screen", SubmenuIndexFakeUpdate, predator_scene_social_engineering_submenu_callback, app);
    submenu_add_item(
        submenu, "📱 QR Code Generator", SubmenuIndexQRCodeGen, predator_scene_social_engineering_submenu_callback, app);
    submenu_add_item(
        submenu, "🦆 USB Rubber Ducky", SubmenuIndexUSBRubberDucky, predator_scene_social_engineering_submenu_callback, app);

    submenu_set_selected_item(
        submenu, scene_manager_get_scene_state(app->scene_manager, PredatorSceneSocialEngineering));

    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

bool predator_scene_social_engineering_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        scene_manager_set_scene_state(app->scene_manager, PredatorSceneSocialEngineering, event.event);
        consumed = true;
        switch(event.event) {
        case SubmenuIndexCaptivePortal:
            // Launch captive portal attack
            popup_set_header(app->popup, "Captive Portal", 64, 10, AlignCenter, AlignTop);
            popup_set_text(app->popup, "Starting captive portal...\nTarget: Free WiFi\nPress Back to stop", 64, 25, AlignCenter, AlignTop);
            view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
            break;
        default:
            break;
        }
    }

    return consumed;
}

void predator_scene_social_engineering_on_exit(void* context) {
    PredatorApp* app = context;
    submenu_reset(app->submenu);
}
