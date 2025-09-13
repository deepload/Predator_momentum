#include "../predator_i.h"
#include "../helpers/predator_view_helpers.h"
#include "../helpers/predator_ui_elements.h"
#include "predator_scene.h"

// Social Engineering menu items (submenu-based)
typedef enum {
    SocialItemCaptivePortal,
    SocialItemPhishingAP,
    SocialItemFakeUpdate,
    SocialItemQRCodeGen,
    SocialItemUSBRubberDucky,
    SocialItemCount,
} SocialItem;

// Submenu callback: forward index as custom event
static void predator_scene_social_engineering_new_submenu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void predator_scene_social_engineering_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    if(!app) {
        return;
    }
    
    if(!app->scene_manager) {
        return;
    }
    
    if(!app->view_dispatcher) {
        return;
    }
    
    Submenu* submenu = app->submenu;
    submenu_reset(submenu);
    submenu_set_header(submenu, "Social Engineering");
    submenu_add_item(submenu, "Captive Portal", SocialItemCaptivePortal, predator_scene_social_engineering_new_submenu_callback, app);
    submenu_add_item(submenu, "Phishing AP", SocialItemPhishingAP, predator_scene_social_engineering_new_submenu_callback, app);
    submenu_add_item(submenu, "Fake Update", SocialItemFakeUpdate, predator_scene_social_engineering_new_submenu_callback, app);
    submenu_add_item(submenu, "QR Code", SocialItemQRCodeGen, predator_scene_social_engineering_new_submenu_callback, app);
    submenu_add_item(submenu, "USB Rubber Ducky", SocialItemUSBRubberDucky, predator_scene_social_engineering_new_submenu_callback, app);

    submenu_set_selected_item(submenu, 0);
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

bool predator_scene_social_engineering_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        const char* title = NULL;
        switch(event.event) {
        case SocialItemCaptivePortal: title = "Captive Portal"; break;
        case SocialItemPhishingAP:    title = "Phishing AP"; break;
        case SocialItemFakeUpdate:    title = "Fake Update"; break;
        case SocialItemQRCodeGen:     title = "QR Code"; break;
        case SocialItemUSBRubberDucky:title = "USB Rubber Ducky"; break;
        default:                      title = "Action"; break;
        }

        popup_set_header(app->popup, title, 64, 10, AlignCenter, AlignTop);
        popup_set_text(app->popup, "Starting...\nPress Back to stop", 64, 25, AlignCenter, AlignTop);
        popup_set_context(app->popup, app);
        popup_set_timeout(app->popup, 0);
        popup_enable_timeout(app->popup);
        app->attack_running = true;
        view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
        consumed = true;
    } else if(event.type == SceneManagerEventTypeBack) {
        app->attack_running = false;
        scene_manager_previous_scene(app->scene_manager);
        consumed = true;
    } else if(event.type == SceneManagerEventTypeTick) {
        if(app->attack_running) {
            app->packets_sent += 1;
            if(app->packets_sent >= 20) {
                popup_set_text(app->popup, "Operation active\nPress Back to stop", 64, 25, AlignCenter, AlignTop);
                app->packets_sent = 0;
            }
            consumed = true;
        }
    }
    return consumed;
}

void predator_scene_social_engineering_new_on_exit(void* context) {
    PredatorApp* app = context;
    if(app) {
        submenu_reset(app->submenu);
        app->attack_running = false;
    }
}
