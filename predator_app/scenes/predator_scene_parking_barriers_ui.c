#include "../predator_i.h"
#include "../helpers/predator_subghz.h"
#include "../helpers/predator_logging.h"
#include "../helpers/predator_constants.h"
#include "../helpers/predator_crypto_engine.h"  // ADDED: Real crypto for barriers
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
    bool use_crypto_engine;  // ADDED: Enable crypto packet generation
    uint32_t packets_sent;    // ADDED: Track packets sent
    KeeloqContext keeloq_ctx; // ADDED: Keeloq for modern barriers
    PredatorApp* app;
} ParkingBarrierState;

static ParkingBarrierState barrier_state;

// UNUSED: Frequencies commented out - using SubGHz Jamming scene instead
/*
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
*/

#define PARKING_FREQUENCY_COUNT 8

static const char* barrier_type_names[] = {
    "Private Parking",
    "Public Parking", 
    "Hospital Parking",
    "Shopping Mall",
    "Airport Parking",
    "Government Facility"
};

// UNUSED: Icons not needed for submenu-only implementation
/*
static const char* barrier_type_icons[] = {
    "🏠", "🏛️", "🏥", "🛒", "✈️", "🇨🇭"
};
*/

#define PARKING_FREQUENCY_COUNT 8

// UNUSED: Parking Barriers uses submenu + inline attack execution
/*
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
    
    // Barrier stats (opened count + packets sent)
    char stats_str[32];
    if(state->status == BarrierStatusAttacking) {
        snprintf(stats_str, sizeof(stats_str), "Packets: %lu", state->packets_sent);
    } else {
        snprintf(stats_str, sizeof(stats_str), "Opened: %lu", state->barriers_opened);
    }
    canvas_draw_str(canvas, 2, 48, stats_str);
    
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
        canvas_draw_str(canvas, 2, 58, "Barrier opened! Back=Continue");
    } else {
        canvas_draw_str(canvas, 2, 58, "Back=Exit");
    }
}
*/

// UNUSED: Inline attack execution commented out - using SubGHz Jamming scene instead
/*
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
    
    // Start parking barrier attack - transmit rolling codes for barriers
    // Use SubGHz core to start transmission
    predator_subghz_set_frequency(state->app, state->current_frequency);
    bool attack_started = true;  // Attack starts immediately with rolling codes
    
    if(attack_started) {
        state->status = BarrierStatusAttacking;
        state->start_tick = furi_get_tick();
        state->packets_sent = 0;
        
        // PRODUCTION: Initialize crypto engine for barriers
        state->use_crypto_engine = true;
        
        // Initialize Keeloq for modern parking barriers (most common)
        state->keeloq_ctx.manufacturer_key = 0x0123456789ABCDEF; // Standard parking barrier key
        state->keeloq_ctx.serial_number = 0x100000 + state->barrier_type; // Unique per barrier type
        state->keeloq_ctx.counter = 0;
        state->keeloq_ctx.button_code = 0x01; // Open command
        
        predator_log_append(state->app, "CRYPTO ENGINE: Keeloq initialized for barrier");
        FURI_LOG_I("ParkingBarriers", "[CRYPTO] Keeloq engine ready for %s", 
                  barrier_type_names[state->barrier_type]);
        
        // Swiss Government mode - enhanced attack
        if(state->swiss_mode) {
            predator_log_append(state->app, "SWISS MODE: Government-grade crypto enabled");
            FURI_LOG_I("ParkingBarriers", "[SWISS GOV] Enhanced protocols active");
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
        
        // PRODUCTION: Generate real crypto packets for barriers
        if(barrier_state.use_crypto_engine) {
            // Increment counter for rolling code barriers
            barrier_state.keeloq_ctx.counter++;
            uint8_t packet[16];
            size_t len = 0;
            
            // Generate encrypted Keeloq packet (most modern parking barriers use Keeloq)
            if(predator_crypto_keeloq_generate_packet(&barrier_state.keeloq_ctx, packet, &len)) {
                FURI_LOG_D("ParkingBarriers", "[CRYPTO] Keeloq packet %u generated for barrier", 
                          barrier_state.keeloq_ctx.counter);
                
                // CRITICAL: Actually transmit the encrypted packet via SubGHz!
                if(app->subghz_txrx) {
                    // Send encrypted packet via SubGHz hardware
                    predator_subghz_send_raw_packet(app, packet, len);
                    barrier_state.packets_sent++;
                    
                    // Log progress every 10 packets
                    if(barrier_state.packets_sent % 10 == 0) {
                        FURI_LOG_I("ParkingBarriers", "[REAL HW + CRYPTO] Sent %lu encrypted packets", 
                                  barrier_state.packets_sent);
                        
                        char log_msg[64];
                        snprintf(log_msg, sizeof(log_msg), "Crypto packets: %lu sent", barrier_state.packets_sent);
                        predator_log_append(app, log_msg);
                    }
                } else {
                    FURI_LOG_W("ParkingBarriers", "[CRYPTO] SubGHz not ready, packet generated but not sent");
                }
            }
        }
        
        // SWISS GOVERNMENT: Real success detection - ONLY when barrier actually responds
        // REMOVED FAKE SUCCESS FALLBACKS - government contract requires real results
        if(app->subghz_txrx && furi_hal_subghz_rx_pipe_not_empty()) {
            // Verify it's actually a barrier response signal
            bool signal_detected = furi_hal_subghz_get_data_gpio();
            int8_t rssi = furi_hal_subghz_get_rssi();
            
            if(signal_detected && rssi > -80) {
                // Real barrier response detected!
                barrier_state.barriers_opened++;
                barrier_state.status = BarrierStatusSuccess;
                
                char success_msg[96];
                snprintf(success_msg, sizeof(success_msg), 
                         "✅ [SWISS GOV] BARRIER OPENED: %s (RSSI:%d, %lu packets)", 
                         barrier_type_names[barrier_state.barrier_type],
                         rssi,
                         barrier_state.packets_sent);
                predator_log_append(app, success_msg);
                
                FURI_LOG_I("ParkingBarriers", "[REAL HW] Barrier responded after %lu packets (RSSI:%d)", 
                          barrier_state.packets_sent, rssi);
                
                // Stop attack on success
                predator_subghz_stop_attack(app);
                
                // Success notification
                if(app->notifications) {
                    notification_message(app->notifications, &sequence_success);
                }
            } else {
                FURI_LOG_D("ParkingBarriers", "[REAL HW] Signal detected but too weak: RSSI %d", rssi);
            }
        }
        
        // Auto-stop after 30 seconds or 500 packets (no fake success)
        if(barrier_state.attack_time_ms > 30000 || barrier_state.packets_sent >= 500) {
            // Only stop, don't fake success
            predator_subghz_stop_attack(app);
            barrier_state.status = BarrierStatusIdle;
            
            char final_msg[128];
            if(barrier_state.barriers_opened > 0) {
                snprintf(final_msg, sizeof(final_msg), 
                         "✅ [SWISS GOV] COMPLETE: %lu barriers opened, %lu encrypted packets in %lums",
                         barrier_state.barriers_opened,
                         barrier_state.packets_sent,
                         barrier_state.attack_time_ms);
            } else {
                snprintf(final_msg, sizeof(final_msg), 
                         "⚠️ [SWISS GOV] TIMEOUT: No barrier response. %lu encrypted packets sent in %lums",
                         barrier_state.packets_sent,
                         barrier_state.attack_time_ms);
            }
            predator_log_append(app, final_msg);
            
            FURI_LOG_I("ParkingBarriers", "[SWISS GOV] Attack ended: %lu barriers, %lu packets", 
                      barrier_state.barriers_opened, barrier_state.packets_sent);
        }
        
        // Trigger redraw
        if(app->view_dispatcher) {
            view_dispatcher_send_custom_event(app->view_dispatcher, 0);
        }
    }
}
*/

static void parking_barriers_submenu_cb(void* context, uint32_t index) {
    PredatorApp* app = context;
    if(!app || !app->view_dispatcher) return;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void predator_scene_parking_barriers_ui_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    // Initialize parking barrier state
    memset(&barrier_state, 0, sizeof(ParkingBarrierState));
    barrier_state.app = app;
    barrier_state.status = BarrierStatusIdle;
    barrier_state.frequency_index = 0;  // Default to 433.92 MHz
    barrier_state.swiss_mode = true;     // Swiss Government KKS mode
    
    // Critical: Ensure submenu exists
    if(!app->submenu) {
        FURI_LOG_E("ParkingBarriers", "ERROR: Submenu is NULL!");
        return;
    }
    
    // Reset and rebuild submenu
    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "🚧 PARKING BARRIERS");
    
    // Actual barrier types that can be attacked (like car models)
    submenu_add_item(app->submenu, "🏛️ Public Parking", 1, parking_barriers_submenu_cb, app);
    submenu_add_item(app->submenu, "🏠 Private Parking", 2, parking_barriers_submenu_cb, app);
    submenu_add_item(app->submenu, "🏥 Hospital Parking", 3, parking_barriers_submenu_cb, app);
    submenu_add_item(app->submenu, "🛒 Shopping Mall", 4, parking_barriers_submenu_cb, app);
    submenu_add_item(app->submenu, "✈️ Airport Parking", 5, parking_barriers_submenu_cb, app);
    submenu_add_item(app->submenu, "🇨🇭 Government Facility", 6, parking_barriers_submenu_cb, app);
    
    // Reset selection to first item (critical for proper focus after returning)
    submenu_set_selected_item(app->submenu, 0);
    
    // Critical: Ensure view_dispatcher exists before switching
    if(!app->view_dispatcher) {
        FURI_LOG_E("ParkingBarriers", "ERROR: View dispatcher is NULL!");
        return;
    }
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
    
    // Log Swiss Government activation
    predator_log_append(app, "PARKING BARRIERS: Swiss Gov KKS");
    predator_log_append(app, "Swiss Gov Mode: 433.92 MHz");
    
    FURI_LOG_I("ParkingBarriers", "Swiss Government parking barrier testing initialized");
}

bool predator_scene_parking_barriers_ui_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    if(!app) return false;
    
    // Handle back button - return false to let scene manager handle navigation
    if(event.type == SceneManagerEventTypeBack) {
        // Return false to let scene manager do default back navigation
        // Returning true would exit the app!
        return false;
    }
    
    // Handle menu selections - navigate to Barrier Attack with proper manufacturer
    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event >= 1 && event.event <= 6) {
            // Store barrier type for logging
            barrier_state.barrier_type = (ParkingBarrierType)(event.event - 1);
            
            // CRITICAL: Store barrier type in app context for manufacturer selection
            app->selected_barrier_type = event.event;  // 1-6
            
            // Log barrier type selected
            char log_msg[128];
            snprintf(log_msg, sizeof(log_msg), 
                     "BARRIER TYPE: %s",
                     barrier_type_names[barrier_state.barrier_type]);
            predator_log_append(app, log_msg);
            
            // Navigate to manufacturer selection (choose specific or try all)
            scene_manager_next_scene(app->scene_manager, PredatorSceneBarrierManufacturerSelectUI);
            return true;
        }
        return true;
    }
    
    return false;
}

void predator_scene_parking_barriers_ui_on_exit(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    // Nothing to cleanup: using shared submenu (match CarContinentUI pattern)
}
