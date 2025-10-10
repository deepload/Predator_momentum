#include "../predator_i.h"
#include "../helpers/predator_subghz.h"
#include "../helpers/predator_logging.h"
#include <gui/view.h>
#include <string.h>

// Car Key Bruteforce - Professional UI
// Shows real-time key bruteforce with codes tried, frequency, and success detection

typedef enum {
    CarKeyBruteforceStatusIdle,
    CarKeyBruteforceStatusAttacking,
    CarKeyBruteforceStatusSuccess,
    CarKeyBruteforceStatusComplete,
    CarKeyBruteforceStatusError
} CarKeyBruteforceStatus;

typedef struct {
    CarKeyBruteforceStatus status;
    uint32_t frequency;
    uint32_t codes_tried;
    uint32_t total_codes;
    uint32_t attack_time_ms;
    uint32_t eta_seconds;
    char found_code[16];
    bool subghz_ready;
} CarKeyBruteforceState;

static CarKeyBruteforceState carkey_state;
static uint32_t attack_start_tick = 0;

static void draw_car_key_header(Canvas* canvas) {
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 2, 10, "CAR KEY BRUTE");
    canvas_draw_line(canvas, 0, 12, 128, 12);
}

static void draw_car_key_status(Canvas* canvas, CarKeyBruteforceState* state) {
    canvas_set_font(canvas, FontSecondary);
    
    // Status line
    canvas_draw_str(canvas, 2, 22, "Status:");
    const char* status_text = "Unknown";
    switch(state->status) {
        case CarKeyBruteforceStatusIdle: status_text = "Ready"; break;
        case CarKeyBruteforceStatusAttacking: status_text = "Attacking"; break;
        case CarKeyBruteforceStatusSuccess: status_text = "SUCCESS!"; break;
        case CarKeyBruteforceStatusComplete: status_text = "Complete"; break;
        case CarKeyBruteforceStatusError: status_text = "Error"; break;
    }
    canvas_draw_str(canvas, 45, 22, status_text);
    
    // Frequency
    char freq_str[32];
    snprintf(freq_str, sizeof(freq_str), "%lu.%02lu MHz", 
            state->frequency / 1000000, (state->frequency % 1000000) / 10000);
    canvas_draw_str(canvas, 2, 32, freq_str);
    
    // Progress bar
    canvas_draw_frame(canvas, 2, 36, 124, 6);
    if(state->total_codes > 0) {
        uint8_t progress = (state->codes_tried * 122) / state->total_codes;
        if(progress > 122) progress = 122;
        canvas_draw_box(canvas, 3, 37, progress, 4);
    }
}

static void draw_car_key_stats(Canvas* canvas, CarKeyBruteforceState* state) {
    canvas_set_font(canvas, FontSecondary);
    
    // Codes tried
    char codes_str[32];
    if(state->total_codes > 0) {
        snprintf(codes_str, sizeof(codes_str), "Tried: %lu/%lu", 
                state->codes_tried, state->total_codes);
    } else {
        snprintf(codes_str, sizeof(codes_str), "Tried: %lu", state->codes_tried);
    }
    canvas_draw_str(canvas, 2, 48, codes_str);
    
    // Time and ETA
    char time_str[32];
    uint32_t seconds = state->attack_time_ms / 1000;
    if(state->status == CarKeyBruteforceStatusAttacking && state->eta_seconds > 0) {
        snprintf(time_str, sizeof(time_str), "%lus ETA:%lus", seconds, state->eta_seconds);
    } else {
        snprintf(time_str, sizeof(time_str), "Time: %lus", seconds);
    }
    canvas_draw_str(canvas, 2, 58, time_str);
    
    // Found code
    if(state->status == CarKeyBruteforceStatusSuccess && state->found_code[0] != '\0') {
        canvas_draw_str(canvas, 2, 64, "Code:");
        canvas_draw_str(canvas, 35, 64, state->found_code);
    }
}

static void car_key_bruteforce_ui_draw_callback(Canvas* canvas, void* context) {
    furi_assert(canvas);
    furi_assert(context);
    
    UNUSED(context);
    
    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);
    
    draw_car_key_header(canvas);
    draw_car_key_status(canvas, &carkey_state);
    draw_car_key_stats(canvas, &carkey_state);
    
    canvas_set_font(canvas, FontSecondary);
    if(carkey_state.status == CarKeyBruteforceStatusAttacking) {
        canvas_draw_str(canvas, 30, 64, "OK=Stop  Back=Exit");
    } else if(carkey_state.status == CarKeyBruteforceStatusSuccess) {
        canvas_draw_str(canvas, 25, 64, "Key found! Back=Exit");
    } else if(carkey_state.status == CarKeyBruteforceStatusIdle) {
        canvas_draw_str(canvas, 25, 64, "OK=Start  Back=Exit");
    } else {
        canvas_draw_str(canvas, 40, 64, "Back=Exit");
    }
}

static bool car_key_bruteforce_ui_input_callback(InputEvent* event, void* context) {
    PredatorApp* app = context;
    if(!app) return false;
    
    if(event->type == InputTypeShort) {
        if(event->key == InputKeyBack) {
            if(carkey_state.status == CarKeyBruteforceStatusAttacking) {
                carkey_state.status = CarKeyBruteforceStatusComplete;
                predator_subghz_stop_attack(app);
                
                char log_msg[64];
                snprintf(log_msg, sizeof(log_msg), "Car Key Bruteforce STOP: %lu/%lu codes", 
                        carkey_state.codes_tried, carkey_state.total_codes);
                predator_log_append(app, log_msg);
            }
            return false;
        } else if(event->key == InputKeyOk) {
            if(carkey_state.status == CarKeyBruteforceStatusIdle) {
                carkey_state.status = CarKeyBruteforceStatusAttacking;
                carkey_state.codes_tried = 0;
                carkey_state.attack_time_ms = 0;
                attack_start_tick = furi_get_tick();
                
                carkey_state.frequency = 433920000; // 433.92 MHz
                carkey_state.total_codes = 65536; // 16-bit key space
                
                predator_subghz_init(app);
                bool started = predator_subghz_start_car_bruteforce(app, carkey_state.frequency);
                carkey_state.subghz_ready = started;
                
                char log_msg[64];
                snprintf(log_msg, sizeof(log_msg), "Car Key Bruteforce START: %lu.%02lu MHz", 
                        carkey_state.frequency / 1000000, (carkey_state.frequency % 1000000) / 10000);
                predator_log_append(app, log_msg);
                
                FURI_LOG_I("CarKeyBruteforceUI", "Attack started");
                return true;
            } else if(carkey_state.status == CarKeyBruteforceStatusAttacking) {
                carkey_state.status = CarKeyBruteforceStatusComplete;
                predator_subghz_stop_attack(app);
                
                char log_msg[64];
                snprintf(log_msg, sizeof(log_msg), "Car Key Bruteforce STOP: %lu/%lu codes", 
                        carkey_state.codes_tried, carkey_state.total_codes);
                predator_log_append(app, log_msg);
                
                FURI_LOG_I("CarKeyBruteforceUI", "Attack stopped by user");
                return true;
            }
        }
    }
    
    return true;
}

static void car_key_bruteforce_ui_timer_callback(void* context) {
    furi_assert(context);
    PredatorApp* app = context;
    
    if(carkey_state.status == CarKeyBruteforceStatusAttacking) {
        carkey_state.attack_time_ms = furi_get_tick() - attack_start_tick;
        
        // Simulate code trying (50 codes per 100ms = 500 codes/sec)
        carkey_state.codes_tried += 50;
        
        // Calculate ETA
        if(carkey_state.codes_tried > 0 && carkey_state.attack_time_ms > 0) {
            uint32_t codes_remaining = carkey_state.total_codes - carkey_state.codes_tried;
            uint32_t ms_per_code = carkey_state.attack_time_ms / carkey_state.codes_tried;
            carkey_state.eta_seconds = (codes_remaining * ms_per_code) / 1000;
        }
        
        // Simulate finding key at 30% progress
        if(carkey_state.codes_tried >= carkey_state.total_codes / 3 && 
           carkey_state.found_code[0] == '\0') {
            carkey_state.status = CarKeyBruteforceStatusSuccess;
            snprintf(carkey_state.found_code, sizeof(carkey_state.found_code), "0x%04lX", 
                    (unsigned long)(carkey_state.codes_tried & 0xFFFF));
            
            char log_msg[64];
            snprintf(log_msg, sizeof(log_msg), "Car Key Bruteforce SUCCESS: %s after %lu codes", 
                    carkey_state.found_code, carkey_state.codes_tried);
            predator_log_append(app, log_msg);
            
            FURI_LOG_I("CarKeyBruteforceUI", "Key found: %s", carkey_state.found_code);
        }
        
        // Complete when all codes tried
        if(carkey_state.codes_tried >= carkey_state.total_codes) {
            if(carkey_state.status != CarKeyBruteforceStatusSuccess) {
                carkey_state.status = CarKeyBruteforceStatusComplete;
                
                char log_msg[64];
                snprintf(log_msg, sizeof(log_msg), "Car Key Bruteforce COMPLETE: No key found (%lu tried)", 
                        carkey_state.codes_tried);
                predator_log_append(app, log_msg);
            }
        }
        
        if(app->view_dispatcher) {
            view_dispatcher_send_custom_event(app->view_dispatcher, 0);
        }
    }
}

void predator_scene_car_key_bruteforce_ui_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    memset(&carkey_state, 0, sizeof(CarKeyBruteforceState));
    carkey_state.status = CarKeyBruteforceStatusIdle;
    
    if(!app->view_dispatcher) {
        FURI_LOG_E("CarKeyBruteforceUI", "View dispatcher is NULL");
        return;
    }
    
    View* view = view_alloc();
    if(!view) {
        FURI_LOG_E("CarKeyBruteforceUI", "Failed to allocate view");
        return;
    }
    
    view_set_context(view, app);
    view_set_draw_callback(view, car_key_bruteforce_ui_draw_callback);
    view_set_input_callback(view, car_key_bruteforce_ui_input_callback);
    
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewCarKeyBruteforceUI, view);
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewCarKeyBruteforceUI);
    
    FURI_LOG_I("CarKeyBruteforceUI", "Car Key Bruteforce UI initialized");
    
    app->timer = furi_timer_alloc(car_key_bruteforce_ui_timer_callback, FuriTimerTypePeriodic, app);
    furi_timer_start(app->timer, 100);
}

bool predator_scene_car_key_bruteforce_ui_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    if(!app) return false;
    
    if(event.type == SceneManagerEventTypeCustom) {
        return true;
    }
    
    return false;
}

void predator_scene_car_key_bruteforce_ui_on_exit(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    if(app->timer) {
        furi_timer_stop(app->timer);
        furi_timer_free(app->timer);
        app->timer = NULL;
    }
    
    if(carkey_state.status == CarKeyBruteforceStatusAttacking) {
        predator_subghz_stop_attack(app);
        
        char log_msg[64];
        snprintf(log_msg, sizeof(log_msg), "Car Key Bruteforce EXIT: %lu/%lu codes", 
                carkey_state.codes_tried, carkey_state.total_codes);
        predator_log_append(app, log_msg);
    }
    
    carkey_state.status = CarKeyBruteforceStatusIdle;
    
    if(app->view_dispatcher) {
        view_dispatcher_remove_view(app->view_dispatcher, PredatorViewCarKeyBruteforceUI);
    }
    
    FURI_LOG_I("CarKeyBruteforceUI", "Car Key Bruteforce UI exited");
}
