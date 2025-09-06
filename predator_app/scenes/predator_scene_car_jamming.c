#include "../predator_i.h"
#include "../helpers/predator_subghz.h"

static void predator_scene_car_jamming_popup_callback(void* context) {
    PredatorApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, PredatorCustomEventPopupBack);
}

void predator_scene_car_jamming_on_enter(void* context) {
    PredatorApp* app = context;
    
    popup_set_header(app->popup, "Car Signal Jamming", 64, 10, AlignCenter, AlignTop);
    popup_set_text(app->popup, 
        "Jamming car frequencies...\n"
        "315MHz + 433MHz + 868MHz\n"
        "⚠️ BLOCKS ALL REMOTES\n"
        "Press Back to stop", 
        64, 25, AlignCenter, AlignTop);
    popup_set_callback(app->popup, predator_scene_car_jamming_popup_callback);
    popup_set_context(app->popup, app);
    popup_set_timeout(app->popup, 0);
    popup_enable_timeout(app->popup);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
    
    predator_subghz_init(app);
    predator_subghz_start_jamming(app, 433920000);
    app->attack_running = true;
}

bool predator_scene_car_jamming_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == PredatorCustomEventPopupBack) {
            consumed = true;
            scene_manager_previous_scene(app->scene_manager);
        }
    } else if(event.type == SceneManagerEventTypeTick) {
        if(app->attack_running) {
            // Cycle through car frequencies for jamming
            static uint8_t freq_index = 0;
            static const uint32_t frequencies[] = {315000000, 433920000, 868350000};
            
            predator_subghz_start_jamming(app, frequencies[freq_index]);
            freq_index = (freq_index + 1) % 3;
        }
    }

    return consumed;
}

void predator_scene_car_jamming_on_exit(void* context) {
    PredatorApp* app = context;
    app->attack_running = false;
    predator_subghz_deinit(app);
    popup_reset(app->popup);
}
