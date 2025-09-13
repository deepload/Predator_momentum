#include "../predator_i.h"
#include "../helpers/predator_view_helpers.h"
#include "../helpers/predator_ui_elements.h"
#include "predator_scene.h"
#include "../helpers/predator_boards.h"

// Callback for submenu selection
static void board_selection_submenu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    if(app) {
        app->board_type = (PredatorBoardType)index;
        FURI_LOG_I("BoardSelection", "Selected board type: %d", app->board_type);
        // Optionally save the selection to storage
        // predator_boards_save_selection(app->storage, app->board_type);
        scene_manager_previous_scene(app->scene_manager);
    }
}

void predator_scene_board_selection_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    if(!app) {
        FURI_LOG_E("BoardSelection", "App context is NULL on enter");
        return;
    }
    
    // Ensure scene_manager and view_dispatcher are valid to prevent crashes
    if(!app->scene_manager) {
        FURI_LOG_E("BoardSelection", "Scene manager is NULL, cannot proceed");
        return;
    }
    
    if(!app->view_dispatcher) {
        FURI_LOG_E("BoardSelection", "View dispatcher is NULL, cannot switch view");
        return;
    }
    
    // Set up submenu for board selection
    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "Select Board Type");
    
    // Add board types to submenu
    submenu_add_item(app->submenu, "Original Predator", PredatorBoardTypeOriginal, board_selection_submenu_callback, app);
    submenu_add_item(app->submenu, "3in1 AIO Board", PredatorBoardType3in1AIO, board_selection_submenu_callback, app);
    submenu_add_item(app->submenu, "DrB0rk Multi V2", PredatorBoardTypeDrB0rkMultiV2, board_selection_submenu_callback, app);
    submenu_add_item(app->submenu, "3in1 NRF+CC+ESP", PredatorBoardType3in1NrfCcEsp, board_selection_submenu_callback, app);
    submenu_add_item(app->submenu, "Screen 2.8 ESP32", PredatorBoardTypeScreen28, board_selection_submenu_callback, app);
    
    // Highlight current board type if set
    submenu_set_selected_item(app->submenu, app->board_type);
    
    // Switch to submenu view
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
    
    FURI_LOG_I("BoardSelection", "Board Selection scene entered with submenu");
}

void predator_scene_board_selection_new_on_exit(void* context) {
    PredatorApp* app = context;
    if(app) {
        submenu_reset(app->submenu);
    }
    FURI_LOG_I("BoardSelection", "Board Selection scene exited");
}

bool predator_scene_board_selection_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(event.type == SceneManagerEventTypeBack) {
        if(app && app->scene_manager) {
            scene_manager_previous_scene(app->scene_manager);
            consumed = true;
        }
    }
    
    return consumed;
}
