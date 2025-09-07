#include "../predator_i.h"
#include "../helpers/predator_esp32.h"
#include "../helpers/predator_boards.h"
#include "predator_scene.h"
#include <gui/elements.h>
#include <stdio.h>

static void module_status_button_cb(GuiButtonType type, InputType input, void* context) {
    if(input != InputTypeShort) return;
    if(type != GuiButtonTypeCenter) return;
    PredatorApp* app = context;
    if(!app) return;
    view_dispatcher_send_custom_event(app->view_dispatcher, 1);
}

static void module_status_build_widget(PredatorApp* app) {
    if(!app || !app->widget) return;

    // Gather state
    // Treat front switches as active-low (ON when GPIO reads 0)
    bool marauder_switch_on = !furi_hal_gpio_read(PREDATOR_MARAUDER_SWITCH);
    bool gps_switch_on = !furi_hal_gpio_read(PREDATOR_GPS_POWER_SWITCH);

    // Consider the module "Connected" if any UART was initialized or any switch is ON
    bool connected = (app->esp32_uart || app->gps_uart || marauder_switch_on || gps_switch_on);

    // Rebuild widget contents
    widget_reset(app->widget);
    widget_add_string_element(app->widget, 64, 4, AlignCenter, AlignTop, FontSecondary, "Module Status");
    
    // Add board type information
    const char* board_name = predator_boards_get_name(app->board_type);
    widget_add_string_element(app->widget, 64, 15, AlignCenter, AlignTop, FontPrimary, board_name);

    char line[64];

    // Board is now displayed at the top, so this line is redundant
    // Just show connection status
    snprintf(line, sizeof(line), "Status: %s", connected ? "Connected" : "Not Detected");
    widget_add_string_element(app->widget, 6, 18, AlignLeft, AlignTop, FontSecondary, line);

    if(app->esp32_uart) {
        snprintf(line, sizeof(line), "ESP32: %s", app->esp32_connected ? "OK" : "UART Up");
    } else {
        snprintf(line, sizeof(line), "ESP32: Not initialized");
    }
    widget_add_string_element(app->widget, 6, 30, AlignLeft, AlignTop, FontSecondary, line);

    if(app->gps_connected) {
        snprintf(line, sizeof(line), "GPS: %lu sats", (unsigned long)app->satellites);
    } else {
        snprintf(line, sizeof(line), "GPS: Not initialized");
    }
    widget_add_string_element(app->widget, 6, 42, AlignLeft, AlignTop, FontSecondary, line);

    snprintf(line, sizeof(line), "Marauder SW: %s (raw=%d)", marauder_switch_on ? "ON" : "OFF", !marauder_switch_on);
    widget_add_string_element(app->widget, 6, 54, AlignLeft, AlignTop, FontSecondary, line);
    snprintf(line, sizeof(line), "GPS SW: %s (raw=%d)", gps_switch_on ? "ON" : "OFF", !gps_switch_on);
    widget_add_string_element(app->widget, 6, 64, AlignLeft, AlignBottom, FontSecondary, line);

    // Add a Probe button (Center) to query ESP32 status when Marauder switch is ON
    widget_add_button_element(app->widget, GuiButtonTypeCenter, "Probe ESP32", module_status_button_cb, app);
    widget_add_string_element(app->widget, 64, 52, AlignCenter, AlignTop, FontSecondary, "Back to exit");
}

// Handle Probe action from Center button via navigation events
static void module_status_probe(PredatorApp* app) {
    if(!app) return;
    // Only probe if Marauder switch is ON to avoid risky init when hardware is off
    bool marauder_on = !furi_hal_gpio_read(PREDATOR_MARAUDER_SWITCH);
    if(marauder_on) {
        // Initialize if not yet and then send status
        if(!app->esp32_uart) {
            predator_esp32_init(app);
        }
        if(app->esp32_uart) {
            predator_esp32_get_status(app);
        }
    }
}

void predator_scene_module_status_on_enter(void* context) {
    PredatorApp* app = context;
    module_status_build_widget(app);
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewWidget);
}

bool predator_scene_module_status_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeBack) {
        consumed = true;
        scene_manager_previous_scene(app->scene_manager);
    } else if(event.type == SceneManagerEventTypeTick) {
        // Periodically refresh the status text
        module_status_build_widget(app);
    } else if(event.type == SceneManagerEventTypeCustom) {
        // Treat any custom event here as a Probe request (widget center pressed)
        module_status_probe(app);
        module_status_build_widget(app);
    }

    return consumed;
}

void predator_scene_module_status_on_exit(void* context) {
    PredatorApp* app = context;
    widget_reset(app->widget);
}
