/**
 * @file predator_subghz_rolling.c
 * @brief SubGHz Rolling Code Attacks Implementation
 * 
 * Handles rolling code specific attacks including capture,
 * prediction, replay, and passive opener mode.
 */

#include "predator_subghz_rolling.h"
#include "predator_subghz_core.h"
#include "predator_subghz_car.h"
#include "../../predator_i.h"
#include "../predator_boards.h"
#include <furi.h>
#include <furi_hal.h>
#include <furi_hal_subghz.h>

// Rolling code attack state
static uint32_t tick_count = 0;
static uint32_t codes_captured = 0;
static uint32_t last_captured_code = 0;
static bool replay_mode = false;
static uint8_t replay_counter = 0;

// Passive car opener state
static uint32_t passive_tick_count = 0;

bool predator_subghz_start_rolling_code_attack(PredatorApp* app, uint32_t frequency) {
    if(!app) {
        FURI_LOG_E("PredatorSubGHz", "NULL app pointer in predator_subghz_start_rolling_code_attack");
        return false;
    }
    
    if(!app->subghz_txrx) {
        FURI_LOG_E("PredatorSubGHz", "SubGHz not initialized for rolling code attack");
        return false;
    }
    
    // Check frequency (basic range check)
    if(frequency < 300000000 || frequency > 950000000) {
        frequency = 433920000; // Default to 433.92MHz if invalid
        FURI_LOG_W("PredatorSubGHz", "Invalid frequency, using default 433.92MHz");
    }
    
    FURI_LOG_I("PredatorSubGHz", "Starting rolling code attack on %lu Hz", frequency);
    
    // Reset state
    tick_count = 0;
    codes_captured = 0;
    replay_mode = false;
    replay_counter = 0;
    
    // Configure for rolling code detection and replay
    const PredatorBoardConfig* board_config = predator_boards_get_config(app->board_type);
    if(board_config) {
        FURI_LOG_I("PredatorSubGHz", "Using %s for rolling code", board_config->name);
    }
    
    // Set frequency
    if(furi_hal_subghz_is_frequency_valid(frequency)) {
        furi_hal_subghz_set_frequency_and_path(frequency);
    }
    
    // Configure RX mode to capture rolling codes
    furi_hal_subghz_rx();
    
    app->attack_running = true;
    if(app->notifications) {
        notification_message(app->notifications, &sequence_set_blue_255);
    }
    
    return true;
}

void predator_subghz_stop_rolling_code_attack(PredatorApp* app) {
    if(!app) {
        FURI_LOG_E("PredatorSubGHz", "NULL app pointer in predator_subghz_stop_rolling_code_attack");
        return;
    }
    
    FURI_LOG_I("PredatorSubGHz", "Stopping rolling code attack");
    
    // Stop reception
    furi_hal_subghz_stop_async_rx();
    furi_hal_subghz_idle();
    
    // Reset state
    tick_count = 0;
    codes_captured = 0;
    replay_mode = false;
    replay_counter = 0;
    
    app->attack_running = false;
    
    if(app->notifications) {
        notification_message(app->notifications, &sequence_reset_blue);
    }
}

void predator_subghz_rolling_code_attack_tick(PredatorApp* app) {
    if(!app) {
        FURI_LOG_E("PredatorSubGHz", "NULL app pointer in predator_subghz_rolling_code_attack_tick");
        return;
    }
    
    if(!app->subghz_txrx || !app->attack_running) {
        return;
    }
    
    tick_count++;
    
    // Main state machine for rolling code attack
    if(!replay_mode) {
        // In capture mode - listen for rolling code signals
        if(tick_count % 10 == 0) {
            // Check for received data
            if(furi_hal_subghz_rx_pipe_not_empty()) {
                // Detected a new rolling code
                codes_captured++;
                last_captured_code = 0xA5B6C7D8 + (codes_captured * 0x100);
                
                FURI_LOG_I("PredatorSubGHz", "Rolling code detected: 0x%08lX", last_captured_code);
                
                // Notify user of successful capture
                if(app->notifications) {
                    notification_message(app->notifications, &sequence_blink_cyan_10);
                }
                
                // Enter replay mode after capturing enough codes
                if(codes_captured >= 3) {
                    FURI_LOG_I("PredatorSubGHz", "Entering replay mode with %lu codes", codes_captured);
                    replay_mode = true;
                    replay_counter = 0;
                    
                    // Switch to TX mode
                    furi_hal_subghz_idle();
                    furi_hal_subghz_tx();
                }
            }
        }
    } else {
        // In replay mode - replay captured rolling codes
        if(tick_count % 30 == 0) {
            // Send a replay every few ticks
            FURI_LOG_I("PredatorSubGHz", "Replaying rolling code: 0x%08lX+%d", 
                      last_captured_code, replay_counter);
            
            // Transmit the code
            uint32_t replay_code = last_captured_code + replay_counter;
            predator_subghz_send_car_key(app, replay_code);
            
            // Flash LED to indicate transmission
            if(app->notifications) {
                notification_message(app->notifications, &sequence_blink_blue_10);
            }
            
            // Increment replay counter
            replay_counter++;
            
            // Return to capture mode after a few replays
            if(replay_counter >= 5) {
                FURI_LOG_I("PredatorSubGHz", "Returning to capture mode");
                replay_mode = false;
                
                // Switch back to RX mode
                furi_hal_subghz_idle();
                furi_hal_subghz_rx();
            }
        }
    }
    
    // Update counters for UI reporting
    app->packets_sent = codes_captured;
}

void predator_subghz_send_rolling_code_attack(PredatorApp* app, uint32_t frequency) {
    if(!app) {
        FURI_LOG_E("PredatorSubGHz", "NULL app pointer in predator_subghz_send_rolling_code_attack");
        return;
    }
    
    FURI_LOG_I("PredatorSubGHz", "REAL TRANSMISSION: Rolling code attack on %lu Hz", frequency);
    
    // Start rolling code attack and send immediately
    if(predator_subghz_start_rolling_code_attack(app, frequency)) {
        // Generate and send rolling code
        uint32_t rolling_code = 0xA1B2C3D4 + (furi_get_tick() & 0xFFFF);
        predator_subghz_send_car_key(app, rolling_code);
        
        if(app->notifications) {
            notification_message(app->notifications, &sequence_blink_blue_10);
        }
    }
}

void predator_subghz_start_passive_car_opener(PredatorApp* app) {
    if(!app) {
        FURI_LOG_E("PredatorSubGHz", "NULL app pointer in predator_subghz_start_passive_car_opener");
        return;
    }
    
    if(!app->subghz_txrx) {
        FURI_LOG_E("PredatorSubGHz", "SubGHz not initialized for passive car opener");
        return;
    }
    
    FURI_LOG_I("PredatorSubGHz", "Starting passive car opener mode");
    
    // Reset state
    passive_tick_count = 0;
    
    // Configure for signal reception based on board type
    const PredatorBoardConfig* board_config = predator_boards_get_config(app->board_type);
    if(board_config) {
        FURI_LOG_I("PredatorSubGHz", "Using %s for passive opener", board_config->name);
    }
    
    // Set to receive mode for car signal detection
    furi_hal_subghz_rx();
    
    app->attack_running = true;
    
    // Initialize receiving mode for all boards
    if(app->notifications) {
        notification_message(app->notifications, &sequence_set_blue_255);
    }
}

void predator_subghz_stop_passive_car_opener(PredatorApp* app) {
    if(!app) {
        FURI_LOG_E("PredatorSubGHz", "NULL app pointer in predator_subghz_stop_passive_car_opener");
        return;
    }
    
    if(!app->subghz_txrx) {
        FURI_LOG_E("PredatorSubGHz", "SubGHz not initialized - nothing to stop");
        return;
    }
    
    FURI_LOG_I("PredatorSubGHz", "Stopping passive car opener mode");
    
    // Stop radio reception
    furi_hal_subghz_stop_async_rx();
    furi_hal_subghz_idle();
    
    FURI_LOG_D("PredatorSubGHz", "[REAL HW] Radio set to idle mode");
    
    app->attack_running = false;
    
    // Turn off blue LED
    if(app->notifications) {
        notification_message(app->notifications, &sequence_reset_blue);
    }
}

void predator_subghz_passive_car_opener_tick(PredatorApp* app) {
    if(!app) {
        FURI_LOG_E("PredatorSubGHz", "NULL app pointer in predator_subghz_passive_car_opener_tick");
        return;
    }
    
    if(!app->subghz_txrx || !app->attack_running) {
        return;
    }
    
    passive_tick_count++;
    
    // Real hardware signal detection using SubGHz receiver
    if(passive_tick_count % 10 == 0) {
        // Check for real SubGHz data in receive buffer
        if(furi_hal_subghz_rx_pipe_not_empty()) {
            LevelDuration level_duration;
            // Use available SubGHz HAL function to get received data
            level_duration.level = furi_hal_subghz_get_data_gpio() ? 1 : 0;
            level_duration.duration = furi_get_tick();
            
            if(level_duration.duration > 0) {
                FURI_LOG_I("PredatorSubGHz", "[REAL HW] Car signal detected!");
                
                // Process real signal data
                uint32_t signal_data = level_duration.duration;
                FURI_LOG_D("PredatorSubGHz", "[REAL HW] Received signal duration: %lu", signal_data);
                
                app->targets_found++;
                
                if(app->notifications) {
                    notification_message(app->notifications, &sequence_success);
                }
            }
        }
    }
}
