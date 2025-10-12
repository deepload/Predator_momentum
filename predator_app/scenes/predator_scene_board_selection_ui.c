#include "../predator_i.h"
#include "../helpers/predator_boards.h"
#include "../helpers/predator_logging.h"
#include "../helpers/predator_full_detection.h"
#include <gui/view.h>

// PROFESSIONAL BOARD SELECTION - FIXED API CALLS

static const char* board_names[] = {
    "Original Predator",
    "3in1 AIO V1.4", 
    "DrB0rk Multi V2",
    "3in1 NRF24+CC1101",
    "2.8\" Screen"
};

static const uint8_t board_count = 5;

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
    
    // CLEAN HEADER
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, 64, 8, AlignCenter, AlignCenter, "BOARD SELECTION");
    canvas_draw_line(canvas, 0, 12, 128, 12);
    
    // CLEAN INSTRUCTIONS
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str(canvas, 2, 18, "Select your board:");
    
    // CLEAN BOARD LIST - Simple and readable
    uint8_t y_start = 28;
    
    for(uint8_t i = 0; i < board_count && i < 4; i++) { // Show max 4 items
        uint8_t y = y_start + (i * 10);
        
        if(i == board_state.selected_index) {
            // HIGHLIGHT SELECTED ITEM
            canvas_set_color(canvas, ColorBlack);
            canvas_draw_box(canvas, 1, y - 7, 126, 9);
            canvas_set_color(canvas, ColorWhite);
            canvas_draw_str(canvas, 4, y, ">");
            canvas_draw_str(canvas, 12, y, board_names[i]);
        } else {
            canvas_set_color(canvas, ColorBlack);
            canvas_draw_str(canvas, 12, y, board_names[i]);
        }
    }
    
    // CLEAN NAVIGATION
    canvas_set_color(canvas, ColorBlack);
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str_aligned(canvas, 64, 60, AlignCenter, AlignCenter, "↑↓=Select  OK=Choose  Back=Exit");
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
            if(board_state.selected_index < board_count - 1) {
                board_state.selected_index++;
                consumed = true;
            }
            break;
            
        case InputKeyOk:
                // PROFESSIONAL: Apply selection with real hardware validation
                if(board_state.app) {
                    PredatorBoardType old_board = board_state.app->board_type;
                    board_state.app->board_type = (PredatorBoardType)(board_state.selected_index + 1);
                    
                    // VALIDATE HARDWARE for new board
                    if(predator_full_detection_scan(board_state.app)) {
                        // Show professional status popup with hardware details
                        if(board_state.app->popup) {
                            popup_reset(board_state.app->popup);
                            popup_set_header(board_state.app->popup, "Board Configured", 64, 10, AlignCenter, AlignTop);
                            
                            char status_text[256];
                            predator_full_detection_get_detailed_status(board_state.app, status_text, sizeof(status_text));
                            
                            popup_set_text(board_state.app->popup, status_text, 64, 32, AlignCenter, AlignCenter);
                            popup_set_timeout(board_state.app->popup, 4000); // 4 seconds to read hardware status
                            popup_enable_timeout(board_state.app->popup);
                            view_dispatcher_switch_to_view(board_state.app->view_dispatcher, PredatorViewPopup);
                            
                            // Log the selection with hardware status
                            char log_msg[128];
                            snprintf(log_msg, sizeof(log_msg), "Board: %s - Hardware validated", board_names[board_state.selected_index]);
                            predator_log_append(board_state.app, log_msg);
                        }
                    } else {
                        // Hardware validation failed - revert and show error
                        board_state.app->board_type = old_board;
                        
                        if(board_state.app->popup) {
                            popup_reset(board_state.app->popup);
                            popup_set_header(board_state.app->popup, "Hardware Error", 64, 10, AlignCenter, AlignTop);
                            popup_set_text(board_state.app->popup, "Failed to validate hardware\nfor selected board", 64, 32, AlignCenter, AlignCenter);
                            popup_set_timeout(board_state.app->popup, 3000);
                            popup_enable_timeout(board_state.app->popup);
                            view_dispatcher_switch_to_view(board_state.app->view_dispatcher, PredatorViewPopup);
                        }
                    }
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
    if(app->board_type > 0 && app->board_type <= board_count) {
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
