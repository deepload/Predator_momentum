#include "../predator_i.h"
#include "../helpers/predator_view_helpers.h"
#include "../helpers/predator_boards.h"
#include "../helpers/predator_ui_elements.h"
#include "../helpers/predator_view_helpers.h"
#include "predator_scene.h"

typedef struct {
    View* view;
    uint8_t selected_index;
    uint8_t scroll_position;
    uint8_t menu_items_count;
    PredatorBoardType current_board;
    bool confirmation_view;
    char selected_board_name[32];
} BoardSelectionView;

// Item definitions for the board selection menu
typedef struct {
    const char* name;
    const char* icon;
    PredatorBoardType board_type;
} BoardMenuItem;

// Menu items array - match the original enum SubmenuIndex order!
static const BoardMenuItem board_items[] = {
    {"Original Board", "🔧", PredatorBoardTypeOriginal},
    {"3-in-1 AIO Board", "📡", PredatorBoardType3in1AIO},
    {"DrB0rk Multi Board", "🛠️", PredatorBoardTypeDrB0rkMultiV2},
    {"3-in-1 NRF+CC+ESP", "💻", PredatorBoardType3in1NrfCcEsp},
    {"2.8\" Screen Board", "📱", PredatorBoardTypeScreen28},
    {"Auto Detect", "🔍", PredatorBoardTypeAutoDetect},
};

#define BOARD_ITEMS_COUNT (sizeof(board_items) / sizeof(board_items[0]))
#define ITEMS_ON_SCREEN 4

static void board_selection_view_draw_callback(Canvas* canvas, void* context) {
    PredatorApp* app = context;
    
    if(!app) return;
    
    // Get view state using helper macro
    BoardSelectionView* state = PREDATOR_GET_MODEL(app->view_dispatcher, BoardSelectionView);
    if(!state) return;
    
    canvas_clear(canvas);
    
    // Draw title
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, 64, 8, AlignCenter, AlignCenter, "Board Selection");
    
    // Draw separator
    canvas_draw_line(canvas, 0, 16, 128, 16);
    
    // Handle different views - confirmation or selection
    if(state->confirmation_view) {
        // Draw confirmation screen
        predator_ui_draw_status_box(canvas, "Board Selected", 10, 22, 108, 34);
        
        canvas_set_font(canvas, FontSecondary);
        canvas_draw_str_aligned(canvas, 64, 32, AlignCenter, AlignCenter, state->selected_board_name);
        canvas_draw_str_aligned(canvas, 64, 42, AlignCenter, AlignCenter, "Settings saved");
        
        // Instruction
        canvas_draw_str_aligned(canvas, 64, 62, AlignCenter, AlignBottom, "Press back to continue");
        
        return;
    }
    
    // Draw board icon
    uint8_t icon_x = 20;
    uint8_t icon_y = 30;
    
    // Draw stylized hardware board icon
    canvas_draw_box(canvas, icon_x - 8, icon_y - 10, 16, 20);
    
    // Draw "pins" on board
    for(uint8_t i = 0; i < 5; i++) {
        uint8_t pin_x = icon_x - 6 + (i * 3);
        canvas_draw_box(canvas, pin_x, icon_y - 12, 1, 2);
        canvas_draw_box(canvas, pin_x, icon_y + 10, 1, 2);
    }
    
    // Draw chip on board
    canvas_set_color(canvas, ColorWhite);
    canvas_draw_box(canvas, icon_x - 4, icon_y - 4, 8, 8);
    canvas_set_color(canvas, ColorBlack);
    canvas_draw_frame(canvas, icon_x - 4, icon_y - 4, 8, 8);
    
    // Draw scrolling menu
    canvas_set_font(canvas, FontSecondary);
    
    // Calculate visible range based on selection and scroll position
    for(uint8_t i = 0; i < ITEMS_ON_SCREEN && (i + state->scroll_position) < state->menu_items_count; i++) {
        uint8_t item_idx = i + state->scroll_position;
        const BoardMenuItem* item = &board_items[item_idx];
        
        uint8_t y_pos = 25 + 10 * i;
        
        // Draw selection indicator or space for non-selected items
        if(item_idx == state->selected_index) {
            elements_slightly_rounded_box(canvas, 40, y_pos - 1, 82, 11);
            canvas_set_color(canvas, ColorWhite);
        } else {
            canvas_set_color(canvas, ColorBlack);
        }
        
        // Draw icon
        canvas_draw_str(canvas, 42, y_pos + 4, item->icon);
        
        // Draw menu item text
        canvas_draw_str(canvas, 54, y_pos + 4, item->name);
        
        // Reset color
        canvas_set_color(canvas, ColorBlack);
    }
    
    // Draw scroll indicators if needed
    if(state->scroll_position > 0) {
        canvas_draw_icon(canvas, 120, 25, &I_ButtonUp_4x7);
    }
    
    if(state->scroll_position + ITEMS_ON_SCREEN < state->menu_items_count) {
        canvas_draw_icon(canvas, 120, 55, &I_ButtonDown_4x7);
    }
    
    // Draw current board info
    predator_ui_draw_status_box(canvas, "Current Board", 10, 65, 108, 14);
    
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str_aligned(canvas, 64, 73, AlignCenter, AlignCenter, 
                          predator_boards_get_name(state->current_board));
    
    // Draw controls
    elements_button_left(canvas, "Back");
    elements_button_right(canvas, "Select");
}

static bool board_selection_view_input_callback(InputEvent* event, void* context) {
    PredatorApp* app = context;
    bool consumed = false;
    
    // Get view state using helper macro
    BoardSelectionView* state = PREDATOR_GET_MODEL(app->view_dispatcher, BoardSelectionView);
    if(!state) return consumed;
    
    // Navigation controls
    if(event->type == InputTypeShort || event->type == InputTypeRepeat) {
        // Handle confirmation view
        if(state->confirmation_view) {
            if(event->key == InputKeyBack) {
                scene_manager_previous_scene(app->scene_manager);
                consumed = true;
            }
            return consumed;
        }
        
        // Handle menu navigation
        switch(event->key) {
        case InputKeyUp:
            if(state->selected_index > 0) {
                state->selected_index--;
                
                // Adjust scroll position if needed
                if(state->selected_index < state->scroll_position) {
                    state->scroll_position = state->selected_index;
                }
                consumed = true;
            }
            break;
            
        case InputKeyDown:
            if(state->selected_index < state->menu_items_count - 1) {
                state->selected_index++;
                
                // Adjust scroll position if needed
                if(state->selected_index >= state->scroll_position + ITEMS_ON_SCREEN) {
                    state->scroll_position = state->selected_index - ITEMS_ON_SCREEN + 1;
                }
                consumed = true;
            }
            break;
            
        case InputKeyRight:
        case InputKeyOk:
            // Select the current board type
            if(state->selected_index < state->menu_items_count) {
                PredatorBoardType selected_type = board_items[state->selected_index].board_type;
                
                // Save selection and update app state
                app->board_type = selected_type;
                predator_boards_save_selection(app->storage, selected_type);
                
                // Show confirmation
                state->confirmation_view = true;
                strncpy(state->selected_board_name, board_items[state->selected_index].name, 
                       sizeof(state->selected_board_name));
                consumed = true;
            }
            break;
            
        case InputKeyLeft:
        case InputKeyBack:
            scene_manager_previous_scene(app->scene_manager);
            consumed = true;
            break;
            
        default:
            break;
        }
    }
    
    return consumed;
}

static View* board_selection_view_alloc(PredatorApp* app) {
    View* view = view_alloc();
    
    // Initialize the model
    BoardSelectionView* state = malloc(sizeof(BoardSelectionView));
    state->selected_index = app->board_type;
    state->scroll_position = 0;
    state->menu_items_count = BOARD_ITEMS_COUNT;
    state->current_board = app->board_type;
    state->confirmation_view = false;
    memset(state->selected_board_name, 0, sizeof(state->selected_board_name));
    
    // Set view callbacks
    view_set_context(view, app);
    view_set_draw_callback(view, board_selection_view_draw_callback);
    view_set_input_callback(view, board_selection_view_input_callback);
    
    // Set model and free callback
    predator_predator_view_set_model(view, state);
    predator_predator_view_set_model_free_callback(view, free);
    
    return view;
}

static void board_selection_view_free(View* view) {
    view_free(view);
}

void predator_scene_board_selection_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    // Create custom view
    View* view = board_selection_view_alloc(app);
    
    // Replace widget view with custom view
    view_dispatcher_remove_view(app->view_dispatcher, PredatorViewWidget);
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewWidget, view);
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewWidget);
}

bool predator_scene_board_selection_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    // Get view state using helper macro
    BoardSelectionView* state = PREDATOR_GET_MODEL(app->view_dispatcher, BoardSelectionView);
    
    if(event.type == SceneManagerEventTypeBack) {
        // Handle back differently depending on state
        if(state && state->confirmation_view) {
            // If in confirmation view, just go back to selection menu
            state->confirmation_view = false;
            consumed = true;
        }
    }
    
    return consumed;
}

void predator_scene_board_selection_new_on_exit(void* context) {
    PredatorApp* app = context;
    
    // Free custom view
    View* view = predator_view_dispatcher_get_view(app->view_dispatcher, PredatorViewWidget);
    if(view) {
        board_selection_view_free(view);
    }
    
    // Remove the view from dispatcher
    view_dispatcher_remove_view(app->view_dispatcher, PredatorViewWidget);
    
    // Restore standard widget view
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewWidget, widget_get_view(app->widget));
}



