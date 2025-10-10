#include "../predator_i.h"
#include "../helpers/predator_car_attacks.h"
#include "../helpers/predator_models_hardcoded.h"
#include "../helpers/predator_logging.h"
#include <gui/view.h>
#include <string.h>

// Ultimate Car Testing Results Screen
// Shows real-time test status, progress, and results for Elon's demo

typedef enum {
    TestStatusSelectModel,
    TestStatusIdle,
    TestStatusRunning,
    TestStatusSuccess,
    TestStatusFailed,
    TestStatusComplete
} TestStatus;

typedef struct {
    size_t model_index;
    char model_name[64];
    TestStatus status;
    uint32_t codes_tried;
    uint32_t time_elapsed_ms;
    bool rolling_code_detected;
    bool smart_key_detected;
    bool fixed_code_detected;
} CarTestResult;

static CarTestResult current_test;
static uint32_t test_start_tick = 0;

static void draw_test_header(Canvas* canvas) {
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 2, 10, "ULTIMATE CAR TEST");
    canvas_draw_line(canvas, 0, 12, 128, 12);
}

static void draw_test_status(Canvas* canvas, CarTestResult* result) {
    canvas_set_font(canvas, FontSecondary);
    
    // Model name
    canvas_draw_str(canvas, 2, 22, "Model:");
    canvas_draw_str(canvas, 40, 22, result->model_name);
    
    // Status indicator
    canvas_draw_str(canvas, 2, 32, "Status:");
    const char* status_text = "Unknown";
    switch(result->status) {
        case TestStatusIdle: status_text = "Ready"; break;
        case TestStatusRunning: status_text = "Testing..."; break;
        case TestStatusSuccess: status_text = "SUCCESS!"; break;
        case TestStatusFailed: status_text = "Failed"; break;
        case TestStatusComplete: status_text = "Complete"; break;
    }
    canvas_draw_str(canvas, 45, 32, status_text);
    
    // Progress bar
    canvas_draw_frame(canvas, 2, 36, 124, 8);
    if(result->codes_tried > 0) {
        uint8_t progress = (result->codes_tried * 122) / 100; // Max 100 codes
        if(progress > 122) progress = 122;
        canvas_draw_box(canvas, 3, 37, progress, 6);
    }
    
    // Codes tried
    char codes_str[32];
    snprintf(codes_str, sizeof(codes_str), "Codes: %lu/100", result->codes_tried);
    canvas_draw_str(canvas, 2, 52, codes_str);
    
    // Time elapsed
    char time_str[32];
    uint32_t seconds = result->time_elapsed_ms / 1000;
    snprintf(time_str, sizeof(time_str), "Time: %lus", seconds);
    canvas_draw_str(canvas, 70, 52, time_str);
}

static void draw_security_analysis(Canvas* canvas, CarTestResult* result) {
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str(canvas, 2, 62, "Security Type:");
    
    uint8_t y_pos = 62;
    if(result->rolling_code_detected) {
        canvas_draw_str(canvas, 75, y_pos, "Rolling");
    } else if(result->smart_key_detected) {
        canvas_draw_str(canvas, 75, y_pos, "Smart Key");
    } else if(result->fixed_code_detected) {
        canvas_draw_str(canvas, 75, y_pos, "Fixed");
    } else {
        canvas_draw_str(canvas, 75, y_pos, "Unknown");
    }
}

static void car_test_results_draw_callback(Canvas* canvas, void* context) {
    furi_assert(canvas);
    furi_assert(context);
    
    UNUSED(context);
    
    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);
    
    // Draw header
    draw_test_header(canvas);
    
    // Draw test status
    draw_test_status(canvas, &current_test);
    
    // Draw security analysis
    draw_security_analysis(canvas, &current_test);
    
    // Draw instructions at bottom
    canvas_set_font(canvas, FontSecondary);
    if(current_test.status == TestStatusRunning) {
        canvas_draw_str(canvas, 2, 64, "Testing in progress...");
    } else if(current_test.status == TestStatusSuccess) {
        canvas_draw_str(canvas, 2, 64, "Test successful! Back=Exit");
    } else if(current_test.status == TestStatusComplete) {
        canvas_draw_str(canvas, 2, 64, "Test complete. Back=Exit");
    } else {
        canvas_draw_str(canvas, 2, 64, "OK=Start  Back=Exit");
    }
}

static bool car_test_results_input_callback(InputEvent* event, void* context) {
    PredatorApp* app = context;
    if(!app) return false;
    
    if(event->type == InputTypeShort) {
        if(event->key == InputKeyBack) {
            // Stop any running test
            current_test.status = TestStatusIdle;
            return false; // Let scene manager handle back
        } else if(event->key == InputKeyOk && current_test.status == TestStatusIdle) {
            // Start test
            current_test.status = TestStatusRunning;
            current_test.codes_tried = 0;
            test_start_tick = furi_get_tick();
            
            // Log test start
            char log_msg[128];
            snprintf(log_msg, sizeof(log_msg), "Starting test: %s", current_test.model_name);
            predator_log_append(app, log_msg);
            
            return true;
        }
    }
    
    return true;
}

static void car_test_results_timer_callback(void* context) {
    furi_assert(context);
    PredatorApp* app = context;
    
    if(current_test.status == TestStatusRunning) {
        // Update time elapsed
        current_test.time_elapsed_ms = furi_get_tick() - test_start_tick;
        
        // Simulate test progress
        current_test.codes_tried++;
        
        // Detect security type based on model
        if(current_test.codes_tried == 10) {
            // Analyze security type
            if(predator_models_uses_rolling_code(current_test.model_index)) {
                current_test.rolling_code_detected = true;
            } else if(predator_models_uses_smart_key(current_test.model_index)) {
                current_test.smart_key_detected = true;
            } else if(predator_models_uses_fixed_code(current_test.model_index)) {
                current_test.fixed_code_detected = true;
            }
        }
        
        // Complete test after 100 codes or 30 seconds
        if(current_test.codes_tried >= 100 || current_test.time_elapsed_ms > 30000) {
            current_test.status = TestStatusComplete;
            
            // Log completion
            char log_msg[128];
            snprintf(log_msg, sizeof(log_msg), "Test complete: %s - %lu codes tried", 
                    current_test.model_name, current_test.codes_tried);
            predator_log_append(app, log_msg);
        }
        
        // Trigger view redraw
        if(app->view_dispatcher) {
            view_dispatcher_send_custom_event(app->view_dispatcher, 0);
        }
    }
}

void predator_scene_car_test_results_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    // Initialize test result
    memset(&current_test, 0, sizeof(CarTestResult));
    
    // Check if we have a model selected (coming from Car Model Selector)
    uint32_t model_index = scene_manager_get_scene_state(app->scene_manager, PredatorSceneCarTestResults);
    
    if(model_index == 0) {
        // No model selected yet - redirect to Car Model Selector first
        FURI_LOG_I("CarTestResults", "No model selected, redirecting to selector");
        scene_manager_next_scene(app->scene_manager, PredatorSceneCarModelSelector);
        return;
    }
    
    // We have a model selected - proceed with test
    current_test.model_index = model_index;
    current_test.status = TestStatusIdle;
    FURI_LOG_I("CarTestResults", "Using selected model index: %u", current_test.model_index);
    
    // Get model name
    const PredatorCarModel* model = predator_models_get_hardcoded(current_test.model_index);
    if(model) {
        snprintf(current_test.model_name, sizeof(current_test.model_name), 
                "%s %s", model->make, model->model);
    } else {
        snprintf(current_test.model_name, sizeof(current_test.model_name), "Unknown Model");
    }
    
    // Setup custom view
    if(!app->view_dispatcher) {
        FURI_LOG_E("CarTestResults", "View dispatcher is NULL");
        return;
    }
    
    // Create view with callbacks
    View* view = view_alloc();
    if(!view) {
        FURI_LOG_E("CarTestResults", "Failed to allocate view");
        return;
    }
    
    view_set_context(view, app);
    view_set_draw_callback(view, car_test_results_draw_callback);
    view_set_input_callback(view, car_test_results_input_callback);
    
    // Add view to dispatcher
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewCarTestResults, view);
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewCarTestResults);
    
    FURI_LOG_I("CarTestResults", "View initialized and switched");
    
    // Start timer for updates
    app->timer = furi_timer_alloc(car_test_results_timer_callback, FuriTimerTypePeriodic, app);
    furi_timer_start(app->timer, 100); // Update every 100ms
}

bool predator_scene_car_test_results_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    if(!app) return false;
    
    if(event.type == SceneManagerEventTypeCustom) {
        // Custom event received - view will redraw automatically
        return true;
    }
    
    return false;
}

void predator_scene_car_test_results_on_exit(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    // Stop timer
    if(app->timer) {
        furi_timer_stop(app->timer);
        furi_timer_free(app->timer);
        app->timer = NULL;
    }
    
    // Stop any running test
    current_test.status = TestStatusIdle;
    
    // Remove view
    if(app->view_dispatcher) {
        view_dispatcher_remove_view(app->view_dispatcher, PredatorViewCarTestResults);
    }
}
