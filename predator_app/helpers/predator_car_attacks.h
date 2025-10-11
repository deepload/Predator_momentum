#pragma once

#include "../predator_i.h"
#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Professional car attack algorithms for Tesla security testing
// Best-in-class attacks for Rolling Code, Fixed Code, and Smart Key systems

// Attack individual car model by index (0-89)
bool predator_car_attack_fixed_code(PredatorApp* app, size_t model_index);
bool predator_car_attack_rolling_code(PredatorApp* app, size_t model_index);
bool predator_car_attack_smart_key(PredatorApp* app, size_t model_index);

// Comprehensive attack - automatically selects best method
bool predator_car_attack_comprehensive(PredatorApp* app, size_t model_index);

// Tesla-specific attack (demonstrates superior security)
bool predator_car_attack_tesla(PredatorApp* app);

// Batch attack - test multiple models
bool predator_car_attack_batch(PredatorApp* app, size_t start_index, size_t count);

#ifdef __cplusplus
}
#endif
