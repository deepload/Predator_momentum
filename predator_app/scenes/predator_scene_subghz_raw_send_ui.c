#include "../predator_i.h"
#include "../helpers/predator_subghz.h"
#include "../helpers/predator_compliance.h"
#include "../helpers/predator_logging.h"
#include <gui/view.h>
#include <string.h>

// SubGHz Raw Send - Professional UI
// Shows real-time raw signal transmission with frame count and status

typedef enum {
    RawSendStatusIdle,
    RawSendStatusSending,
    RawSendStatusComplete,
    RawSendStatusError
} RawSendStatus;

typedef struct {
    RawSendStatus status;
    uint32_t frames_sent;
    uint32_t send_time_ms;
    uint32_t frequency;
    uint32_t frame_data;
    uint8_t repeat_count;
    bool subghz_ready;
    char frequency_str[16];
} RawSendState;

static RawSendState rawsend_state;
static uint32_t send_start_tick = 0;

// Common frequencies
static const uint32_t frequencies[] = {
    315000000,  // 315 MHz
    433920000,  // 433.92 MHz
    868350000,  // 868.35 MHz
    915000000   // 915 MHz
};
static const char* frequency_names[] = {
    "315 MHz",
    "433.92 MHz",
    "868.35 MHz",
    "915 MHz"
};
static const uint8_t freq_count = 4;

static void draw_raw_send_header(Canvas* canvas) {
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 2, 10, "RAW SEND");
    canvas_draw_line(canvas, 0, 12, 128, 12);
}

static void draw_raw_send_status(Canvas* canvas, RawSendState* state) {
    canvas_set_font(canvas, FontSecondary);
    
    // Status line
    canvas_draw_str(canvas, 2, 22, "Status:");
    const char* status_text = "Unknown";
    switch(state->status) {
        case RawSendStatusIdle: status_text = "Ready"; break;
        case RawSendStatusSending: status_text = "Sending"; break;
        case RawSendStatusComplete: status_text = "Sent!"; break;
        case RawSendStatusError: status_text = "Error"; break;
    }
    canvas_draw_str(canvas, 45, 22, status_text);
    
    // Frequency
    canvas_draw_str(canvas, 2, 32, "Freq:");
    canvas_draw_str(canvas, 35, 32, state->frequency_str);
    
    // Progress bar
    canvas_draw_frame(canvas, 2, 36, 124, 6);
    if(state->status == RawSendStatusSending) {
        uint8_t progress = (state->frames_sent * 122) / state->repeat_count;
        if(progress > 122) progress = 122;
        canvas_draw_box(canvas, 3, 37, progress, 4);
    } else if(state->status == RawSendStatusComplete) {
        canvas_draw_box(canvas, 3, 37, 122, 4);
    }
}

static void draw_raw_send_info(Canvas* canvas, RawSendState* state) {
    canvas_set_font(canvas, FontSecondary);
    
    // Frames sent
    char frames_str[32];
    snprintf(frames_str, sizeof(frames_str), "Frames: %lu/%u", 
            state->frames_sent, (unsigned)state->repeat_count);
    canvas_draw_str(canvas, 2, 48, frames_str);
    
    // Time
    char time_str[32];
    uint32_t ms = state->send_time_ms;
    snprintf(time_str, sizeof(time_str), "Time: %lums", ms);
    canvas_draw_str(canvas, 2, 58, time_str);
    
    // Frame data
    char data_str[32];
    snprintf(data_str, sizeof(data_str), "Data: 0x%08lX", state->frame_data);
    canvas_draw_str(canvas, 2, 64, data_str);
}

static void raw_send_ui_draw_callback(Canvas* canvas, void* context) {
    furi_assert(canvas);
    furi_assert(context);
    
    UNUSED(context);
    
    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);
    
    draw_raw_send_header(canvas);
    draw_raw_send_status(canvas, &rawsend_state);
    draw_raw_send_info(canvas, &rawsend_state);
}

static bool raw_send_ui_input_callback(InputEvent* event, void* context) {
    PredatorApp* app = context;
    if(!app) return false;
    
    if(event->type == InputTypeShort) {
        if(event->key == InputKeyBack) {
            if(rawsend_state.status == RawSendStatusSending) {
                rawsend_state.status = RawSendStatusComplete;
                predator_subghz_stop_attack(app);
                
                char log_msg[64];
                snprintf(log_msg, sizeof(log_msg), "Raw Send STOP: %lu frames sent", 
                        rawsend_state.frames_sent);
                predator_log_append(app, log_msg);
            }
            return false;
        } else if(event->key == InputKeyOk) {
            if(rawsend_state.status == RawSendStatusIdle) {
                // Check compliance
                PredatorFeature feat = PredatorFeatureSubGhz433Tx;
                if(rawsend_state.frequency == 315000000) feat = PredatorFeatureSubGhz315Tx;
                
                bool allowed = predator_compliance_is_feature_allowed(app, feat, app->authorized);
                if(!allowed) {
                    rawsend_state.status = RawSendStatusError;
                    predator_log_append(app, "Raw Send DENIED: Authorization required");
                    return true;
                }
                
                rawsend_state.status = RawSendStatusSending;
                rawsend_state.frames_sent = 0;
                rawsend_state.send_time_ms = 0;
                send_start_tick = furi_get_tick();
                
                predator_subghz_init(app);
                
                char log_msg[80];
                snprintf(log_msg, sizeof(log_msg), "Raw Send START: %s, data=0x%08lX, repeat=%u", 
                        rawsend_state.frequency_str, rawsend_state.frame_data, 
                        (unsigned)rawsend_state.repeat_count);
                predator_log_append(app, log_msg);
                
                FURI_LOG_I("RawSendUI", "Sending started");
                return true;
            } else if(rawsend_state.status == RawSendStatusComplete) {
                // Reset to idle
                rawsend_state.status = RawSendStatusIdle;
                rawsend_state.frames_sent = 0;
                rawsend_state.send_time_ms = 0;
                return true;
            }
        } else if(event->key == InputKeyLeft && rawsend_state.status == RawSendStatusIdle) {
            // Cycle frequency
            if(rawsend_state.frequency == frequencies[0]) {
                rawsend_state.frequency = frequencies[freq_count - 1];
                snprintf(rawsend_state.frequency_str, sizeof(rawsend_state.frequency_str), 
                        "%.15s", frequency_names[freq_count - 1]);
            } else {
                for(uint8_t i = 1; i < freq_count; i++) {
                    if(rawsend_state.frequency == frequencies[i]) {
                        rawsend_state.frequency = frequencies[i - 1];
                        snprintf(rawsend_state.frequency_str, sizeof(rawsend_state.frequency_str), 
                                "%.15s", frequency_names[i - 1]);
                        break;
                    }
                }
            }
            return true;
        } else if(event->key == InputKeyRight && rawsend_state.status == RawSendStatusIdle) {
            // Cycle frequency
            for(uint8_t i = 0; i < freq_count - 1; i++) {
                if(rawsend_state.frequency == frequencies[i]) {
                    rawsend_state.frequency = frequencies[i + 1];
                    snprintf(rawsend_state.frequency_str, sizeof(rawsend_state.frequency_str), 
                            "%.15s", frequency_names[i + 1]);
                    return true;
                }
            }
            rawsend_state.frequency = frequencies[0];
            snprintf(rawsend_state.frequency_str, sizeof(rawsend_state.frequency_str), 
                    "%.15s", frequency_names[0]);
            return true;
        } else if(event->key == InputKeyUp && rawsend_state.status == RawSendStatusIdle) {
            if(rawsend_state.repeat_count < 100) {
                rawsend_state.repeat_count += 10;
            }
            return true;
        } else if(event->key == InputKeyDown && rawsend_state.status == RawSendStatusIdle) {
            if(rawsend_state.repeat_count > 10) {
                rawsend_state.repeat_count -= 10;
            }
            return true;
        }
    }
    
    return true;
}

static void raw_send_ui_timer_callback(void* context) {
    furi_assert(context);
    PredatorApp* app = context;
    
    if(rawsend_state.status == RawSendStatusSending) {
        rawsend_state.send_time_ms = furi_get_tick() - send_start_tick;
        
        // Simulate frame sending (1 frame per 100ms)
        if(rawsend_state.frames_sent < rawsend_state.repeat_count) {
            rawsend_state.frames_sent++;
            
            // Actually send the frame
            predator_subghz_send_car_key(app, rawsend_state.frame_data);
        }
        
        // Complete when all frames sent
        if(rawsend_state.frames_sent >= rawsend_state.repeat_count) {
            rawsend_state.status = RawSendStatusComplete;
            predator_subghz_stop_attack(app);
            
            char log_msg[80];
            snprintf(log_msg, sizeof(log_msg), "Raw Send COMPLETE: %lu frames sent in %lums", 
                    rawsend_state.frames_sent, rawsend_state.send_time_ms);
            predator_log_append(app, log_msg);
            
            FURI_LOG_I("RawSendUI", "Send complete");
        }
        
        if(app->view_dispatcher) {
            view_dispatcher_send_custom_event(app->view_dispatcher, 0);
        }
    }
}

void predator_scene_subghz_raw_send_ui_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    memset(&rawsend_state, 0, sizeof(RawSendState));
    rawsend_state.status = RawSendStatusIdle;
    
    // Set default frequency based on region
    uint8_t freq_idx = 1; // Default 433.92 MHz
    if(app->region == PredatorRegionUS || app->region == PredatorRegionJP) {
        freq_idx = 0; // 315 MHz for US/JP
    }
    
    rawsend_state.frequency = frequencies[freq_idx];
    rawsend_state.frame_data = 0xA5B6C7D8; // Example frame data
    rawsend_state.repeat_count = 10;
    snprintf(rawsend_state.frequency_str, sizeof(rawsend_state.frequency_str), 
            "%.15s", frequency_names[freq_idx]);
    
    if(!app->view_dispatcher) {
        FURI_LOG_E("RawSendUI", "View dispatcher is NULL");
        return;
    }
    
    View* view = view_alloc();
    if(!view) {
        FURI_LOG_E("RawSendUI", "Failed to allocate view");
        return;
    }
    
    view_set_context(view, app);
    view_set_draw_callback(view, raw_send_ui_draw_callback);
    view_set_input_callback(view, raw_send_ui_input_callback);
    
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewSubGhzRawSendUI, view);
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubGhzRawSendUI);
    
    FURI_LOG_I("RawSendUI", "Raw Send UI initialized");
    
    app->timer = furi_timer_alloc(raw_send_ui_timer_callback, FuriTimerTypePeriodic, app);
    furi_timer_start(app->timer, 100);
}

bool predator_scene_subghz_raw_send_ui_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    if(!app) return false;
    
    if(event.type == SceneManagerEventTypeCustom) {
        return true;
    }
    
    return false;
}

void predator_scene_subghz_raw_send_ui_on_exit(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    if(app->timer) {
        furi_timer_stop(app->timer);
        furi_timer_free(app->timer);
        app->timer = NULL;
    }
    
    if(rawsend_state.status == RawSendStatusSending) {
        predator_subghz_stop_attack(app);
        
        char log_msg[64];
        snprintf(log_msg, sizeof(log_msg), "Raw Send EXIT: %lu frames sent", 
                rawsend_state.frames_sent);
        predator_log_append(app, log_msg);
    }
    
    rawsend_state.status = RawSendStatusIdle;
    
    if(app->view_dispatcher) {
        view_dispatcher_remove_view(app->view_dispatcher, PredatorViewSubGhzRawSendUI);
    }
    
    FURI_LOG_I("RawSendUI", "Raw Send UI exited");
}
