#include "predator_ai_optimizer.h"
#include "../predator_i.h"
#include <furi.h>

// MEMORY-OPTIMIZED AI IMPLEMENTATION
// Total memory usage: ~64 bytes (extremely lightweight)

// Global AI data (static allocation, no malloc)
static AILearningData ai_data = {0};
static bool ai_initialized = false;

bool predator_ai_init(PredatorApp* app) {
    if(!app) return false;
    
    // Initialize with safe defaults (no dynamic allocation)
    ai_data.car_attacks.success_rate = 50;
    ai_data.car_attacks.optimal_power = 128;
    ai_data.car_attacks.best_frequency = 433;
    ai_data.car_attacks.attempts = 0;
    ai_data.car_attacks.successes = 0;
    
    ai_data.wifi_attacks.success_rate = 60;
    ai_data.wifi_attacks.optimal_power = 200;
    ai_data.wifi_attacks.best_frequency = 2412;
    ai_data.wifi_attacks.attempts = 0;
    ai_data.wifi_attacks.successes = 0;
    
    ai_initialized = true;
    
    FURI_LOG_I("AI", "🧠 AI Optimizer initialized (64 bytes memory)");
    return true;
}

void predator_ai_deinit(PredatorApp* app) {
    UNUSED(app);
    ai_initialized = false;
    FURI_LOG_I("AI", "🧠 AI Optimizer deinitialized");
}

// Lightweight optimization using simple heuristics (no complex ML)
bool predator_ai_optimize_car_attack(PredatorApp* app, uint32_t frequency, uint8_t power) {
    if(!app || !ai_initialized) return false;
    
    AttackStats* stats = &ai_data.car_attacks;
    
    // Simple learning: adjust based on success rate
    if(stats->attempts > 10) {
        if(stats->success_rate < 30) {
            // Low success rate - try different frequency
            stats->best_frequency = (frequency + 1000) % 434000;
            stats->optimal_power = (power < 200) ? power + 20 : 255;
        } else if(stats->success_rate > 80) {
            // High success rate - optimize for efficiency
            stats->optimal_power = (power > 50) ? power - 10 : power;
        }
    }
    
    FURI_LOG_D("AI", "🧠 Car attack optimized: freq=%u, power=%u", 
              stats->best_frequency, stats->optimal_power);
    return true;
}

bool predator_ai_optimize_wifi_attack(PredatorApp* app, uint8_t channel, uint8_t power) {
    UNUSED(power); // Suppress unused parameter warning
    if(!app || !ai_initialized) return false;
    
    AttackStats* stats = &ai_data.wifi_attacks;
    
    // Simple channel optimization
    if(stats->attempts > 5) {
        if(stats->success_rate < 40) {
            // Try different channel
            uint16_t new_freq = 2412 + (channel % 13) * 5;
            stats->best_frequency = new_freq;
        }
    }
    
    FURI_LOG_D("AI", "🧠 WiFi attack optimized: freq=%u, power=%u", 
              stats->best_frequency, stats->optimal_power);
    return true;
}

uint32_t predator_ai_get_optimal_frequency(PredatorApp* app, uint8_t attack_type) {
    if(!app || !ai_initialized || attack_type >= AI_ATTACK_COUNT) {
        return (attack_type == AI_ATTACK_CAR) ? 433920000 : 2412000000;
    }
    
    if(attack_type == AI_ATTACK_CAR) {
        return ai_data.car_attacks.best_frequency * 1000; // Convert to Hz
    } else {
        return ai_data.wifi_attacks.best_frequency * 1000000; // Convert to Hz
    }
}

uint8_t predator_ai_get_optimal_power(PredatorApp* app, uint8_t attack_type) {
    if(!app || !ai_initialized || attack_type >= AI_ATTACK_COUNT) {
        return 128; // Safe default
    }
    
    if(attack_type == AI_ATTACK_CAR) {
        return ai_data.car_attacks.optimal_power;
    } else {
        return ai_data.wifi_attacks.optimal_power;
    }
}

void predator_ai_record_result(PredatorApp* app, uint8_t attack_type, bool success) {
    if(!app || !ai_initialized || attack_type >= AI_ATTACK_COUNT) return;
    
    AttackStats* stats = (attack_type == AI_ATTACK_CAR) ? 
                        &ai_data.car_attacks : &ai_data.wifi_attacks;
    
    // Update statistics (prevent overflow)
    if(stats->attempts < UINT32_MAX) {
        stats->attempts++;
        if(success && stats->successes < UINT32_MAX) {
            stats->successes++;
        }
        
        // Recalculate success rate (avoid division by zero)
        if(stats->attempts > 0) {
            stats->success_rate = (stats->successes * 100) / stats->attempts;
        }
    }
    
    FURI_LOG_D("AI", "🧠 Recorded %s result: %s (rate: %u%%)", 
              (attack_type == AI_ATTACK_CAR) ? "car" : "wifi",
              success ? "SUCCESS" : "FAIL", 
              stats->success_rate);
}

// Memory usage report (for debugging)
size_t predator_ai_get_memory_usage(void) {
    return sizeof(AILearningData); // Should be ~32 bytes
}
