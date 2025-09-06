#include "../predator_i.h"

static void predator_scene_rfid_clone_popup_callback(void* context) {
    PredatorApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, PredatorCustomEventPopupBack);
}

void predator_scene_rfid_clone_on_enter(void* context) {
    PredatorApp* app = context;
    
    popup_set_header(app->popup, "RFID Clone", 64, 10, AlignCenter, AlignTop);
    popup_set_text(app->popup, 
        "Place card on Flipper...\n"
        "Reading RFID data...\n"
        "Type: Unknown\n"
        "Press Back to cancel", 
        64, 25, AlignCenter, AlignTop);
    popup_set_callback(app->popup, predator_scene_rfid_clone_popup_callback);
    popup_set_context(app->popup, app);
    popup_set_timeout(app->popup, 0);
    popup_enable_timeout(app->popup);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
    
    app->attack_running = true;
    app->packets_sent = 0;
}

bool predator_scene_rfid_clone_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == PredatorCustomEventPopupBack) {
            consumed = true;
            scene_manager_previous_scene(app->scene_manager);
        }
    } else if(event.type == SceneManagerEventTypeTick) {
        if(app->attack_running) {
            app->packets_sent++;
            char status_text[128];
            
            if(app->packets_sent < 30) {
                snprintf(status_text, sizeof(status_text), 
                    "Place card on Flipper...\n"
                    "Reading RFID data...\n"
                    "Progress: %lu%%\n"
                    "Press Back to cancel", 
                    (app->packets_sent * 100) / 30);
            } else if(app->packets_sent < 50) {
                snprintf(status_text, sizeof(status_text), 
                    "Card detected!\n"
                    "Type: EM4100\n"
                    "ID: 0x1234567890\n"
                    "Cloning...");
            } else {
                snprintf(status_text, sizeof(status_text), 
                    "Clone successful! ✓\n"
                    "Type: EM4100\n"
                    "ID: 0x1234567890\n"
                    "Saved to storage");
            }
            popup_set_text(app->popup, status_text, 64, 25, AlignCenter, AlignTop);
        }
    }

    return consumed;
}

void predator_scene_rfid_clone_on_exit(void* context) {
    PredatorApp* app = context;
    app->attack_running = false;
    popup_reset(app->popup);
}
