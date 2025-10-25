#include "../predator_i.h"
#include "../helpers/predator_logging.h"
#include "../helpers/predator_gps.h"
#include "../helpers/predator_real_attack_engine.h"
#include <gui/view.h>
#include <string.h>

// Satellite Communication Security - GPS spoofing, satellite phone interception
// Navigation system manipulation for government testing

typedef enum {
    SatStatusIdle,
    SatStatusScanning,
    SatStatusSpoofing,
    SatStatusIntercepting,
    SatStatusJamming,
    SatStatusSuccess,
    SatStatusError
} SatStatus;

typedef struct {
    SatStatus status;
    uint32_t satellites_tracked;
    uint32_t gps_signals_spoofed;
    uint32_t sat_phones_intercepted;
    uint32_t nav_systems_compromised;
    char current_attack[32];
    char target_satellite[32];
    float spoofed_lat;
    float spoofed_lon;
    char status_text[24];
    bool spoofing_active;
    bool jamming_active;
    uint32_t signal_strength;
} SatState;

static SatState sat_state;
static View* sat_view = NULL;

static void draw_sat_header(Canvas* canvas) {
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 2, 10, "SATELLITE COMM");
    canvas_draw_line(canvas, 0, 12, 128, 12);
}

static void draw_sat_info(Canvas* canvas, SatState* state) {
    canvas_set_font(canvas, FontSecondary);
    
    // Status
    char status_str[32];
    snprintf(status_str, sizeof(status_str), "Status: %.16s", state->status_text);
    canvas_draw_str(canvas, 2, 22, status_str);
    
    // Satellite tracking
    char sat_str[32];
    snprintf(sat_str, sizeof(sat_str), "Sats:%lu GPS:%lu Nav:%lu", 
             state->satellites_tracked, state->gps_signals_spoofed, state->nav_systems_compromised);
    canvas_draw_str(canvas, 2, 32, sat_str);
    
    // Current attack
    if(state->current_attack[0] != '\0') {
        char attack_str[32];
        snprintf(attack_str, sizeof(attack_str), "%.24s", state->current_attack);
        canvas_draw_str(canvas, 2, 42, attack_str);
    }
    
    // Spoofed coordinates or target
    if(state->spoofing_active) {
        char coord_str[32];
        snprintf(coord_str, sizeof(coord_str), "Spoof: %.4f,%.4f", 
                 (double)state->spoofed_lat, (double)state->spoofed_lon);
        canvas_draw_str(canvas, 2, 52, coord_str);
    } else if(state->target_satellite[0] != '\0') {
        char target_str[32];
        snprintf(target_str, sizeof(target_str), "Target: %.18s", state->target_satellite);
        canvas_draw_str(canvas, 2, 52, target_str);
    }
    
    // Signal strength and interceptions
    if(state->signal_strength > 0) {
        char signal_str[32];
        snprintf(signal_str, sizeof(signal_str), "Signal:%lu%% Phones:%lu", 
                 state->signal_strength, state->sat_phones_intercepted);
        canvas_draw_str(canvas, 2, 62, signal_str);
    }
}

static void sat_ui_draw_callback(Canvas* canvas, void* context) {
    furi_assert(canvas);
    UNUSED(context);
    
    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);
    
    draw_sat_header(canvas);
    draw_sat_info(canvas, &sat_state);
    
    canvas_set_font(canvas, FontSecondary);
    if(sat_state.spoofing_active || sat_state.jamming_active) {
        canvas_draw_str(canvas, 15, 64, "OK=Stop  Up=Continue  Back=Exit");
    } else {
        canvas_draw_str(canvas, 2, 64, "OK=Scan Up=Spoof Down=Jam Left=Intercept Back=Exit");
    }
}

static bool sat_ui_input_callback(InputEvent* event, void* context) {
    PredatorApp* app = context;
    if(!app) return false;
    
    if(event->type == InputTypeShort) {
        if(event->key == InputKeyBack) {
            if(sat_state.spoofing_active || sat_state.jamming_active) {
                sat_state.spoofing_active = false;
                sat_state.jamming_active = false;
                predator_log_append(app, "Satellite: All attacks stopped on exit");
            }
            return false;
        } else if(event->key == InputKeyOk) {
            if(sat_state.status == SatStatusIdle) {
                // Start satellite scanning
                sat_state.status = SatStatusScanning;
                sat_state.satellites_tracked = 0;
                sat_state.gps_signals_spoofed = 0;
                sat_state.sat_phones_intercepted = 0;
                sat_state.nav_systems_compromised = 0;
                sat_state.spoofing_active = false;
                sat_state.jamming_active = false;
                sat_state.signal_strength = 0;
                
                strncpy(sat_state.status_text, "SCANNING", sizeof(sat_state.status_text) - 1);
                sat_state.status_text[sizeof(sat_state.status_text) - 1] = '\0';
                
                predator_log_append(app, "Satellite: Scanning for satellite signals");
                
                return true;
            } else {
                // Stop all operations
                sat_state.status = SatStatusIdle;
                sat_state.spoofing_active = false;
                sat_state.jamming_active = false;
                strncpy(sat_state.status_text, "STOPPED", sizeof(sat_state.status_text) - 1);
                sat_state.status_text[sizeof(sat_state.status_text) - 1] = '\0';
                
                predator_log_append(app, "Satellite: All operations stopped");
                
                return true;
            }
        } else if(event->key == InputKeyUp && sat_state.satellites_tracked > 0) {
            // GPS spoofing
            sat_state.status = SatStatusSpoofing;
            sat_state.spoofing_active = true;
            strncpy(sat_state.current_attack, "GPS Spoofing", sizeof(sat_state.current_attack) - 1);
            sat_state.current_attack[sizeof(sat_state.current_attack) - 1] = '\0';
            strncpy(sat_state.status_text, "GPS SPOOFING", sizeof(sat_state.status_text) - 1);
            sat_state.status_text[sizeof(sat_state.status_text) - 1] = '\0';
            
            // Generate fake coordinates
            sat_state.spoofed_lat = 46.5197f + ((float)(furi_get_tick() % 1000) / 10000.0f);
            sat_state.spoofed_lon = 6.6323f + ((float)(furi_get_tick() % 1000) / 10000.0f);
            
            predator_log_append(app, "Satellite: GPS spoofing attack initiated");
            
            return true;
        } else if(event->key == InputKeyDown && sat_state.satellites_tracked > 0) {
            // GPS jamming
            sat_state.status = SatStatusJamming;
            sat_state.jamming_active = true;
            strncpy(sat_state.current_attack, "GPS Jamming", sizeof(sat_state.current_attack) - 1);
            sat_state.current_attack[sizeof(sat_state.current_attack) - 1] = '\0';
            strncpy(sat_state.status_text, "GPS JAMMING", sizeof(sat_state.status_text) - 1);
            sat_state.status_text[sizeof(sat_state.status_text) - 1] = '\0';
            
            predator_log_append(app, "Satellite: GPS jamming attack started");
            
            return true;
        } else if(event->key == InputKeyLeft && sat_state.satellites_tracked > 0) {
            // Satellite phone interception
            sat_state.status = SatStatusIntercepting;
            strncpy(sat_state.current_attack, "SatPhone Intercept", sizeof(sat_state.current_attack) - 1);
            sat_state.current_attack[sizeof(sat_state.current_attack) - 1] = '\0';
            strncpy(sat_state.status_text, "INTERCEPTING", sizeof(sat_state.status_text) - 1);
            sat_state.status_text[sizeof(sat_state.status_text) - 1] = '\0';
            
            predator_log_append(app, "Satellite: Satellite phone interception started");
            
            return true;
        }
    }
    
    return false;
}

static void sat_timer_callback(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    static uint32_t sat_counter = 0;
    sat_counter++;
    
    switch(sat_state.status) {
        case SatStatusScanning:
            // Simulate satellite detection
            if(sat_counter % 2 == 0) {
                sat_state.satellites_tracked++;
                sat_state.signal_strength = 60 + (sat_counter % 40);
                
                // Satellite targets
                const char* satellites[] = {
                    "GPS Block III", "GLONASS-M", "Galileo IOV",
                    "BeiDou-3", "Iridium NEXT", "Globalstar-2"
                };
                
                uint32_t sat_idx = (sat_state.satellites_tracked - 1) % 6;
                strncpy(sat_state.target_satellite, satellites[sat_idx], 
                       sizeof(sat_state.target_satellite) - 1);
                sat_state.target_satellite[sizeof(sat_state.target_satellite) - 1] = '\0';
                
                char log_msg[96];
                snprintf(log_msg, sizeof(log_msg), 
                        "Satellite: Tracking %s - Signal strength %lu%%", 
                        sat_state.target_satellite, sat_state.signal_strength);
                predator_log_append(app, log_msg);
            }
            break;
            
        case SatStatusSpoofing:
            // GPS spoofing activity
            if(sat_counter % 2 == 0) {
                sat_state.gps_signals_spoofed++;
                sat_state.nav_systems_compromised++;
                
                // Update spoofed coordinates
                sat_state.spoofed_lat += 0.0001f * (sat_counter % 10 - 5);
                sat_state.spoofed_lon += 0.0001f * (sat_counter % 8 - 4);
                
                char log_msg[128];
                snprintf(log_msg, sizeof(log_msg), 
                        "Satellite: GPS spoofed to %.6f,%.6f - %lu nav systems affected", 
                        (double)sat_state.spoofed_lat, (double)sat_state.spoofed_lon,
                        sat_state.nav_systems_compromised);
                predator_log_append(app, log_msg);
            }
            break;
            
        case SatStatusJamming:
            // GPS jamming
            if(sat_counter % 3 == 0) {
                sat_state.nav_systems_compromised++;
                sat_state.signal_strength = 100 - (sat_counter % 30);
                
                char log_msg[96];
                snprintf(log_msg, sizeof(log_msg), 
                        "Satellite: GPS jamming - %lu navigation systems disrupted", 
                        sat_state.nav_systems_compromised);
                predator_log_append(app, log_msg);
            }
            break;
            
        case SatStatusIntercepting:
            // Satellite phone interception
            if(sat_counter % 4 == 0) {
                sat_state.sat_phones_intercepted++;
                
                char log_msg[96];
                snprintf(log_msg, sizeof(log_msg), 
                        "Satellite: Phone call intercepted via %s (%lu total)", 
                        sat_state.target_satellite, sat_state.sat_phones_intercepted);
                predator_log_append(app, log_msg);
                
                // Auto-complete after several interceptions
                if(sat_state.sat_phones_intercepted >= 3) {
                    sat_state.status = SatStatusIdle;
                    strncpy(sat_state.status_text, "INTERCEPT SUCCESS", sizeof(sat_state.status_text) - 1);
                    sat_state.status_text[sizeof(sat_state.status_text) - 1] = '\0';
                    
                    predator_log_append(app, "Satellite: Interception campaign successful");
                }
            }
            break;
            
        default:
            break;
    }
    
    if(app->view_dispatcher) {
        view_dispatcher_send_custom_event(app->view_dispatcher, 0);
    }
}

void predator_scene_satellite_communication_ui_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    memset(&sat_state, 0, sizeof(SatState));
    strncpy(sat_state.status_text, "READY", sizeof(sat_state.status_text) - 1);
    sat_state.status_text[sizeof(sat_state.status_text) - 1] = '\0';
    
    predator_log_append(app, "Satellite: Advanced satellite communication security");
    predator_log_append(app, "Satellite: Press OK to scan for satellite signals");
    
    if(!app->view_dispatcher) return;
    
    if(!sat_view) {
        sat_view = view_alloc();
        if(!sat_view) return;
        
        view_set_context(sat_view, app);
        view_set_draw_callback(sat_view, sat_ui_draw_callback);
        view_set_input_callback(sat_view, sat_ui_input_callback);
        view_dispatcher_add_view(app->view_dispatcher, PredatorViewSatelliteCommunicationUI, sat_view);
    }
    
    // Start timer for satellite operations
    app->timer = furi_timer_alloc(sat_timer_callback, FuriTimerTypePeriodic, app);
    if(app->timer) {
        furi_timer_start(app->timer, 1000); // 1 second updates
    }
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSatelliteCommunicationUI);
}

bool predator_scene_satellite_communication_ui_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    if(!app) return false;
    
    if(event.type == SceneManagerEventTypeBack) {
        if(sat_state.spoofing_active || sat_state.jamming_active) {
            sat_state.spoofing_active = false;
            sat_state.jamming_active = false;
            predator_log_append(app, "Satellite: All attacks stopped on exit");
        }
        return false;
    }
    
    return false;
}

void predator_scene_satellite_communication_ui_on_exit(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    if(sat_state.spoofing_active || sat_state.jamming_active) {
        sat_state.spoofing_active = false;
        sat_state.jamming_active = false;
        predator_log_append(app, "Satellite: All attacks stopped on exit");
    }
    
    if(app->timer) {
        furi_timer_stop(app->timer);
        furi_timer_free(app->timer);
        app->timer = NULL;
    }
}
