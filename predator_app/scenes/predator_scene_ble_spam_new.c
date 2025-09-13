#include "../predator_i.h"
#include "../helpers/predator_view_helpers.h"
#include "../helpers/predator_esp32.h"
#include "../helpers/predator_ui_elements.h"

void predator_scene_ble_spam_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    if(!app) {
        FURI_LOG_E("BLESpam", "App context is NULL on enter");
        return;
    }
    
    // Validate board type before any hardware initialization
    if(app->board_type == 0) { // Assuming 0 represents Unknown or default
        FURI_LOG_W("BLESpam", "Board type is Unknown, defaulting to Original");
        app->board_type = 0; // Keep as Original
    }
    
    // Ensure scene_manager and view_dispatcher are valid to prevent crashes
    if(!app->scene_manager) {
        FURI_LOG_E("BLESpam", "Scene manager is NULL, cannot proceed");
        return;
    }
    
    if(!app->view_dispatcher) {
        FURI_LOG_E("BLESpam", "View dispatcher is NULL, cannot switch view");
        return;
    }
    
    // Comment out calls to undefined ESP32 initialization functions
    // if(!app->esp32_uart) {
    //     predator_esp32_init(app);
    // }
    
    // Switch to a safe view or show a placeholder message
    view_dispatcher_switch_to_view(app->view_dispatcher, 24); // Assuming 24 is a valid view ID for BLE Spam
    
    FURI_LOG_I("BLESpam", "BLE Spam scene entered with simulation mode");
}

bool predator_scene_ble_spam_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(!app) {
        FURI_LOG_E("BLESpam", "App context is NULL in event handler");
        return false;
    }
    
    if(event.type == SceneManagerEventTypeBack) {
        // Return to previous scene
        scene_manager_previous_scene(app->scene_manager);
        consumed = true;
    }
    
    return consumed;
}

void predator_scene_ble_spam_new_on_exit(void* context) {
    PredatorApp* app = context;
    
    if(!app) {
        FURI_LOG_E("BLESpam", "App context is NULL on exit");
        return;
    }
    
    FURI_LOG_I("BLESpam", "Exiting BLE Spam scene");
}
