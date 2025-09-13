#include "../predator_i.h"
#include "../helpers/predator_view_helpers.h"
#include "../helpers/predator_ui_elements.h"

void predator_scene_settings_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    if(!app) {
        FURI_LOG_E("Settings", "App context is NULL on enter");
        return;
    }
    
    // Validate board type before any hardware initialization
    if(app->board_type == PredatorBoardTypeUnknown) {
        FURI_LOG_W("Settings", "Board type is Unknown, defaulting to Original");
        app->board_type = PredatorBoardTypeOriginal;
    }
    
    // Load settings from storage - Comment out if functions are not defined
    // if(!predator_load_settings(app)) {
    //     FURI_LOG_W("Settings", "Failed to load settings, using defaults");
    //     // Initialize default settings if load fails
    //     predator_init_default_settings(app);
    // }
    
    // Configure popup content to avoid blank screen
    if(app->view_dispatcher && app->popup) {
        popup_reset(app->popup);
        popup_set_header(app->popup, "Settings", 64, 10, AlignCenter, AlignTop);
        popup_set_text(app->popup, "Configure options\nPress Back to return", 64, 28, AlignCenter, AlignTop);
        popup_set_context(app->popup, app);
        popup_set_timeout(app->popup, 0);
        popup_enable_timeout(app->popup);
        view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
    } else {
        FURI_LOG_E("Settings", "View dispatcher or popup is NULL, cannot switch view");
    }
    
    FURI_LOG_I("Settings", "Settings scene entered with simulation mode");
}

bool predator_scene_settings_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(!app) {
        FURI_LOG_E("Settings", "App context is NULL in event handler");
        return false;
    }
    
    if(event.type == SceneManagerEventTypeBack) {
        scene_manager_previous_scene(app->scene_manager);
        consumed = true;
    } else if(event.type == SceneManagerEventTypeCustom) {
        // Handle board selection and other settings
        switch(event.event) {
        case 0: // Assuming event 0 is for Board Selection
            // Navigate to Board Selection scene with correct scene ID
            // Using a hardcoded value that should match the scene ID for Board Selection
            // Ensure this matches the ID defined in your scene configuration
            if(app->scene_manager) {
                scene_manager_next_scene(app->scene_manager, PredatorSceneBoardSelection);
                consumed = true;
            } else {
                FURI_LOG_E("Settings", "Scene manager is NULL, cannot navigate to Board Selection");
            }
            break;
        default:
            consumed = false;
            break;
        }
    }
    
    return consumed;
}

void predator_scene_settings_new_on_exit(void* context) {
    PredatorApp* app = context;
    
    if(!app) {
        FURI_LOG_E("Settings", "App context is NULL on exit");
        return;
    }
    
    // Save settings to storage - Comment out if function is not defined
    // if(!predator_save_settings(app)) {
    //     FURI_LOG_E("Settings", "Failed to save settings");
    // }
    
    FURI_LOG_I("Settings", "Exiting Settings scene");
}
