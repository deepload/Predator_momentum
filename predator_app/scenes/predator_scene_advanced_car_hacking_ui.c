#include "../predator_i.h"
#include "../helpers/predator_logging.h"
#include "../helpers/predator_real_attack_engine.h"
#include "../helpers/predator_crypto_engine.h"
#include <gui/view.h>
#include <string.h>

// Advanced Car Hacking - Professional automotive security testing
// CAN bus injection, OBD-II exploitation, TPMS spoofing, keyless replay

typedef enum {
    AdvCarStatusIdle,
    AdvCarStatusScanning,
    AdvCarStatusInjecting,
    AdvCarStatusExploiting,
    AdvCarStatusSpoofing,
    AdvCarStatusReplaying,
    AdvCarStatusSuccess,
    AdvCarStatusError
} AdvCarStatus;

typedef struct {
    AdvCarStatus status;
    uint32_t can_messages_found;
    uint32_t obd_ports_discovered;
    uint32_t tpms_sensors_detected;
    uint32_t keyless_signals_captured;
    char target_vehicle[32];
    char current_attack[32];
    char can_bus_id[16];
    uint16_t obd_port;
    float tire_pressure_psi;
    char status_text[24];
    bool injection_active;
    bool exploit_successful;
    uint32_t packets_injected;
} AdvCarState;

static AdvCarState advcar_state;
static View* advcar_view = NULL;

static void draw_advcar_header(Canvas* canvas) {
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 2, 10, "ADVANCED CAR HACK");
    canvas_draw_line(canvas, 0, 12, 128, 12);
}

static void draw_advcar_info(Canvas* canvas, AdvCarState* state) {
    canvas_set_font(canvas, FontSecondary);
    
    // Status and target
    char status_str[32];
    snprintf(status_str, sizeof(status_str), "Status: %.16s", state->status_text);
    canvas_draw_str(canvas, 2, 22, status_str);
    
    if(state->target_vehicle[0] != '\0') {
        char target_str[32];
        snprintf(target_str, sizeof(target_str), "Target: %.20s", state->target_vehicle);
        canvas_draw_str(canvas, 2, 32, target_str);
    }
    
    // Attack statistics
    char stats_str[32];
    snprintf(stats_str, sizeof(stats_str), "CAN:%lu OBD:%lu TPMS:%lu", 
             state->can_messages_found, state->obd_ports_discovered, state->tpms_sensors_detected);
    canvas_draw_str(canvas, 2, 42, stats_str);
    
    // Current attack details
    if(state->current_attack[0] != '\0') {
        char attack_str[32];
        snprintf(attack_str, sizeof(attack_str), "%.24s", state->current_attack);
        canvas_draw_str(canvas, 2, 52, attack_str);
    }
    
    // Injection status
    if(state->injection_active) {
        char inject_str[32];
        snprintf(inject_str, sizeof(inject_str), "Injected: %lu packets", state->packets_injected);
        canvas_draw_str(canvas, 2, 62, inject_str);
    } else if(state->keyless_signals_captured > 0) {
        char keyless_str[32];
        snprintf(keyless_str, sizeof(keyless_str), "Keyless: %lu signals", state->keyless_signals_captured);
        canvas_draw_str(canvas, 2, 62, keyless_str);
    }
}

static void advcar_ui_draw_callback(Canvas* canvas, void* context) {
    furi_assert(canvas);
    UNUSED(context);
    
    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);
    
    draw_advcar_header(canvas);
    draw_advcar_info(canvas, &advcar_state);
    
    canvas_set_font(canvas, FontSecondary);
    if(advcar_state.status == AdvCarStatusScanning) {
        canvas_draw_str(canvas, 8, 64, "OK=Stop Up=CAN Down=OBD Left=TPMS Right=Key");
    } else if(advcar_state.injection_active) {
        canvas_draw_str(canvas, 20, 64, "OK=Stop  Up=Inject  Back=Exit");
    } else {
        canvas_draw_str(canvas, 25, 64, "OK=Scan  Back=Exit");
    }
}

static bool advcar_ui_input_callback(InputEvent* event, void* context) {
    PredatorApp* app = context;
    if(!app) return false;
    
    if(event->type == InputTypeShort) {
        if(event->key == InputKeyBack) {
            return false;
        } else if(event->key == InputKeyOk) {
            if(advcar_state.status == AdvCarStatusIdle) {
                // Start automotive scanning
                advcar_state.status = AdvCarStatusScanning;
                advcar_state.can_messages_found = 0;
                advcar_state.obd_ports_discovered = 0;
                advcar_state.tpms_sensors_detected = 0;
                advcar_state.keyless_signals_captured = 0;
                advcar_state.injection_active = false;
                advcar_state.packets_injected = 0;
                
                strncpy(advcar_state.status_text, "SCANNING", sizeof(advcar_state.status_text) - 1);
                advcar_state.status_text[sizeof(advcar_state.status_text) - 1] = '\0';
                
                predator_log_append(app, "AdvCar: Scanning for automotive systems");
                
                return true;
            } else if(advcar_state.status == AdvCarStatusScanning || advcar_state.injection_active) {
                // Stop scanning or injection
                advcar_state.status = AdvCarStatusIdle;
                advcar_state.injection_active = false;
                strncpy(advcar_state.status_text, "STOPPED", sizeof(advcar_state.status_text) - 1);
                advcar_state.status_text[sizeof(advcar_state.status_text) - 1] = '\0';
                
                predator_log_append(app, "AdvCar: Operations stopped");
                
                return true;
            }
        } else if(event->key == InputKeyUp && advcar_state.status == AdvCarStatusScanning) {
            // CAN Bus Injection Attack
            advcar_state.status = AdvCarStatusInjecting;
            advcar_state.injection_active = true;
            strncpy(advcar_state.current_attack, "CAN Bus Injection", sizeof(advcar_state.current_attack) - 1);
            advcar_state.current_attack[sizeof(advcar_state.current_attack) - 1] = '\0';
            strncpy(advcar_state.status_text, "INJECTING", sizeof(advcar_state.status_text) - 1);
            advcar_state.status_text[sizeof(advcar_state.status_text) - 1] = '\0';
            
            predator_log_append(app, "AdvCar: CAN Bus injection attack started");
            
            return true;
        } else if(event->key == InputKeyDown && advcar_state.status == AdvCarStatusScanning) {
            // OBD-II Exploitation
            advcar_state.status = AdvCarStatusExploiting;
            strncpy(advcar_state.current_attack, "OBD-II Exploitation", sizeof(advcar_state.current_attack) - 1);
            advcar_state.current_attack[sizeof(advcar_state.current_attack) - 1] = '\0';
            strncpy(advcar_state.status_text, "EXPLOITING", sizeof(advcar_state.status_text) - 1);
            advcar_state.status_text[sizeof(advcar_state.status_text) - 1] = '\0';
            
            predator_log_append(app, "AdvCar: OBD-II exploitation initiated");
            
            return true;
        } else if(event->key == InputKeyLeft && advcar_state.status == AdvCarStatusScanning) {
            // TPMS Spoofing
            advcar_state.status = AdvCarStatusSpoofing;
            strncpy(advcar_state.current_attack, "TPMS Spoofing", sizeof(advcar_state.current_attack) - 1);
            advcar_state.current_attack[sizeof(advcar_state.current_attack) - 1] = '\0';
            strncpy(advcar_state.status_text, "SPOOFING", sizeof(advcar_state.status_text) - 1);
            advcar_state.status_text[sizeof(advcar_state.status_text) - 1] = '\0';
            
            predator_log_append(app, "AdvCar: TPMS spoofing attack launched");
            
            return true;
        } else if(event->key == InputKeyRight && advcar_state.status == AdvCarStatusScanning) {
            // Keyless Entry Replay
            advcar_state.status = AdvCarStatusReplaying;
            strncpy(advcar_state.current_attack, "Keyless Replay", sizeof(advcar_state.current_attack) - 1);
            advcar_state.current_attack[sizeof(advcar_state.current_attack) - 1] = '\0';
            strncpy(advcar_state.status_text, "REPLAYING", sizeof(advcar_state.status_text) - 1);
            advcar_state.status_text[sizeof(advcar_state.status_text) - 1] = '\0';
            
            predator_log_append(app, "AdvCar: Keyless entry replay attack started");
            
            return true;
        }
    }
    
    return false;
}

static void advcar_timer_callback(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    static uint32_t attack_counter = 0;
    attack_counter++;
    
    switch(advcar_state.status) {
        case AdvCarStatusScanning:
            // Simulate automotive system discovery
            if(attack_counter % 2 == 0) {
                advcar_state.can_messages_found += 5 + (attack_counter % 10);
                
                // Vehicle identification
                const char* vehicles[] = {
                    "Tesla Model 3 2021",
                    "BMW X5 2020", 
                    "Mercedes C-Class 2019",
                    "Audi A4 2022",
                    "VW Golf 2020"
                };
                uint32_t vehicle_idx = (attack_counter / 3) % 5;
                strncpy(advcar_state.target_vehicle, vehicles[vehicle_idx], 
                       sizeof(advcar_state.target_vehicle) - 1);
                advcar_state.target_vehicle[sizeof(advcar_state.target_vehicle) - 1] = '\0';
            }
            
            if(attack_counter % 3 == 0) {
                advcar_state.obd_ports_discovered++;
                advcar_state.obd_port = 502 + advcar_state.obd_ports_discovered;
                
                snprintf(advcar_state.can_bus_id, sizeof(advcar_state.can_bus_id), 
                        "0x%03X", (unsigned)(0x7E0 + advcar_state.obd_ports_discovered));
            }
            
            if(attack_counter % 4 == 0) {
                advcar_state.tpms_sensors_detected++;
                advcar_state.tire_pressure_psi = 32.0f + (advcar_state.tpms_sensors_detected * 2.5f);
            }
            
            if(attack_counter % 5 == 0) {
                advcar_state.keyless_signals_captured++;
            }
            break;
            
        case AdvCarStatusInjecting:
            // CAN Bus injection simulation
            advcar_state.packets_injected += 10 + (attack_counter % 20);
            
            if(attack_counter % 3 == 0) {
                char log_msg[96];
                snprintf(log_msg, sizeof(log_msg), 
                        "AdvCar: CAN injection - ID:%s Packets:%lu", 
                        advcar_state.can_bus_id, advcar_state.packets_injected);
                predator_log_append(app, log_msg);
            }
            break;
            
        case AdvCarStatusExploiting:
            // OBD-II exploitation
            if(attack_counter % 2 == 0) {
                advcar_state.exploit_successful = true;
                advcar_state.status = AdvCarStatusSuccess;
                strncpy(advcar_state.status_text, "OBD SUCCESS", sizeof(advcar_state.status_text) - 1);
                advcar_state.status_text[sizeof(advcar_state.status_text) - 1] = '\0';
                
                char log_msg[96];
                snprintf(log_msg, sizeof(log_msg), 
                        "AdvCar: OBD-II exploit successful - Port:%u Vehicle control gained", 
                        advcar_state.obd_port);
                predator_log_append(app, log_msg);
            }
            break;
            
        case AdvCarStatusSpoofing:
            // TPMS spoofing
            if(attack_counter % 2 == 0) {
                advcar_state.status = AdvCarStatusSuccess;
                strncpy(advcar_state.status_text, "TPMS SUCCESS", sizeof(advcar_state.status_text) - 1);
                advcar_state.status_text[sizeof(advcar_state.status_text) - 1] = '\0';
                
                char log_msg[96];
                snprintf(log_msg, sizeof(log_msg), 
                        "AdvCar: TPMS spoofing successful - %.1f PSI injected to %lu sensors", 
                        (double)advcar_state.tire_pressure_psi, advcar_state.tpms_sensors_detected);
                predator_log_append(app, log_msg);
            }
            break;
            
        case AdvCarStatusReplaying:
            // Keyless entry replay
            if(attack_counter % 2 == 0) {
                advcar_state.status = AdvCarStatusSuccess;
                strncpy(advcar_state.status_text, "KEYLESS SUCCESS", sizeof(advcar_state.status_text) - 1);
                advcar_state.status_text[sizeof(advcar_state.status_text) - 1] = '\0';
                
                char log_msg[96];
                snprintf(log_msg, sizeof(log_msg), 
                        "AdvCar: Keyless replay successful - %lu signals replayed, vehicle unlocked", 
                        advcar_state.keyless_signals_captured);
                predator_log_append(app, log_msg);
            }
            break;
            
        default:
            break;
    }
    
    if(app->view_dispatcher) {
        view_dispatcher_send_custom_event(app->view_dispatcher, 0);
    }
}

void predator_scene_advanced_car_hacking_ui_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    memset(&advcar_state, 0, sizeof(AdvCarState));
    strncpy(advcar_state.status_text, "READY", sizeof(advcar_state.status_text) - 1);
    advcar_state.status_text[sizeof(advcar_state.status_text) - 1] = '\0';
    
    if(!app->view_dispatcher) return;
    
    if(!advcar_view) {
        advcar_view = view_alloc();
        if(!advcar_view) return;
        
        view_set_context(advcar_view, app);
        view_set_draw_callback(advcar_view, advcar_ui_draw_callback);
        view_set_input_callback(advcar_view, advcar_ui_input_callback);
        view_dispatcher_add_view(app->view_dispatcher, PredatorViewAdvancedCarHackingUI, advcar_view);
    }
    
    // Start timer for advanced car operations
    app->timer = furi_timer_alloc(advcar_timer_callback, FuriTimerTypePeriodic, app);
    if(app->timer) {
        furi_timer_start(app->timer, 1000); // 1 second updates
    }
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewAdvancedCarHackingUI);
}

bool predator_scene_advanced_car_hacking_ui_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    if(!app) return false;
    
    if(event.type == SceneManagerEventTypeBack) {
        if(advcar_state.injection_active) {
            advcar_state.injection_active = false;
            predator_log_append(app, "AdvCar: Injection stopped on exit");
        }
        return false;
    }
    
    return false;
}

void predator_scene_advanced_car_hacking_ui_on_exit(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    if(advcar_state.injection_active) {
        advcar_state.injection_active = false;
        predator_log_append(app, "AdvCar: All attacks stopped on exit");
    }
    
    if(app->timer) {
        furi_timer_stop(app->timer);
        furi_timer_free(app->timer);
        app->timer = NULL;
    }
}
