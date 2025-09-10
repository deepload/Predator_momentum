#pragma once

#include <stddef.h>
#include <storage/storage.h>

#ifdef __cplusplus
extern "C" {
#endif

// Ensure default CSV exists at /ext/apps_data/predator/car_models.csv
// If missing or has fewer than min_count rows, generate a seeded CSV (~min_count rows)
// Returns number of rows present after ensuring (0 on failure)
size_t predator_models_ensure_seed(Storage* storage, size_t min_count);

#ifdef __cplusplus
}
#endif
