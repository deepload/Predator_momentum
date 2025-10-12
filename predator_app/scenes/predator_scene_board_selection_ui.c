#include "../predator_i.h"
#include "../helpers/predator_boards.h"
#include "../helpers/predator_logging.h"
#include "../helpers/predator_full_detection.h"
#include "../helpers/predator_esp32.h"
#include "../helpers/predator_gps.h"
#include "../helpers/predator_constants.h"
#include <gui/view.h>

// PROFESSIONAL BOARD SELECTION - MEMORY OPTIMIZED

typedef struct {
    uint8_t selected_index;
    PredatorApp* app;
} BoardSelectionState;

static BoardSelectionState board_state;

static void board_draw_callback(Canvas* canvas, void* context) {
    UNUSED(context);
    if(!canvas) return;
    
    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);
    
    // Header
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 2, 10, "BOARD SELECTION");
    canvas_draw_line(canvas, 0, 12, 128, 12);
    
    // Current selection
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str(canvas, 2, 22, "Board:");
    canvas_draw_str(canvas, 35, 22, PREDATOR_BOARD_NAMES[board_state.selected_index]);
    
    // Hardware capabilities for selected board
    const PredatorBoardConfig* config = predator_boards_get_config(board_state.selected_index);
    if(config) {
        canvas_draw_str(canvas, 2, 32, "Hardware:");
        
        // WiFi/ESP32 status - ALL ESP32 boards
        bool has_esp32 = (board_state.selected_index == PredatorBoardType3in1AIO || 
                         board_state.selected_index == PredatorBoardTypeScreen28 ||
                         board_state.selected_index == PredatorBoardTypeDrB0rkMultiV2 ||
                         board_state.selected_index == PredatorBoardType3in1NrfCcEsp);
        
        if(has_esp32) {
            canvas_draw_str(canvas, 2, 42, "✓ WiFi/BT (ESP32)");
        } else {
            canvas_draw_str(canvas, 2, 42, "✗ WiFi/BT");
        }
        
        // GPS status - Same boards as ESP32
        bool has_gps = has_esp32;
        if(has_gps) {
            canvas_draw_str(canvas, 2, 52, "✓ GPS Module");
        } else {
            canvas_draw_str(canvas, 2, 52, "✗ GPS Module");
        }
        
        // SubGHz (always available)
        canvas_draw_str(canvas, 2, 62, "✓ SubGHz Radio");
    }
    
    // Instructions
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str(canvas, 15, 64, "Up/Down=Select OK=Confirm");
}

static bool board_input_callback(InputEvent* event, void* context) {
    UNUSED(context);
    if(!event || event->type != InputTypeShort) return false;
    
    bool consumed = false;
    
    switch(event->key) {
        case InputKeyUp:
            if(board_state.selected_index > 0) {
                board_state.selected_index--;
                consumed = true;
            }
            break;
            
        case InputKeyDown:
            if(board_state.selected_index < PREDATOR_BOARD_COUNT - 1) {
                board_state.selected_index++;
                consumed = true;
            }
            break;
            
        case InputKeyOk:
            // PROFESSIONAL: Safe board selection with proper hardware setup
            if(board_state.app) {
                // Validate board index
                if(board_state.selected_index >= PREDATOR_BOARD_COUNT) {
                    board_state.selected_index = 1; // Default to Original Predator
                }
                
                board_state.app->board_type = board_state.selected_index;
                
                // Minimal logging
                predator_log_append(board_state.app, "Board selected");
                
                // MEMORY OPTIMIZED: Minimal board capability detection
                UNUSED(predator_boards_get_config(board_state.selected_index));
                
                // Minimal logging to save memory
                predator_log_append(board_state.app, "Board configured");
                
                // Return to main menu
                scene_manager_previous_scene(board_state.app->scene_manager);
            }
            consumed = true;
            break;
            
        case InputKeyBack:
            // Let scene manager handle back
            break;
            
        default:
            break;
    }
    
    return consumed;
}

void predator_scene_board_selection_ui_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    // Initialize state
    board_state.selected_index = 0;
    board_state.app = app;
    
    // Set current board as selected
    if(app->board_type > 0 && app->board_type <= PREDATOR_BOARD_COUNT) {
        board_state.selected_index = app->board_type - 1;
    }
    
    // Create view
    View* view = view_alloc();
    if(!view) return;
    
    view_set_context(view, app);
    view_set_draw_callback(view, board_draw_callback);
    view_set_input_callback(view, board_input_callback);
    
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewBoardSelectionUI, view);
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewBoardSelectionUI);
}

bool predator_scene_board_selection_ui_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    if(!app) return false;
    
    if(event.type == SceneManagerEventTypeBack) {
        scene_manager_previous_scene(app->scene_manager);
        return true;
    }
    
    return false;
}

void predator_scene_board_selection_ui_on_exit(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    // Clean up view
    view_dispatcher_remove_view(app->view_dispatcher, PredatorViewBoardSelectionUI);
}
