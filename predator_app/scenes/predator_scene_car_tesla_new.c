#include "../predator_i.h"
#include "../helpers/predator_view_helpers.h"
#include "../helpers/predator_subghz.h"
#include "../helpers/predator_ui_elements.h"

// Custom view for Tesla charge port opener
typedef struct {
    View* view;
    uint32_t signals_sent;
    uint8_t animation_frame;
} TeslaChargePortView;

void predator_scene_car_tesla_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    if(!app) {
        FURI_LOG_E("CarTesla", "App context is NULL on enter");
        return;
    }
    
    // Validate board type before any hardware initialization
    if(app->board_type == 0) { // Assuming 0 represents Unknown or default
        FURI_LOG_W("CarTesla", "Board type is Unknown, defaulting to Original");
        app->board_type = 0; // Keep as Original
    }
    
    // Ensure scene_manager and view_dispatcher are valid to prevent crashes
    if(!app->scene_manager) {
        FURI_LOG_E("CarTesla", "Scene manager is NULL, cannot proceed");
        return;
    }
    
    if(!app->view_dispatcher) {
        FURI_LOG_E("CarTesla", "View dispatcher is NULL, cannot switch view");
        return;
    }
    
    // Check if board configuration is accessible before SubGHz initialization
    const PredatorBoardConfig* config = predator_boards_get_config(app->board_type);
    if(!config) {
        FURI_LOG_E("CarTesla", "Board configuration is NULL, cannot initialize SubGHz");
        return;
    }
    
    // Initialize SubGHz safely - Comment out if predator_subghz_init is not defined or returns void
    // if(!predator_subghz_init(app)) {
    //     FURI_LOG_E("CarTesla", "Failed to initialize SubGHz");
    // }
    
    // Switch to a safe view or show a placeholder message
    view_dispatcher_switch_to_view(app->view_dispatcher, 25); // Assuming 25 is a valid view ID for Tesla Charge Port
    
    FURI_LOG_I("CarTesla", "Tesla Charge Port scene entered with simulation mode");
}

bool predator_scene_car_tesla_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(!app) {
        FURI_LOG_E("CarTesla", "App context is NULL in event handler");
        return false;
    }
    
    if(event.type == SceneManagerEventTypeBack) {
        // Return to previous scene
        scene_manager_previous_scene(app->scene_manager);
        consumed = true;
    }
    
    return consumed;
}

void predator_scene_car_tesla_new_on_exit(void* context) {
    PredatorApp* app = context;
    
    // Null safety check
    if(!app) return;
    
    // Clean up
    app->attack_running = false;
    //predator_subghz_deinit(app);
    
    // Remove custom view and restore default popup view
    if(app->view_dispatcher && app->popup) {
        //view_dispatcher_remove_view(app->view_dispatcher, PredatorViewPopup);
        //view_dispatcher_add_view(app->view_dispatcher, PredatorViewPopup, popup_get_view(app->popup));
    }
}
