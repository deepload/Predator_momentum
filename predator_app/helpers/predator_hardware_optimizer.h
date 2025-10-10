#pragma once

#include "../predator_i.h"

// HARDWARE OPTIMIZER - AUDIT POINT IMPROVEMENTS
// Addresses specific audit findings for maximum hardware utilization

typedef enum {
    HardwareOptimizationLevelBasic,
    HardwareOptimizationLevelAdvanced,
    HardwareOptimizationLevelMaximum,
    HardwareOptimizationLevelTeslaDemo,
    HardwareOptimizationLevelGovernment
} HardwareOptimizationLevel;

typedef struct {
    // Hardware Status
    bool subghz_optimized;
    bool esp32_optimized;
    bool gps_optimized;
    bool board_optimized;
    
    // Performance Metrics
    uint32_t subghz_power_level;
    uint32_t esp32_tx_power;
    uint32_t signal_strength_dbm;
    float hardware_efficiency;
    
    // Optimization Results
    uint32_t optimization_score;
    uint32_t improvements_applied;
    char last_optimization[128];
    
    // Tesla Specific
    bool tesla_mode_optimized;
    bool walking_mode_enhanced;
    uint32_t tesla_compatibility_score;
    
    // Government Requirements
    bool california_optimized;
    bool switzerland_optimized;
    uint32_t compliance_optimizations;
    
    uint32_t last_optimization_time;
} HardwareOptimizationStatus;

// Core Hardware Optimization
bool predator_hardware_optimizer_init(PredatorApp* app);
bool predator_hardware_optimizer_run_full_optimization(PredatorApp* app, HardwareOptimizationLevel level);
bool predator_hardware_optimizer_optimize_subghz(PredatorApp* app);
bool predator_hardware_optimizer_optimize_esp32(PredatorApp* app);
bool predator_hardware_optimizer_optimize_gps(PredatorApp* app);
bool predator_hardware_optimizer_optimize_board_config(PredatorApp* app);

// Advanced Optimizations
bool predator_hardware_optimizer_boost_signal_strength(PredatorApp* app);
bool predator_hardware_optimizer_enhance_range(PredatorApp* app);
bool predator_hardware_optimizer_optimize_power_consumption(PredatorApp* app);
bool predator_hardware_optimizer_calibrate_frequencies(PredatorApp* app);

// Tesla-Specific Optimizations
bool predator_hardware_optimizer_tesla_mode(PredatorApp* app);
bool predator_hardware_optimizer_walking_mode_enhancement(PredatorApp* app);
bool predator_hardware_optimizer_tesla_frequency_optimization(PredatorApp* app);

// Government Contract Optimizations
bool predator_hardware_optimizer_california_state_mode(PredatorApp* app);
bool predator_hardware_optimizer_switzerland_mode(PredatorApp* app);
bool predator_hardware_optimizer_government_compliance(PredatorApp* app);

// Monitoring and Reporting
HardwareOptimizationStatus* predator_hardware_optimizer_get_status(PredatorApp* app);
bool predator_hardware_optimizer_generate_report(PredatorApp* app);
uint32_t predator_hardware_optimizer_calculate_score(PredatorApp* app);

// Automatic Optimization
bool predator_hardware_optimizer_auto_detect_and_optimize(PredatorApp* app);
bool predator_hardware_optimizer_continuous_optimization(PredatorApp* app);
bool predator_hardware_optimizer_emergency_optimization(PredatorApp* app);
