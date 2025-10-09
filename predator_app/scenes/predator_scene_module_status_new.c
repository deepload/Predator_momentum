#include "../predator_i.h"
#include "../helpers/predator_view_helpers.h"
#include "../helpers/predator_esp32.h"
#include "../helpers/predator_boards.h"
#include "../helpers/predator_ui_elements.h"
#include "../helpers/predator_compliance.h"
#include "predator_scene.h"
#include <gui/elements.h>
#include <stdio.h>

typedef struct {
    uint8_t current_page;
    uint8_t total_pages;
    uint8_t anim;
} ModuleStatusView;

static void module_status_draw(Canvas* canvas, void* context) {
    PredatorApp* app = context;
    if(!app) return;

    ModuleStatusView* state = PREDATOR_GET_MODEL(app->view_dispatcher, ModuleStatusView);
    if(!state) return;

    state->anim = (furi_get_tick() / 250) % 4;

    canvas_clear(canvas);
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, 64, 8, AlignCenter, AlignCenter, "Module Status");
    canvas_draw_line(canvas, 0, 16, 128, 16);

    canvas_set_font(canvas, FontSecondary);

    if(state->current_page == 0) {
        // Hardware connections
        const char* esp = app->esp32_connected ? "ESP32: Connected" : "ESP32: Not connected";
        const char* gps = app->gps_connected ? "GPS: Active" : "GPS: Not active";
        char sats[24];
        snprintf(sats, sizeof(sats), "Satellites: %lu", app->satellites);

        predator_ui_draw_status_box(canvas, "Hardware", 8, 24, 112, 36);
        canvas_draw_str(canvas, 12, 36, esp);
        canvas_draw_str(canvas, 12, 46, gps);
        canvas_draw_str(canvas, 12, 56, sats);
    } else if(state->current_page == 1) {
        // Board / Region / Auth
        const char* board = predator_boards_get_name(app->board_type);
        const char* region = predator_compliance_region_str(app->region);
        const char* auth = app->authorized ? "Live" : "Demo Mode";

        predator_ui_draw_status_box(canvas, "Configuration", 8, 24, 112, 36);
        char line[32];
        snprintf(line, sizeof(line), "Board: %s", board ? board : "Unknown");
        canvas_draw_str(canvas, 12, 36, line);
        snprintf(line, sizeof(line), "Region: %s", region);
        canvas_draw_str(canvas, 12, 46, line);
        snprintf(line, sizeof(line), "Access: %s", auth);
        canvas_draw_str(canvas, 12, 56, line);
    } else {
        // Compliance summary
        predator_ui_draw_status_box(canvas, "Compliance", 8, 24, 112, 36);
        canvas_draw_str(canvas, 12, 36, "WiFi Deauth: Auth required");
        canvas_draw_str(canvas, 12, 46, "Evil Twin: Auth required");
        canvas_draw_str(canvas, 12, 56, "SubGHz TX: Region limits");
    }

    // Page indicator and hints
    char pagebuf[12];
    snprintf(pagebuf, sizeof(pagebuf), "%u/%u", (unsigned)(state->current_page + 1), (unsigned)state->total_pages);
    canvas_draw_str_aligned(canvas, 64, 62, AlignCenter, AlignCenter, pagebuf);
    predator_ui_draw_nav_hints_lr(canvas, 64, state->current_page > 0, state->current_page + 1 < state->total_pages);
}

static bool module_status_input(InputEvent* event, void* context) {
    PredatorApp* app = context;
    if(!app) return false;
    ModuleStatusView* state = PREDATOR_GET_MODEL(app->view_dispatcher, ModuleStatusView);
    if(!state) return false;

    if(event->type == InputTypeShort) {
        if(event->key == InputKeyBack) {
            scene_manager_previous_scene(app->scene_manager);
            return true;
        } else if(event->key == InputKeyLeft) {
            if(state->current_page > 0) state->current_page--;
            return true;
        } else if(event->key == InputKeyRight) {
            if(state->current_page + 1 < state->total_pages) state->current_page++;
            return true;
        }
    }
    return false;
}

void predator_scene_module_status_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    if(!app) {
        FURI_LOG_E("ModuleStatus", "App context is NULL on enter");
        return;
    }
    if(!app->scene_manager || !app->view_dispatcher) {
        FURI_LOG_E("ModuleStatus", "Invalid scene/view dispatcher");
        return;
    }

    // Create a custom view and attach model
    View* view = view_alloc();
    view_set_context(view, app);
    view_set_draw_callback(view, module_status_draw);
    view_set_input_callback(view, module_status_input);

    ModuleStatusView* state = malloc(sizeof(ModuleStatusView));
    state->current_page = 0;
    state->total_pages = 3;
    state->anim = 0;
    predator_view_set_model(view, state);
    predator_view_set_model_free_callback(view, free);

    // Replace widget with custom view for this scene
    view_dispatcher_remove_view(app->view_dispatcher, PredatorViewWidget);
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewWidget, view);
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewWidget);

    FURI_LOG_I("ModuleStatus", "Module Status scene entered (paginated view)");
}

bool predator_scene_module_status_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    if(!app) return false;
    if(event.type == SceneManagerEventTypeTick) {
        // Could update live stats here if needed
        return true;
    } else if(event.type == SceneManagerEventTypeBack) {
        scene_manager_previous_scene(app->scene_manager);
        return true;
    }
    return false;
}

void predator_scene_module_status_new_on_exit(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    // Restore default widget view
    view_dispatcher_remove_view(app->view_dispatcher, PredatorViewWidget);
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewWidget, widget_get_view(app->widget));
    FURI_LOG_I("ModuleStatus", "Exited Module Status scene");
}
