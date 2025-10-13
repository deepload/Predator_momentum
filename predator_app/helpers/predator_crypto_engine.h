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

// PRODUCTION: Real Protocol Implementations

// Keeloq Rolling Code Algorithm (Used by Chrysler, GM, Honda, VW, etc.)
typedef struct {
    uint64_t manufacturer_key;  // 64-bit manufacturer key
    uint32_t serial_number;     // Device serial
    uint16_t counter;           // Rolling counter
    uint8_t button_code;        // Button pressed
} KeeloqContext;

uint32_t predator_crypto_keeloq_encrypt(uint32_t data, uint64_t key);
uint32_t predator_crypto_keeloq_decrypt(uint32_t data, uint64_t key);
bool predator_crypto_keeloq_generate_packet(KeeloqContext* ctx, uint8_t* packet_out, size_t* len);

// Hitag2 Protocol (Used by BMW, Audi, VW)
typedef struct {
    uint64_t key_uid;          // Unique ID
    uint32_t auth_response;    // Authentication
    uint16_t rolling_code;     // Counter
} Hitag2Context;

bool predator_crypto_hitag2_auth_challenge(Hitag2Context* ctx, uint32_t challenge, uint32_t* response);
bool predator_crypto_hitag2_generate_packet(Hitag2Context* ctx, uint8_t cmd, uint8_t* packet_out, size_t* len);

// Smart Key AES-128 Challenge-Response (Tesla, BMW, Mercedes)
typedef struct {
    uint8_t aes_key[16];       // 128-bit AES key
    uint8_t vehicle_id[8];     // Vehicle identifier
    uint32_t challenge;        // Random challenge
    uint32_t response;         // Encrypted response
} SmartKeyContext;

bool predator_crypto_smart_key_challenge(SmartKeyContext* ctx, uint8_t* challenge_data, size_t len);
bool predator_crypto_smart_key_response(SmartKeyContext* ctx, uint8_t* response_out, size_t* len);
bool predator_crypto_aes128_encrypt(uint8_t* data, uint8_t* key, uint8_t* output);

// Manufacturer-Specific Packet Formats
typedef enum {
    ModulationOOK,    // On-Off Keying
    ModulationASK,    // Amplitude Shift Keying
    ModulationFSK,    // Frequency Shift Keying
    ModulationPSK     // Phase Shift Keying
} ModulationType;

typedef struct {
    uint8_t preamble[8];       // Sync preamble
    uint8_t preamble_len;      // Preamble length
    uint32_t sync_word;        // Sync word
    uint8_t data[64];          // Payload data
    uint8_t data_len;          // Data length
    uint16_t crc;              // CRC checksum
    ModulationType modulation; // Modulation type
    uint32_t bit_rate;         // Bit rate (bps)
} RFPacket;

bool predator_crypto_format_toyota_packet(uint8_t cmd, uint32_t serial, RFPacket* packet);
bool predator_crypto_format_honda_packet(uint8_t cmd, uint32_t serial, RFPacket* packet);
bool predator_crypto_format_ford_packet(uint8_t cmd, uint32_t serial, RFPacket* packet);
bool predator_crypto_format_bmw_packet(uint8_t cmd, uint32_t serial, RFPacket* packet);
bool predator_crypto_format_mercedes_packet(uint8_t cmd, uint32_t serial, RFPacket* packet);
bool predator_crypto_format_tesla_packet(uint8_t cmd, uint32_t serial, RFPacket* packet);

// CRC Calculation
uint16_t predator_crypto_crc16(uint8_t* data, size_t len);
uint8_t predator_crypto_crc8(uint8_t* data, size_t len);

// Advanced Cryptographic Engine Functions
bool predator_crypto_predict_rolling_code(struct PredatorApp* app, uint32_t* captured_codes, 
                                         size_t count, uint32_t* predicted_code);

bool predator_crypto_analyze_tesla_security(struct PredatorApp* app, TeslaSecurityLevel* level);

bool predator_crypto_multi_vector_attack(struct PredatorApp* app, MultiVectorConfig* config);

bool predator_crypto_scan_vulnerabilities(struct PredatorApp* app, VulnScanResult* result);

bool predator_crypto_analyze_signal_spectrum(struct PredatorApp* app, uint32_t frequency, 
                                           SpectrumAnalysis* analysis);
