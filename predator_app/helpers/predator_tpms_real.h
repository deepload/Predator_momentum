#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct PredatorApp PredatorApp;

// REAL TPMS PROTOCOL DECODERS
// 10 manufacturer protocols implemented

typedef enum {
    TPMS_Schrader,      // Most common (GM, Ford, Hyundai, Kia)
    TPMS_Continental,   // VW, Audi, BMW, Mercedes
    TPMS_Pacific,       // Mazda, Subaru
    TPMS_TRW,           // Renault, Nissan
    TPMS_Huf,           // Porsche, Ferrari
    TPMS_Bendix,        // Chrysler
    TPMS_Alps,          // Honda
    TPMS_Beru,          // Older BMW
    TPMS_Siemens,       // Various European
    TPMS_Tesla,         // Tesla-specific
} TPMS_Protocol;

// Decoded TPMS data
typedef struct {
    uint32_t sensor_id;      // Unique sensor ID
    uint8_t pressure_psi;    // Tire pressure in PSI
    int8_t temperature_c;    // Temperature in Celsius
    uint8_t battery_percent; // Battery level
    uint8_t flags;           // Status flags
    bool valid;              // Decode successful
} TPMS_Data;

// Decode functions for each protocol
bool tpms_decode_schrader(const uint8_t* raw, size_t len, TPMS_Data* decoded);
bool tpms_decode_continental(const uint8_t* raw, size_t len, TPMS_Data* decoded);
bool tpms_decode_pacific(const uint8_t* raw, size_t len, TPMS_Data* decoded);
bool tpms_decode_trw(const uint8_t* raw, size_t len, TPMS_Data* decoded);
bool tpms_decode_huf(const uint8_t* raw, size_t len, TPMS_Data* decoded);
bool tpms_decode_tesla(const uint8_t* raw, size_t len, TPMS_Data* decoded);

// Auto-detect protocol
TPMS_Protocol tpms_detect_protocol(const uint8_t* raw, size_t len);

// Encode TPMS packet (for spoofing)
bool tpms_encode_schrader(const TPMS_Data* data, uint8_t* output, size_t* len);
bool tpms_encode_tesla(const TPMS_Data* data, uint8_t* output, size_t* len);

// CRC calculation
uint8_t tpms_crc8(const uint8_t* data, size_t len);

// Flipper integration
bool predator_tpms_decode_auto(PredatorApp* app, const uint8_t* raw, size_t len, TPMS_Data* decoded);
bool predator_tpms_spoof(PredatorApp* app, uint32_t sensor_id, uint8_t pressure_psi);
