#pragma once

#include "../predator_i.h"

// ATTACK OPTIMIZER - AUDIT POINT IMPROVEMENTS
// Optimizes attack success rates and timing based on audit findings

typedef enum {
    AttackOptimizationTypeSuccessRate,
    AttackOptimizationTypeTiming,
    AttackOptimizationTypeFrequency,
    AttackOptimizationTypePower,
    AttackOptimizationTypeSequence,
    AttackOptimizationTypeCount
} AttackOptimizationType;

typedef struct {
    // Success Rate Optimization
    float baseline_success_rate;
    float optimized_success_rate;
    float improvement_percentage;
    
    // Timing Optimization
    uint32_t baseline_attack_time_ms;
    uint32_t optimized_attack_time_ms;
    uint32_t time_reduction_ms;
    
    // Frequency Optimization
    uint32_t optimal_frequency_hz;
    uint32_t frequency_accuracy;
    bool frequency_calibrated;
    
    // Power Optimization
    uint32_t optimal_power_dbm;
    bool power_boosted;
    float signal_quality;
    
    // Attack Sequence Optimization
    uint32_t optimal_sequence_length;
    uint32_t sequence_timing_ms;
    bool sequence_optimized;
    
    // Performance Metrics
    uint32_t total_attacks_optimized;
    uint32_t successful_optimizations;
    float overall_improvement;
    
    // Tesla Specific
    bool tesla_attacks_optimized;
    float tesla_success_rate;
    uint32_t walking_mode_effectiveness;
    
    uint32_t last_optimization_time;
    bool optimizer_active;
} AttackOptimizationStatus;

// Core Attack Optimization
bool predator_attack_optimizer_init(PredatorApp* app);
bool predator_attack_optimizer_optimize_all_attacks(PredatorApp* app);
bool predator_attack_optimizer_optimize_success_rates(PredatorApp* app);
bool predator_attack_optimizer_optimize_timing(PredatorApp* app);
bool predator_attack_optimizer_optimize_frequencies(PredatorApp* app);
bool predator_attack_optimizer_optimize_power_levels(PredatorApp* app);

// Specific Attack Optimizations
bool predator_attack_optimizer_optimize_car_attacks(PredatorApp* app);
bool predator_attack_optimizer_optimize_wifi_attacks(PredatorApp* app);
bool predator_attack_optimizer_optimize_bluetooth_attacks(PredatorApp* app);
bool predator_attack_optimizer_optimize_rfid_attacks(PredatorApp* app);

// Tesla-Specific Optimizations
bool predator_attack_optimizer_optimize_tesla_attacks(PredatorApp* app);
bool predator_attack_optimizer_optimize_walking_mode(PredatorApp* app);
bool predator_attack_optimizer_optimize_tesla_frequencies(PredatorApp* app);

// Advanced Optimizations
bool predator_attack_optimizer_ai_learning_optimization(PredatorApp* app);
bool predator_attack_optimizer_adaptive_timing(PredatorApp* app);
bool predator_attack_optimizer_predictive_optimization(PredatorApp* app);

// Monitoring and Reporting
AttackOptimizationStatus* predator_attack_optimizer_get_status(PredatorApp* app);
bool predator_attack_optimizer_generate_report(PredatorApp* app);
float predator_attack_optimizer_calculate_improvement(PredatorApp* app);

// Real-time Optimization
bool predator_attack_optimizer_real_time_adjustment(PredatorApp* app);
bool predator_attack_optimizer_continuous_learning(PredatorApp* app);
