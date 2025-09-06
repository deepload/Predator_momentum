#include "../predator_i.h"
#include "../helpers/predator_esp32.h"
#include "../helpers/predator_gps.h"

static void predator_scene_wardriving_popup_callback(void* context) {
    PredatorApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, PredatorCustomEventPopupBack);
}

void predator_scene_wardriving_on_enter(void* context) {
    PredatorApp* app = context;
    
    // Initialize both ESP32 and GPS
    predator_esp32_init(app);
    predator_gps_init(app);
    
    popup_set_header(app->popup, "Wardriving Mode", 64, 10, AlignCenter, AlignTop);
    popup_set_text(app->popup, 
        "Collecting WiFi + GPS data...\n"
        "Networks: 0\n"
        "GPS: Searching...\n"
        "Press Back to stop", 
        64, 25, AlignCenter, AlignTop);
    popup_set_callback(app->popup, predator_scene_wardriving_popup_callback);
    popup_set_context(app->popup, app);
    popup_set_timeout(app->popup, 0);
    popup_enable_timeout(app->popup);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
    
    // Start wardriving with Marauder
    predator_esp32_send_command(app, MARAUDER_CMD_WARDRIVE);
    app->attack_running = true;
    app->targets_found = 0;
}

bool predator_scene_wardriving_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == PredatorCustomEventPopupBack) {
            consumed = true;
            scene_manager_previous_scene(app->scene_manager);
        }
    } else if(event.type == SceneManagerEventTypeTick) {
        if(app->attack_running) {
            predator_gps_update(app);
            app->targets_found++;
            
            char status_text[128];
            if(app->gps_connected) {
                snprintf(status_text, sizeof(status_text), 
                    "Collecting WiFi + GPS data...\n"
                    "Networks: %lu\n"
                    "GPS: %.6f, %.6f\n"
                    "Press Back to stop", 
                    app->targets_found, (double)app->latitude, (double)app->longitude);
            } else {
                snprintf(status_text, sizeof(status_text), 
                    "Collecting WiFi + GPS data...\n"
                    "Networks: %lu\n"
                    "GPS: Searching...\n"
                    "Press Back to stop", 
                    app->targets_found);
            }
            popup_set_text(app->popup, status_text, 64, 25, AlignCenter, AlignTop);
        }
    }

    return consumed;
}

void predator_scene_wardriving_on_exit(void* context) {
    PredatorApp* app = context;
    app->attack_running = false;
    predator_esp32_send_command(app, MARAUDER_CMD_STOP);
    predator_esp32_deinit(app);
    predator_gps_deinit(app);
    popup_reset(app->popup);
}
