#include "../predator_i.h"
#include "../helpers/predator_view_helpers.h"
#include "../helpers/predator_subghz.h"
#include "../helpers/predator_ui_elements.h"
#include "predator_scene.h"

// Popup callback for passive opener
static void predator_scene_car_passive_opener_popup_callback(void* context) {
    PredatorApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, PredatorCustomEventPopupBack);
}


void predator_scene_car_passive_opener_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    // Initialize SubGHz hardware
    predator_subghz_init(app);
    
    // Use popup for passive opener attack
    popup_set_header(app->popup, "Passive Car Opener", 64, 10, AlignCenter, AlignTop);
    popup_set_text(app->popup, "Transmitting passive\nkey fob signals...\nPress Back to stop", 64, 25, AlignCenter, AlignTop);
    popup_set_context(app->popup, app);
    popup_set_callback(app->popup, predator_scene_car_passive_opener_popup_callback);
    popup_set_timeout(app->popup, 0);
    popup_enable_timeout(app->popup);
    
    // Start the passive opener attack
    predator_subghz_passive_opener_attack(app);
    app->attack_running = true;
    app->packets_sent = 0;
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
}

bool predator_scene_car_passive_opener_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == PredatorCustomEventPopupBack) {
            // Stop attack and return to previous scene
            app->attack_running = false;
            predator_subghz_stop_attack(app);
            scene_manager_previous_scene(app->scene_manager);
            consumed = true;
        }
    } else if(event.type == SceneManagerEventTypeTick) {
        if(app->attack_running) {
            // Update attack progress every 10 ticks
            if(app->packets_sent % 10 == 0) {
                char attack_text[64];
                snprintf(attack_text, sizeof(attack_text), "Transmitting signals...\nCars attempted: %lu\nPress Back to stop", app->packets_sent / 10);
                popup_set_text(app->popup, attack_text, 64, 25, AlignCenter, AlignTop);
                
                // Continue the passive opener attack
                predator_subghz_passive_opener_attack(app);
            }
            
            app->packets_sent++;
            consumed = true;
        }
    }
    
    return consumed;
}

void predator_scene_car_passive_opener_new_on_exit(void* context) {
    PredatorApp* app = context;
    
    // Stop any running attack
    app->attack_running = false;
    predator_subghz_stop_attack(app);
    
    // Clean up popup
    popup_reset(app->popup);
}


