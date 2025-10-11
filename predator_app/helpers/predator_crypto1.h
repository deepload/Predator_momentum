#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct PredatorApp PredatorApp;

// REAL CRYPTO1 CIPHER IMPLEMENTATION
// MIFARE Classic broken cipher (CVE-2008-0166)
// Used to crack MIFARE Classic 1K/4K cards

// Crypto1 state (48-bit LFSR)
typedef struct {
    uint64_t state;  // 48-bit state
} Crypto1State;

// Initialize Crypto1 with key
void crypto1_init(Crypto1State* s, uint64_t key);

// Generate keystream bit
uint8_t crypto1_bit(Crypto1State* s, uint8_t in, uint8_t is_encrypted);

// Generate keystream byte
uint8_t crypto1_byte(Crypto1State* s, uint8_t in, uint8_t is_encrypted);

// Filter function (f)
uint8_t crypto1_filter(uint64_t state);

// LFSR feedback function
uint8_t crypto1_lfsr_feedback(uint64_t state);

// Authentication word generation
uint32_t crypto1_word(Crypto1State* s, uint32_t in, uint8_t is_encrypted);

// Nested attack functions
bool crypto1_nested_attack(
    const uint32_t* nonces,
    size_t count,
    uint64_t known_key,
    uint64_t* target_key
);

// Darkside attack
bool crypto1_darkside_attack(
    const uint32_t* nonces,
    size_t count,
    uint64_t* recovered_key
);

// Key recovery from nonces
bool crypto1_crack_nonces(
    const uint32_t* nonces,
    size_t count,
    uint64_t* key
);

// Flipper integration (renamed to avoid conflicts with predator_mifare.c)
bool predator_crypto1_nested_attack(PredatorApp* app, uint8_t target_sector);
bool predator_crypto1_darkside_attack(PredatorApp* app);
