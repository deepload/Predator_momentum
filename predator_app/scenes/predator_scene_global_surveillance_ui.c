#include "../predator_i.h"
#include "../helpers/predator_global_surveillance.h"
#include "../helpers/predator_logging.h"
#include "../helpers/predator_boards.h"
#include "../helpers/predator_ui_elements.h"
#include <gui/view.h>
#include <string.h>

// 🌍 GLOBAL SURVEILLANCE NETWORK UI
// Ultimate worldwide monitoring interface using ALL helpers

typedef enum {
    GlobalSurveillanceEventTimer = 0,
} GlobalSurveillanceEvent;

static GlobalSurveillanceState* surveillance_state = NULL;
static View* surveillance_view = NULL;

static void draw_surveillance_header(Canvas* canvas) {
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 2, 10, "🌍 GLOBAL SURVEILLANCE");
    canvas_draw_line(canvas, 0, 12, 128, 12);
}

static void draw_surveillance_status(Canvas* canvas, GlobalSurveillanceState* state) {
    canvas_set_font(canvas, FontSecondary);
    
    // Status line
    canvas_draw_str(canvas, 2, 22, "Status:");
    const char* status_text = "Unknown";
    switch(state->status) {
        case SurveillanceStatusIdle: status_text = "Idle"; break;
        case SurveillanceStatusInitializing: status_text = "Init"; break;
        case SurveillanceStatusActive: status_text = "ACTIVE"; break;
        case SurveillanceStatusAnalyzing: status_text = "Analyzing"; break;
        case SurveillanceStatusSyncing: status_text = "Syncing"; break;
    }
    canvas_draw_str(canvas, 45, 22, status_text);
    
    // Location
    char location_str[32];
    snprintf(location_str, sizeof(location_str), "GPS: %.2f,%.2f", 
            (double)state->current_latitude, (double)state->current_longitude);
    canvas_draw_str(canvas, 2, 32, location_str);
}

static void draw_surveillance_stats(Canvas* canvas, GlobalSurveillanceState* state) {
    canvas_set_font(canvas, FontSecondary);
    
    // WiFi networks mapped
    char wifi_str[32];
    snprintf(wifi_str, sizeof(wifi_str), "WiFi: %lu", state->wifi_networks_mapped);
    canvas_draw_str(canvas, 2, 42, wifi_str);
    
    // SubGHz signals analyzed
    char subghz_str[32];
    snprintf(subghz_str, sizeof(subghz_str), "SubGHz: %lu", state->subghz_signals_analyzed);
    canvas_draw_str(canvas, 70, 42, subghz_str);
    
    // Crypto protocols identified
    char crypto_str[32];
    snprintf(crypto_str, sizeof(crypto_str), "Crypto: %lu", state->crypto_protocols_identified);
    canvas_draw_str(canvas, 2, 52, crypto_str);
    
    // Global sync packets
    char sync_str[32];
    snprintf(sync_str, sizeof(sync_str), "Sync: %lu", state->global_sync_packets_sent);
    canvas_draw_str(canvas, 70, 52, sync_str);
    
    // Active systems indicators
    canvas_draw_str(canvas, 2, 62, "Active:");
    uint8_t x_pos = 40;
    if(state->gps_tracking_active) {
        canvas_draw_str(canvas, x_pos, 62, "G");
        x_pos += 10;
    }
    if(state->wifi_scanning_active) {
        canvas_draw_str(canvas, x_pos, 62, "W");
        x_pos += 10;
    }
    if(state->subghz_analysis_active) {
        canvas_draw_str(canvas, x_pos, 62, "S");
        x_pos += 10;
    }
    if(state->crypto_analysis_active) {
        canvas_draw_str(canvas, x_pos, 62, "C");
        x_pos += 10;
    }
    if(state->global_sync_active) {
        canvas_draw_str(canvas, x_pos, 62, "N");
    }
}

static void global_surveillance_ui_draw_callback(Canvas* canvas, void* context) {
    furi_assert(canvas);
    furi_assert(context);
    
    UNUSED(context);
    
    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);
    
    if(!surveillance_state) return;
    
    // Draw header
    draw_surveillance_header(canvas);
    
    // Draw status
    draw_surveillance_status(canvas, surveillance_state);
    
    // Draw statistics
    draw_surveillance_stats(canvas, surveillance_state);
    
    // Draw instructions
    canvas_set_font(canvas, FontSecondary);
    if(surveillance_state->status == SurveillanceStatusIdle) {
        canvas_draw_str(canvas, 25, 64, "OK=Start  Back=Exit");
    } else {
        canvas_draw_str(canvas, 25, 64, "OK=Stop   Back=Exit");
    }
}

static bool global_surveillance_ui_input_callback(InputEvent* event, void* context) {
    PredatorApp* app = context;
    if(!app) return false;
    
    if(event->type == InputTypeShort) {
        if(event->key == InputKeyBack) {
            // Stop surveillance if active
            if(surveillance_state && surveillance_state->status != SurveillanceStatusIdle) {
                predator_global_surveillance_stop(app);
                predator_log_append(app, "🌍 Global Surveillance STOPPED");
            }
            return false; // Let scene manager handle back
        } else if(event->key == InputKeyOk) {
            if(!surveillance_state) {
                surveillance_state = predator_global_surveillance_get_state(app);
            }
            
            if(surveillance_state->status == SurveillanceStatusIdle) {
                // Start global surveillance
                if(predator_global_surveillance_start(app)) {
                    predator_log_append(app, "🌍 Global Surveillance STARTED");
                    FURI_LOG_I("GlobalSurveillanceUI", "Global surveillance started");
                }
            } else {
                // Stop global surveillance
                if(predator_global_surveillance_stop(app)) {
                    predator_log_append(app, "🌍 Global Surveillance STOPPED");
                    FURI_LOG_I("GlobalSurveillanceUI", "Global surveillance stopped");
                }
            }
            return true;
        }
    }
    
    return true;
}

static void global_surveillance_ui_timer_callback(void* context) {
    PredatorApp* app = context;
    if(!app || !app->view_dispatcher) return;
    
    if(surveillance_state && surveillance_state->status == SurveillanceStatusActive) {
        // Process surveillance data
        predator_global_surveillance_process_data(app);
    }
    
    view_dispatcher_send_custom_event(app->view_dispatcher, GlobalSurveillanceEventTimer);
}

void predator_scene_global_surveillance_ui_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    // Get surveillance state
    surveillance_state = predator_global_surveillance_get_state(app);
    
    // Setup custom view
    if(!app->view_dispatcher) {
        FURI_LOG_E("GlobalSurveillanceUI", "View dispatcher is NULL");
        return;
    }
    
    // Create view with callbacks
    surveillance_view = view_alloc();
    if(!surveillance_view) {
        FURI_LOG_E("GlobalSurveillanceUI", "Failed to allocate view");
        return;
    }
    
    view_set_context(surveillance_view, app);
    view_set_draw_callback(surveillance_view, global_surveillance_ui_draw_callback);
    view_set_input_callback(surveillance_view, global_surveillance_ui_input_callback);
    
    // Add view to dispatcher
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewGlobalSurveillanceUI, surveillance_view);
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewGlobalSurveillanceUI);
    
    FURI_LOG_I("GlobalSurveillanceUI", "🌍 Global Surveillance UI initialized");
    
    // Start timer for updates
    app->timer = furi_timer_alloc(global_surveillance_ui_timer_callback, FuriTimerTypePeriodic, app);
    furi_timer_start(app->timer, 100); // Update every 100ms
}

bool predator_scene_global_surveillance_ui_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    if(!app) return false;
    
    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == GlobalSurveillanceEventTimer) {
            // Timer event - view will redraw automatically
            return true;
        }
    }
    
    return false;
}

void predator_scene_global_surveillance_ui_on_exit(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    // Stop timer
    if(app->timer) {
        furi_timer_stop(app->timer);
        furi_timer_free(app->timer);
        app->timer = NULL;
    }
    
    // Stop surveillance if active
    if(surveillance_state && surveillance_state->status != SurveillanceStatusIdle) {
        predator_global_surveillance_stop(app);
        predator_log_append(app, "🌍 Global Surveillance EXIT");
    }
    
    // Remove view
    if(app->view_dispatcher && surveillance_view) {
        view_dispatcher_remove_view(app->view_dispatcher, PredatorViewGlobalSurveillanceUI);
        view_free(surveillance_view);
        surveillance_view = NULL;
    }
    
    surveillance_state = NULL;
    
    FURI_LOG_I("GlobalSurveillanceUI", "🌍 Global Surveillance UI exited");
}
