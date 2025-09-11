#pragma once

#include <furi.h>
#include <gui/view.h>
#include <gui/view_dispatcher.h>

/**
 * Get the model from the current view in the view dispatcher
 * @param view_dispatcher The view dispatcher
 * @return The model of the current view, or NULL if not available
 */
void* predator_get_view_model_safe(ViewDispatcher* view_dispatcher);

/**
 * Cast the void pointer to the specified type
 * This is a helper macro to safely cast the model to the correct type
 */
#define PREDATOR_GET_MODEL(view_dispatcher, type) ((type*)predator_get_view_model_safe(view_dispatcher))

// Momentum SDK may not expose view_get_model reliably for custom views.
// Provide a safe compatibility macro that returns the last model set via
// predator_view_set_model(), ignoring the view pointer.
#ifndef view_get_model
#define view_get_model(view) (predator_get_view_model_safe(NULL))
#endif

/**
 * Set the model for the view
 * This is a compatibility wrapper for SDK function
 * 
 * @param view The view
 * @param model The model
 */
void predator_view_set_model(View* view, void* model);

/**
 * Set the model free callback for the view
 * This is a compatibility wrapper for SDK function
 * 
 * @param view The view
 * @param callback The callback to free the model
 */
void predator_view_set_model_free_callback(View* view, void (*callback)(void*));

/**
 * Get the current view from the view dispatcher
 * This is a compatibility wrapper for SDK function
 * 
 * @param view_dispatcher The view dispatcher
 * @return The current view
 */
View* predator_view_dispatcher_get_current_view(ViewDispatcher* view_dispatcher);

/**
 * Get a view from the view dispatcher by ID
 * This is a compatibility wrapper for SDK function
 * 
 * @param view_dispatcher The view dispatcher
 * @param view_id The view ID to get
 * @return The view, or NULL if not found
 */
View* predator_view_dispatcher_get_view(ViewDispatcher* view_dispatcher, uint32_t view_id);

// Define InputKeyCenter if not already defined
#ifndef InputKeyCenter
#define InputKeyCenter InputKeyOk
#endif

// Common screen size definition
#ifndef ITEMS_ON_SCREEN
#define ITEMS_ON_SCREEN 4
#endif
