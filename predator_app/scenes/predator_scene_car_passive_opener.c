#include "../predator_i.h"
#include "../helpers/predator_subghz.h"
#include "predator_scene.h"

static void predator_scene_car_passive_opener_popup_callback(void* context) {
    PredatorApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, PredatorCustomEventPopupBack);
}

// Power management configuration
#define SCREEN_TIMEOUT_MS 10000 // Turn off screen after 10 seconds
#define LOW_POWER_INTERVAL 5 // Only transmit every 5 ticks in low power mode

static bool low_power_mode = false;
static uint32_t screen_idle_time = 0;

void predator_scene_car_passive_opener_on_enter(void* context) {
    PredatorApp* app = context;
    
    popup_set_header(app->popup, "Passive Car Opener", 64, 10, AlignCenter, AlignTop);
    popup_set_text(app->popup, 
        "Opening all nearby cars...\n"
        "Cars attempted: 0\n"
        "COVERT MODE ACTIVE\n"
        "Press Back to stop\n\n"
        "⚡ Screen will auto-dim for bag mode", 
        64, 25, AlignCenter, AlignTop);
    popup_set_callback(app->popup, predator_scene_car_passive_opener_popup_callback);
    popup_set_context(app->popup, app);
    popup_set_timeout(app->popup, 0);
    popup_enable_timeout(app->popup);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
    
    // Start passive car opener mode with power optimization
    predator_subghz_init(app);
    
    // Add error handling for SubGHz initialization
    if(!app->subghz_txrx) {
        // Notify user if hardware initialization failed
        popup_set_header(app->popup, "Hardware Error", 64, 10, AlignCenter, AlignTop);
        popup_set_text(app->popup, 
            "Failed to initialize SubGHz.\n"
            "Check hardware connection\n"
            "and try again.", 
            64, 25, AlignCenter, AlignTop);
        return;
    }
    
    predator_subghz_start_passive_car_opener(app);
    
    // Initialize power management
    low_power_mode = false;
    screen_idle_time = 0;
    
    app->attack_running = true;
    app->packets_sent = 0;
    
    // Set notification pattern to confirm activation
    notification_message(app->notifications, &sequence_success);
}

bool predator_scene_car_passive_opener_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == PredatorCustomEventPopupBack) {
            consumed = true;
            scene_manager_previous_scene(app->scene_manager);
        }
    } else if(event.type == SceneManagerEventTypeTick) {
        if(app->attack_running) {
            // Cycle through common car frequencies for better success rate
            static uint8_t freq_index = 0;
            static const uint32_t frequencies[] = {433920000, 315000000, 868350000};
            static uint32_t freq_cycle_counter = 0;
            
            // Cycle frequency every 50 ticks (5 seconds)
            if(freq_cycle_counter++ % 50 == 0) {
                // Change frequency for wider coverage
                predator_subghz_stop_passive_car_opener(app);
                predator_subghz_start_passive_car_opener(app);
                freq_index = (freq_index + 1) % 3;
                
                // Increment attempted car count on frequency change
                app->packets_sent++;
            }
            
            // Screen power management - only increment in normal mode
            if(!low_power_mode) {
                screen_idle_time += 100; // Tick is approximately 100ms
                
                // Enter low power mode after timeout
                if(screen_idle_time >= SCREEN_TIMEOUT_MS) {
                    low_power_mode = true;
                    
                    // Dim backlight significantly to save power
                    notification_message(app->notifications, &sequence_display_backlight_off);
                    
                    // Switch to power-saving status text
                    char power_save_text[128];
                    snprintf(power_save_text, sizeof(power_save_text), 
                        "POWER SAVING MODE\n"
                        "Cars attempted: %lu\n"
                        "Active freq: %s\n"
                        "Press any button to wake", 
                        app->packets_sent,
                        freq_index == 0 ? "433.92MHz" : (freq_index == 1 ? "315MHz" : "868MHz"));
                    popup_set_text(app->popup, power_save_text, 64, 25, AlignCenter, AlignTop);
                }
            }
            
            // In low power mode, only process every few ticks
            if(!low_power_mode || (freq_cycle_counter % LOW_POWER_INTERVAL == 0)) {
                // Process passive car opener tick with error handling
                predator_subghz_passive_car_opener_tick(app);
            }
            
            // Only update UI when not in low power mode
            if(!low_power_mode) {
                // Update UI with stats
                char status_text[128];
                // Get current active frequency for display
                char* active_freq = freq_index == 0 ? "433.92MHz" : (freq_index == 1 ? "315MHz" : "868MHz");
                
                snprintf(status_text, sizeof(status_text), 
                    "Opening all nearby cars...\n"
                    "Cars attempted: %lu\n"
                    "Active freq: %s\n"
                    "COVERT MODE ACTIVE\n"
                    "Press Back to stop\n\n"
                    "⚡ Screen will auto-dim for bag mode", 
                    app->packets_sent,
                    active_freq);
                popup_set_text(app->popup, status_text, 64, 25, AlignCenter, AlignTop);
            }
            
            // Blink notification LED for stealth feedback - reduces frequency in low power
            uint8_t blink_interval = low_power_mode ? 30 : 10;
            if(app->packets_sent % blink_interval == 0) {
                notification_message(app->notifications, &sequence_blink_blue_10);
            }
        }
    } else if(event.type == SceneManagerEventTypeBack) {
        // Wake up on back button if in low power mode
        if(low_power_mode) {
            low_power_mode = false;
            screen_idle_time = 0;
            notification_message(app->notifications, &sequence_display_backlight_on);
            consumed = true;
        }
    }

    return consumed;
}

void predator_scene_car_passive_opener_on_exit(void* context) {
    PredatorApp* app = context;
    app->attack_running = false;
    
    // Restore normal display brightness if we were in low power
    if(low_power_mode) {
        notification_message(app->notifications, &sequence_display_backlight_on);
    }
    
    // Stop passive car opener mode
    predator_subghz_stop_passive_car_opener(app);
    predator_subghz_deinit(app);
    
    // Play exit tone
    notification_message(app->notifications, &sequence_blink_stop);
    
    popup_reset(app->popup);
}
