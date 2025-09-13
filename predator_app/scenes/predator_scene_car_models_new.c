#include "../predator_i.h"
#include "../helpers/predator_view_helpers.h"
#include "../helpers/predator_subghz.h"
#include "../helpers/predator_models.h"
#include "../helpers/predator_models_seed.h"
#include "../helpers/predator_ui_elements.h"
#include "predator_scene.h"
#include "predator_submenu_index.h"

void predator_scene_car_models_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    if(!app) {
        FURI_LOG_E("CarModels", "App context is NULL on enter");
        return;
    }
    
    // Validate board type before any hardware initialization
    if(app->board_type == 0) { // Assuming 0 represents Unknown or default
        FURI_LOG_W("CarModels", "Board type is Unknown, defaulting to Original");
        app->board_type = 0; // Keep as Original
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
    
    // Check if board configuration is accessible before SubGHz initialization
    const PredatorBoardConfig* config = predator_boards_get_config(app->board_type);
    if(!config) {
        FURI_LOG_E("CarModels", "Board configuration is NULL, cannot initialize SubGHz");
        return;
    }
    
    // Initialize SubGHz safely - Comment out if predator_subghz_init is not defined or returns void
    // if(!predator_subghz_init(app)) {
    //     FURI_LOG_E("CarModels", "Failed to initialize SubGHz");
    // }
    
    // Configure popup content to avoid blank screen
    popup_reset(app->popup);
    popup_set_header(app->popup, "Car Models", 64, 10, AlignCenter, AlignTop);
    popup_set_text(app->popup, "Car models database\nPress Back to return", 64, 28, AlignCenter, AlignTop);
    popup_set_context(app->popup, app);
    popup_set_timeout(app->popup, 0);
    popup_enable_timeout(app->popup);

    // Switch to popup view
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
    
    FURI_LOG_I("CarModels", "Car Models scene entered with simulation mode");
}

bool predator_scene_car_models_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(!app) {
        FURI_LOG_E("CarModels", "App context is NULL in event handler");
        return false;
    }
    
    if(event.type == SceneManagerEventTypeBack) {
        // Return to previous scene
        scene_manager_previous_scene(app->scene_manager);
        consumed = true;
    }
    
    return consumed;
}

void predator_scene_car_models_new_on_exit(void* context) {
    PredatorApp* app = context;
    
    if(!app) {
        FURI_LOG_E("CarModels", "App context is NULL on exit");
        return;
    }
    
    // Cleanup SubGHz resources to prevent reboots - Comment out if predator_subghz_deinit is not defined
    // predator_subghz_deinit(app);
    
    // Stop any running attack
    app->attack_running = false;
    
    // Clean up submenu with null check
    if(app->submenu) {
        submenu_reset(app->submenu);
    }
    
    FURI_LOG_I("CarModels", "Exiting Car Models scene");
}
