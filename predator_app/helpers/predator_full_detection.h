#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

// Forward declare to avoid circular dependency
struct PredatorApp;

// Attack types for hardware requirement checking
typedef enum {
    PredatorAttackWiFi,
    PredatorAttackBluetooth,
    PredatorAttackSubGHz,
    PredatorAttackRFID,
    PredatorAttackGPS,
    PredatorAttackWardriving,
} PredatorAttackType;

// FULL HARDWARE DETECTION SYSTEM
// Real-world hardware detection with detailed status and error messages

// Perform comprehensive hardware detection scan
// Returns true if scan completed successfully
bool predator_full_detection_scan(struct PredatorApp* app);

// Check if all hardware requirements are met for specific attack type
// Returns false and fills error_msg with detailed requirements if not met
bool predator_full_detection_check_attack_requirements(struct PredatorApp* app, PredatorAttackType attack_type, char* error_msg, size_t error_size);

// Get comprehensive hardware status with detailed information
void predator_full_detection_get_detailed_status(struct PredatorApp* app, char* status, size_t status_size);

// Show professional popup with detailed hardware requirements if not met
// Returns true if requirements met, false if popup shown with detailed error
bool predator_full_detection_show_requirements_popup(struct PredatorApp* app, PredatorAttackType attack_type);
