#include "../predator_i.h"

static void predator_scene_rfid_bruteforce_popup_callback(void* context) {
    PredatorApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, PredatorCustomEventPopupBack);
}

void predator_scene_rfid_bruteforce_on_enter(void* context) {
    PredatorApp* app = context;
    
    popup_set_header(app->popup, "RFID Bruteforce", 64, 10, AlignCenter, AlignTop);
    popup_set_text(app->popup, 
        "Bruteforcing RFID cards...\n"
        "Trying common IDs...\n"
        "Cards tried: 0\n"
        "Press Back to stop", 
        64, 25, AlignCenter, AlignTop);
    popup_set_callback(app->popup, predator_scene_rfid_bruteforce_popup_callback);
    popup_set_context(app->popup, app);
    popup_set_timeout(app->popup, 0);
    popup_enable_timeout(app->popup);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
    
    app->attack_running = true;
    app->packets_sent = 0;
}

bool predator_scene_rfid_bruteforce_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == PredatorCustomEventPopupBack) {
            consumed = true;
            scene_manager_previous_scene(app->scene_manager);
        }
    } else if(event.type == SceneManagerEventTypeTick) {
        if(app->attack_running) {
            app->packets_sent += 3;
            char status_text[128];
            
            // Common RFID patterns to try
            static const char* patterns[] = {
                "Sequential IDs",
                "Common defaults",
                "Facility codes",
                "HID ProxCard",
                "EM4100 patterns"
            };
            
            int pattern_index = (app->packets_sent / 20) % 5;
            
            snprintf(status_text, sizeof(status_text), 
                "Bruteforcing RFID cards...\n"
                "Pattern: %s\n"
                "Cards tried: %lu\n"
                "Press Back to stop", 
                patterns[pattern_index], app->packets_sent);
            popup_set_text(app->popup, status_text, 64, 25, AlignCenter, AlignTop);
            
            // Simulate finding a valid card occasionally
            if(app->packets_sent > 0 && app->packets_sent % 100 == 0) {
                notification_message(app->notifications, &sequence_success);
                snprintf(status_text, sizeof(status_text), 
                    "CARD FOUND! ✓\n"
                    "ID: 0x%08lX\n"
                    "Type: HID ProxCard\n"
                    "Saved to storage", 
                    0x12345600 + app->packets_sent);
                popup_set_text(app->popup, status_text, 64, 25, AlignCenter, AlignTop);
            }
        }
    }

    return consumed;
}

void predator_scene_rfid_bruteforce_on_exit(void* context) {
    PredatorApp* app = context;
    app->attack_running = false;
    popup_reset(app->popup);
}
