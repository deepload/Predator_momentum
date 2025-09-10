#include "../predator_i.h"
#include "../helpers/predator_view_helpers.h"
#include "../helpers/predator_esp32.h"
#include "../helpers/predator_boards.h"
#include "../helpers/predator_ui_elements.h"
#include "predator_scene.h"
#include <gui/elements.h>
#include <stdio.h>

typedef struct {
    View* view;
    bool marauder_switch_on;
    bool gps_switch_on;
    bool esp32_connected;
    bool gps_connected;
    uint32_t satellites;
    PredatorBoardType board_type;
    uint8_t animation_frame;
} ModuleStatusView;

static void module_status_view_draw_callback(Canvas* canvas, void* context) {
    PredatorApp* app = context;
    
    if(!app) return;
    
    // Get view state
    ModuleStatusView* state = PREDATOR_GET_MODEL(app->view_dispatcher, ModuleStatusView);
    if(!state) return;
    
    // Gather current hardware state
    bool marauder_switch_on = !furi_hal_gpio_read(PREDATOR_MARAUDER_SWITCH);
    bool gps_switch_on = !furi_hal_gpio_read(PREDATOR_GPS_POWER_SWITCH);
    bool esp32_connected = app->esp32_connected;
    bool gps_connected = app->gps_connected;
    uint32_t satellites = app->satellites;
    PredatorBoardType board_type = app->board_type;
    
    // Update animation frame
    uint8_t animation_frame = (furi_get_tick() / 200) % 4;
    
    // Update state
    state->marauder_switch_on = marauder_switch_on;
    state->gps_switch_on = gps_switch_on;
    state->esp32_connected = esp32_connected;
    state->gps_connected = gps_connected;
    state->satellites = satellites;
    state->board_type = board_type;
    state->animation_frame = animation_frame;
    
    // Consider the module "Connected" if any UART is initialized or any switch is ON
    bool connected = (app->esp32_uart || app->gps_uart || marauder_switch_on || gps_switch_on);
    
    canvas_clear(canvas);
    
    // Draw title with board type
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, 64, 8, AlignCenter, AlignCenter, "Module Status");
    
    // Draw board type
    const char* board_name = predator_boards_get_name(board_type);
    canvas_set_font(canvas, FontSecondary);
    predator_ui_draw_status_box(canvas, "Board Type", 4, 16, 120, 14);
    canvas_draw_str_aligned(canvas, 64, 24, AlignCenter, AlignCenter, board_name);
    
    // Draw connection status box
    predator_ui_draw_status_box(canvas, "Connection Status", 4, 32, 120, 30);
    
    // Draw visual representation of hardware
    
    // Draw ESP32 status
    canvas_draw_str(canvas, 8, 42, "ESP32:");
    
    // Draw ESP32 indicator
    if(esp32_connected) {
        canvas_draw_str(canvas, 44, 42, "Connected");
        
        // Show activity animation when connected
        for(uint8_t i = 0; i < (animation_frame % 3); i++) {
            canvas_draw_box(canvas, 110 - (i * 6), 42, 4, 4);
        }
    } else if(marauder_switch_on) {
        canvas_draw_str(canvas, 44, 42, "SW ON");
    } else {
        canvas_draw_str(canvas, 44, 42, "Inactive");
    }
    
    // Draw GPS status
    canvas_draw_str(canvas, 8, 54, "GPS:");
    
    if(gps_connected) {
        char sat_text[24];
        snprintf(sat_text, sizeof(sat_text), "Active (%lu sats)", satellites);
        canvas_draw_str(canvas, 44, 54, sat_text);
    } else if(gps_switch_on) {
        canvas_draw_str(canvas, 44, 54, "SW ON");
    } else {
        canvas_draw_str(canvas, 44, 54, "Inactive");
    }
    
    // Draw buttons box
    predator_ui_draw_status_box(canvas, "Actions", 4, 64, 120, 14);
    
    // Draw action buttons
    elements_button_center(canvas, "Probe ESP32");
    canvas_set_font(canvas, FontSecondary);
    elements_button_left(canvas, "Back");
}

static bool module_status_view_input_callback(InputEvent* event, void* context) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(event->type == InputTypeShort || event->type == InputTypeRepeat) {
        switch(event->key) {
        case InputKeyBack:
        case InputKeyLeft:
            scene_manager_previous_scene(app->scene_manager);
            consumed = true;
            break;
        case InputKeyOk:
        case InputKeyCenter:
            // Probe ESP32 status
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
            consumed = true;
            break;
        default:
            break;
        }
    }
    
    return consumed;
}

static View* module_status_view_alloc(PredatorApp* app) {
    View* view = view_alloc();
    view_set_context(view, app);
    view_set_draw_callback(view, module_status_view_draw_callback);
    view_set_input_callback(view, module_status_view_input_callback);
    
    // Initialize model
    ModuleStatusView* state = malloc(sizeof(ModuleStatusView));
    state->marauder_switch_on = false;
    state->gps_switch_on = false;
    state->esp32_connected = false;
    state->gps_connected = false;
    state->satellites = 0;
    state->board_type = app->board_type;
    state->animation_frame = 0;
    
    predator_view_set_model(view, state);
    predator_view_set_model_free_callback(view, free);
    
    return view;
}

static void module_status_view_free(View* view) {
    view_free(view);
}

void predator_scene_module_status_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    // Create custom view
    View* view = module_status_view_alloc(app);
    
    // Replace widget view with custom view
    view_dispatcher_remove_view(app->view_dispatcher, PredatorViewWidget);
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewWidget, view);
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewWidget);
}

bool predator_scene_module_status_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(event.type == SceneManagerEventTypeBack) {
        consumed = true;
        scene_manager_previous_scene(app->scene_manager);
    } else if(event.type == SceneManagerEventTypeTick) {
        // Refresh view on tick for animations
        view_dispatcher_send_custom_event(app->view_dispatcher, 0xFF);
        consumed = true;
    } else if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == PredatorCustomEventBack) {
            consumed = true;
            scene_manager_previous_scene(app->scene_manager);
        }
    }
    
    return consumed;
}

void predator_scene_module_status_new_on_exit(void* context) {
    PredatorApp* app = context;
    
    // Remove and free custom view
    view_dispatcher_remove_view(app->view_dispatcher, PredatorViewWidget);
    View* view = predator_view_dispatcher_get_current_view(app->view_dispatcher);
    if(view) {
        module_status_view_free(view);
    }
    
    // Restore standard widget view
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewWidget, widget_get_view(app->widget));
}


