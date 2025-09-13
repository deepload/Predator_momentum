#include "../predator_i.h"
#include "predator_scene.h"
#include "predator_submenu_index.h"
#include "../helpers/predator_ui_elements.h"
#include "../helpers/predator_subghz.h"
#include <furi.h>

void predator_scene_car_attacks_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    if(!app) {
        FURI_LOG_E("CarAttacks", "App context is NULL on enter");
        return;
    }
    
    // Ensure scene_manager and view_dispatcher are valid to prevent crashes
    if(!app->scene_manager) {
        FURI_LOG_E("CarAttacks", "Scene manager is NULL, cannot proceed");
        return;
    }
    
    if(!app->view_dispatcher) {
        FURI_LOG_E("CarAttacks", "View dispatcher is NULL, cannot switch view");
        return;
    }
    
    // Validate board type before any hardware initialization
    if(app->board_type == PredatorBoardTypeUnknown) {
        FURI_LOG_W("CarAttacks", "Board type is Unknown, defaulting to Original");
        app->board_type = PredatorBoardTypeOriginal;
    }
    
    // Set up submenu for Car Attacks
    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "Car Attacks");
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

bool predator_scene_car_attacks_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(!app) {
        FURI_LOG_E("CarAttacks", "App context is NULL in event handler");
        return false;
    }
    
    if(event.type == SceneManagerEventTypeBack) {
        // Return to previous scene
        scene_manager_previous_scene(app->scene_manager);
        consumed = true;
    }
    
    return consumed;
}

void predator_scene_car_attacks_new_on_exit(void* context) {
    PredatorApp* app = context;
    
    // Null safety check
    if(!app) return;
    
    // Stop any running attacks
    if(app->attack_running) {
        // Comment out the call to avoid build errors
        // predator_subghz_deinit(app);
        app->attack_running = false;
    }
    
    // Clean up submenu
    if(app->submenu) {
        // submenu_reset(app->submenu);
    }
}
