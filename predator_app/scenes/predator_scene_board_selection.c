#include "../predator_i.h"
#include "../helpers/predator_boards.h"
#include "predator_scene.h"

enum SubmenuIndex {
    SubmenuIndexOriginalBoard,
    SubmenuIndex3in1AIOBoard,
    SubmenuIndexDrB0rkMultiBoard,
    SubmenuIndex3in1NrfCcEspBoard,
    SubmenuIndexScreen28Board,
    SubmenuIndexAutoDetect,
};

void predator_scene_board_selection_submenu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

// No forward declarations needed

void predator_scene_board_selection_on_enter(void* context) {
    PredatorApp* app = context;
    Submenu* submenu = app->submenu;
    
    // Add board options
    submenu_add_item(submenu, 
                    "Original Predator Module", 
                    SubmenuIndexOriginalBoard, 
                    predator_scene_board_selection_submenu_callback, 
                    app);
    
    submenu_add_item(submenu, 
                    "3in1 AIO Board V1.4", 
                    SubmenuIndex3in1AIOBoard, 
                    predator_scene_board_selection_submenu_callback, 
                    app);
    
    submenu_add_item(submenu, 
                    "DrB0rk Multi Board V2", 
                    SubmenuIndexDrB0rkMultiBoard, 
                    predator_scene_board_selection_submenu_callback, 
                    app);
    
    submenu_add_item(submenu, 
                    "3-in-1 NRF24+CC1101+ESP32", 
                    SubmenuIndex3in1NrfCcEspBoard, 
                    predator_scene_board_selection_submenu_callback, 
                    app);
    
    submenu_add_item(submenu, 
                    "2.8-inch Predator Screen", 
                    SubmenuIndexScreen28Board, 
                    predator_scene_board_selection_submenu_callback, 
                    app);
    
    submenu_add_item(submenu, 
                    "Auto Detect", 
                    SubmenuIndexAutoDetect, 
                    predator_scene_board_selection_submenu_callback, 
                    app);
    
    // Set current selection
    submenu_set_selected_item(submenu, app->board_type);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

// Setup confirmation view 
static void setup_board_confirmation(PredatorApp* app, const char* board_name) {
    widget_reset(app->widget);
    
    char text[128];
    snprintf(text, sizeof(text), 
            "Board Selected:\n\n%s\n\nSettings saved.\n\nPress back to continue",
            board_name);
    
    widget_add_string_multiline_element(
        app->widget, 64, 32, AlignCenter, AlignCenter, FontPrimary, text);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewWidget);
}

// Forward declaration not needed anymore

bool predator_scene_board_selection_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(event.type == SceneManagerEventTypeCustom) {
        // Handle board selection from submenu
        consumed = true;
        PredatorBoardType selected_type = PredatorBoardTypeUnknown;
        
        switch(event.event) {
        case SubmenuIndexOriginalBoard:
            selected_type = PredatorBoardTypeOriginal;
            break;
        case SubmenuIndex3in1AIOBoard:
            selected_type = PredatorBoardType3in1AIO;
            break;
        case SubmenuIndexDrB0rkMultiBoard:
            selected_type = PredatorBoardTypeDrB0rkMultiV2;
            break;
        case SubmenuIndex3in1NrfCcEspBoard:
            selected_type = PredatorBoardType3in1NrfCcEsp;
            break;
        case SubmenuIndexScreen28Board:
            selected_type = PredatorBoardTypeScreen28;
            break;
        case SubmenuIndexAutoDetect:
            selected_type = predator_boards_detect();
            if(selected_type == PredatorBoardTypeUnknown) {
                // Fall back to original if detection fails
                selected_type = PredatorBoardTypeOriginal;
            }
            break;
        }
        
        if(selected_type != PredatorBoardTypeUnknown) {
            // Save selection and update app state
            app->board_type = selected_type;
            predator_boards_save_selection(app->storage, selected_type);
            
            // Show confirmation
            setup_board_confirmation(app, predator_boards_get_name(selected_type));
        }
    } else if(event.type == SceneManagerEventTypeBack) {
        // If we're in the widget confirmation view, return to previous scene
        // Otherwise, allow normal back button handling
        if(view_dispatcher_get_current_view(app->view_dispatcher) == PredatorViewWidget) {
            scene_manager_previous_scene(app->scene_manager);
            consumed = true;
        } else {
            consumed = false; 
        }
    }
    
    return consumed;
}

void predator_scene_board_selection_on_exit(void* context) {
    PredatorApp* app = context;
    submenu_reset(app->submenu);
    widget_reset(app->widget);
}
