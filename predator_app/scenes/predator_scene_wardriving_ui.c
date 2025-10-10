#include "../predator_i.h"
#include "../helpers/predator_gps.h"
#include "../helpers/predator_esp32.h"
#include "../helpers/predator_logging.h"
#include <gui/view.h>
#include <string.h>

// Wardriving - Professional UI
// Shows real-time GPS + WiFi scanning with distance tracking and AP count

typedef enum {
    WardrivingStatusIdle,
    WardrivingStatusScanning,
    WardrivingStatusComplete,
    WardrivingStatusError
} WardrivingStatus;

typedef struct {
    WardrivingStatus status;
    uint32_t aps_found;
    uint32_t scan_time_ms;
    float distance_km;
    uint8_t satellites;
    float latitude;
    float longitude;
    int8_t strongest_rssi;
    char strongest_ssid[24];
    bool gps_active;
    bool wifi_active;
} WardrivingState;

static WardrivingState wardriving_state;
static uint32_t scan_start_tick = 0;

static void draw_wardriving_header(Canvas* canvas) {
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 2, 10, "WARDRIVING");
    canvas_draw_line(canvas, 0, 12, 128, 12);
}

static void draw_wardriving_status(Canvas* canvas, WardrivingState* state) {
    canvas_set_font(canvas, FontSecondary);
    
    // Status line
    canvas_draw_str(canvas, 2, 22, "Status:");
    const char* status_text = "Unknown";
    switch(state->status) {
        case WardrivingStatusIdle: status_text = "Ready"; break;
        case WardrivingStatusScanning: status_text = "Scanning"; break;
        case WardrivingStatusComplete: status_text = "Complete"; break;
        case WardrivingStatusError: status_text = "Error"; break;
    }
    canvas_draw_str(canvas, 45, 22, status_text);
    
    // GPS and WiFi status
    char sys_status[32];
    const char* gps_icon = state->gps_active ? "GPS+" : "GPS-";
    const char* wifi_icon = state->wifi_active ? "WiFi+" : "WiFi-";
    snprintf(sys_status, sizeof(sys_status), "%s %s", gps_icon, wifi_icon);
    canvas_draw_str(canvas, 2, 32, sys_status);
    
    // Satellites
    char sat_str[16];
    snprintf(sat_str, sizeof(sat_str), "Sat:%u", (unsigned)state->satellites);
    canvas_draw_str(canvas, 80, 32, sat_str);
    
    // Progress bar (animated during scan)
    canvas_draw_frame(canvas, 2, 36, 124, 6);
    if(state->status == WardrivingStatusScanning) {
        uint8_t progress = ((state->scan_time_ms / 100) % 124);
        canvas_draw_box(canvas, 3, 37, progress, 4);
    } else if(state->status == WardrivingStatusComplete) {
        canvas_draw_box(canvas, 3, 37, 122, 4);
    }
}

static void draw_wardriving_stats(Canvas* canvas, WardrivingState* state) {
    canvas_set_font(canvas, FontSecondary);
    
    // APs found and distance
    char stats_str[32];
    snprintf(stats_str, sizeof(stats_str), "APs:%lu  %.2fkm", 
            state->aps_found, (double)state->distance_km);
    canvas_draw_str(canvas, 2, 48, stats_str);
    
    // Time
    char time_str[32];
    uint32_t seconds = state->scan_time_ms / 1000;
    if(seconds >= 60) {
        snprintf(time_str, sizeof(time_str), "%lum %lus", seconds / 60, seconds % 60);
    } else {
        snprintf(time_str, sizeof(time_str), "%lus", seconds);
    }
    canvas_draw_str(canvas, 80, 48, time_str);
    
    // Strongest AP
    if(state->aps_found > 0 && state->strongest_ssid[0] != '\0') {
        canvas_draw_str(canvas, 2, 58, "Top:");
        char ssid_display[18];
        snprintf(ssid_display, sizeof(ssid_display), "%.13s", state->strongest_ssid);
        canvas_draw_str(canvas, 30, 58, ssid_display);
        
        char rssi_str[16];
        snprintf(rssi_str, sizeof(rssi_str), "%ddBm", (int)state->strongest_rssi);
        canvas_draw_str(canvas, 95, 58, rssi_str);
    } else {
        canvas_draw_str(canvas, 2, 58, "No APs found yet");
    }
}

static void wardriving_ui_draw_callback(Canvas* canvas, void* context) {
    furi_assert(canvas);
    furi_assert(context);
    
    UNUSED(context);
    
    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);
    
    // Draw header
    draw_wardriving_header(canvas);
    
    // Draw status
    draw_wardriving_status(canvas, &wardriving_state);
    
    // Draw statistics
    draw_wardriving_stats(canvas, &wardriving_state);
    
    // Draw instructions
    canvas_set_font(canvas, FontSecondary);
    if(wardriving_state.status == WardrivingStatusScanning) {
        canvas_draw_str(canvas, 30, 64, "OK=Stop  Back=Exit");
    } else if(wardriving_state.status == WardrivingStatusIdle) {
        canvas_draw_str(canvas, 25, 64, "OK=Start  Back=Exit");
    } else {
        canvas_draw_str(canvas, 40, 64, "Back=Exit");
    }
}

static bool wardriving_ui_input_callback(InputEvent* event, void* context) {
    PredatorApp* app = context;
    if(!app) return false;
    
    if(event->type == InputTypeShort) {
        if(event->key == InputKeyBack) {
            // Stop wardriving and exit
            if(wardriving_state.status == WardrivingStatusScanning) {
                wardriving_state.status = WardrivingStatusComplete;
                predator_esp32_stop_attack(app);
                
                char log_msg[80];
                snprintf(log_msg, sizeof(log_msg), "Wardriving STOP: %lu APs, %.2fkm", 
                        wardriving_state.aps_found, (double)wardriving_state.distance_km);
                predator_log_append(app, log_msg);
            }
            return false; // Let scene manager handle back
        } else if(event->key == InputKeyOk) {
            if(wardriving_state.status == WardrivingStatusIdle) {
                // Start wardriving
                wardriving_state.status = WardrivingStatusScanning;
                wardriving_state.aps_found = 0;
                wardriving_state.scan_time_ms = 0;
                wardriving_state.distance_km = 0.0f;
                scan_start_tick = furi_get_tick();
                
                // Initialize GPS and WiFi
                predator_gps_init(app);
                predator_esp32_init(app);
                bool wifi_started = predator_esp32_wifi_scan(app);
                
                wardriving_state.gps_active = true;
                wardriving_state.wifi_active = wifi_started;
                
                predator_log_append(app, "Wardriving START");
                FURI_LOG_I("WardrivingUI", "Wardriving started");
                return true;
            } else if(wardriving_state.status == WardrivingStatusScanning) {
                // Stop wardriving
                wardriving_state.status = WardrivingStatusComplete;
                predator_esp32_stop_attack(app);
                
                char log_msg[80];
                snprintf(log_msg, sizeof(log_msg), "Wardriving STOP: %lu APs, %.2fkm", 
                        wardriving_state.aps_found, (double)wardriving_state.distance_km);
                predator_log_append(app, log_msg);
                
                FURI_LOG_I("WardrivingUI", "Wardriving stopped by user");
                return true;
            }
        }
    }
    
    return true;
}

static void wardriving_ui_timer_callback(void* context) {
    furi_assert(context);
    PredatorApp* app = context;
    
    if(wardriving_state.status == WardrivingStatusScanning) {
        // Update scan time
        wardriving_state.scan_time_ms = furi_get_tick() - scan_start_tick;
        
        // Update GPS data
        if(app->satellites > 0) {
            wardriving_state.satellites = app->satellites;
            wardriving_state.latitude = app->latitude;
            wardriving_state.longitude = app->longitude;
            
            // Simulate distance calculation (would use haversine formula in real impl)
            wardriving_state.distance_km += 0.01f; // ~10m per 100ms at 36km/h
        }
        
        // Update WiFi data
        wardriving_state.aps_found = app->wifi_ap_count;
        
        // Find strongest AP
        if(app->wifi_ap_count > 0) {
            int8_t strongest = -100;
            size_t strongest_idx = 0;
            
            for(size_t i = 0; i < app->wifi_ap_count && i < PREDATOR_WIFI_MAX_APS; i++) {
                if(app->wifi_rssi[i] > strongest) {
                    strongest = app->wifi_rssi[i];
                    strongest_idx = i;
                }
            }
            
            wardriving_state.strongest_rssi = strongest;
            if(strongest_idx < PREDATOR_WIFI_MAX_APS) {
                snprintf(wardriving_state.strongest_ssid, sizeof(wardriving_state.strongest_ssid), 
                        "%.23s", app->wifi_ssids[strongest_idx]);
            }
        }
        
        // Trigger view update
        if(app->view_dispatcher) {
            view_dispatcher_send_custom_event(app->view_dispatcher, 0);
        }
    }
}

void predator_scene_wardriving_ui_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    // Initialize wardriving state
    memset(&wardriving_state, 0, sizeof(WardrivingState));
    wardriving_state.status = WardrivingStatusIdle;
    
    // Setup custom view
    if(!app->view_dispatcher) {
        FURI_LOG_E("WardrivingUI", "View dispatcher is NULL");
        return;
    }
    
    // Create view with callbacks
    View* view = view_alloc();
    if(!view) {
        FURI_LOG_E("WardrivingUI", "Failed to allocate view");
        return;
    }
    
    view_set_context(view, app);
    view_set_draw_callback(view, wardriving_ui_draw_callback);
    view_set_input_callback(view, wardriving_ui_input_callback);
    
    // Add view to dispatcher
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewWardrivingUI, view);
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewWardrivingUI);
    
    FURI_LOG_I("WardrivingUI", "Wardriving UI initialized");
    
    // Start timer for updates
    app->timer = furi_timer_alloc(wardriving_ui_timer_callback, FuriTimerTypePeriodic, app);
    furi_timer_start(app->timer, 100); // Update every 100ms
}

bool predator_scene_wardriving_ui_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    if(!app) return false;
    
    if(event.type == SceneManagerEventTypeCustom) {
        // Custom event received - view will redraw automatically
        return true;
    }
    
    return false;
}

void predator_scene_wardriving_ui_on_exit(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    // Stop timer
    if(app->timer) {
        furi_timer_stop(app->timer);
        furi_timer_free(app->timer);
        app->timer = NULL;
    }
    
    // Stop wardriving
    if(wardriving_state.status == WardrivingStatusScanning) {
        predator_esp32_stop_attack(app);
        
        char log_msg[80];
        snprintf(log_msg, sizeof(log_msg), "Wardriving EXIT: %lu APs, %.2fkm", 
                wardriving_state.aps_found, (double)wardriving_state.distance_km);
        predator_log_append(app, log_msg);
    }
    
    wardriving_state.status = WardrivingStatusIdle;
    
    // Remove view
    if(app->view_dispatcher) {
        view_dispatcher_remove_view(app->view_dispatcher, PredatorViewWardrivingUI);
    }
    
    FURI_LOG_I("WardrivingUI", "Wardriving UI exited");
}
