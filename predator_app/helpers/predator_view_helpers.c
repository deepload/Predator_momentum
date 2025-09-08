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
