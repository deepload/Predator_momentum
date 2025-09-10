#include "../predator_i.h"
#include "../helpers/predator_view_helpers.h"
#include "../helpers/predator_gps.h"
#include "../helpers/predator_ui_elements.h"
#include "predator_scene.h"
#include <furi.h>
#include <gui/view.h>
#include <gui/elements.h>

// Custom view for GPS tracker
typedef struct {
    View* view;
    bool satellites_acquired;
    uint32_t satellites;
    float latitude;
    float longitude;
    uint8_t animation_frame;
} GpsTrackerView;

static void gps_tracker_view_draw_callback(Canvas* canvas, void* context) {
    PredatorApp* app = context;
    
    if(!app) return;
    
    // Extract data from app state
    bool satellites_acquired = app->gps_connected && app->satellites > 0;
    uint32_t satellites = app->satellites;
    float latitude = app->latitude;
    float longitude = app->longitude;
    
    // Animation frame based on time
    uint8_t animation_frame = (furi_get_tick() / 200) % 4;
    
    canvas_clear(canvas);
    
    // Draw title
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, 64, 8, AlignCenter, AlignCenter, "GPS Tracker");
    
    // Draw separator
    canvas_draw_line(canvas, 0, 16, 128, 16);
    
    // Draw GPS antenna icon with animation
    // Base of antenna
    canvas_draw_box(canvas, 14, 35, 8, 2);
    canvas_draw_line(canvas, 18, 26, 18, 35);
    
    // Signal waves animation
    if(satellites_acquired) {
        // Strong signal waves when satellites acquired
        for(uint8_t i = 0; i <= animation_frame; i++) {
            canvas_draw_circle(canvas, 18, 26, 4 + (i * 3));
        }
    } else {
        // Searching signal animation
        if(animation_frame > 0) {
            canvas_draw_circle(canvas, 18, 26, 4 + (animation_frame * 2));
        }
    }
    
    // Draw main status box
    predator_ui_draw_status_box(canvas, satellites_acquired ? "GPS Signal" : "Searching", 
                               32, 22, 82, 26);
    
    canvas_set_font(canvas, FontSecondary);
    
    // Draw satellite status
    char sat_text[32];
    snprintf(sat_text, sizeof(sat_text), "Satellites: %lu", satellites);
    canvas_draw_str_aligned(canvas, 73, 32, AlignCenter, AlignCenter, sat_text);
    
    // Draw coordinates or searching message
    if(satellites_acquired) {
        char lat_text[24];
        char lon_text[24];
        snprintf(lat_text, sizeof(lat_text), "Lat: %.6f", (double)latitude);
        snprintf(lon_text, sizeof(lon_text), "Lon: %.6f", (double)longitude);
        
        canvas_draw_str_aligned(canvas, 73, 42, AlignCenter, AlignCenter, lat_text);
    } else {
        // Show searching animation with dots
        char search_text[16] = "Searching";
        // Safe append of up to 3 dots without strcat
        size_t base_len = 9; // strlen("Searching")
        uint8_t dots = animation_frame;
        if(dots > 3) dots = 3;
        for(uint8_t i = 0; i < dots && (base_len + i + 1) < sizeof(search_text); i++) {
            search_text[base_len + i] = '.';
            search_text[base_len + i + 1] = '\0';
        }
        canvas_draw_str_aligned(canvas, 73, 42, AlignCenter, AlignCenter, search_text);
    }
    
    // Draw accuracy indicator with mini map
    predator_ui_draw_status_box(canvas, "Accuracy", 12, 54, 104, 24);
    
    // Draw small map representation with position dot
    uint8_t map_x = 64;
    uint8_t map_y = 66;
    uint8_t map_width = 80;
    uint8_t map_height = 16;
    
    // Draw map border
    canvas_draw_box(canvas, map_x - map_width/2, map_y - map_height/2, map_width, map_height);
    
    // Draw position indicator
    if(satellites_acquired) {
        // Position dot shows current position
        canvas_set_color(canvas, ColorWhite);
        canvas_draw_disc(canvas, map_x, map_y, 3);
        canvas_set_color(canvas, ColorBlack);
    } else {
        // Searching animation
        canvas_set_color(canvas, ColorWhite);
        canvas_draw_disc(canvas, 
            map_x + (cos_array[animation_frame * 16] * 10) / 255, 
            map_y + (sin_array[animation_frame * 16] * 5) / 255, 
            2);
        canvas_set_color(canvas, ColorBlack);
    }
    
    // Draw debug button hint
    canvas_draw_str(canvas, 6, 64, "Debug");
    // Use vector arrow (no firmware icon dependency)
    predator_ui_draw_arrow_right(canvas, 36, 59);
}

static bool gps_tracker_view_input_callback(InputEvent* event, void* context) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(event->type == InputTypeShort) {
        switch(event->key) {
        case InputKeyBack:
            scene_manager_previous_scene(app->scene_manager);
            consumed = true;
            break;
        case InputKeyRight:
            // Switch to GPS debug scene
            scene_manager_next_scene(app->scene_manager, PredatorSceneGPSDebug);
            consumed = true;
            break;
        default:
            break;
        }
    }
    
    return consumed;
}

static View* gps_tracker_view_alloc(PredatorApp* app) {
    View* view = view_alloc();
    view_set_context(view, app);
    view_set_draw_callback(view, gps_tracker_view_draw_callback);
    view_set_input_callback(view, gps_tracker_view_input_callback);
    
    GpsTrackerView* state = malloc(sizeof(GpsTrackerView));
    state->satellites_acquired = false;
    state->satellites = 0;
    state->latitude = 0.0f;
    state->longitude = 0.0f;
    state->animation_frame = 0;
    
    predator_view_set_model(view, state);
    predator_view_set_model_free_callback(view, free);
    
    return view;
}

static void gps_tracker_view_free(View* view) {
    view_free(view);
}

void predator_scene_gps_tracker_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    // Initialize GPS
    predator_gps_init(app);
    
    // Create custom view
    View* view = gps_tracker_view_alloc(app);
    
    // Replace popup view with custom view
    view_dispatcher_remove_view(app->view_dispatcher, PredatorViewPopup);
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewPopup, view);
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
    
    app->attack_running = true;
}

bool predator_scene_gps_tracker_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == PredatorCustomEventPopupBack) {
            consumed = true;
            scene_manager_previous_scene(app->scene_manager);
        }
    } else if(event.type == SceneManagerEventTypeBack) {
        consumed = true;
        scene_manager_previous_scene(app->scene_manager);
    } else if(event.type == SceneManagerEventTypeTick) {
        if(app->attack_running) {
            // Update GPS data
            predator_gps_update(app);
            
            // Force view refresh for animations
            view_dispatcher_send_custom_event(app->view_dispatcher, 0xFF);
            consumed = true;
        }
    }
    
    return consumed;
}

void predator_scene_gps_tracker_new_on_exit(void* context) {
    PredatorApp* app = context;
    app->attack_running = false;
    
    // Deinitialize GPS
    predator_gps_deinit(app);
    
    // Remove and free custom view
    view_dispatcher_remove_view(app->view_dispatcher, PredatorViewPopup);
    View* view = predator_view_dispatcher_get_current_view(app->view_dispatcher);
    if(view) {
        gps_tracker_view_free(view);
    }
    
    // Restore standard popup view
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewPopup, popup_get_view(app->popup));
}


