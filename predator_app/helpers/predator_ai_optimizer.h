#pragma once

#include <stdint.h>
#include <stdbool.h>

// MEMORY-OPTIMIZED AI ATTACK OPTIMIZER
// Uses minimal memory footprint to avoid exceptions

typedef struct PredatorApp PredatorApp;

// Lightweight attack statistics (only 16 bytes)
typedef struct {
    uint8_t success_rate;      // 0-100%
    uint8_t optimal_power;     // 0-255
    uint16_t best_frequency;   // Frequency offset
    uint32_t attempts;         // Total attempts
    uint32_t successes;        // Successful attacks
} AttackStats;

// Compact AI learning data (only 32 bytes total)
typedef struct {
    AttackStats car_attacks;     // 16 bytes
    AttackStats wifi_attacks;    // 16 bytes
} AILearningData;

// Memory-safe AI functions
bool predator_ai_init(PredatorApp* app);
void predator_ai_deinit(PredatorApp* app);

// Lightweight optimization (no dynamic allocation)
bool predator_ai_optimize_car_attack(PredatorApp* app, uint32_t frequency, uint8_t power);
bool predator_ai_optimize_wifi_attack(PredatorApp* app, uint8_t channel, uint8_t power);

// Get optimized parameters (stack-based, no malloc)
uint32_t predator_ai_get_optimal_frequency(PredatorApp* app, uint8_t attack_type);
uint8_t predator_ai_get_optimal_power(PredatorApp* app, uint8_t attack_type);

// Record attack result (minimal memory impact)
void predator_ai_record_result(PredatorApp* app, uint8_t attack_type, bool success);

// Attack types (enum to save memory)
typedef enum {
    AI_ATTACK_CAR = 0,
    AI_ATTACK_WIFI = 1,
    AI_ATTACK_COUNT = 2
} AIAttackType;
