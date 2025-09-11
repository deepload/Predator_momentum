#include "predator_view_dispatcher_patch.h"

/**
 * Simple implementation to access view model without conflicting with SDK types
 * This avoids defining struct types that might conflict with SDK definitions
 */

void* predator_get_view_model(ViewDispatcher* view_dispatcher) {
    if(!view_dispatcher) return NULL;
    
    // Get the current view from view_dispatcher
    View* current_view = view_dispatcher_get_current_view(view_dispatcher);
    if(!current_view) return NULL;
    
    // Get the model from the view
    return view_get_model(current_view);
}
