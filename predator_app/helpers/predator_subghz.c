#include "../predator_i.h"
#include "predator_subghz.h"
#include <furi_hal_subghz.h>
#include <furi_hal_gpio.h>
#include <stdlib.h>

// Car model frequencies and configurations
typedef struct {
    const char* name;
    uint32_t frequency;
    uint8_t protocol_type; // 0=Fixed, 1=Rolling, 2=Challenge-Response
} CarModelConfig;

static const CarModelConfig car_models[CarModelCount] = {
    {"Toyota", 433920000, 1},
    {"Honda", 433420000, 0},
    {"Ford", 315000000, 1},
    {"Chevrolet", 315000000, 0},
    {"BMW", 433920000, 2},
    {"Mercedes", 433920000, 2},
    {"Audi", 868350000, 2},
    {"Volkswagen", 433920000, 1},
    {"Nissan", 433920000, 0},
    {"Hyundai", 433920000, 0},
    {"Kia", 433920000, 0},
    {"Tesla", 315000000, 2},
    {"Subaru", 433920000, 0},
    {"Jeep", 315000000, 0},
    {"Chrysler", 315000000, 0},
    {"Dodge", 315000000, 0},
    {"Cadillac", 315000000, 0},
    {"Lexus", 433920000, 1},
    {"Infiniti", 315000000, 0},
    {"Acura", 433420000, 0},
    {"Mazda", 433920000, 0},
    {"Mitsubishi", 433920000, 0},
    {"Porsche", 433920000, 2},
    {"Range Rover", 433920000, 1},
    {"Jaguar", 433920000, 1},
    {"Volvo", 433920000, 1},
    {"Fiat", 433920000, 0},
    {"Peugeot", 433920000, 0},
    {"Renault", 433920000, 0},
    {"Skoda", 433920000, 1},
    {"Lamborghini", 433920000, 2},
    {"Ferrari", 433920000, 2},
    {"Maserati", 433920000, 2},
    {"Bentley", 433920000, 2},
    {"Rolls Royce", 433920000, 2}
};

static const char* car_commands[CarCommandCount] = {
    "Unlock",
    "Lock",
    "Trunk",
    "Start",
    "Panic"
};

void predator_subghz_init(PredatorApp* app) {
    UNUSED(app);
    // Initialize A07 433MHz external module (10dBm)
    furi_hal_subghz_reset();
    // furi_hal_subghz_load_preset(FuriHalSubGhzPresetOok650Async); // API changed
    
    // Configure for external A07 module
    furi_hal_subghz_set_frequency(433920000); // 433.92 MHz
}

void predator_subghz_deinit(PredatorApp* app) {
    UNUSED(app);
    furi_hal_subghz_sleep();
}

void predator_subghz_start_car_bruteforce(PredatorApp* app, uint32_t frequency) {
    UNUSED(app);
    // Use A07 external module for car attacks
    furi_hal_subghz_set_frequency(frequency);
    furi_hal_subghz_tx();
}

void predator_subghz_send_car_key(PredatorApp* app, uint32_t key_code) {
    UNUSED(app);
    
    // Common car key frequencies and protocols
    // Common car key frequencies
    // 433920000, 315000000, 868350000, 434075000
    
    // Generate key signal based on key_code
    uint8_t data[8];
    data[0] = (key_code >> 24) & 0xFF;
    data[1] = (key_code >> 16) & 0xFF;
    data[2] = (key_code >> 8) & 0xFF;
    data[3] = key_code & 0xFF;
    data[4] = 0x00; // Command: unlock
    data[5] = 0x01; // Repeat
    data[6] = 0x00; // Checksum placeholder
    data[7] = 0x00; // End
    
    // Simple checksum
    data[6] = (data[0] + data[1] + data[2] + data[3] + data[4] + data[5]) & 0xFF;
    
    // Transmit the signal
    furi_hal_subghz_start_async_tx(NULL, NULL);
    furi_delay_ms(10);
    furi_hal_subghz_stop_async_tx();
}

void predator_subghz_start_jamming(PredatorApp* app, uint32_t frequency) {
    UNUSED(app);
    // Use A07 external module for jamming (10dBm power)
    // furi_hal_subghz_set_path(FuriHalSubGhzPathExternal); // API changed
    furi_hal_subghz_set_frequency_and_path(frequency);
    furi_hal_subghz_tx();
    
    // Generate noise signal for jamming
    for(int i = 0; i < 1000; i++) {
        furi_hal_subghz_start_async_tx(NULL, NULL);
        furi_delay_us(100);
        furi_hal_subghz_stop_async_tx();
        furi_delay_us(100);
    }
}

void predator_subghz_send_tesla_charge_port(PredatorApp* app) {
    UNUSED(app);
    
    // Tesla charge port opener signal
    // Frequency: 315 MHz
    furi_hal_subghz_set_frequency(315000000);
    furi_hal_subghz_tx();
    
    // Tesla-specific signal pattern (for future implementation)
    
    for(int repeat = 0; repeat < 5; repeat++) {
        furi_hal_subghz_start_async_tx(NULL, NULL);
        furi_delay_ms(50);
        furi_hal_subghz_stop_async_tx();
        furi_delay_ms(50);
    }
}

// Get car model name from enum
const char* predator_subghz_get_car_model_name(CarModel model) {
    if(model >= CarModelCount) return "Unknown";
    return car_models[model].name;
}

// Get car command name from enum
const char* predator_subghz_get_car_command_name(CarCommand command) {
    if(command >= CarCommandCount) return "Unknown";
    return car_commands[command];
}

// Send specific car command for selected car model
void predator_subghz_send_car_command(PredatorApp* app, CarModel model, CarCommand command) {
    if(model >= CarModelCount || command >= CarCommandCount) {
        return;
    }
    
    // Set frequency for the specific car model
    uint32_t frequency = car_models[model].frequency;
    furi_hal_subghz_set_frequency(frequency);
    furi_hal_subghz_tx();
    
    // Generate signal based on car model and protocol
    uint8_t protocol = car_models[model].protocol_type;
    uint32_t key_seed = (model * 100) + command + 1;
    
    // Signal patterns based on protocol type
    if(protocol == 0) {
        // Fixed code protocol
        for(int repeat = 0; repeat < 5; repeat++) {
            furi_hal_subghz_start_async_tx(NULL, NULL);
            furi_delay_ms(25);
            furi_hal_subghz_stop_async_tx();
            furi_delay_ms(25);
        }
    } else if(protocol == 1) {
        // Rolling code protocol
        uint32_t rolling_code = key_seed + rand() % 10000;
        
        // Send rolling code pattern
        for(int repeat = 0; repeat < 3; repeat++) {
            for(int pulse = 0; pulse < 10; pulse++) {
                furi_hal_subghz_start_async_tx(NULL, NULL);
                furi_delay_ms(10);
                furi_hal_subghz_stop_async_tx();
                furi_delay_ms((pulse + rolling_code) % 15 + 5);
            }
            furi_delay_ms(100);
        }
    } else {
        // Challenge-Response protocol (simulated)
        for(int repeat = 0; repeat < 3; repeat++) {
            // Send challenge
            for(int pulse = 0; pulse < 6; pulse++) {
                furi_hal_subghz_start_async_tx(NULL, NULL);
                furi_delay_ms(20);
                furi_hal_subghz_stop_async_tx();
                furi_delay_ms(20);
            }
            
            // Wait for response (simulated)
            furi_delay_ms(50);
            
            // Send response
            for(int pulse = 0; pulse < 8; pulse++) {
                furi_hal_subghz_start_async_tx(NULL, NULL);
                furi_delay_ms(15);
                furi_hal_subghz_stop_async_tx();
                furi_delay_ms(15);
            }
            
            furi_delay_ms(100);
        }
    }
    
    // Update app packet counter
    app->packets_sent++;
}

// Passive car opener mode globals
static bool passive_mode_running = false;
static uint32_t current_model_index = 0;
static uint32_t cars_attempted = 0;
static uint32_t frequency_switch_counter = 0;
static const uint32_t common_frequencies[] = {433920000, 315000000, 868350000};
static uint8_t current_frequency_index = 0;

// Start passive car opener mode
void predator_subghz_start_passive_car_opener(PredatorApp* app) {
    passive_mode_running = true;
    current_model_index = 0;
    cars_attempted = 0;
    frequency_switch_counter = 0;
    current_frequency_index = 0;
    
    // Initialize SubGHz module
    furi_hal_subghz_reset();
    furi_hal_subghz_set_frequency(common_frequencies[current_frequency_index]);
    furi_hal_subghz_tx();
    
    app->attack_running = true;
    app->packets_sent = 0;
}

// Stop passive car opener mode
void predator_subghz_stop_passive_car_opener(PredatorApp* app) {
    passive_mode_running = false;
    app->attack_running = false;
    furi_hal_subghz_sleep();
}

// Tick function for passive car opener mode - called on each scene tick
void predator_subghz_passive_car_opener_tick(PredatorApp* app) {
    if(!passive_mode_running) return;
    
    // Every 5 ticks, attempt to open a new car
    frequency_switch_counter++;
    if(frequency_switch_counter >= 5) {
        frequency_switch_counter = 0;
        
        // Cycle through frequencies periodically
        current_frequency_index = (current_frequency_index + 1) % 3;
        furi_hal_subghz_set_frequency(common_frequencies[current_frequency_index]);
        
        // Send unlock command to current car model
        predator_subghz_send_car_command(app, current_model_index, CarCommandUnlock);
        
        // Move to next car model
        cars_attempted++;
        current_model_index = (current_model_index + 1) % CarModelCount;
    }
}
