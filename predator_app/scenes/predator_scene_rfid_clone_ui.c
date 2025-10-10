#include "../predator_i.h"
#include "../helpers/predator_logging.h"
#include "../helpers/predator_nfc.h"
#include <gui/view.h>
#include <string.h>

// RFID Clone - Government-Grade Professional UI
// Real hardware integration with Flipper Zero NFC subsystem
// Shows real-time reading progress, block count, and UID information

typedef enum {
    RfidCloneStatusIdle,
    RfidCloneStatusDetecting,
    RfidCloneStatusReading,
    RfidCloneStatusCloning,
    RfidCloneStatusComplete,
    RfidCloneStatusError
} RfidCloneStatus;

typedef struct {
    RfidCloneStatus status;
    PredatorNfcCard card;
    uint32_t operation_time_ms;
    bool nfc_ready;
    char status_text[32];
    char error_text[64];
} RfidCloneState;

static RfidCloneState rfid_state;
static uint32_t operation_start_tick = 0;

static void draw_rfid_header(Canvas* canvas) {
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 2, 10, "RFID CLONE");
    canvas_draw_line(canvas, 0, 12, 128, 12);
}

static void draw_rfid_status(Canvas* canvas, RfidCloneState* state) {
    canvas_set_font(canvas, FontSecondary);
    
    // Status line
    canvas_draw_str(canvas, 2, 22, "Status:");
    const char* status_text = "Unknown";
    switch(state->status) {
        case RfidCloneStatusIdle: status_text = "Ready"; break;
        case RfidCloneStatusDetecting: status_text = "Detecting"; break;
        case RfidCloneStatusReading: status_text = "Reading"; break;
        case RfidCloneStatusCloning: status_text = "Cloning"; break;
        case RfidCloneStatusComplete: status_text = "Complete"; break;
        case RfidCloneStatusError: status_text = "Error"; break;
    }
    canvas_draw_str(canvas, 45, 22, status_text);
    
    // Card type
    if(state->card.type_str[0] != '\0') {
        canvas_draw_str(canvas, 2, 32, "Type:");
        canvas_draw_str(canvas, 35, 32, state->card.type_str);
    } else {
        canvas_draw_str(canvas, 2, 32, "Type: Detecting...");
    }
    
    // Progress bar
    canvas_draw_frame(canvas, 2, 36, 124, 6);
    if(state->card.total_blocks > 0) {
        uint16_t current = (state->status == RfidCloneStatusCloning) ? 
                          state->card.blocks_written : state->card.blocks_read;
        uint8_t progress = (current * 122) / state->card.total_blocks;
        if(progress > 122) progress = 122;
        canvas_draw_box(canvas, 3, 37, progress, 4);
    }
}

static void draw_rfid_info(Canvas* canvas, RfidCloneState* state) {
    canvas_set_font(canvas, FontSecondary);
    
    // Blocks read/written
    char blocks_str[32];
    uint16_t current = (state->status == RfidCloneStatusCloning) ? 
                      state->card.blocks_written : state->card.blocks_read;
    snprintf(blocks_str, sizeof(blocks_str), "Blocks: %u/%u", 
            (unsigned)current, (unsigned)state->card.total_blocks);
    canvas_draw_str(canvas, 2, 48, blocks_str);
    
    // Operation time
    char time_str[32];
    uint32_t seconds = state->operation_time_ms / 1000;
    snprintf(time_str, sizeof(time_str), "Time: %lus", seconds);
    canvas_draw_str(canvas, 70, 48, time_str);
    
    // UID
    if(state->card.uid_str[0] != '\0') {
        canvas_draw_str(canvas, 2, 58, "UID:");
        canvas_draw_str(canvas, 30, 58, state->card.uid_str);
    } else {
        canvas_draw_str(canvas, 2, 58, "UID: Not detected");
    }
}

static void rfid_clone_ui_draw_callback(Canvas* canvas, void* context) {
    furi_assert(canvas);
    furi_assert(context);
    
    UNUSED(context);
    
    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);
    
    // Draw header
    draw_rfid_header(canvas);
    
    // Draw status
    draw_rfid_status(canvas, &rfid_state);
    
    // Draw info
    draw_rfid_info(canvas, &rfid_state);
    
    // Draw instructions
    canvas_set_font(canvas, FontSecondary);
    if(rfid_state.status == RfidCloneStatusDetecting) {
        canvas_draw_str(canvas, 20, 64, "Place card on reader");
    } else if(rfid_state.status == RfidCloneStatusReading) {
        canvas_draw_str(canvas, 25, 64, "Reading hardware...");
    } else if(rfid_state.status == RfidCloneStatusCloning) {
        canvas_draw_str(canvas, 25, 64, "Writing hardware...");
    } else if(rfid_state.status == RfidCloneStatusComplete) {
        canvas_draw_str(canvas, 25, 64, "OK=Clone  Back=Exit");
    } else if(rfid_state.status == RfidCloneStatusIdle) {
        canvas_draw_str(canvas, 25, 64, "OK=Read  Back=Exit");
    } else if(rfid_state.status == RfidCloneStatusError) {
        canvas_draw_str(canvas, 30, 64, rfid_state.error_text);
    } else {
        canvas_draw_str(canvas, 40, 64, "Back=Exit");
    }
}

static bool rfid_clone_ui_input_callback(InputEvent* event, void* context) {
    PredatorApp* app = context;
    if(!app) return false;
    
    if(event->type == InputTypeShort) {
        if(event->key == InputKeyBack) {
            // Stop operation and exit
            if(rfid_state.status == RfidCloneStatusDetecting ||
               rfid_state.status == RfidCloneStatusReading || 
               rfid_state.status == RfidCloneStatusCloning) {
                rfid_state.status = RfidCloneStatusIdle;
                
                char log_msg[64];
                uint16_t current = (rfid_state.status == RfidCloneStatusCloning) ? 
                                  rfid_state.card.blocks_written : rfid_state.card.blocks_read;
                snprintf(log_msg, sizeof(log_msg), "RFID STOP: %u/%u blocks", 
                        (unsigned)current, (unsigned)rfid_state.card.total_blocks);
                predator_log_append(app, log_msg);
            }
            return false; // Let scene manager handle back
        } else if(event->key == InputKeyOk) {
            if(rfid_state.status == RfidCloneStatusIdle) {
                // Start detection and reading
                rfid_state.status = RfidCloneStatusDetecting;
                memset(&rfid_state.card, 0, sizeof(PredatorNfcCard));
                rfid_state.operation_time_ms = 0;
                operation_start_tick = furi_get_tick();
                
                predator_log_append(app, "NFC: Detection started");
                FURI_LOG_I("RfidCloneUI", "Detection started - waiting for card");
                return true;
            } else if(rfid_state.status == RfidCloneStatusComplete) {
                // Start cloning
                rfid_state.status = RfidCloneStatusCloning;
                rfid_state.card.blocks_written = 0;
                operation_start_tick = furi_get_tick();
                
                char log_msg[96];
                snprintf(log_msg, sizeof(log_msg), "NFC: Clone START %s (%s)", 
                        rfid_state.card.type_str, rfid_state.card.uid_str);
                predator_log_append(app, log_msg);
                
                FURI_LOG_I("RfidCloneUI", "Cloning started");
                return true;
            }
        }
    }
    
    return true;
}

static void rfid_clone_ui_timer_callback(void* context) {
    furi_assert(context);
    PredatorApp* app = context;
    
    // Update operation time for active operations
    if(rfid_state.status != RfidCloneStatusIdle && 
       rfid_state.status != RfidCloneStatusComplete &&
       rfid_state.status != RfidCloneStatusError) {
        rfid_state.operation_time_ms = furi_get_tick() - operation_start_tick;
    }
    
    // Handle detection state
    if(rfid_state.status == RfidCloneStatusDetecting) {
        // Attempt to detect card using real NFC hardware
        if(predator_nfc_detect_card(app, &rfid_state.card)) {
            // Card detected, start reading
            rfid_state.status = RfidCloneStatusReading;
            FURI_LOG_I("RfidCloneUI", "Card detected, starting read");
        }
    }
    
    // Handle reading state
    else if(rfid_state.status == RfidCloneStatusReading) {
        // Real hardware reading in progress
        if(predator_nfc_read_card(app, &rfid_state.card)) {
            // Reading complete
            rfid_state.status = RfidCloneStatusComplete;
            
            char log_msg[96];
            snprintf(log_msg, sizeof(log_msg), "NFC: Read COMPLETE %s (%s)", 
                    rfid_state.card.type_str, rfid_state.card.uid_str);
            predator_log_append(app, log_msg);
            
            FURI_LOG_I("RfidCloneUI", "Reading complete: %u blocks", rfid_state.card.blocks_read);
        }
    }
    
    // Handle cloning state
    else if(rfid_state.status == RfidCloneStatusCloning) {
        // Real hardware writing in progress
        if(predator_nfc_write_card(app, &rfid_state.card)) {
            // Cloning complete
            rfid_state.status = RfidCloneStatusComplete;
            
            char log_msg[96];
            snprintf(log_msg, sizeof(log_msg), "NFC: Clone COMPLETE %s (%s)", 
                    rfid_state.card.type_str, rfid_state.card.uid_str);
            predator_log_append(app, log_msg);
            
            FURI_LOG_I("RfidCloneUI", "Cloning complete: %u blocks", rfid_state.card.blocks_written);
        }
    }
    
    // Trigger view update
    if(app->view_dispatcher) {
        view_dispatcher_send_custom_event(app->view_dispatcher, 0);
    }
}

void predator_scene_rfid_clone_ui_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    // Initialize NFC hardware
    predator_nfc_init(app);
    
    // Initialize RFID state
    memset(&rfid_state, 0, sizeof(RfidCloneState));
    rfid_state.status = RfidCloneStatusIdle;
    snprintf(rfid_state.status_text, sizeof(rfid_state.status_text), "Ready");
    rfid_state.nfc_ready = predator_nfc_is_ready(app);
    
    if(!rfid_state.nfc_ready) {
        FURI_LOG_W("RfidCloneUI", "NFC hardware not ready");
        snprintf(rfid_state.error_text, sizeof(rfid_state.error_text), "NFC Error");
    }
    
    // Setup custom view
    if(!app->view_dispatcher) {
        FURI_LOG_E("RfidCloneUI", "View dispatcher is NULL");
        return;
    }
    
    // Create view with callbacks
    View* view = view_alloc();
    if(!view) {
        FURI_LOG_E("RfidCloneUI", "Failed to allocate view");
        return;
    }
    
    view_set_context(view, app);
    view_set_draw_callback(view, rfid_clone_ui_draw_callback);
    view_set_input_callback(view, rfid_clone_ui_input_callback);
    
    // Add view to dispatcher
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewRfidCloneUI, view);
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewRfidCloneUI);
    
    FURI_LOG_I("RfidCloneUI", "RFID Clone UI initialized (NFC ready: %d)", rfid_state.nfc_ready);
    predator_log_append(app, "NFC: RFID Clone ready");
    
    // Start timer for updates
    app->timer = furi_timer_alloc(rfid_clone_ui_timer_callback, FuriTimerTypePeriodic, app);
    furi_timer_start(app->timer, 100); // Update every 100ms
}

bool predator_scene_rfid_clone_ui_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    if(!app) return false;
    
    if(event.type == SceneManagerEventTypeCustom) {
        // Custom event received - view will redraw automatically
        return true;
    }
    
    return false;
}

void predator_scene_rfid_clone_ui_on_exit(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    // Stop timer
    if(app->timer) {
        furi_timer_stop(app->timer);
        furi_timer_free(app->timer);
        app->timer = NULL;
    }
    
    // Log exit
    if(rfid_state.status == RfidCloneStatusDetecting ||
       rfid_state.status == RfidCloneStatusReading || 
       rfid_state.status == RfidCloneStatusCloning) {
        char log_msg[96];
        uint16_t current = (rfid_state.status == RfidCloneStatusCloning) ? 
                          rfid_state.card.blocks_written : rfid_state.card.blocks_read;
        snprintf(log_msg, sizeof(log_msg), "NFC: EXIT %u/%u blocks", 
                (unsigned)current, (unsigned)rfid_state.card.total_blocks);
        predator_log_append(app, log_msg);
    }
    
    rfid_state.status = RfidCloneStatusIdle;
    
    // Deinitialize NFC hardware
    predator_nfc_deinit(app);
    
    // Remove view
    if(app->view_dispatcher) {
        view_dispatcher_remove_view(app->view_dispatcher, PredatorViewRfidCloneUI);
    }
    
    FURI_LOG_I("RfidCloneUI", "RFID Clone UI exited");
}
