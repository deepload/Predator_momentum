#pragma once

#include "../predator_i.h"

// Protocol analysis and reverse engineering
typedef struct {
    uint32_t* timings;
    size_t timing_count;
    uint32_t frequency;
    uint8_t bit_count;
    bool manchester_encoded;
    bool differential_encoded;
} ProtocolSignature;

typedef enum {
    EncodingRaw,
    EncodingManchester,
    EncodingDifferentialManchester,
    EncodingPWM,
    EncodingPPM,
} EncodingType;

// Protocol analysis
bool predator_protocol_analyze_signal(PredatorApp* app, const uint32_t* timings, size_t count, ProtocolSignature* signature);
EncodingType predator_protocol_detect_encoding(const uint32_t* timings, size_t count);
bool predator_protocol_decode_manchester(const uint32_t* timings, size_t count, uint8_t* decoded, size_t* decoded_len);
bool predator_protocol_decode_pwm(const uint32_t* timings, size_t count, uint8_t* decoded, size_t* decoded_len);

// Protocol fuzzing
bool predator_protocol_fuzz_signal(PredatorApp* app, ProtocolSignature* signature, uint8_t mutation_rate);
bool predator_protocol_build_custom(PredatorApp* app, const uint8_t* data, size_t data_len, EncodingType encoding, uint32_t** timings, size_t* timing_count);
