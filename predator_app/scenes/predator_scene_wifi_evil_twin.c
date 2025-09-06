#include "../predator_i.h"
#include "../helpers/predator_esp32.h"

static void predator_scene_wifi_evil_twin_popup_callback(void* context) {
    PredatorApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, PredatorCustomEventPopupBack);
}

void predator_scene_wifi_evil_twin_on_enter(void* context) {
    PredatorApp* app = context;
    
    predator_esp32_init(app);
    
    popup_set_header(app->popup, "Evil Twin AP", 64, 10, AlignCenter, AlignTop);
    popup_set_text(app->popup, 
        "Creating fake access point...\n"
        "SSID: Free_WiFi_Login\n"
        "Clients: 0\n"
        "Press Back to stop", 
        64, 25, AlignCenter, AlignTop);
    popup_set_callback(app->popup, predator_scene_wifi_evil_twin_popup_callback);
    popup_set_context(app->popup, app);
    popup_set_timeout(app->popup, 0);
    popup_enable_timeout(app->popup);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
    
    predator_esp32_send_command(app, MARAUDER_CMD_WIFI_EVIL_TWIN);
    app->attack_running = true;
    app->targets_found = 0;
}

bool predator_scene_wifi_evil_twin_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == PredatorCustomEventPopupBack) {
            consumed = true;
            scene_manager_previous_scene(app->scene_manager);
        }
    } else if(event.type == SceneManagerEventTypeTick) {
        if(app->attack_running) {
            if(furi_get_tick() % 30 == 0) app->targets_found++; // Simulate clients connecting
            char status_text[128];
            snprintf(status_text, sizeof(status_text), 
                "Creating fake access point...\n"
                "SSID: Free_WiFi_Login\n"
                "Clients: %lu\n"
                "Press Back to stop", 
                app->targets_found);
            popup_set_text(app->popup, status_text, 64, 25, AlignCenter, AlignTop);
        }
    }

    return consumed;
}

void predator_scene_wifi_evil_twin_on_exit(void* context) {
    PredatorApp* app = context;
    app->attack_running = false;
    predator_esp32_send_command(app, MARAUDER_CMD_STOP);
    predator_esp32_deinit(app);
    popup_reset(app->popup);
}
