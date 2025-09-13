#include "../predator_i.h"
#include "../helpers/predator_view_helpers.h"
#include "../helpers/predator_ui_elements.h"
#include <gui/canvas.h>

// Custom view for RFID clone
typedef struct {
    View* view;
    uint8_t progress;
    bool card_detected;
    bool clone_complete;
    char card_id[32];
    char card_type[16];
    uint32_t animation_frame;
} RfidCloneView;

// Comment out unused static functions to prevent build warnings
/*
static void rfid_clone_view_draw_callback(Canvas* canvas, void* context) {
    PredatorApp* app = context;
    
    if(!app) return;
    
    // Get state from app
    bool card_detected = app->packets_sent >= 30;
    bool clone_complete = app->packets_sent >= 50;
    uint8_t progress = app->packets_sent > 50 ? 100 : (app->packets_sent * 100) / 50;
    
    canvas_clear(canvas);
    
    // Draw title in header
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, 64, 8, AlignCenter, AlignCenter, "RFID Clone");
    
    // Draw separator
    canvas_draw_line(canvas, 0, 16, 128, 16);
    
    if(clone_complete) {
        // Show completion screen
        predator_ui_draw_status_box(canvas, "Success", 12, 24, 104, 32);
        
        canvas_set_font(canvas, FontSecondary);
        canvas_draw_str_aligned(canvas, 64, 32, AlignCenter, AlignCenter, "Clone successful!");
        
        canvas_set_font(canvas, FontSecondary);
        canvas_draw_str_aligned(canvas, 64, 42, AlignCenter, AlignCenter, "ID: 0x1234567890");
        canvas_draw_str_aligned(canvas, 64, 52, AlignCenter, AlignCenter, "Type: EM4100");
    } else if(card_detected) {
        // Show card reading status
        predator_ui_draw_status_box(canvas, "Card Detected", 12, 24, 104, 32);
        
        canvas_set_font(canvas, FontSecondary);
        canvas_draw_str_aligned(canvas, 64, 32, AlignCenter, AlignCenter, "Type: EM4100");
        canvas_draw_str_aligned(canvas, 64, 42, AlignCenter, AlignCenter, "ID: 0x1234567890");
        
        // Show cloning progress bar at bottom
        canvas_set_font(canvas, FontSecondary);
        canvas_draw_str(canvas, 14, 64, "Cloning:");
        predator_ui_draw_progress_bar(canvas, 58, 60, 56, 6, progress, ProgressBarStyleBordered);
    } else {
        // Show searching animation
        uint8_t animation_frame = (app->packets_sent / 5) % 4;
        
        // Draw scan lines to simulate search pattern
        for(int i = 0; i < 4; i++) {
            if(i == animation_frame) {
                canvas_draw_box(canvas, 44, 24 + (i * 8), 40, 2);
            } else {
                canvas_draw_line(canvas, 44, 25 + (i * 8), 84, 25 + (i * 8));
            }
        }
        
        predator_ui_draw_status_box(canvas, "Scanning", 12, 24, 104, 32);
        
        canvas_set_font(canvas, FontSecondary);
        canvas_draw_str_aligned(canvas, 64, 35, AlignCenter, AlignCenter, "Place card on");
        canvas_draw_str_aligned(canvas, 64, 45, AlignCenter, AlignCenter, "Flipper...");
        
        // Show search progress bar at bottom
        canvas_set_font(canvas, FontSecondary);
        canvas_draw_str(canvas, 14, 64, "Searching:");
        predator_ui_draw_progress_bar(canvas, 58, 60, 56, 6, progress, ProgressBarStyleAnimated);
    }
}
*/

/*
static bool rfid_clone_view_input_callback(InputEvent* event, void* context) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(event->type == InputTypeShort || event->type == InputTypeRepeat) {
        if(event->key == InputKeyBack) {
            view_dispatcher_send_custom_event(app->view_dispatcher, PredatorCustomEventPopupBack);
            consumed = true;
        }
    }
    
    return consumed;
}
*/

/*
static View* rfid_clone_view_alloc(PredatorApp* app) {
    View* view = view_alloc();
    view_set_context(view, app);
    view_set_draw_callback(view, rfid_clone_view_draw_callback);
    view_set_input_callback(view, rfid_clone_view_input_callback);
    
    return view;
}
*/

void predator_scene_rfid_clone_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    if(!app) {
        FURI_LOG_E("RfidClone", "App context is NULL on enter");
        return;
    }
    
    // Validate board type before any hardware initialization
    if(app->board_type == 0) { // Assuming 0 represents Unknown or default
        FURI_LOG_W("RfidClone", "Board type is Unknown, defaulting to Original");
        app->board_type = 0; // Keep as Original
    }
    
    // Ensure scene_manager and view_dispatcher are valid to prevent crashes
    if(!app->scene_manager) {
        FURI_LOG_E("RfidClone", "Scene manager is NULL, cannot proceed");
        return;
    }
    
    if(!app->view_dispatcher) {
        FURI_LOG_E("RfidClone", "View dispatcher is NULL, cannot switch view");
        return;
    }
    
    // Initialize RFID cloning - Comment out if functions are not defined
    // predator_rfid_init(app);
    
    // Set up the view for RFID cloning - Comment out to avoid warnings about unused functions
    // View* view = rfid_clone_view_alloc(app);
    // if(view) {
    //     // Replace popup view with custom view
    //     view_dispatcher_remove_view(app->view_dispatcher, PredatorViewPopup);
    //     view_dispatcher_add_view(app->view_dispatcher, PredatorViewPopup, view);
    //     view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
    //     
    //     // Initialize state
    //     app->attack_running = true;
    // } else {
    //     FURI_LOG_E("RfidClone", "Failed to allocate view for RFID Clone");
    // }
    
    // Switch to a safe view or show a placeholder message
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
    
    FURI_LOG_I("RfidClone", "RFID Clone scene entered with simulation mode");
}

bool predator_scene_rfid_clone_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == PredatorCustomEventPopupBack) {
            consumed = true;
            scene_manager_previous_scene(app->scene_manager);
        }
    } else if(event.type == SceneManagerEventTypeTick) {
        if(app->attack_running) {
            app->packets_sent++;
            // Force a redraw on every tick since we have animations
            view_dispatcher_send_custom_event(app->view_dispatcher, 0xFF);
            consumed = true;
        }
    }

    return consumed;
}

void predator_scene_rfid_clone_new_on_exit(void* context) {
    PredatorApp* app = context;
    app->attack_running = false;
    
    // Remove custom view and restore default popup view
    view_dispatcher_remove_view(app->view_dispatcher, PredatorViewPopup);
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewPopup, popup_get_view(app->popup));
}
