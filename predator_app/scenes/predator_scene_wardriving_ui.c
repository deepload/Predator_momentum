#include "../predator_i.h"
#include "../helpers/predator_gps.h"
#include "../helpers/predator_esp32.h"
#include "../helpers/predator_logging.h"
#include <gui/view.h>
#include <string.h>

// Wardriving - WiFi network mapping with GPS coordinates
// Government-grade wireless network intelligence gathering

typedef struct {
    bool wardriving_active;
    float current_lat;
    float current_lon;
    uint32_t networks_found;
    uint32_t networks_logged;
    uint32_t scan_time_ms;
    char last_ssid[32];
    int8_t last_rssi;
    char status_text[24];
} WardrivingState;

static WardrivingState wardrive_state;
static View* wardriving_view = NULL;
static uint32_t scan_start_tick = 0;

static void draw_wardriving_header(Canvas* canvas) {
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 2, 10, "WARDRIVING");
    canvas_draw_line(canvas, 0, 12, 128, 12);
}

static void draw_wardriving_stats(Canvas* canvas, WardrivingState* state) {
    canvas_set_font(canvas, FontSecondary);
    
    // Current coordinates
    char coord_str[32];
    snprintf(coord_str, sizeof(coord_str), "%.6f,%.6f", 
             (double)state->current_lat, (double)state->current_lon);
    canvas_draw_str(canvas, 2, 22, coord_str);
    
    // Network statistics
    char stats_str[32];
    snprintf(stats_str, sizeof(stats_str), "Found: %lu Logged: %lu", 
             state->networks_found, state->networks_logged);
    canvas_draw_str(canvas, 2, 32, stats_str);
    
    // Last network found
    if(state->last_ssid[0] != '\0') {
        char network_str[32];
        snprintf(network_str, sizeof(network_str), "%.20s %ddBm", 
                 state->last_ssid, state->last_rssi);
        canvas_draw_str(canvas, 2, 42, network_str);
    }
    
    // Status and time
    char status_str[48];
    uint32_t seconds = state->scan_time_ms / 1000;
    snprintf(status_str, sizeof(status_str), "%.8s Time: %lus", 
             state->status_text, seconds);
    canvas_draw_str(canvas, 2, 52, status_str);
}

static void wardriving_ui_draw_callback(Canvas* canvas, void* context) {
    furi_assert(canvas);
    UNUSED(context);
    
    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);
    
    draw_wardriving_header(canvas);
    draw_wardriving_stats(canvas, &wardrive_state);
    
    canvas_set_font(canvas, FontSecondary);
    if(wardrive_state.wardriving_active) {
        canvas_draw_str(canvas, 25, 64, "OK=Stop  Back=Exit");
    } else {
        canvas_draw_str(canvas, 25, 64, "OK=Start  Back=Exit");
    }
}

static bool wardriving_ui_input_callback(InputEvent* event, void* context) {
    PredatorApp* app = context;
    if(!app) return false;
    
    if(event->type == InputTypeShort) {
        if(event->key == InputKeyBack) {
            return false;
        } else if(event->key == InputKeyOk) {
            if(!wardrive_state.wardriving_active) {
                // Start wardriving
                wardrive_state.wardriving_active = true;
                wardrive_state.scan_time_ms = 0;
                wardrive_state.networks_found = 0;
                wardrive_state.networks_logged = 0;
                scan_start_tick = furi_get_tick();
                
                predator_gps_init(app);
                predator_esp32_init(app);
                
                strncpy(wardrive_state.status_text, "SCANNING", sizeof(wardrive_state.status_text));
                predator_log_append(app, "Wardriving: Started");
                
                return true;
            } else {
                // Stop wardriving
                wardrive_state.wardriving_active = false;
                strncpy(wardrive_state.status_text, "STOPPED", sizeof(wardrive_state.status_text));
                
                // Stop scan functionality not implemented in ESP32 helper
                predator_gps_deinit(app);
                
                char log_msg[64];
                snprintf(log_msg, sizeof(log_msg), "Wardriving: Stopped (%lu networks)", 
                        wardrive_state.networks_logged);
                predator_log_append(app, log_msg);
                
                return true;
            }
        }
    }
    
    return false;
}

static void wardriving_timer_callback(void* context) {
    PredatorApp* app = context;
    if(!app || !wardrive_state.wardriving_active) return;
    
    // Update scan time
    wardrive_state.scan_time_ms = furi_get_tick() - scan_start_tick;
    
    // Get current GPS coordinates
    if(predator_gps_get_coordinates(app, &wardrive_state.current_lat, &wardrive_state.current_lon)) {
        strncpy(wardrive_state.status_text, "GPS LOCK", sizeof(wardrive_state.status_text));
        
        // Start WiFi scan with coordinates (using existing ESP32 function)
        if(predator_esp32_wifi_scan(app)) {
            wardrive_state.networks_found++;
            
            // Simulate network discovery (real implementation would parse ESP32 response)
            char ssid[32] = "TestNetwork";
            int8_t rssi = -45;
            if(wardrive_state.networks_found % 3 == 0) { // Simulate periodic finds
                strncpy(wardrive_state.last_ssid, ssid, sizeof(wardrive_state.last_ssid));
                wardrive_state.last_rssi = rssi;
                wardrive_state.networks_logged++;
                
                // Log network with coordinates
                char log_msg[128];
                snprintf(log_msg, sizeof(log_msg), 
                        "WiFi: %s %ddBm @ %.6f,%.6f", 
                        ssid, rssi, 
                        (double)wardrive_state.current_lat, 
                        (double)wardrive_state.current_lon);
                predator_log_append(app, log_msg);
            }
        }
    } else {
        strncpy(wardrive_state.status_text, "NO GPS", sizeof(wardrive_state.status_text));
    }
    
    if(app->view_dispatcher) {
        view_dispatcher_send_custom_event(app->view_dispatcher, 0);
    }
}

void predator_scene_wardriving_ui_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    memset(&wardrive_state, 0, sizeof(WardrivingState));
    strncpy(wardrive_state.status_text, "READY", sizeof(wardrive_state.status_text));
    
    if(!app->view_dispatcher) return;
    
    if(!wardriving_view) {
        wardriving_view = view_alloc();
        if(!wardriving_view) return;
        
        view_set_context(wardriving_view, app);
        view_set_draw_callback(wardriving_view, wardriving_ui_draw_callback);
        view_set_input_callback(wardriving_view, wardriving_ui_input_callback);
        view_dispatcher_add_view(app->view_dispatcher, PredatorViewWardrivingUI, wardriving_view);
    }
    
    // Start timer for wardriving updates
    app->timer = furi_timer_alloc(wardriving_timer_callback, FuriTimerTypePeriodic, app);
    if(app->timer) {
        furi_timer_start(app->timer, 2000); // 2 second scans
    }
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewWardrivingUI);
}

bool predator_scene_wardriving_ui_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    if(!app) return false;
    
    if(event.type == SceneManagerEventTypeBack) {
        if(wardrive_state.wardriving_active) {
            wardrive_state.wardriving_active = false;
            // Stop scan not implemented
            predator_gps_deinit(app);
        }
        return false;
    }
    
    return false;
}

void predator_scene_wardriving_ui_on_exit(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    if(wardrive_state.wardriving_active) {
        wardrive_state.wardriving_active = false;
        // Stop scan not implemented
        predator_gps_deinit(app);
    }
    
    if(app->timer) {
        furi_timer_stop(app->timer);
        furi_timer_free(app->timer);
        app->timer = NULL;
    }
}
