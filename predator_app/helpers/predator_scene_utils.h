#pragma once

#include "../predator_i.h"
#include "../scenes/predator_scene.h"
#include <furi.h>
#include <gui/modules/popup.h>

/**
 * @brief Initialize hardware for a scene with appropriate fallback behavior
 * 
 * This utility ensures consistent hardware initialization across all scenes
 * with proper fallbacks for multiboards if hardware init fails.
 * 
 * @param app Application context
 * @param init_callback Function to initialize hardware (e.g., predator_esp32_init)
 * @param popup_callback Callback for popup back button
 * @param is_initialized Function to check if hardware is initialized (should return bool)
 * @param hardware_name Name of the hardware to display in error messages
 * @return true if initialization succeeded or fallback was applied, false otherwise
 */
bool predator_scene_ensure_hardware(
    PredatorApp* app, 
    void (*init_callback)(PredatorApp*), 
    PopupCallback popup_callback,
    bool (*is_initialized)(PredatorApp*),
    const char* hardware_name);

/**
 * @brief Safely handle scene transitions
 * 
 * Properly handles scene transitions when a hardware error or other exception occurs
 * 
 * @param app Application context
 * @param next_scene Scene to transition to
 * @return true if transition can proceed, false if an error was displayed instead
 */
bool predator_scene_transition_safe(PredatorApp* app, uint32_t next_scene);
