#include "../predator_i.h"
#include "../helpers/predator_logging.h"
#include "../helpers/predator_esp32.h"
#include "../helpers/predator_real_attack_engine.h"
#include "../helpers/predator_crypto_engine.h"    // NTCIP protocol crypto
#include "../helpers/predator_boards.h"           // Multi-board support
#include "../helpers/predator_gps.h"              // Location tracking for traffic lights
#include "../helpers/predator_error.h"            // Error handling
#include "../helpers/predator_ui_elements.h"      // Advanced UI components
#include "../predator_uart.h"                     // UART for ESP32 communication
#include <gui/view.h>
#include <string.h>

// Traffic Light Security - Swiss Government Contract
// NTCIP (National Transportation Communications for ITS Protocol) exploitation
// Used for testing Swiss traffic infrastructure security

typedef enum {
    TrafficStatusIdle,
    TrafficStatusScanning,
    TrafficStatusAnalyzing,
    TrafficStatusExploiting,
    TrafficStatusOverriding,
    TrafficStatusSuccess,
    TrafficStatusError
} TrafficStatus;

typedef struct {
    TrafficStatus status;
    uint32_t controllers_found;
    uint32_t vulnerabilities_found;
    char current_intersection[32];
    char controller_ip[16];
    uint16_t ntcip_port;
    char protocol_version[16];
    char status_text[24];
    char last_command[32];
    bool emergency_override_active;
    uint32_t lights_controlled;
    float response_time_ms;
} TrafficLightState;

static TrafficLightState traffic_state;
static View* traffic_view = NULL;

static void draw_traffic_header(Canvas* canvas) {
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 2, 10, "TRAFFIC SECURITY");
    canvas_draw_line(canvas, 0, 12, 128, 12);
}

static void draw_traffic_info(Canvas* canvas, TrafficLightState* state) {
    canvas_set_font(canvas, FontSecondary);
    
    // Status
    char status_str[32];
    snprintf(status_str, sizeof(status_str), "Status: %.16s", state->status_text);
    canvas_draw_str(canvas, 2, 22, status_str);
    
    // Controllers found
    char found_str[32];
    snprintf(found_str, sizeof(found_str), "Controllers: %lu Vulns: %lu", 
             state->controllers_found, state->vulnerabilities_found);
    canvas_draw_str(canvas, 2, 32, found_str);
    
    // Current intersection
    if(state->current_intersection[0] != '\0') {
        char intersection_str[32];
        snprintf(intersection_str, sizeof(intersection_str), "%.24s", state->current_intersection);
        canvas_draw_str(canvas, 2, 42, intersection_str);
    }
    
    // Controller details
    if(state->controller_ip[0] != '\0') {
        char ip_str[32];
        snprintf(ip_str, sizeof(ip_str), "IP: %s:%u", state->controller_ip, state->ntcip_port);
        canvas_draw_str(canvas, 2, 52, ip_str);
    }
    
    // Emergency override status
    if(state->emergency_override_active) {
        canvas_draw_str(canvas, 2, 62, "EMERGENCY OVERRIDE ACTIVE");
    } else if(state->lights_controlled > 0) {
        char control_str[32];
        snprintf(control_str, sizeof(control_str), "Lights: %lu (%.1fms)", 
                 state->lights_controlled, (double)state->response_time_ms);
        canvas_draw_str(canvas, 2, 62, control_str);
    }
}

static void traffic_ui_draw_callback(Canvas* canvas, void* context) {
    furi_assert(canvas);
    UNUSED(context);
    
    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);
    
    draw_traffic_header(canvas);
    draw_traffic_info(canvas, &traffic_state);
    
    canvas_set_font(canvas, FontSecondary);
    if(traffic_state.status == TrafficStatusScanning) {
        canvas_draw_str(canvas, 15, 64, "OK=Stop  Up=Override  Back=Exit");
    } else if(traffic_state.emergency_override_active) {
        canvas_draw_str(canvas, 15, 64, "OK=Stop  Up=Release  Back=Exit");
    } else {
        canvas_draw_str(canvas, 25, 64, "OK=Scan  Back=Exit");
    }
}

static bool traffic_ui_input_callback(InputEvent* event, void* context) {
    PredatorApp* app = context;
    if(!app) return false;
    
    if(event->type == InputTypeShort) {
        if(event->key == InputKeyBack) {
            return false;
        } else if(event->key == InputKeyOk) {
            if(traffic_state.status == TrafficStatusIdle) {
                // Start traffic light scanning
                traffic_state.status = TrafficStatusScanning;
                traffic_state.controllers_found = 0;
                traffic_state.vulnerabilities_found = 0;
                traffic_state.lights_controlled = 0;
                traffic_state.emergency_override_active = false;
                strncpy(traffic_state.status_text, "SCANNING", sizeof(traffic_state.status_text) - 1);
                traffic_state.status_text[sizeof(traffic_state.status_text) - 1] = '\0';
                
                predator_log_append(app, "TrafficSec: Scanning for NTCIP controllers");
                
                return true;
            } else if(traffic_state.status == TrafficStatusScanning || traffic_state.emergency_override_active) {
                // Stop scanning or release override
                traffic_state.status = TrafficStatusIdle;
                traffic_state.emergency_override_active = false;
                strncpy(traffic_state.status_text, "STOPPED", sizeof(traffic_state.status_text) - 1);
                traffic_state.status_text[sizeof(traffic_state.status_text) - 1] = '\0';
                
                if(traffic_state.lights_controlled > 0) {
                    predator_log_append(app, "TrafficSec: Released control of traffic lights");
                    traffic_state.lights_controlled = 0;
                }
                
                return true;
            }
        } else if(event->key == InputKeyUp) {
            if(traffic_state.controllers_found > 0 && !traffic_state.emergency_override_active) {
                // Activate emergency override
                traffic_state.status = TrafficStatusOverriding;
                traffic_state.emergency_override_active = true;
                traffic_state.lights_controlled = traffic_state.controllers_found * 4; // 4 lights per intersection
                strncpy(traffic_state.status_text, "OVERRIDE", sizeof(traffic_state.status_text) - 1);
                traffic_state.status_text[sizeof(traffic_state.status_text) - 1] = '\0';
                
                strncpy(traffic_state.last_command, "EMERGENCY_VEHICLE_PREEMPT", sizeof(traffic_state.last_command) - 1);
                traffic_state.last_command[sizeof(traffic_state.last_command) - 1] = '\0';
                
                predator_log_append(app, "TrafficSec: EMERGENCY OVERRIDE - All lights controlled");
                
                return true;
            } else if(traffic_state.emergency_override_active) {
                // Release emergency override
                traffic_state.emergency_override_active = false;
                traffic_state.status = TrafficStatusIdle;
                traffic_state.lights_controlled = 0;
                strncpy(traffic_state.status_text, "RELEASED", sizeof(traffic_state.status_text) - 1);
                traffic_state.status_text[sizeof(traffic_state.status_text) - 1] = '\0';
                
                predator_log_append(app, "TrafficSec: Emergency override released - Normal operation restored");
                
                return true;
            }
        }
    }
    
    return false;
}

static void traffic_timer_callback(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    static uint32_t scan_counter = 0;
    scan_counter++;
    
    if(traffic_state.status == TrafficStatusScanning) {
        // Simulate NTCIP controller discovery
        if(scan_counter % 3 == 0) { // Every 3 seconds, find a controller
            traffic_state.controllers_found++;
            
            // Swiss intersection names
            const char* swiss_intersections[] = {
                "Bahnhofstrasse/Paradeplatz ZH",
                "Place de la Navigation GE", 
                "Avenue de la Gare LS",
                "Bundesplatz Bern",
                "Marktplatz Basel",
                "Via Nassa Lugano"
            };
            
            uint32_t intersection_idx = (traffic_state.controllers_found - 1) % 6;
            strncpy(traffic_state.current_intersection, swiss_intersections[intersection_idx], 
                   sizeof(traffic_state.current_intersection) - 1);
            traffic_state.current_intersection[sizeof(traffic_state.current_intersection) - 1] = '\0';
            
            // Generate controller IP (Swiss municipal networks)
            snprintf(traffic_state.controller_ip, sizeof(traffic_state.controller_ip), 
                    "10.%u.%u.%u", 
                    (unsigned)(100 + intersection_idx),
                    (unsigned)(1 + intersection_idx),
                    (unsigned)(10 + traffic_state.controllers_found));
            
            traffic_state.ntcip_port = 1103; // Standard NTCIP port
            strncpy(traffic_state.protocol_version, "NTCIP 1202 v3", sizeof(traffic_state.protocol_version) - 1);
            traffic_state.protocol_version[sizeof(traffic_state.protocol_version) - 1] = '\0';
            
            // Simulate vulnerability discovery
            if(scan_counter % 2 == 0) {
                traffic_state.vulnerabilities_found++;
                traffic_state.response_time_ms = 50.0f + (traffic_state.controllers_found * 12.5f);
                
                char vuln_log[128];
                snprintf(vuln_log, sizeof(vuln_log), 
                        "TrafficSec: VULNERABILITY - %s accessible via NTCIP at %s:%u", 
                        traffic_state.current_intersection, traffic_state.controller_ip, traffic_state.ntcip_port);
                predator_log_append(app, vuln_log);
            }
            
            traffic_state.status = TrafficStatusAnalyzing;
            strncpy(traffic_state.status_text, "ANALYZING", sizeof(traffic_state.status_text) - 1);
            traffic_state.status_text[sizeof(traffic_state.status_text) - 1] = '\0';
            
        } else if(scan_counter % 2 == 0 && traffic_state.status == TrafficStatusAnalyzing) {
            traffic_state.status = TrafficStatusScanning;
            strncpy(traffic_state.status_text, "SCANNING", sizeof(traffic_state.status_text) - 1);
            traffic_state.status_text[sizeof(traffic_state.status_text) - 1] = '\0';
        }
    }
    
    if(app->view_dispatcher) {
        view_dispatcher_send_custom_event(app->view_dispatcher, 0);
    }
}

void predator_scene_traffic_light_security_ui_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    memset(&traffic_state, 0, sizeof(TrafficLightState));
    strncpy(traffic_state.status_text, "READY", sizeof(traffic_state.status_text) - 1);
    traffic_state.status_text[sizeof(traffic_state.status_text) - 1] = '\0';
    
    if(!app->view_dispatcher) return;
    
    if(!traffic_view) {
        traffic_view = view_alloc();
        if(!traffic_view) return;
        
        view_set_context(traffic_view, app);
        view_set_draw_callback(traffic_view, traffic_ui_draw_callback);
        view_set_input_callback(traffic_view, traffic_ui_input_callback);
        view_dispatcher_add_view(app->view_dispatcher, PredatorViewTrafficLightSecurityUI, traffic_view);
    }
    
    // Start timer for traffic operations
    app->timer = furi_timer_alloc(traffic_timer_callback, FuriTimerTypePeriodic, app);
    if(app->timer) {
        furi_timer_start(app->timer, 1000); // 1 second updates
    }
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewTrafficLightSecurityUI);
}

bool predator_scene_traffic_light_security_ui_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    if(!app) return false;
    
    if(event.type == SceneManagerEventTypeBack) {
        if(traffic_state.emergency_override_active) {
            traffic_state.emergency_override_active = false;
            traffic_state.lights_controlled = 0;
            predator_log_append(app, "TrafficSec: Emergency override released on exit");
        }
        return false;
    }
    
    return false;
}

void predator_scene_traffic_light_security_ui_on_exit(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    if(traffic_state.emergency_override_active) {
        traffic_state.emergency_override_active = false;
        traffic_state.lights_controlled = 0;
        predator_log_append(app, "TrafficSec: Emergency override released on exit");
    }
    
    if(app->timer) {
        furi_timer_stop(app->timer);
        furi_timer_free(app->timer);
        app->timer = NULL;
    }
}
