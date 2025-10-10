#include "../predator_i.h"
#include "../helpers/predator_models_hardcoded.h"

// Car Model Selection Scene
// Allows user to select which car model to test before running the test

static void car_model_selector_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    if(!app || !app->view_dispatcher) return;
    
    // Store selected model index in scene state
    scene_manager_set_scene_state(app->scene_manager, PredatorSceneCarModelSelector, index);
    
    // Send event to navigate to test results
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void predator_scene_car_model_selector_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app || !app->submenu) {
        FURI_LOG_E("CarModelSelector", "App or submenu is NULL");
        return;
    }
    
    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "Select Car Model");
    
    // Get total number of hardcoded models
    size_t model_count = predator_models_get_hardcoded_count();
    FURI_LOG_I("CarModelSelector", "Loading %u car models", model_count);
    
    // Add all car models to submenu
    for(size_t i = 0; i < model_count && i < 90; i++) {
        const PredatorCarModel* model = predator_models_get_hardcoded(i);
        if(model) {
            char item_text[64];
            snprintf(item_text, sizeof(item_text), "%s %s", model->make, model->model);
            submenu_add_item(app->submenu, item_text, i, car_model_selector_callback, app);
        }
    }
    
    // Set selected item to previously selected or first item
    uint32_t selected = scene_manager_get_scene_state(app->scene_manager, PredatorSceneCarModelSelector);
    submenu_set_selected_item(app->submenu, selected);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
    FURI_LOG_I("CarModelSelector", "Car model selector initialized with %u models", model_count);
}

bool predator_scene_car_model_selector_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    if(!app) return false;
    
    if(event.type == SceneManagerEventTypeCustom) {
        // User selected a model, store it and navigate to test results
        size_t selected_model = event.event;
        
        const PredatorCarModel* model = predator_models_get_hardcoded(selected_model);
        if(model) {
            FURI_LOG_I("CarModelSelector", "Selected model %u: %s %s", 
                      selected_model, model->make, model->model);
        }
        
        // Store selected model index for test results scene
        scene_manager_set_scene_state(app->scene_manager, PredatorSceneCarTestResults, selected_model);
        
        // Navigate to test results
        scene_manager_next_scene(app->scene_manager, PredatorSceneCarTestResults);
        return true;
    }
    
    return false;
}

void predator_scene_car_model_selector_on_exit(void* context) {
    PredatorApp* app = context;
    if(!app || !app->submenu) return;
    
    submenu_reset(app->submenu);
    FURI_LOG_I("CarModelSelector", "Exited car model selector");
}
