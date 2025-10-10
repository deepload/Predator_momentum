#include "../predator_i.h"
#include <gui/view.h>
#include <string.h>

// About Screen - Professional UI
// Shows app information with page navigation

typedef struct {
    uint8_t current_page;
    uint8_t total_pages;
} AboutState;

static AboutState about_state;

static void draw_about_header(Canvas* canvas) {
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 2, 10, "ABOUT");
    canvas_draw_line(canvas, 0, 12, 128, 12);
}

static void draw_about_page(Canvas* canvas, AboutState* state) {
    canvas_set_font(canvas, FontSecondary);
    
    switch(state->current_page) {
        case 0: // Main info
            canvas_draw_str(canvas, 2, 24, "Predator Momentum");
            canvas_draw_str(canvas, 2, 34, "Version: 1.2");
            canvas_draw_str(canvas, 2, 44, "Build: 2025-01-10");
            canvas_draw_str(canvas, 2, 54, "API: 86.0");
            break;
            
        case 1: // Features
            canvas_draw_str(canvas, 2, 24, "Features:");
            canvas_draw_str(canvas, 2, 34, "- WiFi & BLE attacks");
            canvas_draw_str(canvas, 2, 44, "- Car hacking tools");
            canvas_draw_str(canvas, 2, 54, "- RFID & SubGHz");
            break;
            
        case 2: // Hardware
            canvas_draw_str(canvas, 2, 24, "Hardware Support:");
            canvas_draw_str(canvas, 2, 34, "- ESP32 (WiFi/BLE)");
            canvas_draw_str(canvas, 2, 44, "- GPS Module");
            canvas_draw_str(canvas, 2, 54, "- SubGHz Radio");
            break;
            
        case 3: // Credits
            canvas_draw_str(canvas, 2, 24, "Credits:");
            canvas_draw_str(canvas, 2, 34, "- Momentum Firmware");
            canvas_draw_str(canvas, 2, 44, "- ESP32 Marauder");
            canvas_draw_str(canvas, 2, 54, "- Predator Team");
            break;
            
        case 4: // Legal
            canvas_draw_str(canvas, 2, 24, "Legal Notice:");
            canvas_draw_str(canvas, 2, 34, "For ethical use only");
            canvas_draw_str(canvas, 2, 44, "Educational purposes");
            canvas_draw_str(canvas, 2, 54, "Obey local laws");
            break;
    }
    
    // Page indicator
    char page_str[16];
    snprintf(page_str, sizeof(page_str), "Page %u/%u", 
            state->current_page + 1, state->total_pages);
    canvas_draw_str(canvas, 45, 64, page_str);
}

static void about_ui_draw_callback(Canvas* canvas, void* context) {
    furi_assert(canvas);
    furi_assert(context);
    
    UNUSED(context);
    
    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);
    
    draw_about_header(canvas);
    draw_about_page(canvas, &about_state);
    
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str(canvas, 2, 64, "←→");
}

static bool about_ui_input_callback(InputEvent* event, void* context) {
    PredatorApp* app = context;
    if(!app) return false;
    
    if(event->type == InputTypeShort) {
        if(event->key == InputKeyBack) {
            return false; // Let scene manager handle back
        } else if(event->key == InputKeyLeft) {
            if(about_state.current_page > 0) {
                about_state.current_page--;
                return true;
            }
        } else if(event->key == InputKeyRight) {
            if(about_state.current_page < (about_state.total_pages - 1)) {
                about_state.current_page++;
                return true;
            }
        }
    }
    
    return true;
}

void predator_scene_about_ui_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    memset(&about_state, 0, sizeof(AboutState));
    about_state.current_page = 0;
    about_state.total_pages = 5;
    
    if(!app->view_dispatcher) {
        FURI_LOG_E("AboutUI", "View dispatcher is NULL");
        return;
    }
    
    View* view = view_alloc();
    if(!view) {
        FURI_LOG_E("AboutUI", "Failed to allocate view");
        return;
    }
    
    view_set_context(view, app);
    view_set_draw_callback(view, about_ui_draw_callback);
    view_set_input_callback(view, about_ui_input_callback);
    
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewAboutUI, view);
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewAboutUI);
    
    FURI_LOG_I("AboutUI", "About UI initialized");
}

bool predator_scene_about_ui_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    if(!app) return false;
    
    if(event.type == SceneManagerEventTypeCustom) {
        return true;
    }
    
    return false;
}

void predator_scene_about_ui_on_exit(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    if(app->view_dispatcher) {
        view_dispatcher_remove_view(app->view_dispatcher, PredatorViewAboutUI);
    }
    
    FURI_LOG_I("AboutUI", "About UI exited");
}
