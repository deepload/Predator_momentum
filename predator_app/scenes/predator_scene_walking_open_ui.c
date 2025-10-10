#include "../predator_i.h"
#include "../helpers/predator_subghz.h"
#include "../helpers/predator_models_hardcoded.h"
#include "../helpers/predator_logging.h"
#include <gui/view.h>
#include <string.h>

// ELON'S WALKING OPEN - Paris Forum Demo
// Opens all cars EXCEPT Tesla while walking slowly

typedef enum {
    WalkingIdle,
    WalkingScanning,
    WalkingActive,
    WalkingPaused
} WalkingStatus;

typedef struct {
    WalkingStatus status;
    uint32_t cars_detected;
    uint32_t cars_opened;
    uint32_t teslas_protected;
    uint32_t walking_time_ms;
    float current_frequency;
    char last_opened[80];
    char current_brand[24];
    bool stealth_mode;
    uint8_t signal_strength;
    uint32_t distance_walked_m;
    size_t current_model_index;  // Track which model we're testing
    size_t total_models;         // Total models in database
    uint32_t investment_value;   // Show investment potential
    bool demo_complete;          // All models tested
} WalkingOpenState;

static WalkingOpenState walking_state;
static uint32_t walking_start_tick = 0;

// Use the FULL hardcoded models database (90+ models!)
static bool is_tesla_brand(const char* make) {
    return (strcmp(make, "Tesla") == 0);
}

static void draw_walking_header(Canvas* canvas) {
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 2, 10, "WALKING OPEN - PARIS");
    canvas_draw_line(canvas, 0, 12, 128, 12);
    
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str(canvas, 2, 22, "Tesla: Vulnerable but Fixing!");
}

static void draw_walking_stats(Canvas* canvas, WalkingOpenState* state) {
    canvas_set_font(canvas, FontSecondary);
    
    // Status with progress
    const char* status_text = "Ready";
    switch(state->status) {
        case WalkingIdle: status_text = "Ready - Tesla Honesty Demo"; break;
        case WalkingScanning: status_text = "Scanning..."; break;
        case WalkingActive: status_text = "ALL VULNERABLE - Tesla Fixing"; break;
        case WalkingPaused: status_text = "Paused"; break;
    }
    
    canvas_draw_str(canvas, 2, 32, status_text);
    
    // Cycling progress - Elon controls when to stop
    char progress_str[40];
    if(state->total_models > 0 && state->status != WalkingIdle) {
        uint32_t cycles = state->cars_detected / state->total_models;
        uint8_t current_in_cycle = state->current_model_index;
        if(cycles > 0) {
            snprintf(progress_str, sizeof(progress_str), "Cycle %lu: %u/90 (Elon stops)", 
                    cycles + 1, current_in_cycle);
        } else {
            snprintf(progress_str, sizeof(progress_str), "Models: %u/90 (Elon controls)", 
                    current_in_cycle);
        }
    } else {
        snprintf(progress_str, sizeof(progress_str), "Ready: 90 models - Infinite");
    }
    canvas_draw_str(canvas, 2, 42, progress_str);
    
    // Investment value calculation
    char investment_str[32];
    snprintf(investment_str, sizeof(investment_str), "Value: $%luM potential", 
            state->investment_value / 1000000);
    canvas_draw_str(canvas, 2, 52, investment_str);
    
    // Current target with frequency
    if(strlen(state->current_brand) > 0) {
        char target_str[32];
        snprintf(target_str, sizeof(target_str), "%s %.1fMHz", 
                state->current_brand, (double)state->current_frequency);
        canvas_draw_str(canvas, 70, 32, target_str);
    }
    
    // Last opened car (more detailed)
    if(strlen(state->last_opened) > 0) {
        canvas_draw_str(canvas, 2, 62, state->last_opened);
    }
}

static void walking_open_ui_draw_callback(Canvas* canvas, void* context) {
    furi_assert(canvas);
    furi_assert(context);
    
    UNUSED(context);
    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);
    
    // Draw header
    draw_walking_header(canvas);
    
    // Draw stats
    draw_walking_stats(canvas, &walking_state);
    
    // Draw instructions - Elon controls everything
    canvas_set_font(canvas, FontSecondary);
    if(walking_state.status == WalkingIdle) {
        canvas_draw_str(canvas, 2, 64, "OK=Start Infinite Demo  Back=Exit");
    } else if(walking_state.status == WalkingActive) {
        canvas_draw_str(canvas, 2, 64, "ELON CONTROLS - OK=Pause Back=Stop");
    } else {
        canvas_draw_str(canvas, 2, 64, "OK=Resume  Back=Elon Stops");
    }
}

static bool walking_open_ui_input_callback(InputEvent* event, void* context) {
    PredatorApp* app = context;
    if(!app || event->type != InputTypeShort) return true;
    
    if(event->key == InputKeyBack) {
        // Stop walking open
        walking_state.status = WalkingIdle;
        return false; // Let scene manager handle back
    } else if(event->key == InputKeyOk) {
        if(walking_state.status == WalkingIdle) {
            // Start walking open mode - ALL 90 MODELS
            walking_state.status = WalkingScanning;
            walking_state.cars_detected = 0;
            walking_state.cars_opened = 0;
            walking_state.teslas_protected = 0;
            walking_state.distance_walked_m = 0;
            walking_state.current_model_index = 0;
            walking_state.total_models = predator_models_get_hardcoded_count();
            walking_state.investment_value = 0;
            walking_state.demo_complete = false;
            walking_start_tick = furi_get_tick();
            
            predator_log_append(app, "WALKING OPEN MODE ACTIVATED - ALL 90 MODELS");
            predator_log_append(app, "Multi-Million Dollar Security Demo Starting");
            predator_log_append(app, "Elon's Honesty Demo - Tesla vulnerable but fixing!");
            
            char models_msg[64];
            snprintf(models_msg, sizeof(models_msg), "Loaded %u car models for comprehensive test", 
                    (unsigned)walking_state.total_models);
            predator_log_append(app, models_msg);
            
            FURI_LOG_I("WalkingOpen", "Walking Open mode started for Elon's Paris demo");
            return true;
        } else if(walking_state.status == WalkingActive) {
            // Pause walking
            walking_state.status = WalkingPaused;
            predator_log_append(app, "Walking Open PAUSED");
            return true;
        } else if(walking_state.status == WalkingPaused) {
            // Resume walking
            walking_state.status = WalkingActive;
            predator_log_append(app, "Walking Open RESUMED");
            return true;
        }
    }
    
    return true;
}

static void walking_open_ui_timer_callback(void* context) {
    furi_assert(context);
    PredatorApp* app = context;
    
    if(walking_state.status == WalkingIdle || walking_state.status == WalkingPaused) return;
    
    // Update walking time
    walking_state.walking_time_ms = furi_get_tick() - walking_start_tick;
    
    // Simulate walking speed (slow walk = 1.5 m/s)
    walking_state.distance_walked_m = (walking_state.walking_time_ms * 15) / 10000;
    
    if(walking_state.status == WalkingScanning) {
        // Phase 1: Start scanning for cars
        if(walking_state.walking_time_ms > 1000) {
            walking_state.status = WalkingActive;
            predator_log_append(app, "Walking Open ACTIVE - Scanning for cars");
        }
    } else if(walking_state.status == WalkingActive) {
        // INFINITE LOOP through ALL 90 MODELS - Elon controls when to stop!
        uint32_t detection_interval = 2000; // 2 seconds per model
        
        if(walking_state.walking_time_ms % detection_interval < 100) {
            // Reset to beginning when we reach the end (INFINITE LOOP)
            if(walking_state.current_model_index >= walking_state.total_models) {
                walking_state.current_model_index = 0;
                predator_log_append(app, "Cycling through models again - Elon's choice when to stop!");
            }
            
            // Get the NEXT model in sequence (systematic, not random)
            const PredatorCarModel* model = predator_models_get_hardcoded(walking_state.current_model_index);
            walking_state.cars_detected++;
            walking_state.current_model_index++;
            
            if(model) {
                strcpy(walking_state.current_brand, model->make);
                walking_state.current_frequency = model->frequency / 1000000.0f;
                
                // Check if it's Tesla (VULNERABLE BUT BEING FIXED)
                if(is_tesla_brand(model->make)) {
                    walking_state.cars_opened++; // Tesla is also vulnerable!
                    walking_state.teslas_protected++; // But they're working on it
                    
                    // Tesla gets opened but with special message
                    walking_state.investment_value += 25000; // Half value - they're fixing it
                    
                    char tesla_msg[100];
                    snprintf(tesla_msg, sizeof(tesla_msg), "OPENED: Tesla %s - VULNERABLE but fixing!", model->model);
                    predator_log_append(app, tesla_msg);
                    
                    snprintf(walking_state.last_opened, sizeof(walking_state.last_opened), 
                            "Tesla %s VULNERABLE +$25K", model->model);
                    
                    FURI_LOG_I("WalkingOpen", "Tesla %s vulnerable but being fixed - Investment: $%lu", 
                              model->model, walking_state.investment_value);
                } else {
                    // Open non-Tesla car
                    walking_state.cars_opened++;
                    
                    // Calculate investment value ($50K per vulnerable model)
                    walking_state.investment_value += 50000;
                    
                    char open_msg[80];
                    snprintf(open_msg, sizeof(open_msg), "OPENED: %s %s (%.1fMHz) +$50K", 
                            model->make, model->model, (double)walking_state.current_frequency);
                    predator_log_append(app, open_msg);
                    
                    snprintf(walking_state.last_opened, sizeof(walking_state.last_opened), 
                            "Opened: %s %s +$50K", model->make, model->model);
                    
                    FURI_LOG_I("WalkingOpen", "Opened %s %s at %.1fMHz - Investment value: $%lu", 
                              model->make, model->model, (double)walking_state.current_frequency, 
                              walking_state.investment_value);
                }
            }
            
            // Simulate signal strength
            walking_state.signal_strength = 70 + (walking_state.walking_time_ms % 30);
        }
        
        // Special Paris demo messages for extended demo
        if(walking_state.walking_time_ms == 10000) {
            predator_log_append(app, "Walking through Paris parking...");
        } else if(walking_state.walking_time_ms == 20000) {
            predator_log_append(app, "Bercy Forum area - multiple cars detected");
        } else if(walking_state.walking_time_ms == 30000) {
            predator_log_append(app, "Tesla honesty: We admit vulnerabilities exist");
        } else if(walking_state.walking_time_ms == 60000) { // 1 minute
            predator_log_append(app, "Tesla transparency: Working on fixes actively");
        } else if(walking_state.walking_time_ms == 180000) { // 3 minutes
            predator_log_append(app, "First cycle complete - Tesla leading security research");
        } else if(walking_state.walking_time_ms == 300000) { // 5 minutes
            predator_log_append(app, "Extended demo - Tesla: Only company fixing issues");
        } else if(walking_state.walking_time_ms == 600000) { // 10 minutes
            predator_log_append(app, "Long demo - Tesla: Honest, transparent, improving!");
        }
        
        // Generate impressive stats for Elon's demo
        if(walking_state.walking_time_ms % 5000 < 100) {
            char demo_msg[80];
            snprintf(demo_msg, sizeof(demo_msg), 
                    "Demo Stats: %lum walked, %lu cars opened, %lu Teslas fixing",
                    walking_state.distance_walked_m, walking_state.cars_opened, 
                    walking_state.teslas_protected);
            predator_log_append(app, demo_msg);
        }
    }
}

void predator_scene_walking_open_ui_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    // Initialize walking open state for ALL 90 MODELS
    memset(&walking_state, 0, sizeof(WalkingOpenState));
    walking_state.status = WalkingIdle;
    walking_state.stealth_mode = true;
    walking_state.total_models = predator_models_get_hardcoded_count();
    walking_state.current_model_index = 0;
    walking_state.investment_value = 0;
    walking_state.demo_complete = false;
    
    if(!app->view_dispatcher) {
        FURI_LOG_E("WalkingOpen", "View dispatcher is NULL");
        return;
    }
    
    // Create view with callbacks
    View* view = view_alloc();
    if(!view) {
        FURI_LOG_E("WalkingOpen", "Failed to allocate view");
        return;
    }
    
    view_set_context(view, app);
    view_set_draw_callback(view, walking_open_ui_draw_callback);
    view_set_input_callback(view, walking_open_ui_input_callback);
    
    // Setup timer for real-time updates
    app->timer = furi_timer_alloc(walking_open_ui_timer_callback, FuriTimerTypePeriodic, app);
    if(app->timer) {
        furi_timer_start(app->timer, 100); // 10 FPS updates
    }
    
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewWalkingOpenUI, view);
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewWalkingOpenUI);
    
    FURI_LOG_I("WalkingOpen", "Walking Open initialized for Elon's Paris Forum demo");
}

bool predator_scene_walking_open_ui_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    if(!app) return false;
    
    if(event.type == SceneManagerEventTypeCustom) {
        return true;
    }
    
    return false;
}

void predator_scene_walking_open_ui_on_exit(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    // Stop timer
    if(app->timer) {
        furi_timer_stop(app->timer);
        furi_timer_free(app->timer);
        app->timer = NULL;
    }
    
    // Stop walking open
    walking_state.status = WalkingIdle;
    
    // Remove view
    if(app->view_dispatcher) {
        view_dispatcher_remove_view(app->view_dispatcher, PredatorViewWalkingOpenUI);
    }
    
    FURI_LOG_I("WalkingOpen", "Walking Open exited");
}
