#pragma once

#include "../predator_i.h"

// ADVANCED SIGNAL INTELLIGENCE (SIGINT) - CUTTING-EDGE RF ANALYSIS
// Professional-grade signal intelligence for Tesla and government contracts

typedef enum {
    SigintModePassive,
    SigintModeActive,
    SigintModeTeslaSpecific,
    SigintModeGovernmentGrade,
    SigintModeQuantumEnhanced
} SigintMode;

typedef enum {
    SignalTypeUnknown,
    SignalTypeCarKey,
    SignalTypeTeslaCharge,
    SignalTypeWiFi,
    SignalTypeBluetooth,
    SignalTypeTrafficLight,
    SignalTypeInfrastructure,
    SignalTypeQuantumEncrypted
} SignalType;

typedef struct {
    uint32_t frequency_hz;
    float signal_strength_dbm;
    SignalType signal_type;
    uint32_t detection_timestamp;
    float confidence_score;
    bool tesla_related;
    bool government_interest;
    char description[128];
} DetectedSignal;

typedef struct {
    // SIGINT System Status
    bool sigint_active;
    bool quantum_analysis_enabled;
    bool ai_classification_active;
    SigintMode current_mode;
    
    // Detection Metrics
    uint32_t total_signals_detected;
    uint32_t tesla_signals_detected;
    uint32_t government_signals_detected;
    uint32_t unknown_signals_detected;
    
    // Frequency Analysis
    uint32_t frequency_sweeps_completed;
    uint32_t frequency_bands_analyzed;
    float spectrum_coverage_percent;
    uint32_t interference_patterns_detected;
    
    // Tesla SIGINT
    uint32_t tesla_charge_signals;
    uint32_t tesla_key_signals;
    uint32_t tesla_autopilot_signals;
    uint32_t tesla_sentry_signals;
    
    // Government SIGINT
    uint32_t traffic_light_signals;
    uint32_t infrastructure_signals;
    uint32_t emergency_service_signals;
    uint32_t classified_signals_detected;
    
    // Advanced Analysis
    uint32_t pattern_correlations_found;
    uint32_t encryption_schemes_identified;
    uint32_t zero_day_signal_patterns;
    float quantum_entanglement_detected;
    
    // Real-time Intelligence
    uint32_t active_monitoring_channels;
    uint32_t concurrent_signal_streams;
    float real_time_processing_load;
    
    uint32_t last_sigint_update;
} AdvancedSigintStatus;

// Core SIGINT Functions
bool predator_advanced_sigint_init(PredatorApp* app);
bool predator_advanced_sigint_start_monitoring(PredatorApp* app, SigintMode mode);
bool predator_advanced_sigint_stop_monitoring(PredatorApp* app);
bool predator_advanced_sigint_set_mode(PredatorApp* app, SigintMode mode);

// Signal Detection and Analysis
bool predator_advanced_sigint_passive_scan(PredatorApp* app);
bool predator_advanced_sigint_active_probe(PredatorApp* app);
bool predator_advanced_sigint_frequency_sweep(PredatorApp* app, uint32_t start_freq, uint32_t end_freq);
bool predator_advanced_sigint_analyze_signal(PredatorApp* app, DetectedSignal* signal);

// Tesla-Specific SIGINT
bool predator_advanced_sigint_tesla_monitoring(PredatorApp* app);
bool predator_advanced_sigint_tesla_charge_analysis(PredatorApp* app);
bool predator_advanced_sigint_tesla_key_intercept(PredatorApp* app);
bool predator_advanced_sigint_tesla_autopilot_signals(PredatorApp* app);
bool predator_advanced_sigint_tesla_sentry_detection(PredatorApp* app);

// Government Contract SIGINT
bool predator_advanced_sigint_traffic_light_analysis(PredatorApp* app);
bool predator_advanced_sigint_infrastructure_monitoring(PredatorApp* app);
bool predator_advanced_sigint_california_accident_signals(PredatorApp* app);
bool predator_advanced_sigint_switzerland_infrastructure(PredatorApp* app);

// Advanced Signal Processing
bool predator_advanced_sigint_ai_classification(PredatorApp* app, DetectedSignal* signal);
bool predator_advanced_sigint_quantum_analysis(PredatorApp* app);
bool predator_advanced_sigint_pattern_correlation(PredatorApp* app);
bool predator_advanced_sigint_encryption_analysis(PredatorApp* app);

// Real-time Intelligence
bool predator_advanced_sigint_real_time_processing(PredatorApp* app);
bool predator_advanced_sigint_concurrent_monitoring(PredatorApp* app);
bool predator_advanced_sigint_threat_assessment(PredatorApp* app);
bool predator_advanced_sigint_anomaly_detection(PredatorApp* app);

// Spectrum Analysis
bool predator_advanced_sigint_full_spectrum_scan(PredatorApp* app);
bool predator_advanced_sigint_interference_analysis(PredatorApp* app);
bool predator_advanced_sigint_bandwidth_optimization(PredatorApp* app);
bool predator_advanced_sigint_signal_fingerprinting(PredatorApp* app);

// Intelligence Reporting
bool predator_advanced_sigint_generate_intelligence_report(PredatorApp* app);
bool predator_advanced_sigint_export_signal_data(PredatorApp* app);
bool predator_advanced_sigint_create_threat_assessment(PredatorApp* app);

// Status and Metrics
AdvancedSigintStatus* predator_advanced_sigint_get_status(PredatorApp* app);
uint32_t predator_advanced_sigint_get_detection_count(PredatorApp* app, SignalType type);
float predator_advanced_sigint_calculate_coverage_score(PredatorApp* app);
