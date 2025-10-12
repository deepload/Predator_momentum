#pragma once

#include <stdint.h>

// SHARED CONSTANTS - Memory Optimization
// Eliminates duplicate arrays across 30+ scene files

// Common Frequencies (Used by 8+ scenes)
extern const uint32_t PREDATOR_FREQUENCIES[];
extern const char* PREDATOR_FREQUENCY_NAMES[];
#define PREDATOR_FREQUENCY_COUNT 4

// Board Names (Used by 4+ scenes)
extern const char* PREDATOR_BOARD_NAMES[];
extern const char* PREDATOR_BOARD_DESCRIPTIONS[];
#define PREDATOR_BOARD_COUNT 7

// Region Names (Settings)
extern const char* PREDATOR_REGION_NAMES[];
#define PREDATOR_REGION_COUNT 4

// Generic Scene State (Replaces 30+ custom structs)
typedef enum {
    PredatorSceneStatusIdle,
    PredatorSceneStatusRunning,
    PredatorSceneStatusComplete,
    PredatorSceneStatusError
} PredatorSceneStatus;

typedef struct {
    PredatorSceneStatus status;
    uint32_t counter_primary;   // packets_sent, devices_found, etc.
    uint32_t counter_secondary; // time_ms, codes_tried, etc.
    uint32_t start_tick;
    char target_name[32];       // ssid, card_type, etc.
    uint8_t mode_index;         // current mode/frequency index
} PredatorGenericSceneState;
