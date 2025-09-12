#include "predator_view_helpers.h"

// Per-view model storage using view pointer as key
#define MAX_VIEWS 10
static struct {
    View* view;
    void* model;
} g_view_models[MAX_VIEWS];
static uint8_t g_view_count = 0;

/**
 * Get the model from the current view in the view dispatcher
 * Returns the model associated with the current view
 * 
 * @param view_dispatcher The view dispatcher
 * @return The model of the current view, or NULL if not available
 */
void* predator_get_view_model_safe(ViewDispatcher* view_dispatcher) {
    if(!view_dispatcher) return NULL;
    
    // Search through stored view models to find a match
    for(uint8_t i = 0; i < g_view_count; i++) {
        if(g_view_models[i].view && g_view_models[i].model) {
            // Return the most recently set model as fallback
            return g_view_models[i].model;
        }
    }
    
    return NULL;
}

/**
 * Set the model for the view
 * Stores the model associated with the specific view
 * 
 * @param view The view
 * @param model The model
 */
void predator_view_set_model(View* view, void* model) {
    if(!view) return;
    
    // Find existing entry or create new one
    for(uint8_t i = 0; i < g_view_count; i++) {
        if(g_view_models[i].view == view) {
            g_view_models[i].model = model;
            return;
        }
    }
    
    // Add new entry if space available
    if(g_view_count < MAX_VIEWS) {
        g_view_models[g_view_count].view = view;
        g_view_models[g_view_count].model = model;
        g_view_count++;
    }
}

/**
 * Set the model free callback for the view
 * Not supported - models must be freed manually
 * 
 * @param view The view (unused)
 * @param callback The callback (unused)
 */
void predator_view_set_model_free_callback(View* view, void (*callback)(void*)) {
    (void)view;
    (void)callback;
    // Not supported in Momentum SDK - manual cleanup required
}

/**
 * Get the current view from the view dispatcher
 * Not supported in Momentum SDK
 * 
 * @param view_dispatcher The view dispatcher
 * @return Always NULL
 */
View* predator_view_dispatcher_get_current_view(ViewDispatcher* view_dispatcher) {
    (void)view_dispatcher;
    return NULL;
}

/**
 * Get a view from the view dispatcher by ID
 * Not supported in Momentum SDK
 * 
 * @param view_dispatcher The view dispatcher
 * @param view_id The view ID
 * @return Always NULL
 */
View* predator_view_dispatcher_get_view(ViewDispatcher* view_dispatcher, uint32_t view_id) {
    (void)view_dispatcher;
    (void)view_id;
    return NULL;
}
