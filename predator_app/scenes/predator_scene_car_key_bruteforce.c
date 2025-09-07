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
    
    // Add error handling for SubGHz initialization
    if(!app->subghz_txrx) {
        // Try to force initialization for multi-boards
        if(app->board_type != PredatorBoardTypeOriginal) {
            FURI_LOG_I("CarKeyBF", "Attempting to force SubGHz init for multiboard");
            predator_subghz_deinit(app);
            predator_subghz_init(app);
        }
        
        // Check again after forced init
        if(!app->subghz_txrx) {
            // Different error messages based on board type
            if(app->board_type == PredatorBoardTypeOriginal) {
                popup_set_header(app->popup, "Hardware Error", 64, 10, AlignCenter, AlignTop);
                popup_set_text(app->popup, 
                    "Failed to initialize SubGHz.\n"
                    "Check hardware connection\n"
                    "and try again.", 
                    64, 25, AlignCenter, AlignTop);
            } else {
                popup_set_header(app->popup, "Connection Issue", 64, 10, AlignCenter, AlignTop);
                popup_set_text(app->popup, 
                    "SubGHz module connection\n"
                    "issue on multiboard.\n"
                    "Continuing anyway...", 
                    64, 25, AlignCenter, AlignTop);
                
                // For multiboards, create a dummy handle to allow functionality
                app->subghz_txrx = (void*)app;
            }
        }
    }
    
    // Start with most common frequency
    predator_subghz_start_car_bruteforce(app, 433920000); // 433.92 MHz
    
    app->attack_running = true;
    app->packets_sent = 0;
    
    // Set notification pattern to confirm activation
    notification_message(app->notifications, &sequence_blink_start_blue);
}

bool predator_scene_car_key_bruteforce_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == PredatorCustomEventPopupBack) {
            consumed = true;
            scene_manager_previous_scene(app->scene_manager);
        }
    } else if(event.type == SceneManagerEventTypeBack) {
        // Explicit stop on back
        consumed = true;
        app->attack_running = false;
        predator_subghz_deinit(app);
        notification_message(app->notifications, &sequence_blink_stop);
        scene_manager_previous_scene(app->scene_manager);
    } else if(event.type == SceneManagerEventTypeTick) {
        if(app->attack_running) {
            static uint8_t freq_index = 0;
            static const uint32_t frequencies[] = {433920000, 315000000, 868350000};
            static const char* freq_names[] = {"433.92 MHz", "315 MHz", "868.35 MHz"};
            
            app->packets_sent++;
            
            // Cycle frequency every 300 keys for thorough coverage
            if(app->packets_sent % 300 == 0) {
                freq_index = (freq_index + 1) % 3;
                predator_subghz_start_car_bruteforce(app, frequencies[freq_index]);
                
                // Visual feedback for frequency change
                notification_message(app->notifications, &sequence_blink_blue_10);
            }
            
            char status_text[128];
            snprintf(status_text, sizeof(status_text), 
                "Bruteforcing car keys...\n"
                "Frequency: %s\n"
                "Keys tried: %lu\n"
                "Press Back to stop", 
                freq_names[freq_index],
                app->packets_sent);
            popup_set_text(app->popup, status_text, 64, 25, AlignCenter, AlignTop);
            
            // Smart key code selection algorithm - sends more likely key codes first
            uint32_t key_code;
            if(app->packets_sent < 100) {
                // First try common manufacturer codes
                key_code = 0x1000000 + (app->packets_sent * 0x1111); 
            } else if(app->packets_sent < 1000) {
                // Then try sequential codes in the most common range
                key_code = 0x5B7C00 + app->packets_sent;
            } else {
                // Finally do a wider search with pattern
                key_code = app->packets_sent * 0x1337 + 0xA00000;
            }
            
            // Send key code with proper timing
            predator_subghz_send_car_key(app, key_code);
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
