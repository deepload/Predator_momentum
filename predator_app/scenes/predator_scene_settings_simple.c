#include "../predator_i.h"
#include <gui/view.h>

// Simple Settings - Minimal crash-safe implementation
// Shows basic info only, no complex interactions

static void simple_settings_draw_callback(Canvas* canvas, void* context) {
    UNUSED(context);
    
    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 2, 10, "SETTINGS");
    canvas_draw_line(canvas, 0, 12, 128, 12);
    
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str(canvas, 2, 24, "Region: Auto");
    canvas_draw_str(canvas, 2, 34, "Deauth Burst: ON");
    canvas_draw_str(canvas, 2, 44, "Logging: ON");
    canvas_draw_str(canvas, 2, 54, "Brightness: 80%");
    
    canvas_draw_str(canvas, 30, 64, "Back=Exit");
}

static bool simple_settings_input_callback(InputEvent* event, void* context) {
    UNUSED(context);
    
    if(event->type == InputTypeShort && event->key == InputKeyBack) {
        return false; // Let scene manager handle back
    }
    
    return true;
}

void predator_scene_settings_simple_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app || !app->view_dispatcher) return;
    
    View* view = view_alloc();
    if(!view) return;
    
    view_set_context(view, app);
    view_set_draw_callback(view, simple_settings_draw_callback);
    view_set_input_callback(view, simple_settings_input_callback);
    
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewSettingsUI, view);
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSettingsUI);
    
    FURI_LOG_I("SimpleSettings", "Simple settings initialized");
}

bool predator_scene_settings_simple_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    if(!app) return false;
    
    if(event.type == SceneManagerEventTypeBack) {
        scene_manager_previous_scene(app->scene_manager);
        return true;
    }
    
    return false;
}

void predator_scene_settings_simple_on_exit(void* context) {
    PredatorApp* app = context;
    if(!app || !app->view_dispatcher) return;
    
    view_dispatcher_remove_view(app->view_dispatcher, PredatorViewSettingsUI);
    FURI_LOG_I("SimpleSettings", "Simple settings exited");
}
