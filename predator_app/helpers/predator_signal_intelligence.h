#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// Forward declaration
struct PredatorApp;

// Signal Intelligence Definitions
#define MAX_SIGNALS 32
#define MAX_ATTACK_OPPORTUNITIES 16
#define MAX_DEMOD_DATA 64
#define WATERFALL_WIDTH 128

// Threat Levels
typedef enum {
    ThreatLevelNone,
    ThreatLevelLow,
    ThreatLevelMedium,
    ThreatLevelHigh,
    ThreatLevelCritical
} ThreatLevel;

// Use AttackType from crypto engine - avoid duplicate definition
#include "predator_crypto_engine.h"

// RF Modulation Types (defined in predator_crypto_engine.h)
// Using shared ModulationType from crypto engine

// Signal Fingerprint Database Entry
typedef struct {
    char vehicle_type[32];
    uint32_t freq_min;
    uint32_t freq_max;
    float typical_strength;
    float base_confidence;
    char security_type[32];
} SignalFingerprint;

// Individual Signal Contact
typedef struct {
    uint32_t frequency;
    uint32_t detection_time;
    float signal_strength;
    float noise_floor;
    float snr;
    float confidence;
    bool is_tesla;
    char vehicle_type[32];
    char security_type[32];
} SignalContact;

// Signal Intelligence Environment
typedef struct {
    SignalContact signals[MAX_SIGNALS];
    size_t signal_count;
    uint32_t scan_start_time;
    uint32_t scan_duration_ms;
    ThreatLevel threat_level;
} SigIntEnvironment;

// Attack Opportunity Assessment
typedef struct {
    uint32_t target_frequency;
    float signal_strength;
    float confidence;
    float success_probability;
    uint8_t attack_priority;
    AttackType recommended_attack;
    char target_type[32];
} AttackOpportunity;

// Continuous Signal Monitor
typedef struct {
    bool active;
    uint32_t start_time;
    uint32_t last_scan_time;
    uint32_t scan_interval_ms;
    uint32_t scan_count;
    uint32_t total_detections;
    uint32_t tesla_detections;
    float detection_rate;
} SigIntMonitor;

// Waterfall Display Data
typedef struct {
    uint32_t center_frequency;
    uint32_t span_hz;
    uint32_t timestamp;
    uint8_t spectrum_data[WATERFALL_WIDTH];
} WaterfallData;

// Signal Demodulation Result
typedef struct {
    uint32_t frequency;
    uint32_t demod_time;
    ModulationType modulation_type;
    uint32_t symbol_rate;
    uint8_t data_length;
    uint8_t demodulated_data[MAX_DEMOD_DATA];
    char protocol[32];
    float confidence;
} DemodulationResult;

// Signal Intelligence Functions
bool predator_sigint_analyze_environment(struct PredatorApp* app, SigIntEnvironment* environment);

bool predator_sigint_analyze_frequency(struct PredatorApp* app, uint32_t frequency, SignalContact* contact);

bool predator_sigint_assess_attack_opportunities(struct PredatorApp* app, SigIntEnvironment* environment, 
                                               AttackOpportunity* opportunities, size_t* opp_count);

bool predator_sigint_continuous_monitor(struct PredatorApp* app, SigIntMonitor* monitor);

ThreatLevel predator_sigint_assess_threat_level(SigIntEnvironment* environment);

const char* predator_sigint_threat_level_string(ThreatLevel level);

bool predator_sigint_generate_waterfall(struct PredatorApp* app, uint32_t center_freq, 
                                       uint32_t span_hz, WaterfallData* waterfall);

bool predator_sigint_demodulate_signal(struct PredatorApp* app, uint32_t frequency, 
                                     DemodulationResult* result);
