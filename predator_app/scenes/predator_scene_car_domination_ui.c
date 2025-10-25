#include "../predator_i.h"
#include "../helpers/predator_car_domination.h"
#include "../helpers/predator_logging.h"
#include "../helpers/predator_boards.h"
#include "../helpers/predator_ui_elements.h"
#include <gui/view.h>
#include <string.h>

// 🚗 ULTIMATE CAR DOMINATION MODE UI
// Elon's ultimate demonstration of technological superiority

typedef enum {
    CarDominationEventTimer = 0,
} CarDominationEvent;

static CarDominationState* domination_state = NULL;
static View* domination_view = NULL;

static void draw_domination_header(Canvas* canvas) {
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 2, 10, "🚗 CAR DOMINATION");
    canvas_draw_line(canvas, 0, 12, 128, 12);
}

static void draw_domination_status(Canvas* canvas, CarDominationState* state) {
    canvas_set_font(canvas, FontSecondary);
    
    // Status line
    canvas_draw_str(canvas, 2, 22, "Status:");
    const char* status_text = "Unknown";
    switch(state->status) {
        case CarDominationStatusIdle: status_text = "Ready"; break;
        case CarDominationStatusInitializing: status_text = "Init"; break;
        case CarDominationStatusScanning: status_text = "Scanning"; break;
        case CarDominationStatusAttacking: status_text = "ATTACKING"; break;
        case CarDominationStatusComplete: status_text = "COMPLETE"; break;
    }
    canvas_draw_str(canvas, 45, 22, status_text);
    
    // Current target
    if(state->current_manufacturer[0] != '\0') {
        char target_str[32];
        snprintf(target_str, sizeof(target_str), "Target: %.10s", state->current_manufacturer);
        canvas_draw_str(canvas, 2, 32, target_str);
    }
}

static void draw_domination_progress(Canvas* canvas, CarDominationState* state) {
    canvas_set_font(canvas, FontSecondary);
    
    // Progress
    char progress_str[32];
    snprintf(progress_str, sizeof(progress_str), "Progress: %lu/%lu", 
            state->models_attacked, state->total_models_loaded);
    canvas_draw_str(canvas, 2, 42, progress_str);
    
    // Progress bar
    canvas_draw_frame(canvas, 2, 44, 124, 6);
    if(state->total_models_loaded > 0) {
        uint8_t progress = (state->models_attacked * 122) / state->total_models_loaded;
        if(progress > 122) progress = 122;
        canvas_draw_box(canvas, 3, 45, progress, 4);
    }
    
    // Success/Fail stats
    char success_str[32];
    snprintf(success_str, sizeof(success_str), "Success: %lu", state->successful_attacks);
    canvas_draw_str(canvas, 2, 56, success_str);
    
    char fail_str[32];
    snprintf(fail_str, sizeof(fail_str), "Failed: %lu", state->failed_attacks);
    canvas_draw_str(canvas, 70, 56, fail_str);
    
    // Power indicators
    canvas_draw_str(canvas, 2, 64, "Power:");
    if(state->max_power_enabled) {
        canvas_draw_str(canvas, 35, 64, "MAX");
    }
    if(state->all_frequencies_active) {
        canvas_draw_str(canvas, 60, 64, "ALL_FREQ");
    }
}

static void car_domination_ui_draw_callback(Canvas* canvas, void* context) {
    furi_assert(canvas);
    furi_assert(context);
    
    UNUSED(context);
    
    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);
    
    if(!domination_state) return;
    
    // Draw header
    draw_domination_header(canvas);
    
    // Draw status
    draw_domination_status(canvas, domination_state);
    
    // Draw progress
    draw_domination_progress(canvas, domination_state);
    
    // Draw instructions at bottom
    canvas_set_font(canvas, FontSecondary);
    if(domination_state->status == CarDominationStatusIdle) {
        canvas_draw_str(canvas, 15, 64, "OK=DOMINATE  Back=Exit");
    } else if(domination_state->status == CarDominationStatusComplete) {
        // Calculate and show success rate
        float success_rate = 0.0f;
        if(domination_state->models_attacked > 0) {
            success_rate = ((float)domination_state->successful_attacks / domination_state->models_attacked) * 100.0f;
        }
        char rate_str[32];
        snprintf(rate_str, sizeof(rate_str), "SUCCESS: %.1f%%", (double)success_rate);
        canvas_draw_str(canvas, 30, 64, rate_str);
    } else {
        canvas_draw_str(canvas, 25, 64, "OK=Stop   Back=Exit");
    }
}

static bool car_domination_ui_input_callback(InputEvent* event, void* context) {
    PredatorApp* app = context;
    if(!app) return false;
    
    if(event->type == InputTypeShort) {
        if(event->key == InputKeyBack) {
            // Stop domination if active
            if(domination_state && domination_state->status != CarDominationStatusIdle && 
               domination_state->status != CarDominationStatusComplete) {
                predator_car_domination_stop(app);
                predator_log_append(app, "🚗 Car Domination STOPPED");
            }
            return false; // Let scene manager handle back
        } else if(event->key == InputKeyOk) {
            if(!domination_state) {
                domination_state = predator_car_domination_get_state(app);
            }
            
            if(domination_state->status == CarDominationStatusIdle) {
                // Start car domination
                if(predator_car_domination_activate(app)) {
                    predator_log_append(app, "🚗 CAR DOMINATION MODE ACTIVATED");
                    FURI_LOG_I("CarDominationUI", "Car domination started");
                }
            } else if(domination_state->status != CarDominationStatusComplete) {
                // Stop car domination
                if(predator_car_domination_stop(app)) {
                    predator_log_append(app, "🚗 Car Domination STOPPED");
                    FURI_LOG_I("CarDominationUI", "Car domination stopped");
                }
            }
            return true;
        }
    }
    
    return true;
}

static void car_domination_ui_timer_callback(void* context) {
    PredatorApp* app = context;
    if(!app || !app->view_dispatcher) return;
    
    if(domination_state && (domination_state->status == CarDominationStatusScanning || 
                           domination_state->status == CarDominationStatusAttacking)) {
        // Process next car model
        predator_car_domination_process_next_model(app);
    }
    
    view_dispatcher_send_custom_event(app->view_dispatcher, CarDominationEventTimer);
}

void predator_scene_car_domination_ui_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    // Get domination state
    domination_state = predator_car_domination_get_state(app);
    
    // Setup custom view
    if(!app->view_dispatcher) {
        FURI_LOG_E("CarDominationUI", "View dispatcher is NULL");
        return;
    }
    
    // Create view with callbacks
    domination_view = view_alloc();
    if(!domination_view) {
        FURI_LOG_E("CarDominationUI", "Failed to allocate view");
        return;
    }
    
    view_set_context(domination_view, app);
    view_set_draw_callback(domination_view, car_domination_ui_draw_callback);
    view_set_input_callback(domination_view, car_domination_ui_input_callback);
    
    // Add view to dispatcher
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewCarDominationUI, domination_view);
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewCarDominationUI);
    
    FURI_LOG_I("CarDominationUI", "🚗 Car Domination UI initialized");
    
    // Start timer for updates
    app->timer = furi_timer_alloc(car_domination_ui_timer_callback, FuriTimerTypePeriodic, app);
    furi_timer_start(app->timer, 200); // Update every 200ms for car attacks
}

bool predator_scene_car_domination_ui_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    if(!app) return false;
    
    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == CarDominationEventTimer) {
            // Timer event - view will redraw automatically
            return true;
        }
    }
    
    return false;
}

void predator_scene_car_domination_ui_on_exit(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    // Stop timer
    if(app->timer) {
        furi_timer_stop(app->timer);
        furi_timer_free(app->timer);
        app->timer = NULL;
    }
    
    // Stop domination if active
    if(domination_state && domination_state->status != CarDominationStatusIdle && 
       domination_state->status != CarDominationStatusComplete) {
        predator_car_domination_stop(app);
        predator_log_append(app, "🚗 Car Domination EXIT");
    }
    
    // Remove view
    if(app->view_dispatcher && domination_view) {
        view_dispatcher_remove_view(app->view_dispatcher, PredatorViewCarDominationUI);
        view_free(domination_view);
        domination_view = NULL;
    }
    
    domination_state = NULL;
    
    FURI_LOG_I("CarDominationUI", "🚗 Car Domination UI exited");
}
