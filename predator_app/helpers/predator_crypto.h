#pragma once

#include "../predator_i.h"

// Cryptographic analysis tools
typedef struct {
    uint8_t* data;
    size_t data_len;
    float entropy;
    uint8_t byte_frequency[256];
    bool likely_encrypted;
} CryptoAnalysis;

// Entropy analysis
float predator_crypto_calculate_entropy(const uint8_t* data, size_t data_len);
bool predator_crypto_analyze_data(const uint8_t* data, size_t data_len, CryptoAnalysis* analysis);

// Frequency analysis
void predator_crypto_frequency_analysis(const uint8_t* data, size_t data_len, uint8_t* frequency);
bool predator_crypto_detect_xor_key(const uint8_t* data, size_t data_len, uint8_t* key, size_t* key_len);

// Brute force
typedef void (*CryptoTestCallback)(const uint8_t* key, size_t key_len, bool* found, void* context);
bool predator_crypto_brute_force(PredatorApp* app, size_t key_len, const char* charset, CryptoTestCallback callback, void* context);

// Dictionary attack
bool predator_crypto_dictionary_attack(PredatorApp* app, const char* dict_path, CryptoTestCallback callback, void* context);

// Hash functions
void predator_crypto_md5(const uint8_t* data, size_t data_len, uint8_t* hash);
void predator_crypto_sha1(const uint8_t* data, size_t data_len, uint8_t* hash);
void predator_crypto_sha256(const uint8_t* data, size_t data_len, uint8_t* hash);

// Common crypto operations
bool predator_crypto_xor_decrypt(const uint8_t* data, size_t data_len, const uint8_t* key, size_t key_len, uint8_t* output);
bool predator_crypto_caesar_decrypt(const uint8_t* data, size_t data_len, uint8_t shift, uint8_t* output);
