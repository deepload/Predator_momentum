#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// Forward declaration
struct PredatorApp;

// Performance Profile Structure
typedef struct {
    // System specifications
    uint32_t cpu_frequency;
    uint32_t memory_available;
    uint32_t stack_size;
    
    // Hardware capabilities
    bool subghz_capable;
    bool esp32_capable;
    bool gps_capable;
    bool external_rf_available;
    bool board_optimized;
    
    // RF performance
    uint8_t max_rf_power;
    float rf_range_multiplier;
    float antenna_efficiency;
    
    // Attack performance
    uint32_t attack_throughput;
    float memory_efficiency;
    float power_efficiency;
    float real_time_performance;
    
    // Tesla/Government modes
    bool tesla_exclusion_active;
    bool government_mode_active;
    bool vip_unlimited_active;
} PerformanceProfile;

// Performance Optimization Functions
bool predator_performance_init(struct PredatorApp* app);

bool predator_performance_optimize_memory(struct PredatorApp* app);

bool predator_performance_optimize_rf(struct PredatorApp* app);

bool predator_performance_optimize_attacks(struct PredatorApp* app);

bool predator_performance_optimize_realtime(struct PredatorApp* app);

bool predator_performance_optimize_for_tesla(struct PredatorApp* app);

PerformanceProfile predator_performance_get_profile(void);

bool predator_performance_generate_report(struct PredatorApp* app, char* report_buffer, size_t buffer_size);

void predator_performance_shutdown(struct PredatorApp* app);

// Internal optimization functions
static bool predator_performance_optimize_hardware_init(struct PredatorApp* app);
