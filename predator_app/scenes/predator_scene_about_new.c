#include "../predator_i.h"
#include "../helpers/predator_view_helpers.h"
#include "../helpers/predator_ui_elements.h"
#include "predator_scene.h"

typedef struct {
    View* view;
    uint8_t animation_frame;
    uint8_t current_page;
    uint8_t total_pages;
} AboutView;

// Content for the about pages
typedef struct {
    const char* title;
    const char* text[4]; // Up to 4 lines of text per page
} AboutPage;

static const AboutPage about_pages[] = {
    {
        "Predator",
        {
            "Pentesting toolkit for",
            "Flipper Zero",
            "Version 1.2",
            "© 2025 Predator Team"
        }
    },
    {
        "Features",
        {
            "WiFi, Bluetooth attacks",
            "RFID, SubGHz tools",
            "Car hacking features",
            "GPS tracking & more"
        }
    },
    {
        "Credits",
        {
            "Based on Marauder & Momentum",
            "UI design: Predator Team",
            "Hardware support: DrB0rk",
            "Thanks to all contributors"
        }
    },
    {
        "Legal",
        {
            "For ethical use only",
            "Use responsibly and",
            "with proper authorization",
            "Educational purposes only"
        }
    }
};

#define ABOUT_PAGES_COUNT (sizeof(about_pages) / sizeof(about_pages[0]))

static void about_view_draw_callback(Canvas* canvas, void* context) {
    PredatorApp* app = context;
    
    if(!app) return;
    
    // Get view state
    AboutView* state = PREDATOR_GET_MODEL(app->view_dispatcher, AboutView);
    if(!state) return;
    
    // Update animation frame
    uint8_t animation_frame = (furi_get_tick() / 500) % 4;
    state->animation_frame = animation_frame;
    
    canvas_clear(canvas);
    
    // Draw title
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, 64, 8, AlignCenter, AlignCenter, about_pages[state->current_page].title);
    
    // Draw separator
    canvas_draw_line(canvas, 0, 16, 128, 16);
    
    // Draw logo animation at the top left corner
    uint8_t logo_x = 16;
    uint8_t logo_y = 28;
    
    // Simplified predator logo animation (changes shape slightly with each frame)
    switch(animation_frame) {
        case 0:
            // First frame - basic shape
            canvas_draw_box(canvas, logo_x - 6, logo_y - 6, 12, 12);
            canvas_set_color(canvas, ColorWhite);
            canvas_draw_box(canvas, logo_x - 2, logo_y - 2, 4, 4);
            canvas_set_color(canvas, ColorBlack);
            break;
        case 1:
            // Second frame - slightly different
            canvas_draw_box(canvas, logo_x - 6, logo_y - 6, 12, 12);
            canvas_draw_line(canvas, logo_x - 6, logo_y - 6, logo_x + 6, logo_y + 6);
            canvas_draw_line(canvas, logo_x + 6, logo_y - 6, logo_x - 6, logo_y + 6);
            break;
        case 2:
            // Third frame
            canvas_draw_circle(canvas, logo_x, logo_y, 6);
            canvas_set_color(canvas, ColorWhite);
            canvas_draw_disc(canvas, logo_x, logo_y, 2);
            canvas_set_color(canvas, ColorBlack);
            break;
        case 3:
            // Fourth frame
            canvas_draw_box(canvas, logo_x - 6, logo_y - 6, 12, 12);
            canvas_draw_frame(canvas, logo_x - 3, logo_y - 3, 6, 6);
            break;
    }
    
    // Draw page content
    canvas_set_font(canvas, FontSecondary);
    
    const AboutPage* page = &about_pages[state->current_page];
    for(uint8_t i = 0; i < 4; i++) {
        if(page->text[i]) {
            canvas_draw_str_aligned(canvas, 70, 28 + (i * 10), AlignCenter, AlignCenter, page->text[i]);
        }
    }
    
    // Draw page indicator
    predator_ui_draw_status_box(canvas, "Page", 48, 62, 32, 14);
    
    char page_text[16];
    snprintf(page_text, sizeof(page_text), "%d/%d", state->current_page + 1, state->total_pages);
    canvas_draw_str_aligned(canvas, 64, 70, AlignCenter, AlignCenter, page_text);
    
    // Draw navigation hints (vector arrows)
    predator_ui_draw_nav_hints_lr(
        canvas,
        70,
        state->current_page > 0,
        state->current_page < state->total_pages - 1);
}

static bool about_view_input_callback(InputEvent* event, void* context) {
    PredatorApp* app = context;
    bool consumed = false;
    
    // Get view state
    AboutView* state = PREDATOR_GET_MODEL(app->view_dispatcher, AboutView);
    if(!state) return consumed;
    
    if(event->type == InputTypeShort) {
        switch(event->key) {
        case InputKeyBack:
            scene_manager_previous_scene(app->scene_manager);
            consumed = true;
            break;
            
        case InputKeyLeft:
            if(state->current_page > 0) {
                state->current_page--;
                consumed = true;
            }
            break;
            
        case InputKeyRight:
            if(state->current_page < state->total_pages - 1) {
                state->current_page++;
                consumed = true;
            }
            break;
            
        default:
            break;
        }
    }
    
    return consumed;
}

static View* about_view_alloc(PredatorApp* app) {
    View* view = view_alloc();
    view_set_context(view, app);
    view_set_draw_callback(view, about_view_draw_callback);
    view_set_input_callback(view, about_view_input_callback);
    
    // Initialize model
    AboutView* state = malloc(sizeof(AboutView));
    state->animation_frame = 0;
    state->current_page = 0;
    state->total_pages = ABOUT_PAGES_COUNT;
    
    predator_view_set_model(view, state);
    predator_view_set_model_free_callback(view, free);
    
    return view;
}


void predator_scene_about_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    // Create custom view
    View* view = about_view_alloc(app);
    
    // Switch to a safe view before replacing to avoid dispatcher crash
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewLoading);
    // Replace widget view with custom view
    view_dispatcher_remove_view(app->view_dispatcher, PredatorViewWidget);
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewWidget, view);
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewWidget);
}

bool predator_scene_about_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(event.type == SceneManagerEventTypeTick) {
        // Force view refresh for animations
        view_dispatcher_send_custom_event(app->view_dispatcher, 0xFF);
        consumed = true;
    }
    
    return consumed;
}

void predator_scene_about_new_on_exit(void* context) {
    PredatorApp* app = context;
    
    // Switch to a safe view before removing to avoid dispatcher crash
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewLoading);
    // Remove custom view and restore default widget view
    view_dispatcher_remove_view(app->view_dispatcher, PredatorViewWidget);
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewWidget, widget_get_view(app->widget));
}



