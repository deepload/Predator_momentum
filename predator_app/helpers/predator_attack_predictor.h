#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct PredatorApp PredatorApp;

// PREDICTIVE ATTACK SUCCESS - AI-POWERED ANALYSIS
// Memory-optimized for Flipper Zero (only 64 bytes total)

typedef struct {
    float success_probability;     // 0.0-1.0 (4 bytes)
    uint32_t estimated_time_ms;   // Time to success (4 bytes)
    char recommended_approach[32]; // Best attack method (32 bytes)
    uint8_t confidence_level;     // 0-100% (1 byte)
    uint8_t difficulty_rating;    // 1-10 (1 byte)
    uint16_t historical_attempts; // Previous attempts (2 bytes)
    // Total: 44 bytes + padding = 48 bytes
} AttackPrediction;

typedef enum {
    ATTACK_TYPE_CAR_ROLLING = 0,
    ATTACK_TYPE_CAR_FIXED,
    ATTACK_TYPE_CAR_SMART_KEY,
    ATTACK_TYPE_WIFI_DEAUTH,
    ATTACK_TYPE_WIFI_HANDSHAKE,
    ATTACK_TYPE_RFID_CLONE,
    ATTACK_TYPE_SUBGHZ_JAM,
    ATTACK_TYPE_COUNT
} PredictiveAttackType;

// Memory-safe predictor functions
bool predator_predictor_init(PredatorApp* app);
void predator_predictor_deinit(PredatorApp* app);

// Main prediction function - analyzes target and predicts success
bool predator_predict_attack_success(PredatorApp* app, PredictiveAttackType attack_type, 
                                   uint32_t frequency, size_t target_index, 
                                   AttackPrediction* prediction);

// Update predictor with actual results for learning
void predator_predictor_record_result(PredatorApp* app, PredictiveAttackType attack_type,
                                     uint32_t frequency, bool success, uint32_t time_taken_ms);

// Get success rate for specific attack type
float predator_predictor_get_success_rate(PredatorApp* app, PredictiveAttackType attack_type);

// Get recommended frequency for target
uint32_t predator_predictor_get_optimal_frequency(PredatorApp* app, PredictiveAttackType attack_type,
                                                size_t target_index);

// Professional UI helpers
const char* predator_predictor_get_difficulty_string(uint8_t difficulty);
const char* predator_predictor_get_confidence_string(uint8_t confidence);
