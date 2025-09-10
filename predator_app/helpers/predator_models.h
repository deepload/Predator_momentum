#pragma once

#include <stdint.h>
#include <stddef.h>
#include <storage/storage.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    char make[24];
    char model[48];
    uint32_t frequency; // Hz
    char remote_type[24];
} PredatorCarModel;

// Returns number of models currently loaded from CSV (0 if none)
size_t predator_models_count(void);

// Returns pointer to model at index (NULL if out of range)
const PredatorCarModel* predator_models_get(size_t index);

// Clear any loaded models and free memory
void predator_models_clear(void);

// Load models from a CSV file at path. Returns number of models loaded (0 on failure)
// CSV format: make,model,frequency,remote_type
// frequency accepts integer Hz (e.g., 433920000) or with units (e.g., 433.92MHz, 315MHz, 868.35MHz)
size_t predator_models_load_csv(Storage* storage, const char* path);

// Load models from default path: /ext/apps_data/predator/car_models.csv
size_t predator_models_load_default(Storage* storage);

// Ensure default CSV exists with at least min_count rows at /ext/apps_data/predator/car_models.csv
// Returns number of rows present after ensuring (0 on failure)
size_t predator_models_ensure_seed(Storage* storage, size_t min_count);

#ifdef __cplusplus
}
#endif
