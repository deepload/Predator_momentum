#include "predator_protocol_analyzer.h"
#include "../predator_i.h"

bool predator_protocol_analyze_signal(PredatorApp* app, const uint32_t* timings, size_t count, ProtocolSignature* signature) {
    if(!app || !timings || !signature) return false;
    FURI_LOG_I("Protocol", "Analyzing signal with %zu timings", count);
    signature->timing_count = count;
    signature->timings = malloc(count * sizeof(uint32_t));
    if(signature->timings) {
        memcpy(signature->timings, timings, count * sizeof(uint32_t));
    }
    return true;
}

EncodingType predator_protocol_detect_encoding(const uint32_t* timings, size_t count) {
    if(!timings || count < 10) return EncodingRaw;
    // Stub: Simple heuristic
    return EncodingManchester;
}

// Timing analysis helper
typedef struct {
    uint32_t short_pulse;
    uint32_t long_pulse;
    uint32_t tolerance;
} TimingProfile;

static TimingProfile analyze_timing(const uint32_t* timings, size_t count) {
    TimingProfile profile = {0};
    
    uint32_t min_pulse = UINT32_MAX;
    uint32_t max_pulse = 0;
    
    for(size_t i = 0; i < count; i++) {
        if(timings[i] < min_pulse) min_pulse = timings[i];
        if(timings[i] > max_pulse) max_pulse = timings[i];
    }
    
    uint32_t mid_point = (min_pulse + max_pulse) / 2;
    uint32_t short_sum = 0, short_count = 0;
    uint32_t long_sum = 0, long_count = 0;
    
    for(size_t i = 0; i < count; i++) {
        if(timings[i] < mid_point) {
            short_sum += timings[i];
            short_count++;
        } else {
            long_sum += timings[i];
            long_count++;
        }
    }
    
    profile.short_pulse = short_count ? short_sum / short_count : 0;
    profile.long_pulse = long_count ? long_sum / long_count : 0;
    profile.tolerance = (profile.long_pulse - profile.short_pulse) / 4;
    
    return profile;
}

// Real Manchester decoder
bool predator_protocol_decode_manchester(const uint32_t* timings, size_t count, uint8_t* decoded, size_t* decoded_len) {
    if(!timings || !decoded || !decoded_len || count < 16) return false;
    
    TimingProfile profile = analyze_timing(timings, count);
    
    FURI_LOG_I("Protocol", "Manchester: Short=%luµs, Long=%luµs, Tol=±%luµs",
               profile.short_pulse, profile.long_pulse, profile.tolerance);
    
    uint8_t current_byte = 0;
    uint8_t bit_count = 0;
    size_t out_idx = 0;
    
    for(size_t i = 0; i < count - 1; i += 2) {
        uint32_t pulse1 = timings[i];
        uint32_t pulse2 = timings[i + 1];
        
        bool bit1_short = (pulse1 < (profile.short_pulse + profile.tolerance));
        bool bit2_short = (pulse2 < (profile.short_pulse + profile.tolerance));
        
        uint8_t decoded_bit;
        if(bit1_short && !bit2_short) {
            decoded_bit = 0;  // Short-Long = 0
        } else if(!bit1_short && bit2_short) {
            decoded_bit = 1;  // Long-Short = 1
        } else {
            FURI_LOG_W("Protocol", "Invalid Manchester at position %zu", i);
            *decoded_len = out_idx;
            return out_idx > 0;
        }
        
        current_byte = (current_byte << 1) | decoded_bit;
        bit_count++;
        
        if(bit_count == 8) {
            decoded[out_idx++] = current_byte;
            current_byte = 0;
            bit_count = 0;
        }
    }
    
    *decoded_len = out_idx;
    FURI_LOG_I("Protocol", "Manchester decoded: %zu bytes", out_idx);
    
    return true;
}

// Real PWM decoder
bool predator_protocol_decode_pwm(const uint32_t* timings, size_t count, uint8_t* decoded, size_t* decoded_len) {
    if(!timings || !decoded || !decoded_len || count < 8) return false;
    
    TimingProfile profile = analyze_timing(timings, count);
    
    FURI_LOG_I("Protocol", "PWM: Short=%luµs, Long=%luµs, Tol=±%luµs",
               profile.short_pulse, profile.long_pulse, profile.tolerance);
    
    uint8_t current_byte = 0;
    uint8_t bit_count = 0;
    size_t out_idx = 0;
    
    for(size_t i = 0; i < count; i++) {
        uint32_t pulse = timings[i];
        
        uint8_t decoded_bit;
        if(pulse < (profile.short_pulse + profile.tolerance)) {
            decoded_bit = 0;  // Short pulse = 0
        } else if(pulse > (profile.long_pulse - profile.tolerance)) {
            decoded_bit = 1;  // Long pulse = 1
        } else {
            // Skip sync/gap pulses
            continue;
        }
        
        current_byte = (current_byte << 1) | decoded_bit;
        bit_count++;
        
        if(bit_count == 8) {
            decoded[out_idx++] = current_byte;
            current_byte = 0;
            bit_count = 0;
        }
    }
    
    // Handle partial byte
    if(bit_count > 0) {
        current_byte <<= (8 - bit_count);
        decoded[out_idx++] = current_byte;
    }
    
    *decoded_len = out_idx;
    FURI_LOG_I("Protocol", "PWM decoded: %zu bytes", out_idx);
    
    return true;
}

bool predator_protocol_fuzz_signal(PredatorApp* app, ProtocolSignature* signature, uint8_t mutation_rate) {
    if(!app || !signature) return false;
    FURI_LOG_I("Protocol", "Fuzzing signal (mutation rate: %u%%)", mutation_rate);
    return true;
}

bool predator_protocol_build_custom(PredatorApp* app, const uint8_t* data, size_t data_len, EncodingType encoding, uint32_t** timings, size_t* timing_count) {
    if(!app || !data || !timings || !timing_count) return false;
    UNUSED(encoding);
    FURI_LOG_I("Protocol", "Building custom protocol");
    *timing_count = data_len * 16; // Stub
    *timings = malloc(*timing_count * sizeof(uint32_t));
    return *timings != NULL;
}
