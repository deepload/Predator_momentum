#include "../predator_i.h"
#include "../helpers/predator_gps.h"
#include "../helpers/predator_esp32.h"
#include "../helpers/predator_logging.h"
#include <gui/view.h>
#include <string.h>

// Module Status - Professional UI
// Shows real-time hardware status for all modules

typedef struct {
    bool esp32_connected;
    bool gps_connected;
    bool subghz_ready;
    uint8_t satellites;
    float voltage;
    char board_name[24];
    uint32_t uptime_ms;
} ModuleStatusState;

static ModuleStatusState status_state;
static uint32_t start_tick = 0;

static void draw_module_status_header(Canvas* canvas) {
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 2, 10, "MODULE STATUS");
    canvas_draw_line(canvas, 0, 12, 128, 12);
}

static void draw_module_status_info(Canvas* canvas, ModuleStatusState* state) {
    canvas_set_font(canvas, FontSecondary);
    
    // ESP32 status
    canvas_draw_str(canvas, 2, 22, "ESP32:");
    const char* esp32_icon = state->esp32_connected ? "✓ OK" : "✗ OFF";
    canvas_draw_str(canvas, 50, 22, esp32_icon);
    
    // GPS status
    canvas_draw_str(canvas, 2, 32, "GPS:");
    char gps_status[24];
    if(state->gps_connected) {
        snprintf(gps_status, sizeof(gps_status), "✓ %u sats", (unsigned)state->satellites);
    } else {
        snprintf(gps_status, sizeof(gps_status), "✗ OFF");
    }
    canvas_draw_str(canvas, 50, 32, gps_status);
    
    // SubGHz status
    canvas_draw_str(canvas, 2, 42, "SubGHz:");
    const char* subghz_icon = state->subghz_ready ? "✓ Ready" : "✗ OFF";
    canvas_draw_str(canvas, 50, 42, subghz_icon);
    
    // Board type
    canvas_draw_str(canvas, 2, 52, "Board:");
    canvas_draw_str(canvas, 50, 52, state->board_name);
    
    // Voltage
    char voltage_str[24];
    snprintf(voltage_str, sizeof(voltage_str), "Volt: %.2fV", (double)state->voltage);
    canvas_draw_str(canvas, 2, 62, voltage_str);
    
    // Uptime
    char uptime_str[24];
    uint32_t seconds = state->uptime_ms / 1000;
    if(seconds >= 3600) {
        snprintf(uptime_str, sizeof(uptime_str), "%luh %lum", seconds / 3600, (seconds % 3600) / 60);
    } else if(seconds >= 60) {
        snprintf(uptime_str, sizeof(uptime_str), "%lum %lus", seconds / 60, seconds % 60);
    } else {
        snprintf(uptime_str, sizeof(uptime_str), "%lus", seconds);
    }
    canvas_draw_str(canvas, 70, 62, uptime_str);
}

static void module_status_ui_draw_callback(Canvas* canvas, void* context) {
    furi_assert(canvas);
    furi_assert(context);
    
    UNUSED(context);
    
    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);
    
    draw_module_status_header(canvas);
    draw_module_status_info(canvas, &status_state);
    
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str(canvas, 40, 64, "Back=Exit");
}

static bool module_status_ui_input_callback(InputEvent* event, void* context) {
    PredatorApp* app = context;
    if(!app) return false;
    
    if(event->type == InputTypeShort) {
        if(event->key == InputKeyBack) {
            return false; // Let scene manager handle back
        }
    }
    
    return true;
}

static void module_status_ui_timer_callback(void* context) {
    furi_assert(context);
    PredatorApp* app = context;
    
    // Update uptime
    status_state.uptime_ms = furi_get_tick() - start_tick;
    
    // Update module status from app state
    status_state.esp32_connected = (app->esp32_uart != NULL);
    status_state.gps_connected = (app->satellites > 0);
    status_state.satellites = app->satellites;
    status_state.subghz_ready = (app->subghz_txrx != NULL);
    
    // Simulate voltage (would read from actual hardware)
    status_state.voltage = 3.3f + (status_state.uptime_ms % 100) * 0.001f;
    
    // Trigger view update
    if(app->view_dispatcher) {
        view_dispatcher_send_custom_event(app->view_dispatcher, 0);
    }
}

void predator_scene_module_status_ui_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    memset(&status_state, 0, sizeof(ModuleStatusState));
    start_tick = furi_get_tick();
    
    // Set board name
    switch(app->board_type) {
        case PredatorBoardTypeOriginal:
            snprintf(status_state.board_name, sizeof(status_state.board_name), "Original");
            break;
        case PredatorBoardType3in1AIO:
            snprintf(status_state.board_name, sizeof(status_state.board_name), "3in1 AIO");
            break;
        case PredatorBoardTypeDrB0rkMulti:
            snprintf(status_state.board_name, sizeof(status_state.board_name), "DrB0rk");
            break;
        case PredatorBoardTypeScreen28:
            snprintf(status_state.board_name, sizeof(status_state.board_name), "2.8\" Screen");
            break;
        default:
            snprintf(status_state.board_name, sizeof(status_state.board_name), "Unknown");
            break;
    }
    
    if(!app->view_dispatcher) {
        FURI_LOG_E("ModuleStatusUI", "View dispatcher is NULL");
        return;
    }
    
    View* view = view_alloc();
    if(!view) {
        FURI_LOG_E("ModuleStatusUI", "Failed to allocate view");
        return;
    }
    
    view_set_context(view, app);
    view_set_draw_callback(view, module_status_ui_draw_callback);
    view_set_input_callback(view, module_status_ui_input_callback);
    
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewModuleStatusUI, view);
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewModuleStatusUI);
    
    FURI_LOG_I("ModuleStatusUI", "Module Status UI initialized");
    
    app->timer = furi_timer_alloc(module_status_ui_timer_callback, FuriTimerTypePeriodic, app);
    furi_timer_start(app->timer, 1000); // Update every second
}

bool predator_scene_module_status_ui_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    if(!app) return false;
    
    if(event.type == SceneManagerEventTypeCustom) {
        return true;
    }
    
    return false;
}

void predator_scene_module_status_ui_on_exit(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    if(app->timer) {
        furi_timer_stop(app->timer);
        furi_timer_free(app->timer);
        app->timer = NULL;
    }
    
    if(app->view_dispatcher) {
        view_dispatcher_remove_view(app->view_dispatcher, PredatorViewModuleStatusUI);
    }
    
    FURI_LOG_I("ModuleStatusUI", "Module Status UI exited");
}
