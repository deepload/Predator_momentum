#include "predator_view_helpers.h"

/**
 * Get the model from the current view in the view dispatcher
 * This uses only SDK functions and doesn't rely on accessing internal structures
 * 
 * @param view_dispatcher The view dispatcher
 * @return The model of the current view, or NULL if not available
 */
void* predator_get_view_model_safe(ViewDispatcher* view_dispatcher) {
    if(!view_dispatcher) return NULL;
    
    // Get current view directly from the SDK
    View* current_view = view_dispatcher_get_current_view(view_dispatcher);
    if(!current_view) return NULL;
    
    // Use the SDK function to get the model
    return view_get_model(current_view);
}

/**
 * Set the model for the view
 * This is a compatibility wrapper for SDK function
 * 
 * @param view The view
 * @param model The model
 */
void predator_view_set_model(View* view, void* model) {
    view_set_model(view, model);
}

/**
 * Set the model free callback for the view
 * This is a compatibility wrapper for SDK function
 * 
 * @param view The view
 * @param callback The callback to free the model
 */
void predator_view_set_model_free_callback(View* view, ViewModelFreeCallback callback) {
    view_set_model_free_callback(view, callback);
}

/**
 * Get the current view from the view dispatcher
 * This is a compatibility wrapper for SDK function
 * 
 * @param view_dispatcher The view dispatcher
 * @return The current view
 */
View* predator_view_dispatcher_get_current_view(ViewDispatcher* view_dispatcher) {
    return view_dispatcher_get_current_view(view_dispatcher);
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
    return view_dispatcher_get_view(view_dispatcher, view_id);
}
