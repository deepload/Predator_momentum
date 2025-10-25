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

// =====================================================
// RFID/NFC CARD PROTOCOLS
// =====================================================

// Calypso Card Context (French transport cards)
typedef struct {
    uint8_t card_id[8];        // Card serial number
    uint8_t sam_key[16];       // SAM diversified key
    uint32_t balance;          // Current balance in cents
    uint16_t transaction_counter; // Transaction counter
    uint8_t network_id;        // Transport network identifier
} CalypsoContext;

// MIFARE Classic Context
typedef struct {
    uint8_t uid[4];            // Card UID
    uint8_t sector_keys[16][6]; // Sector keys (A keys)
    uint8_t access_bits[16][4]; // Access conditions per sector
    bool key_found[16];        // Which sector keys are known
} MifareClassicContext;

// MIFARE DESFire Context
typedef struct {
    uint8_t uid[7];            // Card UID (can be 4 or 7 bytes)
    uint8_t master_key[16];    // Master application key
    uint32_t app_id;           // Application ID
    uint8_t file_keys[8][16];  // File-specific keys
} MifareDesfireContext;

// Calypso card functions
bool predator_crypto_calypso_authenticate(CalypsoContext* ctx, uint8_t* challenge, uint8_t* response);
bool predator_crypto_calypso_read_balance(CalypsoContext* ctx, uint32_t* balance);
bool predator_crypto_calypso_clone_card(CalypsoContext* src, CalypsoContext* dst);

// MIFARE Classic functions
bool predator_crypto_mifare_classic_crack_key(MifareClassicContext* ctx, uint8_t sector, uint8_t* key_out);
bool predator_crypto_mifare_classic_read_sector(MifareClassicContext* ctx, uint8_t sector, uint8_t* data);
bool predator_crypto_mifare_classic_clone_card(MifareClassicContext* src, MifareClassicContext* dst);

// MIFARE DESFire functions
bool predator_crypto_mifare_desfire_authenticate(MifareDesfireContext* ctx, uint8_t key_id);
bool predator_crypto_mifare_desfire_read_file(MifareDesfireContext* ctx, uint8_t file_id, uint8_t* data);

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

// Original 6 manufacturers
bool predator_crypto_format_toyota_packet(uint8_t cmd, uint32_t serial, RFPacket* packet);
bool predator_crypto_format_honda_packet(uint8_t cmd, uint32_t serial, RFPacket* packet);
bool predator_crypto_format_ford_packet(uint8_t cmd, uint32_t serial, RFPacket* packet);
bool predator_crypto_format_bmw_packet(uint8_t cmd, uint32_t serial, RFPacket* packet);
bool predator_crypto_format_mercedes_packet(uint8_t cmd, uint32_t serial, RFPacket* packet);
bool predator_crypto_format_tesla_packet(uint8_t cmd, uint32_t serial, RFPacket* packet);

// Extended manufacturers - ALL MAJOR BRANDS (40+ total)
bool predator_crypto_format_audi_packet(uint8_t cmd, uint32_t serial, RFPacket* packet);
bool predator_crypto_format_volkswagen_packet(uint8_t cmd, uint32_t serial, RFPacket* packet);
bool predator_crypto_format_nissan_packet(uint8_t cmd, uint32_t serial, RFPacket* packet);
bool predator_crypto_format_hyundai_packet(uint8_t cmd, uint32_t serial, RFPacket* packet);
bool predator_crypto_format_kia_packet(uint8_t cmd, uint32_t serial, RFPacket* packet);
bool predator_crypto_format_chevrolet_packet(uint8_t cmd, uint32_t serial, RFPacket* packet);
bool predator_crypto_format_subaru_packet(uint8_t cmd, uint32_t serial, RFPacket* packet);
bool predator_crypto_format_mazda_packet(uint8_t cmd, uint32_t serial, RFPacket* packet);
bool predator_crypto_format_lexus_packet(uint8_t cmd, uint32_t serial, RFPacket* packet);
bool predator_crypto_format_infiniti_packet(uint8_t cmd, uint32_t serial, RFPacket* packet);
bool predator_crypto_format_acura_packet(uint8_t cmd, uint32_t serial, RFPacket* packet);
bool predator_crypto_format_cadillac_packet(uint8_t cmd, uint32_t serial, RFPacket* packet);
bool predator_crypto_format_porsche_packet(uint8_t cmd, uint32_t serial, RFPacket* packet);
bool predator_crypto_format_jaguar_packet(uint8_t cmd, uint32_t serial, RFPacket* packet);
bool predator_crypto_format_landrover_packet(uint8_t cmd, uint32_t serial, RFPacket* packet);

// European manufacturers
bool predator_crypto_format_renault_packet(uint8_t cmd, uint32_t serial, RFPacket* packet);
bool predator_crypto_format_peugeot_packet(uint8_t cmd, uint32_t serial, RFPacket* packet);
bool predator_crypto_format_citroen_packet(uint8_t cmd, uint32_t serial, RFPacket* packet);
bool predator_crypto_format_fiat_packet(uint8_t cmd, uint32_t serial, RFPacket* packet);
bool predator_crypto_format_alfa_romeo_packet(uint8_t cmd, uint32_t serial, RFPacket* packet);
bool predator_crypto_format_volvo_packet(uint8_t cmd, uint32_t serial, RFPacket* packet);
bool predator_crypto_format_saab_packet(uint8_t cmd, uint32_t serial, RFPacket* packet);
bool predator_crypto_format_skoda_packet(uint8_t cmd, uint32_t serial, RFPacket* packet);
bool predator_crypto_format_seat_packet(uint8_t cmd, uint32_t serial, RFPacket* packet);

// Asian manufacturers
bool predator_crypto_format_mitsubishi_packet(uint8_t cmd, uint32_t serial, RFPacket* packet);
bool predator_crypto_format_suzuki_packet(uint8_t cmd, uint32_t serial, RFPacket* packet);
bool predator_crypto_format_isuzu_packet(uint8_t cmd, uint32_t serial, RFPacket* packet);
bool predator_crypto_format_daihatsu_packet(uint8_t cmd, uint32_t serial, RFPacket* packet);

// American manufacturers
bool predator_crypto_format_buick_packet(uint8_t cmd, uint32_t serial, RFPacket* packet);
bool predator_crypto_format_gmc_packet(uint8_t cmd, uint32_t serial, RFPacket* packet);
bool predator_crypto_format_lincoln_packet(uint8_t cmd, uint32_t serial, RFPacket* packet);
bool predator_crypto_format_chrysler_packet(uint8_t cmd, uint32_t serial, RFPacket* packet);
bool predator_crypto_format_dodge_packet(uint8_t cmd, uint32_t serial, RFPacket* packet);
bool predator_crypto_format_jeep_packet(uint8_t cmd, uint32_t serial, RFPacket* packet);
bool predator_crypto_format_ram_packet(uint8_t cmd, uint32_t serial, RFPacket* packet);

// Luxury manufacturers
bool predator_crypto_format_bentley_packet(uint8_t cmd, uint32_t serial, RFPacket* packet);
bool predator_crypto_format_rollsroyce_packet(uint8_t cmd, uint32_t serial, RFPacket* packet);
bool predator_crypto_format_aston_martin_packet(uint8_t cmd, uint32_t serial, RFPacket* packet);
bool predator_crypto_format_ferrari_packet(uint8_t cmd, uint32_t serial, RFPacket* packet);
bool predator_crypto_format_lamborghini_packet(uint8_t cmd, uint32_t serial, RFPacket* packet);
bool predator_crypto_format_maserati_packet(uint8_t cmd, uint32_t serial, RFPacket* packet);
bool predator_crypto_format_mclaren_packet(uint8_t cmd, uint32_t serial, RFPacket* packet);

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
