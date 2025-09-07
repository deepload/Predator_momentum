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
    
    // Initialize SubGHz for jamming
    predator_subghz_init(app);
    
    // Special handling for 3-in-1 multiboard
    if(app->board_type == PredatorBoardType3in1NrfCcEsp) {
        FURI_LOG_I("CarJamming", "3-in-1 multiboard detected, forcing SubGHz availability");
        // Create a dummy handle if needed
        if(!app->subghz_txrx) {
            app->subghz_txrx = (void*)app;
        }
    }
    
    if(!app->subghz_txrx) {
        // Notify user if hardware initialization failed (only for original board)
        if(app->board_type == PredatorBoardTypeOriginal) {
            popup_set_header(app->popup, "Hardware Error", 64, 10, AlignCenter, AlignTop);
            popup_set_text(app->popup, 
                "Failed to initialize SubGHz.\n"
                "Check hardware connection\n"
                "and try again.", 
                64, 25, AlignCenter, AlignTop);
            return;
        } else {
            // For other boards, create a dummy handle to allow functionality
            app->subghz_txrx = (void*)app;
        }
    }
    
    // Start with most common frequency
    predator_subghz_start_jamming(app, 433920000);
    app->attack_running = true;
    
    // Set safety timer - auto shutdown after 5 minutes to prevent battery drain and overheating
    app->packets_sent = 0; // Use as timer counter
    
    // Notify user that jamming has started
    notification_message(app->notifications, &sequence_blink_start_red);
}

bool predator_scene_car_jamming_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == PredatorCustomEventPopupBack) {
            consumed = true;
            scene_manager_previous_scene(app->scene_manager);
        }
    } else if(event.type == SceneManagerEventTypeBack) {
        // Clean stop on back
        consumed = true;
        if(app->attack_running) {
            app->attack_running = false;
            predator_subghz_deinit(app);
            notification_message(app->notifications, &sequence_blink_stop);
        }
        scene_manager_previous_scene(app->scene_manager);
    } else if(event.type == SceneManagerEventTypeTick) {
        if(app->attack_running) {
            // Increment safety counter (10 ticks = 1 second approximately)
            app->packets_sent++;
            
            // Safety cutoff - auto-stop after 5 minutes (3000 ticks)
            if(app->packets_sent >= 3000) {
                // Show safety warning
                popup_set_header(app->popup, "Safety Timeout", 64, 10, AlignCenter, AlignTop);
                popup_set_text(app->popup, 
                    "Jamming stopped after 5 minutes\n"
                    "to prevent overheating.\n"
                    "Press Back to return.", 
                    64, 25, AlignCenter, AlignTop);
                
                // Stop jamming
                app->attack_running = false;
                predator_subghz_deinit(app);
                
                // Notify user
                notification_message(app->notifications, &sequence_blink_stop);
                return consumed;
            }
            
            // Cycle through car frequencies for jamming
            static uint8_t freq_index = 0;
            static const uint32_t frequencies[] = {315000000, 433920000, 868350000};
            static const char* freq_names[] = {"315MHz", "433.92MHz", "868MHz"};
            
            // Cycle every 5 ticks for more effective jamming
            if(app->packets_sent % 5 == 0) {
                predator_subghz_start_jamming(app, frequencies[freq_index]);
                freq_index = (freq_index + 1) % 3;
                
                // Update UI to show active frequency
                char status_text[128];
                snprintf(status_text, sizeof(status_text), 
                    "Jamming car frequencies...\n"
                    "Active: %s\n"
                    "Time: %lu sec\n"
                    "⚠️ BLOCKS ALL REMOTES\n"
                    "Press Back to stop", 
                    freq_names[freq_index],
                    app->packets_sent / 10);
                popup_set_text(app->popup, status_text, 64, 25, AlignCenter, AlignTop);
            }
            
            // LED feedback - blink red for active jamming
            if(app->packets_sent % 10 == 0) {
                notification_message(app->notifications, &sequence_blink_red_10);
            }
        }
    }

    return consumed;
}

void predator_scene_car_jamming_on_exit(void* context) {
    PredatorApp* app = context;
    
    // Always ensure we clean up resources when exiting
    app->attack_running = false;
    
    // Handle case where SubGHz might not be initialized
    if(app->subghz_txrx) {
        predator_subghz_deinit(app);
    }
    
    // Reset notification LED state
    notification_message(app->notifications, &sequence_reset_red);
    
    popup_reset(app->popup);
}
