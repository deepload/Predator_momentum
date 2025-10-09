#include "../predator_i.h"
#include "../helpers/predator_view_helpers.h"
#include "../helpers/predator_models.h"
#include "../helpers/predator_models_seed.h"
#include "../helpers/predator_ui_elements.h"
#include "../helpers/predator_ui_status.h"
#include "../helpers/predator_logging.h"
#include "predator_scene.h"
#include "predator_submenu_index.h"

// Paged submenu for car models
#define CAR_MODELS_PER_PAGE 8
static size_t g_car_models_page = 0;

static void car_models_submenu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    if(!app || !app->view_dispatcher) return;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

static void car_models_build_page(PredatorApp* app) {
    if(!app || !app->submenu) return;
    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "Car Models");

    size_t total = predator_models_count();
    size_t start = g_car_models_page * CAR_MODELS_PER_PAGE;
    if(start >= total && total > 0) {
        g_car_models_page = 0; start = 0;
    }
    size_t end = start + CAR_MODELS_PER_PAGE;
    if(end > total) end = total;

    for(size_t i = start; i < end; ++i) {
        const PredatorCarModel* m = predator_models_get(i);
        if(!m) continue;
        char label[40];
        // Show Make Model with safe truncation to avoid warnings and overflow
        // 15 for make, 23 for model leaves 1 for space and 1 for NUL in 40-byte buffer
        snprintf(label, sizeof(label), "%.15s %.23s", m->make, m->model);
        // Use index offset 1000 to distinguish from control items
        submenu_add_item(app->submenu, label, (uint32_t)(1000 + i), car_models_submenu_callback, app);
    }

    // Pager controls
    if(total > CAR_MODELS_PER_PAGE) {
        submenu_add_item(app->submenu, "Prev Page", 900, car_models_submenu_callback, app);
        submenu_add_item(app->submenu, "Next Page", 901, car_models_submenu_callback, app);
    }
}

void predator_scene_car_models_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    if(!app) {
        FURI_LOG_E("CarModels", "App context is NULL on enter");
        return;
    }
    
    // Ensure scene_manager and view_dispatcher are valid to prevent crashes
    if(!app->scene_manager) {
        FURI_LOG_E("CarModels", "Scene manager is NULL, cannot proceed");
        return;
    }
    
    if(!app->view_dispatcher) {
        FURI_LOG_E("CarModels", "View dispatcher is NULL, cannot switch view");
        return;
    }

    // Simulate loading car models database
    predator_log_append(app, "CarModels ENTER");
    Storage* storage = app->storage ? app->storage : furi_record_open(RECORD_STORAGE);
    size_t before = predator_models_count();
    if(before == 0) {
        size_t loaded = predator_models_load_default(storage);
        FURI_LOG_I("CarModels", "Loaded %u car models from CSV", (unsigned int)loaded);
    } else {
        FURI_LOG_I("CarModels", "Models already loaded: %u", (unsigned int)before);
    }
    if(!app->storage) furi_record_close(RECORD_STORAGE);

    // Build first page of submenu
    g_car_models_page = 0;
    if(!app->submenu) {
        FURI_LOG_E("CarModels", "Submenu is NULL, cannot render car models list");
        return;
    }
    car_models_build_page(app);
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

bool predator_scene_car_models_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(!app) {
        FURI_LOG_E("CarModels", "App context is NULL in event handler");
        return false;
    }
    
    if(event.type == SceneManagerEventTypeBack) {
        FURI_LOG_I("CarModels", "Back event received, navigating to previous scene");
        scene_manager_previous_scene(app->scene_manager);
        consumed = true;
    } else if(event.type == SceneManagerEventTypeCustom) {
        consumed = true;
        if(event.event == 900) { // Prev
            if(g_car_models_page > 0) g_car_models_page--;
            car_models_build_page(app);
            view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
        } else if(event.event == 901) { // Next
            size_t total = predator_models_count();
            size_t max_page = (total == 0) ? 0 : (total - 1) / CAR_MODELS_PER_PAGE;
            if(g_car_models_page < max_page) g_car_models_page++;
            car_models_build_page(app);
            view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
        } else if(event.event >= 1000) {
            size_t idx = (size_t)(event.event - 1000);
            const PredatorCarModel* m = predator_models_get(idx);
            if(!m) {
                FURI_LOG_W("CarModels", "Invalid selection index: %lu", (unsigned long)idx);
                consumed = false;
            } else {
                // Stash selection for tests scene
                app->selected_model_index = idx;
                app->selected_model_freq = m->frequency;
                snprintf(app->selected_model_make, sizeof(app->selected_model_make), "%.15s", m->make);
                snprintf(app->selected_model_name, sizeof(app->selected_model_name), "%.39s", m->model);
                FURI_LOG_I("CarModels", "Selected: %s %s (%u Hz)", app->selected_model_make, app->selected_model_name, (unsigned)app->selected_model_freq);
                scene_manager_next_scene(app->scene_manager, PredatorSceneCarModelTests);
            }
        } else {
            consumed = false;
        }
    }
    
    return consumed;
}

void predator_scene_car_models_new_on_exit(void* context) {
    PredatorApp* app = context;
    
    if(!app) {
        FURI_LOG_E("CarModels", "App context is NULL on exit");
        return;
    }
    
    // Clean up submenu with null check
    if(app->submenu) {
        submenu_reset(app->submenu);
        FURI_LOG_I("CarModels", "Submenu reset on exit");
    } else {
        FURI_LOG_W("CarModels", "Submenu is NULL on exit, skipping reset");
    }
    
    predator_log_append(app, "CarModels EXIT");
    FURI_LOG_I("CarModels", "Exited Car Models scene");
}
