#pragma once

#include <furi.h>
#include <gui/view_dispatcher.h>
#include <gui/scene_manager.h>

// Forward declaration to avoid circular dependency
typedef struct PredatorApp PredatorApp;

/**
 * @brief Safe Navigation Pattern for Predator Momentum
 * 
 * This header defines the standardized navigation patterns to prevent
 * the app from quitting unexpectedly during scene transitions.
 * 
 * CRITICAL: All scenes must follow these patterns to ensure stable navigation
 * for Tesla/Government demonstrations.
 */

/**
 * @brief Standard back navigation pattern for input callbacks
 * 
 * Use this pattern in custom view input callbacks:
 * 
 * ```c
 * static bool my_scene_input_callback(InputEvent* event, void* context) {
 *     PredatorApp* app = context;
 *     if(!app) return false;
 *     
 *     if(event->type == InputTypeShort && event->key == InputKeyBack) {
 *         // Perform any necessary cleanup here
 *         cleanup_scene_state();
 *         
 *         // CRITICAL: Return false to let scene manager handle navigation
 *         return false;
 *     }
 *     
 *     // Handle other input events...
 *     return true;
 * }
 * ```
 */

/**
 * @brief Standard back navigation pattern for scene event handlers
 * 
 * Use this pattern in scene on_event functions:
 * 
 * ```c
 * bool my_scene_on_event(void* context, SceneManagerEvent event) {
 *     PredatorApp* app = context;
 *     if(!app) return false;
 *     
 *     if(event.type == SceneManagerEventTypeBack) {
 *         // Navigate to previous scene
 *         scene_manager_previous_scene(app->scene_manager);
 *         return true;
 *     }
 *     
 *     // Handle other events...
 *     return false;
 * }
 * ```
 */

/**
 * @brief Navigation Rules for Tesla/Government Grade Stability
 * 
 * 1. NEVER call scene_manager_stop() except from main menu
 * 2. NEVER call view_dispatcher_stop() except from main menu  
 * 3. Input callbacks should return FALSE for back button to let scene manager handle
 * 4. Scene event handlers should call scene_manager_previous_scene() for back events
 * 5. Always perform cleanup before returning false from input callbacks
 * 6. Use 500ms back-debounce for professional stabilization where needed
 * 7. Multi-screen scenes should use custom events (999) to communicate with scene manager
 */

/**
 * @brief Back-debounce helper for professional stabilization
 */
static inline bool predator_navigation_back_debounce(uint32_t* last_back_press, uint32_t debounce_ms) {
    uint32_t current_tick = furi_get_tick();
    if(current_tick - *last_back_press < debounce_ms) {
        return true; // Debounced - ignore this press
    }
    *last_back_press = current_tick;
    return false; // Not debounced - process this press
}

// Note: The following functions require the full PredatorApp definition
// They should be used as patterns in scenes that include predator_i.h

/**
 * @brief Safe scene transition helper - use this pattern:
 * if(app && app->scene_manager) {
 *     scene_manager_previous_scene(app->scene_manager);
 * }
 */
#define PREDATOR_SAFE_PREVIOUS_SCENE(app) \
    do { \
        if((app) && (app)->scene_manager) { \
            scene_manager_previous_scene((app)->scene_manager); \
        } \
    } while(0)

/**
 * @brief Safe app exit helper - use this pattern (ONLY from main menu):
 * if(app && app->scene_manager && app->view_dispatcher) {
 *     scene_manager_stop(app->scene_manager);
 *     view_dispatcher_stop(app->view_dispatcher);
 * }
 */
#define PREDATOR_SAFE_EXIT_APP(app) \
    do { \
        if((app) && (app)->scene_manager && (app)->view_dispatcher) { \
            scene_manager_stop((app)->scene_manager); \
            view_dispatcher_stop((app)->view_dispatcher); \
        } \
    } while(0)
