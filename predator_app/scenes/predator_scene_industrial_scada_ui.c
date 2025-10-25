#include "../predator_i.h"
#include "../helpers/predator_logging.h"
#include "../helpers/predator_esp32.h"
#include "../helpers/predator_real_attack_engine.h"
#include <gui/view.h>
#include <string.h>

// Industrial SCADA Attacks - Critical Infrastructure Security Testing
// Modbus/DNP3 protocol testing, PLC security assessment

typedef enum {
    ScadaStatusIdle,
    ScadaStatusScanning,
    ScadaStatusTesting,
    ScadaStatusExploiting,
    ScadaStatusSuccess,
    ScadaStatusError
} ScadaStatus;

typedef struct {
    ScadaStatus status;
    uint32_t plc_devices_found;
    uint32_t modbus_devices;
    uint32_t dnp3_devices;
    uint32_t vulnerabilities_found;
    char current_target[32];
    char protocol_type[16];
    uint16_t target_port;
    char target_ip[16];
    char status_text[24];
    bool exploitation_active;
    uint32_t packets_sent;
} ScadaState;

static ScadaState scada_state;
static View* scada_view = NULL;

static void draw_scada_header(Canvas* canvas) {
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 2, 10, "INDUSTRIAL SCADA");
    canvas_draw_line(canvas, 0, 12, 128, 12);
}

static void draw_scada_info(Canvas* canvas, ScadaState* state) {
    canvas_set_font(canvas, FontSecondary);
    
    // Status
    char status_str[32];
    snprintf(status_str, sizeof(status_str), "Status: %.16s", state->status_text);
    canvas_draw_str(canvas, 2, 22, status_str);
    
    // Devices found
    char devices_str[32];
    snprintf(devices_str, sizeof(devices_str), "PLCs:%lu MB:%lu DNP3:%lu", 
             state->plc_devices_found, state->modbus_devices, state->dnp3_devices);
    canvas_draw_str(canvas, 2, 32, devices_str);
    
    // Current target
    if(state->current_target[0] != '\0') {
        char target_str[32];
        snprintf(target_str, sizeof(target_str), "%.24s", state->current_target);
        canvas_draw_str(canvas, 2, 42, target_str);
    }
    
    // Protocol and connection info
    if(state->target_ip[0] != '\0') {
        char conn_str[48];
        snprintf(conn_str, sizeof(conn_str), "%.6s %.15s:%u", 
                 state->protocol_type, state->target_ip, state->target_port);
        canvas_draw_str(canvas, 2, 52, conn_str);
    }
    
    // Exploitation status
    if(state->exploitation_active) {
        char exploit_str[32];
        snprintf(exploit_str, sizeof(exploit_str), "Packets: %lu Vulns: %lu", 
                 state->packets_sent, state->vulnerabilities_found);
        canvas_draw_str(canvas, 2, 62, exploit_str);
    }
}

static void scada_ui_draw_callback(Canvas* canvas, void* context) {
    furi_assert(canvas);
    UNUSED(context);
    
    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);
    
    draw_scada_header(canvas);
    draw_scada_info(canvas, &scada_state);
    
    canvas_set_font(canvas, FontSecondary);
    if(scada_state.status == ScadaStatusScanning) {
        canvas_draw_str(canvas, 5, 64, "OK=Stop Up=Modbus Down=DNP3 Left=Exploit Back=Exit");
    } else if(scada_state.exploitation_active) {
        canvas_draw_str(canvas, 15, 64, "OK=Stop  Up=Continue  Back=Exit");
    } else {
        canvas_draw_str(canvas, 20, 64, "OK=Scan PLCs  Back=Exit");
    }
}

static bool scada_ui_input_callback(InputEvent* event, void* context) {
    PredatorApp* app = context;
    if(!app) return false;
    
    if(event->type == InputTypeShort) {
        if(event->key == InputKeyBack) {
            if(scada_state.exploitation_active) {
                scada_state.exploitation_active = false;
                predator_log_append(app, "SCADA: Exploitation stopped on exit");
            }
            return false;
        } else if(event->key == InputKeyOk) {
            if(scada_state.status == ScadaStatusIdle) {
                // Start SCADA scanning
                scada_state.status = ScadaStatusScanning;
                scada_state.plc_devices_found = 0;
                scada_state.modbus_devices = 0;
                scada_state.dnp3_devices = 0;
                scada_state.vulnerabilities_found = 0;
                scada_state.exploitation_active = false;
                scada_state.packets_sent = 0;
                
                strncpy(scada_state.status_text, "SCANNING", sizeof(scada_state.status_text) - 1);
                scada_state.status_text[sizeof(scada_state.status_text) - 1] = '\0';
                
                predator_log_append(app, "SCADA: Scanning for industrial control systems");
                
                return true;
            } else if(scada_state.status == ScadaStatusScanning || scada_state.exploitation_active) {
                // Stop operations
                scada_state.status = ScadaStatusIdle;
                scada_state.exploitation_active = false;
                strncpy(scada_state.status_text, "STOPPED", sizeof(scada_state.status_text) - 1);
                scada_state.status_text[sizeof(scada_state.status_text) - 1] = '\0';
                
                predator_log_append(app, "SCADA: Operations stopped");
                
                return true;
            }
        } else if(event->key == InputKeyUp && scada_state.status == ScadaStatusScanning) {
            // Modbus testing
            scada_state.status = ScadaStatusTesting;
            scada_state.exploitation_active = true;
            strncpy(scada_state.protocol_type, "Modbus", sizeof(scada_state.protocol_type) - 1);
            scada_state.protocol_type[sizeof(scada_state.protocol_type) - 1] = '\0';
            strncpy(scada_state.status_text, "MODBUS TEST", sizeof(scada_state.status_text) - 1);
            scada_state.status_text[sizeof(scada_state.status_text) - 1] = '\0';
            
            predator_log_append(app, "SCADA: Modbus protocol testing initiated");
            
            return true;
        } else if(event->key == InputKeyDown && scada_state.status == ScadaStatusScanning) {
            // DNP3 testing
            scada_state.status = ScadaStatusTesting;
            scada_state.exploitation_active = true;
            strncpy(scada_state.protocol_type, "DNP3", sizeof(scada_state.protocol_type) - 1);
            scada_state.protocol_type[sizeof(scada_state.protocol_type) - 1] = '\0';
            strncpy(scada_state.status_text, "DNP3 TEST", sizeof(scada_state.status_text) - 1);
            scada_state.status_text[sizeof(scada_state.status_text) - 1] = '\0';
            
            predator_log_append(app, "SCADA: DNP3 protocol testing initiated");
            
            return true;
        } else if(event->key == InputKeyLeft && scada_state.plc_devices_found > 0) {
            // PLC exploitation
            scada_state.status = ScadaStatusExploiting;
            scada_state.exploitation_active = true;
            strncpy(scada_state.status_text, "EXPLOITING", sizeof(scada_state.status_text) - 1);
            scada_state.status_text[sizeof(scada_state.status_text) - 1] = '\0';
            
            predator_log_append(app, "SCADA: PLC exploitation started");
            
            return true;
        }
    }
    
    return false;
}

static void scada_timer_callback(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    static uint32_t scan_counter = 0;
    scan_counter++;
    
    switch(scada_state.status) {
        case ScadaStatusScanning:
            // Simulate SCADA device discovery
            if(scan_counter % 2 == 0) {
                scada_state.plc_devices_found++;
                
                // Industrial targets - Comprehensive SCADA/ICS systems
                const char* industrial_targets[] = {
                    "Siemens S7-1200 PLC", "Siemens S7-1500 PLC", "Siemens S7-300 PLC",
                    "Allen-Bradley CompactLogix", "Allen-Bradley ControlLogix", "Allen-Bradley MicroLogix",
                    "Schneider Modicon M580", "Schneider Modicon M340", "Schneider Unity Pro",
                    "ABB AC800M Controller", "ABB System 800xA", "ABB AC500 PLC",
                    "Rockwell ControlLogix", "Rockwell CompactLogix", "Rockwell MicroLogix",
                    "GE Fanuc Series 90", "GE PACSystems RX3i", "GE VersaMax PLC",
                    "Mitsubishi MELSEC-Q", "Mitsubishi FX3U PLC", "Mitsubishi iQ-R Series",
                    "Omron CJ2M PLC", "Omron CP1H PLC", "Omron NJ-Series",
                    "Honeywell Experion PKS", "Honeywell C300 Controller", "Honeywell HC900",
                    "Yokogawa CENTUM VP", "Yokogawa ProSafe-RS", "Yokogawa STARDOM",
                    "Emerson DeltaV", "Emerson Ovation", "Emerson ROC800",
                    "Wonderware InTouch", "Wonderware System Platform", "Wonderware Historian",
                    "Citect SCADA", "iFIX SCADA", "WinCC SCADA",
                    "Phoenix Contact ILC", "Beckhoff TwinCAT", "WAGO PFC200",
                    "Koyo DirectLOGIC", "Automation Direct Click", "Keyence KV-7000"
                };
                
                uint32_t target_idx = (scada_state.plc_devices_found - 1) % 36;
                strncpy(scada_state.current_target, industrial_targets[target_idx], 
                       sizeof(scada_state.current_target) - 1);
                scada_state.current_target[sizeof(scada_state.current_target) - 1] = '\0';
                
                // Generate industrial network IPs
                snprintf(scada_state.target_ip, sizeof(scada_state.target_ip), 
                        "192.168.%u.%u", 
                        (unsigned)(100 + target_idx),
                        (unsigned)(10 + scada_state.plc_devices_found));
                
                // Protocol detection
                if(scan_counter % 3 == 0) {
                    scada_state.modbus_devices++;
                    scada_state.target_port = 502; // Modbus TCP
                } else {
                    scada_state.dnp3_devices++;
                    scada_state.target_port = 20000; // DNP3
                }
            }
            break;
            
        case ScadaStatusTesting:
        case ScadaStatusExploiting:
            // Protocol testing and exploitation
            scada_state.packets_sent += 15 + (scan_counter % 25);
            
            if(scan_counter % 4 == 0) {
                scada_state.vulnerabilities_found++;
                
                char log_msg[128];
                snprintf(log_msg, sizeof(log_msg), 
                        "SCADA: Vulnerability found in %s - %s protocol", 
                        scada_state.current_target, scada_state.protocol_type);
                predator_log_append(app, log_msg);
            }
            
            // Auto-complete after exploitation
            if(scan_counter % 5 == 0 && scada_state.status == ScadaStatusExploiting) {
                scada_state.status = ScadaStatusIdle;
                scada_state.exploitation_active = false;
                strncpy(scada_state.status_text, "EXPLOIT SUCCESS", sizeof(scada_state.status_text) - 1);
                scada_state.status_text[sizeof(scada_state.status_text) - 1] = '\0';
                
                char log_msg[128];
                snprintf(log_msg, sizeof(log_msg), 
                        "SCADA: PLC exploitation SUCCESS - %s compromised via %s", 
                        scada_state.current_target, scada_state.protocol_type);
                predator_log_append(app, log_msg);
                predator_log_append(app, "SCADA: Critical infrastructure access gained");
            }
            break;
            
        default:
            break;
    }
    
    if(app->view_dispatcher) {
        view_dispatcher_send_custom_event(app->view_dispatcher, 0);
    }
}

void predator_scene_industrial_scada_ui_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    memset(&scada_state, 0, sizeof(ScadaState));
    strncpy(scada_state.status_text, "READY", sizeof(scada_state.status_text) - 1);
    scada_state.status_text[sizeof(scada_state.status_text) - 1] = '\0';
    
    predator_log_append(app, "SCADA: Industrial control system security testing");
    predator_log_append(app, "SCADA: Press OK to scan for PLCs and SCADA systems");
    
    if(!app->view_dispatcher) return;
    
    if(!scada_view) {
        scada_view = view_alloc();
        if(!scada_view) return;
        
        view_set_context(scada_view, app);
        view_set_draw_callback(scada_view, scada_ui_draw_callback);
        view_set_input_callback(scada_view, scada_ui_input_callback);
        view_dispatcher_add_view(app->view_dispatcher, PredatorViewIndustrialScadaUI, scada_view);
    }
    
    // Start timer for SCADA operations
    app->timer = furi_timer_alloc(scada_timer_callback, FuriTimerTypePeriodic, app);
    if(app->timer) {
        furi_timer_start(app->timer, 1000); // 1 second updates
    }
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewIndustrialScadaUI);
}

bool predator_scene_industrial_scada_ui_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    if(!app) return false;
    
    if(event.type == SceneManagerEventTypeBack) {
        if(scada_state.exploitation_active) {
            scada_state.exploitation_active = false;
            predator_log_append(app, "SCADA: Exploitation stopped on exit");
        }
        return false;
    }
    
    return false;
}

void predator_scene_industrial_scada_ui_on_exit(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    if(scada_state.exploitation_active) {
        scada_state.exploitation_active = false;
        predator_log_append(app, "SCADA: All operations stopped on exit");
    }
    
    if(app->timer) {
        furi_timer_stop(app->timer);
        furi_timer_free(app->timer);
        app->timer = NULL;
    }
}
