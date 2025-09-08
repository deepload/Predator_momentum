#pragma once

#include <furi.h>
#include <gui/view_dispatcher.h>
#include <gui/view.h>

// Helper function to access view_model from view_dispatcher->current_view
// in a safe and direct way without conflicting type definitions
void* predator_get_view_model(ViewDispatcher* view_dispatcher);

// Use the SDK's view_get_model directly
void* view_get_model(View* view);
