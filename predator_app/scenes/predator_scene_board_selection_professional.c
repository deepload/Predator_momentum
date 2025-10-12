#include "../predator_i.h"
#include "predator_scene.h"
#include "../helpers/predator_boards.h"
#include "../helpers/predator_logging.h"
#include <gui/view.h>

// PROFESSIONAL BOARD SELECTION - Clean, readable, functional

typedef struct {
    uint8_t selected_index;
    bool selection_made;
} BoardSelectionModel;

static const char* board_names[] = {
    "Original Predator",
    "3in1 AIO V1.4", 
    "DrB0rk Multi V2",
    "3in1 NRF24+CC1101",
    "2.8\" Screen"
};

static const char* board_descriptions[] = {
    "Basic RF only",
    "WiFi + GPS + RF",
    "Multi-protocol",
    "NRF24 + CC1101",
    "Large screen"
};

static const uint8_t board_count = 5;

static void board_selection_draw_callback(Canvas* canvas, void* context) {
    if(!canvas) return;
    
    BoardSelectionModel* model = (BoardSelectionModel*)view_get_model(context);
    if(!model) return;
    
    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);
    canvas_set_font(canvas, FontPrimary);
    
    // CLEAN HEADER
    canvas_draw_str_aligned(canvas, 64, 8, AlignCenter, AlignCenter, "BOARD SELECTION");
    canvas_draw_line(canvas, 0, 12, 128, 12);
    
    // CLEAN INSTRUCTIONS
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str(canvas, 2, 22, "Select your board:");
    
    // CLEAN BOARD LIST - Show current selection clearly
    uint8_t y_start = 30;
    uint8_t item_height = 16;
    
    // Show 3 items centered around selection
    int8_t start_idx = model->selected_index - 1;
    if(start_idx < 0) start_idx = 0;
    if(start_idx + 3 > board_count) start_idx = board_count - 3;
    if(start_idx < 0) start_idx = 0;
    
    for(uint8_t i = 0; i < 3 && (start_idx + i) < board_count; i++) {
        uint8_t idx = start_idx + i;
        uint8_t y = y_start + (i * item_height);
        
        bool is_selected = (idx == model->selected_index);
        
        // CLEAN SELECTION HIGHLIGHT
        if(is_selected) {
            canvas_set_color(canvas, ColorBlack);
            canvas_draw_box(canvas, 1, y - 7, 126, 14);
            canvas_set_color(canvas, ColorWhite);
        } else {
            canvas_set_color(canvas, ColorBlack);
        }
        
        // CLEAN BOARD NAME
        canvas_draw_str(canvas, 4, y, board_names[idx]);
        
        // CLEAN DESCRIPTION ON NEXT LINE
        canvas_set_font(canvas, FontSecondary);
        canvas_draw_str(canvas, 6, y + 8, board_descriptions[idx]);
        canvas_set_font(canvas, FontSecondary);
    }
    
    // CLEAN NAVIGATION HELP
    canvas_set_color(canvas, ColorBlack);
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str_aligned(canvas, 64, 60, AlignCenter, AlignCenter, "↑↓=Navigate  OK=Select  Back=Exit");
}

static bool board_selection_input_callback(InputEvent* event, void* context) {
    if(!event || event->type != InputTypeShort) return false;
    
    bool consumed = false;
    
    with_view_model(
        context,
        BoardSelectionModel* model,
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
                    
                case InputKeyBack:
                    // Let scene manager handle
                    break;
                    
                default:
                    break;
            }
        },
        consumed
    );
    
    return consumed;
}

void predator_scene_board_selection_professional_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    // Create clean view
    View* view = view_alloc();
    if(!view) return;
    
    // Initialize model
    view_allocate_model(view, ViewModelTypeLocking, sizeof(BoardSelectionModel));
    
    with_view_model(
        view,
        BoardSelectionModel* model,
        {
            model->selected_index = (uint8_t)app->board_type;
            if(model->selected_index >= board_count) {
                model->selected_index = 0; // Default to first board
            }
            model->selection_made = false;
        },
        true
    );
    
    view_set_context(view, view);
    view_set_draw_callback(view, board_selection_draw_callback);
    view_set_input_callback(view, board_selection_input_callback);
    
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewBoardSelectionProfessional, view);
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewBoardSelectionProfessional);
}

bool predator_scene_board_selection_professional_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    if(!app) return false;
    
    bool consumed = false;
    
    if(event.type == SceneManagerEventTypeBack) {
        scene_manager_previous_scene(app->scene_manager);
        return true;
    }
    
    // Check if selection was made
    View* view = view_dispatcher_get_view(app->view_dispatcher, PredatorViewBoardSelectionProfessional);
    if(view) {
        bool selection_made = false;
        uint8_t selected_index = 0;
        
        with_view_model(
            view,
            BoardSelectionModel* model,
            {
                selection_made = model->selection_made;
                selected_index = model->selected_index;
                model->selection_made = false; // Reset
            },
            false
        );
        
        if(selection_made) {
            // PROFESSIONAL: Apply board selection immediately
            app->board_type = (PredatorBoardType)(selected_index + 1); // +1 because enum starts at 1
            
            // PROFESSIONAL: Show confirmation and return to menu
            if(app->popup) {
                popup_reset(app->popup);
                popup_set_header(app->popup, "Board Selected", 64, 20, AlignCenter, AlignTop);
                
                char popup_text[64];
                snprintf(popup_text, sizeof(popup_text), "%s\n\nSaved successfully!", board_names[selected_index]);
                popup_set_text(app->popup, popup_text, 64, 32, AlignCenter, AlignCenter);
                
                popup_set_timeout(app->popup, 1500);
                popup_enable_timeout(app->popup);
                view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
                
                // Log the change
                char log_msg[64];
                snprintf(log_msg, sizeof(log_msg), "Board: %s", board_names[selected_index]);
                predator_log_append(app, log_msg);
            }
            
            consumed = true;
        }
    }
    
    return consumed;
}

void predator_scene_board_selection_professional_on_exit(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    View* view = view_dispatcher_get_view(app->view_dispatcher, PredatorViewBoardSelectionProfessional);
    if(view) {
        view_dispatcher_remove_view(app->view_dispatcher, PredatorViewBoardSelectionProfessional);
        view_free(view);
    }
}
