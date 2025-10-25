#include "../predator_i.h"
#include "../helpers/predator_gps.h"
#include "../helpers/predator_logging.h"
#include <gui/view.h>
#include <string.h>

// GPS Tracker - Real-time location tracking for government operations
// Tracks coordinates, speed, altitude with logging capabilities

typedef struct {
    bool tracking_active;
    float latitude;
    float longitude;
    float altitude;
    float speed_kmh;
    uint32_t satellites;
    uint32_t tracking_time_ms;
    uint32_t points_logged;
    char status_text[32];
} GPSTrackerState;

static GPSTrackerState gps_state;
static View* gps_tracker_view = NULL;
static uint32_t tracking_start_tick = 0;

static void draw_gps_header(Canvas* canvas) {
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 2, 10, "GPS TRACKER");
    canvas_draw_line(canvas, 0, 12, 128, 12);
}

static void draw_gps_coordinates(Canvas* canvas, GPSTrackerState* state) {
    canvas_set_font(canvas, FontSecondary);
    
    // Latitude
    char lat_str[32];
    snprintf(lat_str, sizeof(lat_str), "Lat: %.6f", (double)state->latitude);
    canvas_draw_str(canvas, 2, 22, lat_str);
    
    // Longitude  
    char lon_str[32];
    snprintf(lon_str, sizeof(lon_str), "Lon: %.6f", (double)state->longitude);
    canvas_draw_str(canvas, 2, 32, lon_str);
    
    // Altitude and Speed
    char alt_spd_str[32];
    snprintf(alt_spd_str, sizeof(alt_spd_str), "Alt: %.1fm Spd: %.1fkm/h", 
             (double)state->altitude, (double)state->speed_kmh);
    canvas_draw_str(canvas, 2, 42, alt_spd_str);
}

static void draw_gps_status(Canvas* canvas, GPSTrackerState* state) {
    canvas_set_font(canvas, FontSecondary);
    
    // Satellites and status
    char sat_str[48];
    snprintf(sat_str, sizeof(sat_str), "Sats: %lu %.8s", 
             state->satellites, state->status_text);
    canvas_draw_str(canvas, 2, 52, sat_str);
    
    // Tracking info
    if(state->tracking_active) {
        char track_str[32];
        uint32_t seconds = state->tracking_time_ms / 1000;
        snprintf(track_str, sizeof(track_str), "Track: %lus Points: %lu", 
                 seconds, state->points_logged);
        canvas_draw_str(canvas, 2, 62, track_str);
    }
}

static void gps_tracker_ui_draw_callback(Canvas* canvas, void* context) {
    furi_assert(canvas);
    UNUSED(context);
    
    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);
    
    draw_gps_header(canvas);
    draw_gps_coordinates(canvas, &gps_state);
    draw_gps_status(canvas, &gps_state);
    
    canvas_set_font(canvas, FontSecondary);
    if(gps_state.tracking_active) {
        canvas_draw_str(canvas, 25, 64, "OK=Stop  Back=Exit");
    } else {
        canvas_draw_str(canvas, 25, 64, "OK=Start  Back=Exit");
    }
}

static bool gps_tracker_ui_input_callback(InputEvent* event, void* context) {
    PredatorApp* app = context;
    if(!app) return false;
    
    if(event->type == InputTypeShort) {
        if(event->key == InputKeyBack) {
            return false;
        } else if(event->key == InputKeyOk) {
            if(!gps_state.tracking_active) {
                // Start tracking
                gps_state.tracking_active = true;
                gps_state.tracking_time_ms = 0;
                gps_state.points_logged = 0;
                tracking_start_tick = furi_get_tick();
                
                predator_gps_init(app);
                predator_log_append(app, "GPS Tracker: Started");
                
                return true;
            } else {
                // Stop tracking
                gps_state.tracking_active = false;
                predator_gps_deinit(app);
                
                char log_msg[64];
                snprintf(log_msg, sizeof(log_msg), "GPS Tracker: Stopped (%lu points)", 
                        gps_state.points_logged);
                predator_log_append(app, log_msg);
                
                return true;
            }
        }
    }
    
    return false;
}

static void gps_tracker_timer_callback(void* context) {
    PredatorApp* app = context;
    if(!app || !gps_state.tracking_active) return;
    
    // Update tracking time
    gps_state.tracking_time_ms = furi_get_tick() - tracking_start_tick;
    
    // Get GPS data
    if(predator_gps_get_coordinates(app, &gps_state.latitude, &gps_state.longitude)) {
        gps_state.altitude = predator_gps_get_altitude(app);
        gps_state.speed_kmh = predator_gps_get_speed_kmh(app);
        gps_state.satellites = predator_gps_get_satellites(app);
        
        if(gps_state.satellites >= 4) {
            strncpy(gps_state.status_text, "LOCKED", sizeof(gps_state.status_text));
            gps_state.points_logged++;
            
            // Log coordinates every 10 points
            if(gps_state.points_logged % 10 == 0) {
                char log_msg[96];
                snprintf(log_msg, sizeof(log_msg), 
                        "GPS: %.6f,%.6f Alt:%.1f Spd:%.1f", 
                        (double)gps_state.latitude, (double)gps_state.longitude,
                        (double)gps_state.altitude, (double)gps_state.speed_kmh);
                predator_log_append(app, log_msg);
            }
        } else {
            strncpy(gps_state.status_text, "SEARCHING", sizeof(gps_state.status_text));
        }
    } else {
        strncpy(gps_state.status_text, "NO FIX", sizeof(gps_state.status_text));
        gps_state.satellites = 0;
    }
    
    if(app->view_dispatcher) {
        view_dispatcher_send_custom_event(app->view_dispatcher, 0);
    }
}

void predator_scene_gps_tracker_ui_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    memset(&gps_state, 0, sizeof(GPSTrackerState));
    strncpy(gps_state.status_text, "INIT", sizeof(gps_state.status_text));
    
    if(!app->view_dispatcher) return;
    
    if(!gps_tracker_view) {
        gps_tracker_view = view_alloc();
        if(!gps_tracker_view) return;
        
        view_set_context(gps_tracker_view, app);
        view_set_draw_callback(gps_tracker_view, gps_tracker_ui_draw_callback);
        view_set_input_callback(gps_tracker_view, gps_tracker_ui_input_callback);
        view_dispatcher_add_view(app->view_dispatcher, PredatorViewGpsTrackerUI, gps_tracker_view);
    }
    
    // Start timer for GPS updates
    app->timer = furi_timer_alloc(gps_tracker_timer_callback, FuriTimerTypePeriodic, app);
    if(app->timer) {
        furi_timer_start(app->timer, 1000); // 1 second updates
    }
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewGpsTrackerUI);
}

bool predator_scene_gps_tracker_ui_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    if(!app) return false;
    
    if(event.type == SceneManagerEventTypeBack) {
        if(gps_state.tracking_active) {
            gps_state.tracking_active = false;
            predator_gps_deinit(app);
        }
        return false;
    }
    
    return false;
}

void predator_scene_gps_tracker_ui_on_exit(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    if(gps_state.tracking_active) {
        gps_state.tracking_active = false;
        predator_gps_deinit(app);
    }
    
    if(app->timer) {
        furi_timer_stop(app->timer);
        furi_timer_free(app->timer);
        app->timer = NULL;
    }
}
