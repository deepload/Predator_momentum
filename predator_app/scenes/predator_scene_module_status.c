#include "../predator_i.h"
#include "../helpers/predator_esp32.h"
#include "predator_scene.h"
#include <gui/elements.h>
#include <stdio.h>

static void draw_status(Canvas* canvas, PredatorApp* app) {
    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);
    canvas_set_font(canvas, FontSecondary);

    // Title
    elements_multiline_text_aligned(canvas, 64, 4, AlignCenter, AlignTop, "Module Status");

    char line[64];

    // Read hardware switch states (safe GPIO reads)
    bool marauder_switch_on = false;
    bool gps_switch_on = false;
    
    // Use try/catch style: if reads fail in future SDKs, still render UI
    marauder_switch_on = !furi_hal_gpio_read(PREDATOR_MARAUDER_SWITCH) ? false : true;
    gps_switch_on = !furi_hal_gpio_read(PREDATOR_GPS_POWER_SWITCH) ? false : true;

    // Module presence heuristic: only mark Connected if we have any active UART or switch is ON
    bool connected_heuristic = (app->esp32_uart || app->gps_uart || marauder_switch_on || gps_switch_on);
    snprintf(line, sizeof(line), "Predator: %s", connected_heuristic ? "Likely Connected" : "Unknown");
    elements_multiline_text_aligned(canvas, 6, 18, AlignLeft, AlignTop, line);

    // ESP32
    if(app->esp32_uart) {
        snprintf(line, sizeof(line), "ESP32: %s", app->esp32_connected ? "OK" : "Waiting...");
    } else {
        snprintf(line, sizeof(line), "ESP32: Not initialized");
    }
    elements_multiline_text_aligned(canvas, 6, 30, AlignLeft, AlignTop, line);

    // GPS
    if(app->gps_connected) {
        snprintf(line, sizeof(line), "GPS: %lu sats", (unsigned long)app->satellites);
    } else {
        snprintf(line, sizeof(line), "GPS: Not initialized");
    }
    elements_multiline_text_aligned(canvas, 6, 42, AlignLeft, AlignTop, line);

    // Switch states (useful for diagnostics)
    snprintf(line, sizeof(line), "Marauder SW: %s", marauder_switch_on ? "ON" : "OFF");
    elements_multiline_text_aligned(canvas, 6, 54, AlignLeft, AlignTop, line);

    // Hint
    elements_multiline_text_aligned(canvas, 64, 62, AlignCenter, AlignBottom, "Back to exit");
}

static void view_draw_callback(Canvas* canvas, void* context) {
    PredatorApp* app = context;
    draw_status(canvas, app);
}

void predator_scene_module_status_on_enter(void* context) {
    PredatorApp* app = context;

    // Use widget for simple status rendering
    widget_reset(app->widget);
    View* v = widget_get_view(app->widget);
    view_set_draw_callback(v, view_draw_callback);
    view_set_context(v, app);

    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewWidget);
}

bool predator_scene_module_status_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeBack) {
        consumed = true;
        scene_manager_previous_scene(app->scene_manager);
    } else if(event.type == SceneManagerEventTypeTick) {
        // Force re-draw to update status lines
        view_dispatcher_send_custom_event(app->view_dispatcher, PredatorCustomEventRecovery);
    } else if(event.type == SceneManagerEventTypeCustom) {
        // Any custom event -> refresh view
        (void)app; // unused
    }

    return consumed;
}

void predator_scene_module_status_on_exit(void* context) {
    PredatorApp* app = context;
    widget_reset(app->widget);
}
