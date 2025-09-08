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
