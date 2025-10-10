#pragma once

#include "../predator_i.h"

// ENHANCED ATTACK VECTORS - MAXIMUM EFFECTIVENESS
// Advanced attack implementations for Tesla demonstrations
// Government-grade capabilities with real hardware transmission

typedef enum {
    EnhancedAttackTypeCarRollingCode,
    EnhancedAttackTypeCarFixedCode,
    EnhancedAttackTypeCarSmartKey,
    EnhancedAttackTypeCarJamming,
    EnhancedAttackTypeTeslaSpecific,
    EnhancedAttackTypeWiFiDeauth,
    EnhancedAttackTypeWiFiEvilTwin,
    EnhancedAttackTypeWiFiHandshake,
    EnhancedAttackTypeBluetoothSpam,
    EnhancedAttackTypeBluetoothRelay,
    EnhancedAttackTypeRFIDClone,
    EnhancedAttackTypeRFIDFuzzing,
    EnhancedAttackTypeCount
} EnhancedAttackType;

typedef enum {
    AttackEffectivenessLow,
    AttackEffectivenessStandard,
    AttackEffectivenessHigh,
    AttackEffectivenessMaximum,
    AttackEffectivenessUnlimited
} AttackEffectiveness;

typedef struct {
    EnhancedAttackType type;
    AttackEffectiveness effectiveness;
    uint32_t frequency;
    uint32_t power_dbm;
    uint32_t duration_ms;
    bool real_transmission;
    bool tesla_exclusion;
    float success_probability;
    uint32_t targets_affected;
    char description[128];
} EnhancedAttackVector;

typedef struct {
    // Attack Configuration
    EnhancedAttackVector vectors[16];
    uint32_t vector_count;
    bool simultaneous_mode;
    bool ai_optimization;
    
    // Performance Metrics
    uint32_t total_attacks_launched;
    uint32_t successful_attacks;
    float overall_success_rate;
    uint32_t average_attack_time;
    
    // Hardware Utilization
    bool subghz_active;
    bool esp32_active;
    bool external_rf_active;
    uint32_t max_concurrent_attacks;
    
    // Tesla Compliance
    bool tesla_mode_active;
    uint32_t tesla_vehicles_excluded;
    uint32_t non_tesla_compromised;
    
    // Government Requirements
    bool government_approved_only;
    bool logging_enabled;
    uint32_t compliance_level;
    
    uint32_t session_start_time;
    bool session_active;
} EnhancedAttackSession;

// Attack Vector Management
bool predator_enhanced_attacks_init(PredatorApp* app);
bool predator_enhanced_attacks_configure_vector(PredatorApp* app, EnhancedAttackVector* vector);
bool predator_enhanced_attacks_launch_vector(PredatorApp* app, EnhancedAttackType type);
bool predator_enhanced_attacks_launch_simultaneous(PredatorApp* app);

// Car Attack Enhancements
bool predator_enhanced_car_rolling_code_advanced(PredatorApp* app, uint32_t frequency);
bool predator_enhanced_car_fixed_code_optimized(PredatorApp* app, uint32_t frequency);
bool predator_enhanced_car_smart_key_relay(PredatorApp* app);
bool predator_enhanced_car_jamming_selective(PredatorApp* app, uint32_t frequency);
bool predator_enhanced_tesla_charge_port_advanced(PredatorApp* app);

// WiFi Attack Enhancements
bool predator_enhanced_wifi_deauth_targeted(PredatorApp* app, const char* target_ssid);
bool predator_enhanced_wifi_evil_twin_advanced(PredatorApp* app, const char* ssid);
bool predator_enhanced_wifi_handshake_optimized(PredatorApp* app);
bool predator_enhanced_wifi_pmkid_accelerated(PredatorApp* app);

// Bluetooth Attack Enhancements
bool predator_enhanced_bluetooth_spam_advanced(PredatorApp* app);
bool predator_enhanced_bluetooth_relay_attack(PredatorApp* app);
bool predator_enhanced_bluetooth_car_unlock(PredatorApp* app);

// RFID Attack Enhancements
bool predator_enhanced_rfid_clone_advanced(PredatorApp* app);
bool predator_enhanced_rfid_fuzzing_intelligent(PredatorApp* app);
bool predator_enhanced_rfid_emulation_perfect(PredatorApp* app);

// Tesla-Specific Enhancements
bool predator_enhanced_tesla_walking_mode(PredatorApp* app);
bool predator_enhanced_tesla_exclusion_system(PredatorApp* app, bool enable);
bool predator_enhanced_tesla_security_bypass(PredatorApp* app);

// Government Contract Features
bool predator_enhanced_government_compliance_mode(PredatorApp* app);
bool predator_enhanced_california_state_testing(PredatorApp* app);
bool predator_enhanced_switzerland_testing(PredatorApp* app);

// AI-Powered Optimizations
bool predator_enhanced_ai_target_selection(PredatorApp* app);
bool predator_enhanced_ai_attack_optimization(PredatorApp* app);
bool predator_enhanced_ai_success_prediction(PredatorApp* app);

// Performance Monitoring
EnhancedAttackSession* predator_enhanced_attacks_get_session(PredatorApp* app);
bool predator_enhanced_attacks_log_performance(PredatorApp* app);
float predator_enhanced_attacks_calculate_effectiveness(PredatorApp* app);

// Real Hardware Validation
bool predator_enhanced_attacks_validate_hardware(PredatorApp* app);
bool predator_enhanced_attacks_force_real_transmission(PredatorApp* app);
bool predator_enhanced_attacks_disable_simulation(PredatorApp* app);
