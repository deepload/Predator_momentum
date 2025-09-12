#include "../predator_i.h"
#include "../helpers/predator_view_helpers.h"
#include "../helpers/predator_subghz.h"
#include "../helpers/predator_ui_elements.h"

// Popup callback for car jamming
static void predator_scene_car_jamming_popup_callback(void* context) {
    PredatorApp* app = context;
    if(!app || !app->view_dispatcher) return;
    view_dispatcher_send_custom_event(app->view_dispatcher, PredatorCustomEventPopupBack);
}

static void car_jamming_update_popup(PredatorApp* app) {
    if(!app || !app->popup) return;
    
    // Update popup text based on jamming progress
    char popup_text[128];
    uint32_t jamming_time = app->packets_sent / 10;
    uint8_t strength = (jamming_time / 5) % 4;
    bool hardware_ready = (app->subghz_txrx != NULL);
    
    if(!hardware_ready) {
        snprintf(popup_text, sizeof(popup_text), "❌ Hardware Error\nSubGHz not available\nCheck hardware");
    } else if(jamming_time == 0) {
        snprintf(popup_text, sizeof(popup_text), "📡 Starting signal jamming\nFreq: 433.92 MHz\nPress Back to stop");
    } else {
        const char* power_levels[] = {"Low", "Medium", "High", "Max"};
        snprintf(popup_text, sizeof(popup_text), "📡 Jamming active\nTime: %lus\nPower: %s", jamming_time, power_levels[strength]);
    }
    
    popup_set_text(app->popup, popup_text, 64, 32, AlignCenter, AlignTop);
}


void predator_scene_car_jamming_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    // Comprehensive null safety check
    if(!app || !app->popup || !app->view_dispatcher) {
        return;
    }
    
    // Initialize SubGHz hardware
    predator_subghz_init(app);
    
    // Use popup with callback for safe event-driven navigation
    popup_set_header(app->popup, "Signal Jamming", 64, 10, AlignCenter, AlignTop);
    
    bool hardware_ready = (app->subghz_txrx != NULL);
    if(!hardware_ready) {
        popup_set_text(app->popup, "❌ Hardware Error\nSubGHz not available\nCheck hardware", 64, 32, AlignCenter, AlignTop);
    } else {
        popup_set_text(app->popup, "📡 Starting signal jamming\nFreq: 433.92 MHz\nPress Back to stop", 64, 32, AlignCenter, AlignTop);
    }
    
    popup_set_context(app->popup, app);
    popup_set_callback(app->popup, predator_scene_car_jamming_popup_callback);
    popup_set_timeout(app->popup, 0);
    popup_enable_timeout(app->popup);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
    
    // Start jamming if hardware is ready
    if(hardware_ready) {
        predator_subghz_start_jamming(app, 433920000);
    }
    
    app->attack_running = true;
    app->packets_sent = 0;
}

bool predator_scene_car_jamming_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    // Null safety check
    if(!app) return false;
    
    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == PredatorCustomEventPopupBack) {
            app->attack_running = false;
            predator_subghz_deinit(app);
            if(app->scene_manager) {
                scene_manager_previous_scene(app->scene_manager);
            }
            consumed = true;
        }
    } else if(event.type == SceneManagerEventTypeBack) {
        app->attack_running = false;
        predator_subghz_deinit(app);
        if(app->scene_manager) {
            scene_manager_previous_scene(app->scene_manager);
        }
        consumed = true;
    } else if(event.type == SceneManagerEventTypeTick) {
        if(app->attack_running) {
            app->packets_sent++;
            
            // Update popup text every 10 ticks
            if(app->packets_sent % 10 == 0) {
                car_jamming_update_popup(app);
            }
            
            consumed = true;
        }
    }
    
    return consumed;
}

void predator_scene_car_jamming_new_on_exit(void* context) {
    PredatorApp* app = context;
    
    // Null safety check
    if(!app) return;
    
    app->attack_running = false;
    
    // Clean up
    predator_subghz_deinit(app);
}


