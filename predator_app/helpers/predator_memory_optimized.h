#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

// Forward declare core app to avoid circular dependency
struct PredatorApp;

// Watchdog functions (minimal implementation)
void predator_watchdog_init(void);
void predator_watchdog_start(void);
void predator_watchdog_tick(void);
void predator_watchdog_stop(void);

// String functions (minimal implementation)
char* predator_get_next_field(const char* str, int field_index, char delimiter);

// UI callback functions (minimal implementation)
void predator_ui_callback_generic(void* context, uint32_t index);

// Scene utils (minimal implementation)
bool predator_scene_transition_safe(struct PredatorApp* app, uint32_t next_scene);

// Memory optimization utilities
void predator_memory_cleanup(struct PredatorApp* app);
size_t predator_memory_get_free(void);
