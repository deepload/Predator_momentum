#include "../predator_i.h"
#include "../helpers/predator_boards.h"
#include "../helpers/predator_logging.h"
#include <gui/view.h>
#include <string.h>

// Board Selection - Professional UI
// Shows available boards with selection interface

typedef struct {
    PredatorBoardType selected_board;
    PredatorBoardType current_board;
    uint8_t selection_index;
    bool selection_changed;
} BoardSelectionState;

static BoardSelectionState board_state;

static const char* board_names[] = {
    "Unknown",
    "Original Predator",
    "3in1 AIO V1.4",
    "DrB0rk Multi V2",
    "3in1 NRF24+CC1101",
    "2.8\" Screen"
};

static const uint8_t board_count = 6;

static void draw_board_selection_header(Canvas* canvas) {
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 2, 10, "BOARD SELECTION");
    canvas_draw_line(canvas, 0, 12, 128, 12);
}

static void draw_board_list(Canvas* canvas, BoardSelectionState* state) {
    canvas_set_font(canvas, FontSecondary);
    
    // Instructions
    canvas_draw_str(canvas, 2, 22, "Select your board:");
    
    // Draw board list (show 3 at a time)
    uint8_t start_idx = 0;
    if(state->selection_index > 1) {
        start_idx = state->selection_index - 1;
    }
    
    for(uint8_t i = 0; i < 3 && (start_idx + i) < board_count; i++) {
        uint8_t idx = start_idx + i;
        uint8_t y = 34 + (i * 10);
        
        // Selection indicator
        if(idx == state->selection_index) {
            canvas_draw_str(canvas, 2, y, ">");
        }
        
        // Board name
        canvas_draw_str(canvas, 12, y, board_names[idx]);
        
        // Current board indicator
        if(idx == (uint8_t)state->current_board) {
            canvas_draw_str(canvas, 110, y, "*");
        }
    }
    
    // Status line
    if(state->selection_changed) {
        canvas_draw_str(canvas, 2, 58, "Press OK to confirm");
    } else {
        canvas_draw_str(canvas, 2, 58, "* = Current board");
    }
}

static void board_selection_ui_draw_callback(Canvas* canvas, void* context) {
    furi_assert(canvas);
    furi_assert(context);
    
    UNUSED(context);
    
    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);
    
    draw_board_selection_header(canvas);
    draw_board_list(canvas, &board_state);
    
    canvas_set_font(canvas, FontSecondary);
    if(board_state.selection_changed) {
        canvas_draw_str(canvas, 25, 64, "OK=Save  Back=Cancel");
    } else {
        canvas_draw_str(canvas, 30, 64, "↑↓=Select  Back=Exit");
    }
}

static bool board_selection_ui_input_callback(InputEvent* event, void* context) {
    PredatorApp* app = context;
    if(!app) return false;
    
    if(event->type == InputTypeShort) {
        if(event->key == InputKeyBack) {
            if(board_state.selection_changed) {
                // Cancel changes
                board_state.selected_board = board_state.current_board;
                board_state.selection_index = (uint8_t)board_state.current_board;
                board_state.selection_changed = false;
                return true;
            }
            return false; // Let scene manager handle back
        } else if(event->key == InputKeyOk) {
            if(board_state.selection_changed) {
                // Save selection
                app->board_type = board_state.selected_board;
                board_state.current_board = board_state.selected_board;
                board_state.selection_changed = false;
                
                char log_msg[64];
                snprintf(log_msg, sizeof(log_msg), "Board changed to: %s", 
                        board_names[board_state.selection_index]);
                predator_log_append(app, log_msg);
                
                FURI_LOG_I("BoardSelectionUI", "Board set to: %s", board_names[board_state.selection_index]);
                return true;
            }
        } else if(event->key == InputKeyUp) {
            if(board_state.selection_index > 0) {
                board_state.selection_index--;
                board_state.selected_board = (PredatorBoardType)board_state.selection_index;
                board_state.selection_changed = (board_state.selected_board != board_state.current_board);
                return true;
            }
        } else if(event->key == InputKeyDown) {
            if(board_state.selection_index < (board_count - 1)) {
                board_state.selection_index++;
                board_state.selected_board = (PredatorBoardType)board_state.selection_index;
                board_state.selection_changed = (board_state.selected_board != board_state.current_board);
                return true;
            }
        }
    }
    
    return true;
}

void predator_scene_board_selection_ui_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    memset(&board_state, 0, sizeof(BoardSelectionState));
    board_state.current_board = app->board_type;
    board_state.selected_board = app->board_type;
    board_state.selection_index = (uint8_t)app->board_type;
    board_state.selection_changed = false;
    
    if(!app->view_dispatcher) {
        FURI_LOG_E("BoardSelectionUI", "View dispatcher is NULL");
        return;
    }
    
    View* view = view_alloc();
    if(!view) {
        FURI_LOG_E("BoardSelectionUI", "Failed to allocate view");
        return;
    }
    
    view_set_context(view, app);
    view_set_draw_callback(view, board_selection_ui_draw_callback);
    view_set_input_callback(view, board_selection_ui_input_callback);
    
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewBoardSelectionUI, view);
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewBoardSelectionUI);
    
    FURI_LOG_I("BoardSelectionUI", "Board Selection UI initialized");
}

bool predator_scene_board_selection_ui_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    if(!app) return false;
    
    if(event.type == SceneManagerEventTypeCustom) {
        return true;
    }
    
    return false;
}

void predator_scene_board_selection_ui_on_exit(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    if(app->view_dispatcher) {
        view_dispatcher_remove_view(app->view_dispatcher, PredatorViewBoardSelectionUI);
    }
    
    FURI_LOG_I("BoardSelectionUI", "Board Selection UI exited");
}
