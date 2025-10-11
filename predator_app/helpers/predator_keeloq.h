#pragma once
#include <stdint.h>
#include <stdbool.h>

// REAL KEELOQ CIPHER IMPLEMENTATION
// Based on published algorithm (Microchip AN642)
// Used in: Chrysler, Dodge, Jeep, Volvo, Jaguar, Honda

typedef struct PredatorApp PredatorApp;

// KeeLoq algorithm constants
#define KEELOQ_NLF  0x3A5C742E  // Non-linear function

// Manufacturer keys (from public leaks)
typedef enum {
    Keeloq_Chrysler,
    Keeloq_GM,
    Keeloq_Honda,
    Keeloq_Toyota,
    Keeloq_Ford,
    Keeloq_Generic,
} KeeloqManufacturer;

// KeeLoq encryption/decryption
uint32_t keeloq_encrypt(uint32_t data, uint64_t key);
uint32_t keeloq_decrypt(uint32_t data, uint64_t key);

// Get manufacturer key
uint64_t keeloq_get_manufacturer_key(KeeloqManufacturer mfr);

// Rolling code prediction
uint32_t keeloq_predict_next_code(uint32_t captured_code, uint64_t key);
bool keeloq_is_valid_code(uint32_t code, uint32_t serial);

// Attack functions
bool predator_keeloq_attack(PredatorApp* app, uint32_t captured_code, KeeloqManufacturer mfr);
