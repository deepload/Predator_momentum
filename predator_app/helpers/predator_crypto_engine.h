#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// Forward declaration
struct PredatorApp;

// Tesla Security Levels
typedef enum {
    TeslaSecurityStandard,
    TeslaSecurityHigh,
    TeslaSecurityMaximum
} TeslaSecurityLevel;

// Attack Vector Types
typedef enum {
    AttackTypeRollingCode,
    AttackTypeFixedCode,
    AttackTypeSmartKey,
    AttackTypeJamming
} AttackType;

// Vulnerability Types
typedef enum {
    VulnWeakRollingCode,
    VulnReplayAttack,
    VulnJammingAttack,
    VulnCryptoWeakness
} VulnerabilityType;

// Attack Vector Configuration
typedef struct {
    AttackType type;
    uint32_t frequency;
    uint32_t delay_ms;
    uint8_t power_level;
} AttackVector;

// Multi-Vector Attack Configuration
#define MAX_ATTACK_VECTORS 8
typedef struct {
    AttackVector vectors[MAX_ATTACK_VECTORS];
    size_t attack_count;
    bool simultaneous_mode;
} MultiVectorConfig;

// Vulnerability Scan Result
#define MAX_VULNERABILITIES 16
typedef struct {
    VulnerabilityType vulnerabilities[MAX_VULNERABILITIES];
    size_t vuln_count;
    uint32_t scan_start_time;
    uint32_t scan_duration_ms;
    uint8_t risk_score;
} VulnScanResult;

// Spectrum Analysis Result
typedef struct {
    uint32_t center_frequency;
    uint32_t analysis_time;
    float signal_strength;
    float noise_floor;
    float snr;
    bool modulation_detected;
    uint32_t bandwidth_hz;
    uint32_t symbol_rate;
    char signal_type[32];
    float confidence;
} SpectrumAnalysis;

// Advanced Cryptographic Engine Functions
bool predator_crypto_predict_rolling_code(struct PredatorApp* app, uint32_t* captured_codes, 
                                         size_t count, uint32_t* predicted_code);

bool predator_crypto_analyze_tesla_security(struct PredatorApp* app, TeslaSecurityLevel* level);

bool predator_crypto_multi_vector_attack(struct PredatorApp* app, MultiVectorConfig* config);

bool predator_crypto_scan_vulnerabilities(struct PredatorApp* app, VulnScanResult* result);

bool predator_crypto_analyze_signal_spectrum(struct PredatorApp* app, uint32_t frequency, 
                                           SpectrumAnalysis* analysis);

// Helper functions - remove static declarations to avoid unused function errors
