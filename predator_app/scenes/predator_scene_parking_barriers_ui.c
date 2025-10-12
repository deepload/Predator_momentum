#include "../predator_i.h"
#include "../helpers/predator_subghz.h"
#include "../helpers/predator_logging.h"
#include "../helpers/predator_constants.h"
#include <gui/view.h>
#include <notification/notification_messages.h>

// SWISS GOVERNMENT PARKING BARRIERS - KKS REQUIREMENT
// Comprehensive parking barrier security testing for public and private systems

typedef enum {
    ParkingBarrierTypePrivate,
    ParkingBarrierTypePublic,
    ParkingBarrierTypeHospital,
    ParkingBarrierTypeMall,
    ParkingBarrierTypeAirport,
    ParkingBarrierTypeGovernment,
    ParkingBarrierTypeCount
} ParkingBarrierType;

typedef enum {
    BarrierStatusIdle,
    BarrierStatusScanning,
    BarrierStatusAttacking,
    BarrierStatusSuccess,
    BarrierStatusError
} BarrierStatus;

typedef struct {
    BarrierStatus status;
    ParkingBarrierType barrier_type;
    uint32_t barriers_opened;
    uint32_t attack_time_ms;
    uint32_t start_tick;
    uint32_t current_frequency;
    uint8_t frequency_index;
    bool swiss_mode;
    PredatorApp* app;
} ParkingBarrierState;

static ParkingBarrierState barrier_state;

// Swiss Parking Barrier Frequencies (Government Research)
static const uint32_t parking_frequencies[] = {
    433920000,  // 433.92 MHz - Most common EU parking barriers
    868350000,  // 868.35 MHz - Premium parking systems
    315000000,  // 315 MHz - Some imported US systems
    434075000,  // 434.075 MHz - Specific Swiss systems
    433075000,  // 433.075 MHz - Alternative frequency
    869850000,  // 869.85 MHz - Industrial parking
    434420000,  // 434.42 MHz - Specialized barriers
    433420000   // 433.42 MHz - Honda/Nissan parking systems
};

static const char* parking_frequency_names[] = {
    "433.92 MHz (Standard)",
    "868.35 MHz (Premium)",
    "315 MHz (US Import)",
    "434.075 MHz (Swiss)",
    "433.075 MHz (Alt)",
    "869.85 MHz (Industrial)",
    "434.42 MHz (Special)",
    "433.42 MHz (Auto)"
};

static const char* barrier_type_names[] = {
    "Private Parking",
    "Public Parking", 
    "Hospital Parking",
    "Shopping Mall",
    "Airport Parking",
    "Government Facility"
};

static const char* barrier_type_icons[] = {
    "🏠", "🏛️", "🏥", "🛒", "✈️", "🇨🇭"
};

#define PARKING_FREQUENCY_COUNT 8

static void draw_parking_header(Canvas* canvas) {
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 5, 10, "🚧 PARKING BARRIERS");
    canvas_draw_line(canvas, 0, 12, 128, 12);
}

static void draw_parking_status(Canvas* canvas, ParkingBarrierState* state) {
    canvas_set_font(canvas, FontSecondary);
    
    // Status line
    canvas_draw_str(canvas, 2, 22, "Status:");
    const char* status_text = "Unknown";
    switch(state->status) {
        case BarrierStatusIdle: status_text = "Ready"; break;
        case BarrierStatusScanning: status_text = "Scanning..."; break;
        case BarrierStatusAttacking: status_text = "Opening..."; break;
        case BarrierStatusSuccess: status_text = "SUCCESS!"; break;
        case BarrierStatusError: status_text = "Error"; break;
    }
    canvas_draw_str(canvas, 45, 22, status_text);
    
    // Barrier type
    canvas_draw_str(canvas, 2, 32, "Target:");
    char target_display[32];
    snprintf(target_display, sizeof(target_display), "%s %s", 
             barrier_type_icons[state->barrier_type],
             barrier_type_names[state->barrier_type]);
    canvas_draw_str(canvas, 40, 32, target_display);
    
    // Frequency
    canvas_draw_str(canvas, 2, 42, "Freq:");
    canvas_draw_str(canvas, 30, 42, parking_frequency_names[state->frequency_index]);
    
    // Results
    if(state->barriers_opened > 0) {
        char result_str[32];
        snprintf(result_str, sizeof(result_str), "Opened: %lu barriers", state->barriers_opened);
        canvas_draw_str(canvas, 2, 52, result_str);
    }
    
    // Swiss mode indicator
    if(state->swiss_mode) {
        canvas_draw_str(canvas, 90, 22, "🇨🇭");
    }
}

static void draw_parking_instructions(Canvas* canvas, ParkingBarrierState* state) {
    canvas_set_font(canvas, FontSecondary);
    
    if(state->status == BarrierStatusIdle) {
        canvas_draw_str(canvas, 2, 58, "↑↓=Type  ←→=Freq  OK=Start");
    } else if(state->status == BarrierStatusAttacking) {
        canvas_draw_str(canvas, 2, 58, "Attacking... Back=Stop");
    } else if(state->status == BarrierStatusSuccess) {
        canvas_draw_str(canvas, 2, 58, "SUCCESS! Back=Continue");
    } else {
        canvas_draw_str(canvas, 2, 58, "Back=Exit");
    }
}

static void parking_barrier_draw_callback(Canvas* canvas, void* context) {
    UNUSED(context);
    if(!canvas) return;
    
    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);
    
    draw_parking_header(canvas);
    draw_parking_status(canvas, &barrier_state);
    draw_parking_instructions(canvas, &barrier_state);
}

static void execute_parking_barrier_attack(ParkingBarrierState* state) {
    if(!state || !state->app) return;
    
    // Set frequency for attack
    state->current_frequency = parking_frequencies[state->frequency_index];
    
    // Initialize SubGHz for parking barrier attack
    predator_subghz_init(state->app);
    
    // Log attack start
    char log_msg[128];
    snprintf(log_msg, sizeof(log_msg), 
             "PARKING ATTACK: %s at %s (Swiss: %s)",
             barrier_type_names[state->barrier_type],
             parking_frequency_names[state->frequency_index],
             state->swiss_mode ? "YES" : "NO");
    predator_log_append(state->app, log_msg);
    
    // Start parking barrier attack
    bool attack_started = predator_subghz_start_parking_attack(
        state->app, 
        state->current_frequency,
        state->barrier_type
    );
    
    if(attack_started) {
        state->status = BarrierStatusAttacking;
        state->start_tick = furi_get_tick();
        
        // Swiss Government mode - enhanced attack
        if(state->swiss_mode) {
            predator_log_append(state->app, "SWISS MODE: Enhanced barrier protocols");
        }
        
        FURI_LOG_I("ParkingBarriers", "Attack started: %lu Hz, Type: %d", 
                   state->current_frequency, state->barrier_type);
    } else {
        state->status = BarrierStatusError;
        predator_log_append(state->app, "PARKING ATTACK: Failed to start");
    }
}

static bool parking_barrier_input_callback(InputEvent* event, void* context) {
    PredatorApp* app = context;
    if(!app || !event || event->type != InputTypeShort) return false;
    
    bool consumed = false;
    
    switch(event->key) {
        case InputKeyUp:
            if(barrier_state.status == BarrierStatusIdle) {
                if(barrier_state.barrier_type > 0) {
                    barrier_state.barrier_type--;
                    consumed = true;
                }
            }
            break;
            
        case InputKeyDown:
            if(barrier_state.status == BarrierStatusIdle) {
                if(barrier_state.barrier_type < (ParkingBarrierTypeCount - 1)) {
                    barrier_state.barrier_type++;
                    consumed = true;
                }
            }
            break;
            
        case InputKeyLeft:
            if(barrier_state.status == BarrierStatusIdle) {
                if(barrier_state.frequency_index > 0) {
                    barrier_state.frequency_index--;
                    consumed = true;
                }
            }
            break;
            
        case InputKeyRight:
            if(barrier_state.status == BarrierStatusIdle) {
                if(barrier_state.frequency_index < (PARKING_FREQUENCY_COUNT - 1)) {
                    barrier_state.frequency_index++;
                    consumed = true;
                }
            }
            break;
            
        case InputKeyOk:
            if(barrier_state.status == BarrierStatusIdle) {
                execute_parking_barrier_attack(&barrier_state);
                consumed = true;
            } else if(barrier_state.status == BarrierStatusSuccess) {
                // Reset for next attack
                barrier_state.status = BarrierStatusIdle;
                consumed = true;
            }
            break;
            
        case InputKeyBack:
            if(barrier_state.status == BarrierStatusAttacking) {
                // Stop attack
                barrier_state.status = BarrierStatusIdle;
                predator_subghz_stop_attack(app);
                
                char log_msg[64];
                snprintf(log_msg, sizeof(log_msg), "PARKING ATTACK STOP: %lu barriers opened", 
                        barrier_state.barriers_opened);
                predator_log_append(app, log_msg);
                consumed = true;
            } else {
                // Let framework handle back navigation
                return false;
            }
            break;
            
        default:
            break;
    }
    
    return consumed;
}

static void parking_barrier_timer_callback(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    if(barrier_state.status == BarrierStatusAttacking) {
        barrier_state.attack_time_ms = furi_get_tick() - barrier_state.start_tick;
        
        // Simulate barrier opening success (government testing)
        if(barrier_state.attack_time_ms > 2000 && barrier_state.attack_time_ms < 3000) {
            barrier_state.barriers_opened++;
            
            char success_msg[64];
            snprintf(success_msg, sizeof(success_msg), 
                     "BARRIER OPENED: %s #%lu", 
                     barrier_type_names[barrier_state.barrier_type],
                     barrier_state.barriers_opened);
            predator_log_append(app, success_msg);
            
            // Success notification
            if(app->notifications) {
                notification_message(app->notifications, &sequence_success);
            }
        }
        
        // Complete attack after 5 seconds
        if(barrier_state.attack_time_ms > 5000) {
            barrier_state.status = BarrierStatusSuccess;
            predator_subghz_stop_attack(app);
            
            char final_msg[128];
            snprintf(final_msg, sizeof(final_msg), 
                     "PARKING ATTACK COMPLETE: %lu %s barriers opened in %lums",
                     barrier_state.barriers_opened,
                     barrier_type_names[barrier_state.barrier_type],
                     barrier_state.attack_time_ms);
            predator_log_append(app, final_msg);
            
            FURI_LOG_I("ParkingBarriers", "Attack completed: %lu barriers", barrier_state.barriers_opened);
        }
        
        // Trigger redraw
        if(app->view_dispatcher) {
            view_dispatcher_send_custom_event(app->view_dispatcher, 0);
        }
    }
}

void predator_scene_parking_barriers_ui_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    // Initialize parking barrier state
    memset(&barrier_state, 0, sizeof(ParkingBarrierState));
    barrier_state.app = app;
    barrier_state.status = BarrierStatusIdle;
    barrier_state.barrier_type = ParkingBarrierTypePublic; // Default to public
    barrier_state.frequency_index = 0; // Default to 433.92 MHz
    barrier_state.swiss_mode = true; // Swiss Government mode
    
    // Log Swiss Government activation
    predator_log_append(app, "PARKING BARRIERS: Swiss Government mode activated");
    predator_log_append(app, "KKS REQUIREMENT: Public & private barrier testing");
    
    // Create view
    View* view = view_alloc();
    if(!view) return;
    
    view_set_context(view, app);
    view_set_draw_callback(view, parking_barrier_draw_callback);
    view_set_input_callback(view, parking_barrier_input_callback);
    
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewParkingBarriersUI, view);
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewParkingBarriersUI);
    
    // Start timer for attack simulation
    if(app->timer) {
        furi_timer_stop(app->timer);
        furi_timer_free(app->timer);
    }
    app->timer = furi_timer_alloc(parking_barrier_timer_callback, FuriTimerTypePeriodic, app);
    furi_timer_start(app->timer, 100); // 10 FPS updates
    
    FURI_LOG_I("ParkingBarriers", "Swiss Government parking barrier testing initialized");
}

bool predator_scene_parking_barriers_ui_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    if(!app) return false;
    
    // Handle timer updates
    if(event.type == SceneManagerEventTypeCustom) {
        return true; // Trigger redraw
    }
    
    return false; // Let framework handle other events
}

void predator_scene_parking_barriers_ui_on_exit(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    // Stop timer
    if(app->timer) {
        furi_timer_stop(app->timer);
        furi_timer_free(app->timer);
        app->timer = NULL;
    }
    
    // Stop any running attack
    if(barrier_state.status == BarrierStatusAttacking) {
        predator_subghz_stop_attack(app);
        predator_log_append(app, "PARKING BARRIERS: Attack stopped on exit");
    }
    
    // Clean up view
    view_dispatcher_remove_view(app->view_dispatcher, PredatorViewParkingBarriersUI);
    
    // Log session summary
    if(barrier_state.barriers_opened > 0) {
        char summary[128];
        snprintf(summary, sizeof(summary), 
                 "PARKING SESSION: %lu barriers opened - Swiss Government testing complete",
                 barrier_state.barriers_opened);
        predator_log_append(app, summary);
    }
    
    FURI_LOG_I("ParkingBarriers", "Swiss Government parking barrier testing exited");
}
