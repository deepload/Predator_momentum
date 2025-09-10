#include "predator_view_helpers.h"

// Hold the most recently set model so scenes can fetch it without SDK internals
static void* g_current_view_model = NULL;

/**
 * Get the model from the current view in the view dispatcher
 * This uses only SDK functions and doesn't rely on accessing internal structures
 * 
 * @param view_dispatcher The view dispatcher
 * @return The model of the current view, or NULL if not available
 */
void* predator_get_view_model_safe(ViewDispatcher* view_dispatcher) {
    (void)view_dispatcher;
    // Return the last model we set via predator_view_set_model
    return g_current_view_model;
}

/**
 * Set the model for the view
 * This is a compatibility wrapper for SDK function
 * 
 * @param view The view
 * @param model The model
 */
void predator_view_set_model(View* view, void* model) {
    (void)view;
    // Store model for retrieval via PREDATOR_GET_MODEL
    g_current_view_model = model;
}

/**
 * Set the model free callback for the view
 * This is a compatibility wrapper for SDK function
 * 
 * @param view The view
 * @param callback The callback to free the model
 */
void predator_view_set_model_free_callback(View* view, void (*callback)(void*)) {
    (void)view;
    (void)callback;
    // Not supported in this SDK variant; model is freed by scene code
}

/**
 * Get the current view from the view dispatcher
 * This is a compatibility wrapper for SDK function
 * 
 * @param view_dispatcher The view dispatcher
 * @return The current view
 */
View* predator_view_dispatcher_get_current_view(ViewDispatcher* view_dispatcher) {
    (void)view_dispatcher;
    // Not available in this SDK variant; callers guard for NULL
    return NULL;
}

/**
 * Get a view from the view dispatcher by ID
 * This is a compatibility wrapper for SDK function
 * 
 * @param view_dispatcher The view dispatcher
 * @param view_id The view ID to get
 * @return The view, or NULL if not found
 */
View* predator_view_dispatcher_get_view(ViewDispatcher* view_dispatcher, uint32_t view_id) {
    (void)view_dispatcher;
    (void)view_id;
    // Not available in Momentum SDK -> return NULL so callers can skip freeing
    return NULL;
}
