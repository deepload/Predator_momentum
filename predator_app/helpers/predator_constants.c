#include "predator_constants.h"

// SHARED CONSTANTS - Memory Optimization Implementation
// Single definition for all shared arrays

// Common Frequencies (Used by 8+ scenes) - OPTIMIZED MACROS
#define F315 315000000
#define F433 433920000  
#define F868 868350000
#define F915 915000000

const uint32_t PREDATOR_FREQUENCIES[] = {
    F315,  // 315 MHz - North America
    F433,  // 433.92 MHz - Europe/Asia  
    F868,  // 868.35 MHz - Europe Premium
    F915   // 915 MHz - US ISM
};

const char* PREDATOR_FREQUENCY_NAMES[] = {
    "315 MHz",
    "433.92 MHz", 
    "868.35 MHz",
    "915 MHz"
};

// Board Names (Used by 4+ scenes)
const char* PREDATOR_BOARD_NAMES[] = {
    "Unknown Board",
    "Original Predator",
    "3in1 AIO V1.4",
    "DrB0rk Multi V2", 
    "3in1 NRF24+CC1101",
    "2.8\" Screen",
    "Auto-Detect"
};

const char* PREDATOR_BOARD_DESCRIPTIONS[] = {
    "Manual config",
    "Basic RF only",
    "WiFi + GPS + RF",
    "Multi-protocol",
    "NRF24 + CC1101",
    "Standalone + Battery",
    "Hardware detection"
};

// Region Names (Settings)
const char* PREDATOR_REGION_NAMES[] = {
    "Auto",
    "US/CA", 
    "EU",
    "UNBLOCK"
};
