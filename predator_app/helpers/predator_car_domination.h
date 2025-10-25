#pragma once

#include "../predator_i.h"
#include "predator_models_hardcoded.h"
#include "predator_gps.h"
#include "predator_crypto_engine.h"
#include "subghz/predator_subghz_core.h"
#include "predator_logging.h"

// 🚗 ULTIMATE CAR DOMINATION MODE
// Uses ALL 178+ car models + crypto + SubGHz + GPS for total car control
// Elon's ultimate demonstration of technological superiority

typedef enum {
    CarDominationStatusIdle,
    CarDominationStatusInitializing,
    CarDominationStatusScanning,
    CarDominationStatusAttacking,
    CarDominationStatusComplete
} CarDominationStatus;

typedef struct {
    CarDominationStatus status;
    GPSCoordinates current_location;
    uint32_t total_models_loaded;
    uint32_t models_attacked;
    uint32_t successful_attacks;
    uint32_t failed_attacks;
    uint32_t current_model_index;
    uint32_t attack_time_ms;
    char current_manufacturer[32];
    char current_model[32];
    uint32_t current_frequency;
    bool location_optimized;
    bool max_power_enabled;
    bool all_frequencies_active;
} CarDominationState;

/**
 * @brief Activate ultimate car domination mode
 * @param app Application context
 * @return true if activated successfully, false otherwise
 */
bool predator_car_domination_activate(PredatorApp* app);

/**
 * @brief Stop car domination mode
 * @param app Application context
 * @return true if stopped successfully, false otherwise
 */
bool predator_car_domination_stop(PredatorApp* app);

/**
 * @brief Get current car domination state
 * @param app Application context
 * @return Pointer to car domination state
 */
CarDominationState* predator_car_domination_get_state(PredatorApp* app);

/**
 * @brief Process next car model in domination sequence
 * @param app Application context
 * @return true if processed successfully, false otherwise
 */
bool predator_car_domination_process_next_model(PredatorApp* app);

/**
 * @brief Attack specific car model with optimized parameters
 * @param app Application context
 * @param model Car model to attack
 * @return true if attack successful, false otherwise
 */
bool predator_car_domination_attack_model(PredatorApp* app, const CarModel* model);

/**
 * @brief Enable maximum power mode for car domination
 * @param app Application context
 * @return true if enabled successfully, false otherwise
 */
bool predator_car_domination_enable_max_power(PredatorApp* app);

/**
 * @brief Get attack statistics for car domination session
 * @param app Application context
 * @param stats Output statistics structure
 * @return true if statistics retrieved successfully, false otherwise
 */
bool predator_car_domination_get_stats(PredatorApp* app, CarDominationState* stats);
