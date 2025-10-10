#include "predator_crypto.h"
#include "../predator_i.h"
#include <math.h>

float predator_crypto_calculate_entropy(const uint8_t* data, size_t data_len) {
    if(!data || data_len == 0) return 0.0f;
    
    uint32_t frequency[256] = {0};
    for(size_t i = 0; i < data_len; i++) {
        frequency[data[i]]++;
    }
    
    float entropy = 0.0f;
    for(int i = 0; i < 256; i++) {
        if(frequency[i] > 0) {
            float p = (float)frequency[i] / (float)data_len;
            entropy -= p * log2f(p);
        }
    }
    
    return entropy;
}

bool predator_crypto_analyze_data(const uint8_t* data, size_t data_len, CryptoAnalysis* analysis) {
    if(!data || !analysis) return false;
    
    analysis->data = (uint8_t*)data;
    analysis->data_len = data_len;
    analysis->entropy = predator_crypto_calculate_entropy(data, data_len);
    
    memset(analysis->byte_frequency, 0, sizeof(analysis->byte_frequency));
    for(size_t i = 0; i < data_len; i++) {
        analysis->byte_frequency[data[i]]++;
    }
    
    analysis->likely_encrypted = (analysis->entropy > 7.5f);
    
    FURI_LOG_I("Crypto", "Entropy: %.2f (encrypted: %s)", 
               (double)analysis->entropy, 
               analysis->likely_encrypted ? "yes" : "no");
    
    return true;
}

void predator_crypto_frequency_analysis(const uint8_t* data, size_t data_len, uint8_t* frequency) {
    if(!data || !frequency) return;
    
    memset(frequency, 0, 256);
    for(size_t i = 0; i < data_len; i++) {
        frequency[data[i]]++;
    }
}

bool predator_crypto_detect_xor_key(const uint8_t* data, size_t data_len, uint8_t* key, size_t* key_len) {
    if(!data || !key || !key_len) return false;
    FURI_LOG_I("Crypto", "XOR key detection");
    return false; // Stub: Complex algorithm
}

bool predator_crypto_brute_force(PredatorApp* app, size_t key_len, const char* charset, CryptoTestCallback callback, void* context) {
    if(!app || !charset || !callback) return false;
    FURI_LOG_I("Crypto", "Brute force (key_len=%zu)", key_len);
    return false; // Stub: Time-intensive
}

bool predator_crypto_dictionary_attack(PredatorApp* app, const char* dict_path, CryptoTestCallback callback, void* context) {
    if(!app || !dict_path || !callback) return false;
    FURI_LOG_I("Crypto", "Dictionary attack: %s", dict_path);
    return false; // Stub
}

void predator_crypto_md5(const uint8_t* data, size_t data_len, uint8_t* hash) {
    if(!data || !hash) return;
    UNUSED(data_len);
    memset(hash, 0, 16); // Stub
}

void predator_crypto_sha1(const uint8_t* data, size_t data_len, uint8_t* hash) {
    if(!data || !hash) return;
    UNUSED(data_len);
    memset(hash, 0, 20); // Stub
}

void predator_crypto_sha256(const uint8_t* data, size_t data_len, uint8_t* hash) {
    if(!data || !hash) return;
    UNUSED(data_len);
    memset(hash, 0, 32); // Stub
}

bool predator_crypto_xor_decrypt(const uint8_t* data, size_t data_len, const uint8_t* key, size_t key_len, uint8_t* output) {
    if(!data || !key || !output || key_len == 0) return false;
    
    for(size_t i = 0; i < data_len; i++) {
        output[i] = data[i] ^ key[i % key_len];
    }
    
    return true;
}

bool predator_crypto_caesar_decrypt(const uint8_t* data, size_t data_len, uint8_t shift, uint8_t* output) {
    if(!data || !output) return false;
    
    for(size_t i = 0; i < data_len; i++) {
        if(data[i] >= 'A' && data[i] <= 'Z') {
            output[i] = 'A' + (data[i] - 'A' + shift) % 26;
        } else if(data[i] >= 'a' && data[i] <= 'z') {
            output[i] = 'a' + (data[i] - 'a' + shift) % 26;
        } else {
            output[i] = data[i];
        }
    }
    
    return true;
}
