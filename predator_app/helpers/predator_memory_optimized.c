#include "predator_memory_optimized.h"
#include "../predator_i.h"
#include "predator_boards.h"
#include "predator_full_detection.h"
#include <furi.h>

// MEMORY OPTIMIZED HELPER - Replaces multiple helpers to save RAM

// Minimal watchdog implementation
static bool watchdog_active = false;

void predator_watchdog_init(void) {
    watchdog_active = true;
}

void predator_watchdog_start(void) {
    watchdog_active = true;
}

void predator_watchdog_tick(void) {
    // Minimal implementation
}

void predator_watchdog_stop(void) {
    watchdog_active = false;
}

// Minimal string helper - replaces predator_string.c
char* predator_get_next_field(const char* str, int field_index, char delimiter) {
    if (!str || field_index < 0) return NULL;
    
    static char buffer[64]; // REDUCED from 128
    memset(buffer, 0, sizeof(buffer));
    
    const char* current = str;
    int current_field = 0;
    
    // Skip to the desired field
    while (current_field < field_index && *current) {
        if (*current == delimiter) {
            current_field++;
        }
        current++;
    }
    
    if (current_field != field_index || !*current) {
        return NULL;
    }
    
    // Copy the field content
    int i = 0;
    while (*current && *current != delimiter && i < (int)sizeof(buffer) - 1) {
        buffer[i++] = *current++;
    }
    buffer[i] = '\0';
    
    return buffer;
}

// Minimal UI callbacks - essential only
void predator_ui_callback_generic(void* context, uint32_t index) {
    PredatorApp* app = context;
    if(app && app->view_dispatcher) {
        view_dispatcher_send_custom_event(app->view_dispatcher, index);
    }
}

// Minimal scene utils - essential only
bool predator_scene_transition_safe(PredatorApp* app, uint32_t next_scene) {
    if(!app) return false;
    
    // Simplified - always allow transition for memory optimization
    (void)next_scene; // Suppress unused parameter warning
    return true;
}

// Memory optimization utilities
void predator_memory_cleanup(PredatorApp* app) {
    if(!app) return;
    
    // Force garbage collection if available
    // This is a placeholder for potential memory cleanup
}

size_t predator_memory_get_free(void) {
    // Return estimated free memory (simplified for memory optimization)
    return 8192; // Conservative estimate
}

// SIMPLE DETECTION STUBS - Memory optimized
bool predator_full_detection_scan(PredatorApp* app) { UNUSED(app); return true; }
bool predator_full_detection_check_attack_requirements(PredatorApp* app, PredatorAttackType attack_type, char* error_msg, size_t error_size) { UNUSED(app); UNUSED(attack_type); UNUSED(error_msg); UNUSED(error_size); return true; }
void predator_full_detection_get_detailed_status(PredatorApp* app, char* status, size_t status_size) { UNUSED(app); if(status) snprintf(status, status_size, "Board: Ready"); }
bool predator_full_detection_show_requirements_popup(PredatorApp* app, PredatorAttackType attack_type) { UNUSED(app); UNUSED(attack_type); return true; }
