#include "../predator_i.h"
#include "../helpers/predator_subghz.h"
#include "predator_scene.h"

// Scene state enum
typedef enum {
    CarModelsStateModel,
    CarModelsStateCommand
} CarModelsState;

// Local state variables to track car model and command selection
typedef struct {
    CarModel selected_model;
    CarCommand selected_command;
    CarModelsState state;
    bool transmitting;
} CarModelsSceneState;

// Create local state object to persist between scene callbacks
static CarModelsSceneState* car_models_state = NULL;

// Helper function to format the menu headers
static void predator_scene_car_models_update_menu(PredatorApp* app, CarModelsState state) {
    Submenu* submenu = app->submenu;
    submenu_reset(submenu);
    
    if (state == CarModelsStateModel) {
        // Show car models menu
        for(uint8_t i = 0; i < CarModelCount; i++) {
            submenu_add_item(submenu, 
                predator_subghz_get_car_model_name(i), 
                i, 
                predator_scene_car_models_submenu_callback, 
                app);
        }
        submenu_set_header(submenu, "Select Car Model");
    } else {
        // Show car commands menu
        for(uint8_t i = 0; i < CarCommandCount; i++) {
            submenu_add_item(submenu, 
                predator_subghz_get_car_command_name(i), 
                i, 
                predator_scene_car_models_submenu_callback, 
                app);
        }
        char header[64];
        snprintf(header, sizeof(header), "%s - Select Command", 
            predator_subghz_get_car_model_name(car_models_state->selected_model));
        submenu_set_header(submenu, header);
    }
}

// Popup callback for when a command is being transmitted
static void predator_scene_car_models_popup_callback(void* context) {
    PredatorApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, PredatorCustomEventPopupBack);
}

// Submenu callback for selections
void predator_scene_car_models_submenu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void predator_scene_car_models_on_enter(void* context) {
    PredatorApp* app = context;
    
    // Initialize the scene state if not yet created
    if (!car_models_state) {
        car_models_state = malloc(sizeof(CarModelsSceneState));
        car_models_state->selected_model = CarModelToyota;
        car_models_state->selected_command = CarCommandUnlock;
        car_models_state->state = CarModelsStateModel;
        car_models_state->transmitting = false;
    }
    
    // Restore the last state of the scene
    if (car_models_state->transmitting) {
        // Show transmission screen
        popup_set_header(app->popup, "Transmitting...", 64, 10, AlignCenter, AlignTop);
        popup_set_text(app->popup, 
            "Sending car command...\n"
            "Please wait", 
            64, 25, AlignCenter, AlignTop);
        popup_set_callback(app->popup, predator_scene_car_models_popup_callback);
        popup_set_context(app->popup, app);
        popup_set_timeout(app->popup, 0);
        popup_enable_timeout(app->popup);
        
        view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
        
        // Initialize SubGHz
        predator_subghz_init(app);
        
        // Send the command
        predator_subghz_send_car_command(app, 
            car_models_state->selected_model, 
            car_models_state->selected_command);
            
        // Update UI with completion info
        char result_text[128];
        snprintf(result_text, sizeof(result_text), 
            "Transmission complete!\n"
            "Model: %s\n"
            "Command: %s", 
            predator_subghz_get_car_model_name(car_models_state->selected_model),
            predator_subghz_get_car_command_name(car_models_state->selected_command));
        popup_set_text(app->popup, result_text, 64, 25, AlignCenter, AlignTop);
        
        car_models_state->transmitting = false;
    } else {
        // Show selection menu based on state
        predator_scene_car_models_update_menu(app, car_models_state->state);
        view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
    }
}

bool predator_scene_car_models_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        if(car_models_state->state == CarModelsStateModel) {
            // Car model was selected
            car_models_state->selected_model = event.event;
            car_models_state->state = CarModelsStateCommand;
            predator_scene_car_models_update_menu(app, CarModelsStateCommand);
            consumed = true;
        } else if(car_models_state->state == CarModelsStateCommand) {
            // Car command was selected
            car_models_state->selected_command = event.event;
            car_models_state->transmitting = true;
            
            // Switch to transmitting screen and restart the scene
            scene_manager_previous_scene(app->scene_manager);
            scene_manager_next_scene(app->scene_manager, PredatorSceneCarModels);
            consumed = true;
        } else if(event.event == PredatorCustomEventPopupBack) {
            // Return to car model selection
            car_models_state->state = CarModelsStateModel;
            predator_scene_car_models_update_menu(app, CarModelsStateModel);
            view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
            consumed = true;
        }
    } else if(event.type == SceneManagerEventTypeBack) {
        if(car_models_state->state == CarModelsStateCommand) {
            // Go back to model selection instead of leaving the scene
            car_models_state->state = CarModelsStateModel;
            predator_scene_car_models_update_menu(app, CarModelsStateModel);
            consumed = true;
        } else {
            // Actually go back
            consumed = false;
        }
    }

    return consumed;
}

void predator_scene_car_models_on_exit(void* context) {
    PredatorApp* app = context;
    
    // Clean up if transmitting
    if(car_models_state->transmitting) {
        predator_subghz_deinit(app);
        car_models_state->transmitting = false;
    }
    
    // Reset views
    submenu_reset(app->submenu);
    popup_reset(app->popup);
}
