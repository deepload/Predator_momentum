#include "../predator_i.h"
#include "../helpers/predator_gps.h"
#include "predator_scene.h"

#define GPS_UPDATE_INTERVAL_MS 500
#define GPS_STATS_BUFFER_SIZE 128

typedef struct {
    uint32_t nmea_count;
    uint32_t gga_count;
    uint32_t rmc_count;
    uint32_t gsv_count;
    char last_nmea[128];
    bool last_fix_valid;
    FuriTimer* update_timer;
} GpsDebugState;

static GpsDebugState* gps_debug_state = NULL;

// Timer callback for updating GPS data
static void predator_gps_debug_update_callback(void* context) {
    PredatorApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, PredatorCustomEventGpsUpdate);
}

// Widget callback for back button
static void predator_scene_gps_debug_widget_callback(GuiButtonType result, InputType type, void* context) {
    PredatorApp* app = context;
    if(type == InputTypeShort) {
        view_dispatcher_send_custom_event(app->view_dispatcher, result);
    }
}

void predator_scene_gps_debug_on_enter(void* context) {
    PredatorApp* app = context;
    Widget* widget = app->widget;
    
    // Initialize debug state if needed
    if(!gps_debug_state) {
        gps_debug_state = malloc(sizeof(GpsDebugState));
        memset(gps_debug_state, 0, sizeof(GpsDebugState));
        gps_debug_state->update_timer = furi_timer_alloc(predator_gps_debug_update_callback, FuriTimerTypePeriodic, app);
    }
    
    // Clear widget and set up UI
    widget_reset(widget);
    widget_set_font(widget, FontPrimary);
    widget_add_text_scroll_element(widget, 0, 0, 128, 64, "Initializing GPS debug...");
    widget_set_context(widget, app);
    widget_set_button_callback(widget, predator_scene_gps_debug_widget_callback);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewWidget);
    
    // Start update timer
    furi_timer_start(gps_debug_state->update_timer, GPS_UPDATE_INTERVAL_MS);
    
    // Force immediate update
    predator_scene_gps_debug_update_callback(app);
}

bool predator_scene_gps_debug_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == PredatorCustomEventGpsUpdate) {
            // Update GPS debug information
            Widget* widget = app->widget;
            widget_reset(widget);
            
            char stats_buf[GPS_STATS_BUFFER_SIZE];
            
            // Generate GPS status text
            int written = snprintf(stats_buf, GPS_STATS_BUFFER_SIZE,
                "GPS Debug Info\n"
                "------------------\n"
                "Connected: %s\n"
                "Satellites: %lu\n"
                "Lat: %.6f\n"
                "Lon: %.6f\n"
                "NMEA msgs: %lu\n"
                "GGA msgs: %lu\n"
                "RMC msgs: %lu\n"
                "GSV msgs: %lu\n"
                "Fix valid: %s\n",
                app->gps_connected ? "YES" : "NO",
                app->satellites,
                (double)app->latitude,
                (double)app->longitude,
                gps_debug_state->nmea_count,
                gps_debug_state->gga_count,
                gps_debug_state->rmc_count,
                gps_debug_state->gsv_count,
                gps_debug_state->last_fix_valid ? "YES" : "NO");
            
            // Add last NMEA sentence if available
            if(gps_debug_state->last_nmea[0] != '\0' && written < GPS_STATS_BUFFER_SIZE - 20) {
                snprintf(stats_buf + written, GPS_STATS_BUFFER_SIZE - written, 
                    "\nLast NMEA:\n%.30s", gps_debug_state->last_nmea);
            }
            
            // Update widget
            widget_add_text_scroll_element(widget, 0, 0, 128, 52, stats_buf);
            widget_add_button_element(widget, GuiButtonTypeLeft, "Back", scene_manager_handle_back_event, app);
            
            // Increment NMEA counter if we received data
            if(app->gps_connected) {
                gps_debug_state->nmea_count++;
            }
            
            consumed = true;
        } else if(event.event == GuiButtonTypeLeft) {
            scene_manager_previous_scene(app->scene_manager);
            consumed = true;
        }
    }

    return consumed;
}

void predator_scene_gps_debug_on_exit(void* context) {
    PredatorApp* app = context;
    
    // Stop the update timer
    if(gps_debug_state && gps_debug_state->update_timer) {
        furi_timer_stop(gps_debug_state->update_timer);
    }
    
    // Reset widget
    widget_reset(app->widget);
}

// Helper function for other files to track NMEA sentences
void predator_gps_debug_track_nmea(const char* nmea) {
    if(!gps_debug_state || !nmea) return;
    
    // Save most recent NMEA sentence
    strncpy(gps_debug_state->last_nmea, nmea, sizeof(gps_debug_state->last_nmea) - 1);
    gps_debug_state->last_nmea[sizeof(gps_debug_state->last_nmea) - 1] = '\0';
    
    // Track message types
    if(strncmp(nmea, "$GPGGA", 6) == 0 || strncmp(nmea, "$GNGGA", 6) == 0) {
        gps_debug_state->gga_count++;
    } else if(strncmp(nmea, "$GPRMC", 6) == 0 || strncmp(nmea, "$GNRMC", 6) == 0) {
        gps_debug_state->rmc_count++;
        // Check if position fix is valid (field 2 contains 'A')
        char* token = strchr(nmea, ',');
        if(token) {
            token++; // Skip to field 1
            token = strchr(token, ',');
            if(token) {
                token++; // Skip to field 2
                gps_debug_state->last_fix_valid = (*token == 'A');
            }
        }
    } else if(strncmp(nmea, "$GPGSV", 6) == 0 || strncmp(nmea, "$GNGSV", 6) == 0) {
        gps_debug_state->gsv_count++;
    }
}
