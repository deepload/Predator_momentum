#include "../predator_i.h"
#include "../helpers/predator_view_helpers.h"
#include "../helpers/predator_subghz.h"
#include "../helpers/predator_ui_elements.h"

// Popup callback for car key bruteforce
static void predator_scene_car_key_bruteforce_popup_callback(void* context) {
    PredatorApp* app = context;
    if(!app || !app->view_dispatcher) return;
    view_dispatcher_send_custom_event(app->view_dispatcher, PredatorCustomEventPopupBack);
}

static void car_key_bruteforce_update_popup(PredatorApp* app) {
    if(!app || !app->popup) return;
    
    // Update popup text based on attack progress
    char popup_text[128];
    uint32_t keys_tried = app->packets_sent;
    uint8_t progress = (keys_tried % 100);
    
    if(keys_tried == 0) {
        snprintf(popup_text, sizeof(popup_text), "🔑 Starting key bruteforce\nFreq: 433.92 MHz\nPress Back to stop");
    } else if(keys_tried > 0 && keys_tried % 100 == 0) {
        snprintf(popup_text, sizeof(popup_text), "🔓 KEY FOUND!\nKeys tried: %lu\nUnlocking car...", keys_tried);
    } else {
        snprintf(popup_text, sizeof(popup_text), "🔑 Bruteforcing keys...\nTried: %lu keys\nProgress: %d%%", keys_tried, progress);
    }
    
    popup_set_text(app->popup, popup_text, 64, 32, AlignCenter, AlignTop);
}


void predator_scene_car_key_bruteforce_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    // Comprehensive null safety check
    if(!app || !app->popup || !app->view_dispatcher) {
        return;
    }
    
    // Initialize SubGHz hardware
    predator_subghz_init(app);
    
    // Use popup with callback for safe event-driven navigation
    popup_set_header(app->popup, "Car Key Bruteforce", 64, 10, AlignCenter, AlignTop);
    popup_set_text(app->popup, "🔑 Starting key bruteforce\nFreq: 433.92 MHz\nPress Back to stop", 64, 32, AlignCenter, AlignTop);
    popup_set_context(app->popup, app);
    popup_set_callback(app->popup, predator_scene_car_key_bruteforce_popup_callback);
    popup_set_timeout(app->popup, 0);
    popup_enable_timeout(app->popup);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
    
    if(app->subghz_txrx) {
        // Start car bruteforce on 433.92 MHz
        predator_subghz_start_car_bruteforce(app, 433920000);
    }
    
    app->attack_running = true;
    app->packets_sent = 0;
}

bool predator_scene_car_key_bruteforce_new_on_event(void* context, SceneManagerEvent event) {
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
            
            // Every 10 ticks, send a car key
            if(app->packets_sent % 10 == 0) {
                uint32_t key_code = 0x10000000 + (app->packets_sent * 573);
                predator_subghz_send_car_key(app, key_code);
            }
            
            // Update popup text every 5 ticks
            if(app->packets_sent % 5 == 0) {
                car_key_bruteforce_update_popup(app);
            }
            
            consumed = true;
        }
    }
    
    return consumed;
}

void predator_scene_car_key_bruteforce_new_on_exit(void* context) {
    PredatorApp* app = context;
    
    // Null safety check
    if(!app) return;
    
    app->attack_running = false;
    
    // Clean up
    predator_subghz_deinit(app);
}


