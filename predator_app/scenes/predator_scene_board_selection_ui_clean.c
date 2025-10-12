#include "../predator_i.h"
#include "../helpers/predator_boards.h"
#include "../helpers/predator_logging.h"
#include <gui/view.h>

// COMPLETELY REWRITTEN PROFESSIONAL BOARD SELECTION

typedef struct {
    uint8_t selected_index;
    bool selection_made;
} BoardModel;

static const char* board_names[] = {
    "Original Predator",
    "3in1 AIO V1.4", 
    "DrB0rk Multi V2",
    "3in1 NRF24+CC1101",
    "2.8\" Screen"
};

static const char* board_descriptions[] = {
    "Basic RF",
    "WiFi+GPS+RF",
    "Multi-protocol", 
    "NRF24+CC1101",
    "Large screen"
};

static const uint8_t board_count = 5;

static void board_draw_callback(Canvas* canvas, void* context) {
    if(!canvas) return;
    
    BoardModel* model = (BoardModel*)view_get_model(context);
    if(!model) return;
    
    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);
    
    // CLEAN HEADER
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, 64, 8, AlignCenter, AlignCenter, "BOARD SELECTION");
    canvas_draw_line(canvas, 0, 12, 128, 12);
    
    // CLEAN BOARD LIST
    canvas_set_font(canvas, FontSecondary);
    uint8_t y_start = 20;
    
    for(uint8_t i = 0; i < board_count; i++) {
        uint8_t y = y_start + (i * 8);
        
        if(i == model->selected_index) {
            // HIGHLIGHT SELECTED
            canvas_draw_str(canvas, 2, y, ">");
            canvas_draw_str(canvas, 8, y, board_names[i]);
        } else {
            canvas_draw_str(canvas, 8, y, board_names[i]);
        }
    }
    
    // CLEAN INSTRUCTIONS
    canvas_draw_str_aligned(canvas, 64, 60, AlignCenter, AlignCenter, "↑↓=Select OK=Choose Back=Exit");
}

static bool board_input_callback(InputEvent* event, void* context) {
    if(!event || event->type != InputTypeShort) return false;
    
    bool consumed = false;
    
    with_view_model(
        context,
        BoardModel* model,
        {
            switch(event->key) {
                case InputKeyUp:
                    if(model->selected_index > 0) {
                        model->selected_index--;
                        consumed = true;
                    }
                    break;
                    
                case InputKeyDown:
                    if(model->selected_index < board_count - 1) {
                        model->selected_index++;
                        consumed = true;
                    }
                    break;
                    
                case InputKeyOk:
                    model->selection_made = true;
                    consumed = true;
                    break;
                    
                default:
                    break;
            }
        },
        consumed
    );
    
    return consumed;
}

void predator_scene_board_selection_ui_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    View* view = view_alloc();
    if(!view) return;
    
    view_allocate_model(view, ViewModelTypeLocking, sizeof(BoardModel));
    
    with_view_model(
        view,
        BoardModel* model,
        {
            model->selected_index = 0;
            model->selection_made = false;
        },
        true
    );
    
    view_set_context(view, view);
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
    
    // Check for selection
    View* view = view_dispatcher_get_view(app->view_dispatcher, PredatorViewBoardSelectionUI);
    if(view) {
        bool selection_made = false;
        uint8_t selected_index = 0;
        
        with_view_model(
            view,
            BoardModel* model,
            {
                selection_made = model->selection_made;
                selected_index = model->selected_index;
                model->selection_made = false;
            },
            false
        );
        
        if(selection_made) {
            // APPLY SELECTION
            app->board_type = (PredatorBoardType)(selected_index + 1);
            
            // SHOW CONFIRMATION
            if(app->popup) {
                popup_reset(app->popup);
                popup_set_header(app->popup, "Board Selected", 64, 20, AlignCenter, AlignTop);
                
                char text[64];
                snprintf(text, sizeof(text), "%s\n\nSaved!", board_names[selected_index]);
                popup_set_text(app->popup, text, 64, 32, AlignCenter, AlignCenter);
                
                popup_set_timeout(app->popup, 1500);
                popup_enable_timeout(app->popup);
                view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
                
                // LOG
                char log_msg[64];
                snprintf(log_msg, sizeof(log_msg), "Board: %s", board_names[selected_index]);
                predator_log_append(app, log_msg);
            }
            
            return true;
        }
    }
    
    return false;
}

void predator_scene_board_selection_ui_on_exit(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    View* view = view_dispatcher_get_view(app->view_dispatcher, PredatorViewBoardSelectionUI);
    if(view) {
        view_dispatcher_remove_view(app->view_dispatcher, PredatorViewBoardSelectionUI);
        view_free(view);
    }
}
