#include "../predator_i.h"
#include "../helpers/predator_esp32.h"

static void predator_scene_ble_spam_popup_callback(void* context) {
    PredatorApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, PredatorCustomEventPopupBack);
}

void predator_scene_ble_spam_on_enter(void* context) {
    PredatorApp* app = context;
    
    predator_esp32_init(app);
    
    popup_set_header(app->popup, "BLE Spam Attack", 64, 10, AlignCenter, AlignTop);
    popup_set_text(app->popup, 
        "Spamming BLE advertisements...\n"
        "Fake devices: 0\n"
        "Target: iOS/Android\n"
        "Press Back to stop", 
        64, 25, AlignCenter, AlignTop);
    popup_set_callback(app->popup, predator_scene_ble_spam_popup_callback);
    popup_set_context(app->popup, app);
    popup_set_timeout(app->popup, 0);
    popup_enable_timeout(app->popup);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
    
    predator_esp32_send_command(app, MARAUDER_CMD_BLE_SPAM);
    app->attack_running = true;
    app->packets_sent = 0;
}

bool predator_scene_ble_spam_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == PredatorCustomEventPopupBack) {
            consumed = true;
            scene_manager_previous_scene(app->scene_manager);
        }
    } else if(event.type == SceneManagerEventTypeTick) {
        if(app->attack_running) {
            app->packets_sent += 5;
            char status_text[128];
            snprintf(status_text, sizeof(status_text), 
                "Spamming BLE advertisements...\n"
                "Fake devices: %lu\n"
                "Target: iOS/Android\n"
                "Press Back to stop", 
                app->packets_sent);
            popup_set_text(app->popup, status_text, 64, 25, AlignCenter, AlignTop);
        }
    }

    return consumed;
}

void predator_scene_ble_spam_on_exit(void* context) {
    PredatorApp* app = context;
    app->attack_running = false;
    predator_esp32_send_command(app, MARAUDER_CMD_STOP);
    predator_esp32_deinit(app);
    popup_reset(app->popup);
}
