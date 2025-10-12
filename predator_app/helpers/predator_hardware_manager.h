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

// PROFESSIONAL HARDWARE MANAGER
// Validates hardware availability and shows clear error messages

// Validate all hardware for current board configuration
bool predator_hardware_validate_board(struct PredatorApp* app);

// Check if hardware requirements are met for specific attack type
// Returns false and fills error_msg if requirements not met
bool predator_hardware_check_requirements(struct PredatorApp* app, PredatorAttackType attack_type, char* error_msg, size_t error_size);

// Get comprehensive hardware status summary
void predator_hardware_get_status_summary(struct PredatorApp* app, char* summary, size_t summary_size);

// Show professional popup with hardware requirements if not met
// Returns true if requirements met, false if popup shown
bool predator_hardware_show_requirements_popup(struct PredatorApp* app, PredatorAttackType attack_type);
