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

// Real XOR key detection using statistical analysis
bool predator_crypto_detect_xor(const uint8_t* data, size_t len, uint8_t* key, size_t* key_len) {
    if(!data || !key || !key_len || len < 4) return false;
    
    FURI_LOG_W("Crypto", "========================================");
    FURI_LOG_W("Crypto", "REAL XOR KEY DETECTION ENGINE");
    FURI_LOG_W("Crypto", "========================================");
    FURI_LOG_I("Crypto", "Analyzing %zu bytes for XOR patterns", len);
    
    // REAL XOR KEY DETECTION ALGORITHM
    uint8_t best_key[16] = {0};
    size_t best_key_size = 0;
    float best_score = 0;
    
    // Test key lengths from 1 to 16
    for(size_t test_key_len = 1; test_key_len <= 16 && test_key_len < len; test_key_len++) {
        uint8_t test_key[16] = {0};
        float score = 0;
        
        // Extract potential key by analyzing byte patterns
        for(size_t i = 0; i < test_key_len; i++) {
            uint32_t byte_sum = 0;
            uint32_t count = 0;
            
            // Collect bytes at positions i, i+key_len, i+2*key_len, etc.
            for(size_t j = i; j < len; j += test_key_len) {
                byte_sum += data[j];
                count++;
            // Count byte frequencies at this position
            for(size_t j = i; j < data_len; j += klen) {
                freq[data[j]]++;
            }
            
            // Find most common byte (likely XOR'd with space 0x20)
            uint8_t max_byte = 0;
            uint32_t max_count = 0;
            for(int b = 0; b < 256; b++) {
                if(freq[b] > max_count) {
                    max_count = freq[b];
                    max_byte = b;
                }
            }
            
            // Assume most common byte is space (0x20)
            candidate_key[i] = max_byte ^ 0x20;
        }
        
        // Score this key by checking if result looks like ASCII text
        float score = 0.0f;
        for(size_t i = 0; i < data_len && i < 100; i++) {
            uint8_t decoded = data[i] ^ candidate_key[i % klen];
            // Score based on printable ASCII range
            if(decoded >= 0x20 && decoded <= 0x7E) score += 1.0f;
            if(decoded >= 'a' && decoded <= 'z') score += 0.5f;
            if(decoded >= 'A' && decoded <= 'Z') score += 0.5f;
            if(decoded == ' ') score += 0.3f;
        }
        
        if(score > best_score) {
            best_score = score;
            best_key_len = klen;
            memcpy(best_key, candidate_key, klen);
        }
    }
    
    if(best_score > 50.0f) {
        *key_len = best_key_len;
        memcpy(key, best_key, best_key_len);
        
        FURI_LOG_I("Crypto", "XOR key detected: length=%u, score=%.1f", best_key_len, (double)best_score);
        return true;
    }
    
    FURI_LOG_W("Crypto", "XOR key not detected (best score: %.1f)", (double)best_score);
    return false;
}

bool predator_crypto_brute_force(PredatorApp* app, size_t key_len, const char* charset, CryptoTestCallback callback, void* context) {
    if(!app || !charset || !callback) return false;
    UNUSED(context);
    FURI_LOG_I("Crypto", "Brute force (key_len=%zu)", key_len);
    return false; // Stub: Time-intensive
}

bool predator_crypto_dictionary_attack(PredatorApp* app, const char* dict_path, CryptoTestCallback callback, void* context) {
    if(!app || !dict_path || !callback) return false;
    UNUSED(context);
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
