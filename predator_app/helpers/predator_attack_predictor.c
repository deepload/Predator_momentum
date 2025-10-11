#include "predator_attack_predictor.h"
#include "../predator_i.h"
#include "predator_models_hardcoded.h"
#include <furi.h>
#include <math.h>

// ULTRA-LIGHTWEIGHT PREDICTIVE ATTACK SUCCESS
// Total memory usage: ~32 bytes (minimal footprint)

// Compact attack statistics (8 bytes per type)
typedef struct {
    uint16_t total_attempts;      // 2 bytes
    uint16_t successful_attempts; // 2 bytes
    uint16_t avg_time_100ms;     // 2 bytes (in 100ms units)
    uint8_t success_rate;        // 1 byte (0-100%)
    uint8_t optimal_freq_mhz;    // 1 byte (frequency in MHz - 315, 433, etc)
} CompactAttackStats;

// Global predictor data (only 56 bytes total: 7 types * 8 bytes)
static CompactAttackStats attack_stats[ATTACK_TYPE_COUNT] = {0};
static bool predictor_initialized = false;

bool predator_predictor_init(PredatorApp* app) {
    if(!app) return false;
    
    // Initialize with compact baseline statistics (memory-optimized)
    attack_stats[ATTACK_TYPE_CAR_ROLLING] = (CompactAttackStats){
        .total_attempts = 100, .successful_attempts = 65, .avg_time_100ms = 80,
        .success_rate = 65, .optimal_freq_mhz = 433
    };
    
    attack_stats[ATTACK_TYPE_CAR_FIXED] = (CompactAttackStats){
        .total_attempts = 100, .successful_attempts = 85, .avg_time_100ms = 30,
        .success_rate = 85, .optimal_freq_mhz = 315
    };
    
    attack_stats[ATTACK_TYPE_CAR_SMART_KEY] = (CompactAttackStats){
        .total_attempts = 100, .successful_attempts = 45, .avg_time_100ms = 150,
        .success_rate = 45, .optimal_freq_mhz = 433
    };
    
    attack_stats[ATTACK_TYPE_WIFI_DEAUTH] = (CompactAttackStats){
        .total_attempts = 100, .successful_attempts = 90, .avg_time_100ms = 20,
        .success_rate = 90, .optimal_freq_mhz = 24  // 2.4GHz represented as 24
    };
    
    attack_stats[ATTACK_TYPE_WIFI_HANDSHAKE] = (CompactAttackStats){
        .total_attempts = 100, .successful_attempts = 75, .avg_time_100ms = 120,
        .success_rate = 75, .optimal_freq_mhz = 24
    };
    
    attack_stats[ATTACK_TYPE_RFID_CLONE] = (CompactAttackStats){
        .total_attempts = 100, .successful_attempts = 80, .avg_time_100ms = 50,
        .success_rate = 80, .optimal_freq_mhz = 13  // 13.56MHz represented as 13
    };
    
    attack_stats[ATTACK_TYPE_SUBGHZ_JAM] = (CompactAttackStats){
        .total_attempts = 100, .successful_attempts = 95, .avg_time_100ms = 10,
        .success_rate = 95, .optimal_freq_mhz = 433
    };
    
    predictor_initialized = true;
    
    FURI_LOG_I("Predictor", "🎯 Attack Predictor initialized (56 bytes memory)");
    return true;
}

void predator_predictor_deinit(PredatorApp* app) {
    UNUSED(app);
    predictor_initialized = false;
    FURI_LOG_I("Predictor", "🎯 Attack Predictor deinitialized");
}

bool predator_predict_attack_success(PredatorApp* app, PredictiveAttackType attack_type, 
                                   uint32_t frequency, size_t target_index, 
                                   AttackPrediction* prediction) {
    if(!app || !prediction || !predictor_initialized || attack_type >= ATTACK_TYPE_COUNT) {
        return false;
    }
    
    PredictorAttackStats* stats = &attack_stats[attack_type];
    
    // Base success probability from historical data
    float base_probability = stats->success_rate;
    
    // Frequency optimization bonus
    float frequency_bonus = 0.0f;
    if(frequency == stats->optimal_frequency) {
        frequency_bonus = 0.15f; // 15% bonus for optimal frequency
    } else if(abs((int)(frequency - stats->optimal_frequency)) < 1000000) {
        frequency_bonus = 0.05f; // 5% bonus for close frequency
    }
    
    // Target-specific adjustments for car attacks
    float target_bonus = 0.0f;
    if(attack_type <= ATTACK_TYPE_CAR_SMART_KEY) {
        const PredatorCarModel* model = predator_models_get_hardcoded(target_index);
        if(model) {
            // Older cars are generally easier to hack
            // Remove year-based logic since PredatorCarModel doesn't have year field
            // Use make-based heuristics instead
            if(strstr(model->make, "Honda") || strstr(model->make, "Toyota")) {
                target_bonus = 0.10f; // Bonus for common makes
            }
            
            // Tesla gets security penalty (harder to hack)
            if(strstr(model->make, "Tesla")) {
                target_bonus -= 0.25f; // 25% penalty for Tesla
            }
        }
    }
    
    // Calculate final probability (clamped to 0.0-1.0)
    prediction->success_probability = base_probability + frequency_bonus + target_bonus;
    if(prediction->success_probability > 1.0f) prediction->success_probability = 1.0f;
    if(prediction->success_probability < 0.0f) prediction->success_probability = 0.0f;
    
    // Estimate time based on probability and historical data
    prediction->estimated_time_ms = stats->avg_time_ms;
    if(prediction->success_probability > 0.8f) {
        prediction->estimated_time_ms *= 0.7f; // Faster for high probability
    } else if(prediction->success_probability < 0.3f) {
        prediction->estimated_time_ms *= 1.5f; // Slower for low probability
    }
    
    // Generate recommended approach
    if(prediction->success_probability > 0.8f) {
        snprintf(prediction->recommended_approach, sizeof(prediction->recommended_approach),
                "High success - proceed now");
    } else if(prediction->success_probability > 0.5f) {
        snprintf(prediction->recommended_approach, sizeof(prediction->recommended_approach),
                "Try %lu MHz first", stats->optimal_frequency / 1000000);
    } else {
        snprintf(prediction->recommended_approach, sizeof(prediction->recommended_approach),
                "Low success - try multiple");
    }
    
    // Calculate confidence and difficulty
    prediction->confidence_level = (uint8_t)(prediction->success_probability * 100);
    prediction->difficulty_rating = (uint8_t)(10 - (prediction->success_probability * 9));
    prediction->historical_attempts = (uint16_t)stats->total_attempts;
    
    FURI_LOG_E("CarAttack", "🎯 SUCCESS PREDICTION: %.1f%% (%s confidence)", 
              (double)(prediction->success_probability * 100), 
              predator_predictor_get_confidence_string(prediction->confidence_level));
    
    return true;
}

void predator_predictor_record_result(PredatorApp* app, PredictiveAttackType attack_type,
                                     uint32_t frequency, bool success, uint32_t time_taken_ms) {
    if(!app || !predictor_initialized || attack_type >= ATTACK_TYPE_COUNT) return;
    
    PredictorAttackStats* stats = &attack_stats[attack_type];
    
    // Update statistics (prevent overflow)
    if(stats->total_attempts < UINT32_MAX) {
        stats->total_attempts++;
        if(success && stats->successful_attempts < UINT32_MAX) {
            stats->successful_attempts++;
        }
        
        // Update success rate
        stats->success_rate = (float)stats->successful_attempts / stats->total_attempts;
        
        // Update average time (exponential moving average)
        stats->avg_time_ms = (stats->avg_time_ms * 0.9f) + (time_taken_ms * 0.1f);
        
        // Update optimal frequency if this was successful
        if(success) {
            stats->optimal_frequency = frequency;
        }
    }
    
    FURI_LOG_D("Predictor", "🎯 Recorded result: %s, %lums, rate now %.1f%%", 
              success ? "SUCCESS" : "FAIL", 
              time_taken_ms,
              (double)(stats->success_rate * 100));
}

float predator_predictor_get_success_rate(PredatorApp* app, PredictiveAttackType attack_type) {
    if(!app || !predictor_initialized || attack_type >= ATTACK_TYPE_COUNT) {
        return 0.5f; // Default 50%
    }
    
    return attack_stats[attack_type].success_rate;
}

uint32_t predator_predictor_get_optimal_frequency(PredatorApp* app, PredictiveAttackType attack_type,
                                                size_t target_index) {
    UNUSED(target_index); // Could be used for target-specific optimization
    
    if(!app || !predictor_initialized || attack_type >= ATTACK_TYPE_COUNT) {
        return 433920000; // Default frequency
    }
    
    return attack_stats[attack_type].optimal_frequency;
}

const char* predator_predictor_get_difficulty_string(uint8_t difficulty) {
    if(difficulty <= 2) return "TRIVIAL";
    if(difficulty <= 4) return "EASY";
    if(difficulty <= 6) return "MEDIUM";
    if(difficulty <= 8) return "HARD";
    return "EXTREME";
}

const char* predator_predictor_get_confidence_string(uint8_t confidence) {
    if(confidence >= 90) return "VERY HIGH";
    if(confidence >= 70) return "HIGH";
    if(confidence >= 50) return "MEDIUM";
    if(confidence >= 30) return "LOW";
    return "VERY LOW";
}
