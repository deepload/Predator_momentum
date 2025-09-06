#include "../predator_i.h"
#include "../helpers/predator_subghz.h"

static void predator_scene_car_key_bruteforce_popup_callback(void* context) {
    PredatorApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, PredatorCustomEventPopupBack);
}

void predator_scene_car_key_bruteforce_on_enter(void* context) {
    PredatorApp* app = context;
    
    popup_set_header(app->popup, "Car Key Bruteforce", 64, 10, AlignCenter, AlignTop);
    popup_set_text(app->popup, 
        "Bruteforcing car keys...\n"
        "Frequency: 433.92 MHz\n"
        "Keys tried: 0\n"
        "Press Back to stop", 
        64, 25, AlignCenter, AlignTop);
    popup_set_callback(app->popup, predator_scene_car_key_bruteforce_popup_callback);
    popup_set_context(app->popup, app);
    popup_set_timeout(app->popup, 0);
    popup_enable_timeout(app->popup);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
    
    // Initialize SubGHz for car key attacks
    predator_subghz_init(app);
    predator_subghz_start_car_bruteforce(app, 433920000); // 433.92 MHz
    
    app->attack_running = true;
    app->packets_sent = 0;
}

bool predator_scene_car_key_bruteforce_on_event(void* context, SceneManagerEvent event) {
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
            snprintf(status_text, sizeof(status_text), 
                "Bruteforcing car keys...\n"
                "Frequency: 433.92 MHz\n"
                "Keys tried: %lu\n"
                "Press Back to stop", 
                app->packets_sent);
            popup_set_text(app->popup, status_text, 64, 25, AlignCenter, AlignTop);
            
            // Send next key code
            predator_subghz_send_car_key(app, app->packets_sent);
        }
    }

    return consumed;
}

void predator_scene_car_key_bruteforce_on_exit(void* context) {
    PredatorApp* app = context;
    app->attack_running = false;
    predator_subghz_deinit(app);
    popup_reset(app->popup);
}
