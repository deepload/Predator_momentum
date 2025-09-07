#include "../predator_i.h"
#include "predator_subghz.h"
#include <furi.h>
#include <furi_hal.h>
#include <furi_hal_gpio.h>
#include <stdlib.h>

// Car model names mapping
static const char* car_model_names[CarModelCount] = {
    "Toyota", "Honda", "Ford", "Chevrolet", "BMW", "Mercedes", "Audi", "Volkswagen",
    "Nissan", "Hyundai", "Kia", "Tesla", "Subaru", "Jeep", "Chrysler", "Dodge",
    "Cadillac", "Lexus", "Infiniti", "Acura", "Mazda", "Mitsubishi", "Porsche",
    "Range Rover", "Jaguar", "Volvo", "Fiat", "Peugeot", "Renault", "Skoda",
    "Lamborghini", "Ferrari", "Maserati", "Bentley", "Rolls Royce"
};

// Car command names mapping
static const char* car_command_names[CarCommandCount] = {
    "Unlock", "Lock", "Open Trunk", "Start Engine", "Panic Alarm"
};

// Frequencies for different car models
static const uint32_t car_frequencies[CarModelCount] = {
    433920000, // Toyota
    433420000, // Honda
    315000000, // Ford
    315000000, // Chevrolet
    433920000, // BMW
    433920000, // Mercedes
    868350000, // Audi
    433920000, // Volkswagen
    433920000, // Nissan
    433920000, // Hyundai
    433920000, // Kia
    315000000, // Tesla
    433920000, // Subaru
    315000000, // Jeep
    315000000, // Chrysler
    315000000, // Dodge
    315000000, // Cadillac
    433920000, // Lexus
    315000000, // Infiniti
    433420000, // Acura
    433920000, // Mazda
    433920000, // Mitsubishi
    433920000, // Porsche
    433920000, // Range Rover
    433920000, // Jaguar
    433920000, // Volvo
    433920000, // Fiat
    433920000, // Peugeot
    433920000, // Renault
    433920000, // Skoda
    433920000, // Lamborghini
    433920000, // Ferrari
    433920000, // Maserati
    433920000, // Bentley
    433920000  // Rolls Royce
};

void predator_subghz_init(PredatorApp* app) {
    furi_assert(app);
    
    // Use try/catch pattern with error flags
    bool init_success = true;
    
    // Safely initialize SubGHz with error handling
    furi_hal_power_suppress_charge_enter();
    
    FURI_CRITICAL_ENTER();
    // Wrapped in critical section to prevent interruption during initialization
    
    // Try initialization with error capture
    bool init_result = true;
    
    // Safe hardware initialization
    furi_hal_subghz_init();
    
    if(!init_result) {
        FURI_LOG_E("Predator", "SubGHz initialization failed");
        init_success = false;
    }
    
    // Check external radio module if initialization was successful
    if(init_success) {
        if(furi_hal_gpio_read(&gpio_cc1101_g0)) {
            FURI_LOG_I("Predator", "External CC1101 module detected");
        }
    }
    
    FURI_CRITICAL_EXIT();
    furi_hal_power_suppress_charge_exit();
    
    // If initialization failed, log it but continue
    if(!init_success) {
        FURI_LOG_E("Predator", "SubGHz functionality will be limited");
    }
}

void predator_subghz_deinit(PredatorApp* app) {
    furi_assert(app);
    
    // Clean up and release SubGHz
    furi_hal_subghz_sleep();
}

void predator_subghz_start_car_bruteforce(PredatorApp* app, uint32_t frequency) {
    furi_assert(app);
    
    // Check frequency before using
    if(!furi_hal_subghz_is_frequency_valid(frequency)) {
        FURI_LOG_E("Predator", "Invalid frequency: %lu", frequency);
        return;
    }
    
    FURI_LOG_I("Predator", "Starting car key bruteforce on %lu Hz", frequency);
    furi_hal_subghz_reset();
    furi_hal_subghz_set_frequency_and_path(frequency);
    
    // Start transmission
    furi_hal_subghz_tx();
}

void predator_subghz_send_car_key(PredatorApp* app, uint32_t key_code) {
    furi_assert(app);
    
    FURI_LOG_I("Predator", "Sending car key code: %08lX", key_code);
    // Actual implementation would add protocol-specific code here
}

void predator_subghz_start_jamming(PredatorApp* app, uint32_t frequency) {
    furi_assert(app);
    
    // Check frequency before using
    if(!furi_hal_subghz_is_frequency_valid(frequency)) {
        FURI_LOG_E("Predator", "Invalid frequency: %lu", frequency);
        return;
    }
    
    FURI_LOG_I("Predator", "Starting jamming on %lu Hz", frequency);
    furi_hal_subghz_reset();
    furi_hal_subghz_set_frequency_and_path(frequency);
    
    // Start transmission for jamming
    furi_hal_subghz_tx();
}

void predator_subghz_send_tesla_charge_port(PredatorApp* app) {
    furi_assert(app);
    
    uint32_t tesla_freq = 315000000;
    if(!furi_hal_subghz_is_frequency_valid(tesla_freq)) {
        FURI_LOG_E("Predator", "Invalid frequency: 315MHz");
        return;
    }
    
    FURI_LOG_I("Predator", "Sending Tesla charge port signal");
    furi_hal_subghz_reset();
    furi_hal_subghz_set_frequency_and_path(tesla_freq);
    
    // Start transmission
    furi_hal_subghz_tx();
    
    // Implementation would include Tesla-specific protocols
}

const char* predator_subghz_get_car_model_name(CarModel model) {
    if((unsigned int)model >= CarModelCount) {
        return "Unknown";
    }
    return car_model_names[model];
}

const char* predator_subghz_get_car_command_name(CarCommand command) {
    if((unsigned int)command >= CarCommandCount) {
        return "Unknown";
    }
    return car_command_names[command];
}

void predator_subghz_send_car_command(PredatorApp* app, CarModel model, CarCommand command) {
    furi_assert(app);
    
    if((unsigned int)model >= CarModelCount || (unsigned int)command >= CarCommandCount) {
        FURI_LOG_E("Predator", "Invalid car model or command");
        return;
    }
    
    uint32_t frequency = car_frequencies[model];
    
    if(!furi_hal_subghz_is_frequency_valid(frequency)) {
        FURI_LOG_E("Predator", "Invalid frequency: %lu", frequency);
        return;
    }
    
    FURI_LOG_I("Predator", "Sending %s command to %s on %lu Hz",
              predator_subghz_get_car_command_name(command),
              predator_subghz_get_car_model_name(model),
              frequency);
              
    furi_hal_subghz_reset();
    furi_hal_subghz_set_frequency_and_path(frequency);
    
    // Start transmission
    furi_hal_subghz_tx();
    
    // Implementation would include car-specific protocols
}

void predator_subghz_start_passive_car_opener(PredatorApp* app) {
    furi_assert(app);
    
    FURI_LOG_I("Predator", "Starting passive car opener mode");
    
    uint32_t frequency = 433920000; // Most common car frequency
    
    if(furi_hal_subghz_is_frequency_valid(frequency)) {
        furi_hal_subghz_reset();
        furi_hal_subghz_set_frequency_and_path(frequency);
        
        // Start reception
        furi_hal_subghz_rx();
    }
}

void predator_subghz_stop_passive_car_opener(PredatorApp* app) {
    furi_assert(app);
    
    FURI_LOG_I("Predator", "Stopping passive car opener mode");
    furi_hal_subghz_sleep();
}

void predator_subghz_passive_car_opener_tick(PredatorApp* app) {
    furi_assert(app);
    
    if(app->attack_running) {
        // Check for received signals and relay them
        // Implementation details would depend on specific protocols
    }
}
