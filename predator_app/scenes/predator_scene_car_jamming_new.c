#include "../predator_i.h"
#include "../helpers/predator_view_helpers.h"
#include "../helpers/predator_subghz.h"
#include "../helpers/predator_ui_elements.h"

typedef struct {
    View* view;
    uint32_t jamming_time;  // Time in seconds
    uint8_t animation_frame;
    uint8_t strength;       // 0-3 strength levels
    uint32_t frequency;     // Current jamming frequency
    bool hardware_ready;
} CarJammingView;

static void car_jamming_view_draw_callback(Canvas* canvas, void* context) {
    PredatorApp* app = context;
    
    if(!app) return;
    
    // Get view state
    CarJammingView* state = PREDATOR_GET_MODEL(app->view_dispatcher, CarJammingView);
    if(!state) return;
    
    // Update animation frame
    uint8_t animation_frame = (furi_get_tick() / 200) % 4;
    state->animation_frame = animation_frame;
    
    // Update jamming time (in seconds)
    state->jamming_time = app->packets_sent / 10;
    
    // Update jamming strength (cycles through 0-3)
    state->strength = (state->jamming_time / 5) % 4;
    
    // Get hardware state
    state->hardware_ready = (app->subghz_txrx != NULL);
    
    canvas_clear(canvas);
    
    // Draw title
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, 64, 8, AlignCenter, AlignCenter, "Signal Jamming");
    
    // Draw separator
    canvas_draw_line(canvas, 0, 16, 128, 16);
    
    if(!state->hardware_ready) {
        // Hardware error display
        predator_ui_draw_status_box(canvas, "Hardware Error", 10, 25, 108, 30);
        
        canvas_set_font(canvas, FontSecondary);
        canvas_draw_str_aligned(canvas, 64, 35, AlignCenter, AlignCenter, "SubGHz not available");
        canvas_draw_str_aligned(canvas, 64, 45, AlignCenter, AlignCenter, "Check hardware");
    } else {
        // Jamming visualization
        uint8_t jammer_x = 30;
        uint8_t jammer_y = 35;
        
        // Draw jammer device
        canvas_draw_box(canvas, jammer_x - 10, jammer_y - 5, 20, 10);
        
        // Draw antenna
        canvas_draw_line(canvas, jammer_x, jammer_y - 5, jammer_x, jammer_y - 15);
        
        // Draw jamming waves based on strength and animation
        for(uint8_t i = 0; i <= state->strength; i++) {
            uint8_t wave_size = 4 + (i * 4);
            
            // Only draw waves on certain animation frames for pulsing effect
            if(i <= animation_frame) {
                canvas_draw_circle(canvas, jammer_x, jammer_y - 15, wave_size);
            }
        }
        
        // Target car with key fob
        uint8_t car_x = 85;
        uint8_t car_y = 35;
        
        // Simple car outline
        canvas_draw_line(canvas, car_x - 15, car_y - 5, car_x + 15, car_y - 5); // Top
        canvas_draw_line(canvas, car_x - 15, car_y - 5, car_x - 15, car_y + 5); // Left
        canvas_draw_line(canvas, car_x + 15, car_y - 5, car_x + 15, car_y + 5); // Right
        canvas_draw_line(canvas, car_x - 15, car_y + 5, car_x - 10, car_y + 5); // Bottom left
        canvas_draw_line(canvas, car_x + 10, car_y + 5, car_x + 15, car_y + 5); // Bottom right
        
        // Car wheels
        canvas_draw_circle(canvas, car_x - 10, car_y + 5, 3);
        canvas_draw_circle(canvas, car_x + 10, car_y + 5, 3);
        
        // Key fob to the left of car (being jammed)
        uint8_t key_x = car_x - 25;
        uint8_t key_y = car_y;
        canvas_draw_box(canvas, key_x - 5, key_y - 5, 10, 10);
        
        // Draw key signal being jammed with X marks
        if(animation_frame > 0) {
            uint8_t x_pos = key_x + 8 + (animation_frame * 2);
            uint8_t y_pos = key_y;
            
            // Jamming X mark
            canvas_draw_line(canvas, x_pos - 3, y_pos - 3, x_pos + 3, y_pos + 3);
            canvas_draw_line(canvas, x_pos - 3, y_pos + 3, x_pos + 3, y_pos - 3);
        }
        
        // Status information
        predator_ui_draw_status_box(canvas, "Jamming Status", 10, 50, 108, 28);
        
        canvas_set_font(canvas, FontSecondary);
        
        // Frequency
        char freq_text[24];
        snprintf(freq_text, sizeof(freq_text), "Freq: 433.92MHz");
        canvas_draw_str(canvas, 16, 60, freq_text);
        
        // Time counter
        char time_text[24];
        snprintf(time_text, sizeof(time_text), "Time: %lus", state->jamming_time);
        canvas_draw_str(canvas, 16, 70, time_text);
        
        // Strength indicator
        canvas_draw_str(canvas, 80, 60, "Power:");
        
        // Draw power level bars
        for(uint8_t i = 0; i <= 3; i++) {
            uint8_t bar_height = 2 + (i * 2);
            uint8_t bar_x = 80 + (i * 5);
            uint8_t bar_y = 70;
            
            if(i <= state->strength) {
                canvas_draw_box(canvas, bar_x, bar_y - bar_height, 3, bar_height);
            } else {
                canvas_draw_frame(canvas, bar_x, bar_y - bar_height, 3, bar_height);
            }
        }
    }
    
    // Draw control hints
    canvas_set_font(canvas, FontSecondary);
    elements_button_left(canvas, "Back");
}

static bool car_jamming_view_input_callback(InputEvent* event, void* context) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(event->type == InputTypeShort) {
        switch(event->key) {
        case InputKeyBack:
            scene_manager_previous_scene(app->scene_manager);
            consumed = true;
            break;
        default:
            break;
        }
    }
    
    return consumed;
}

static View* car_jamming_view_alloc(PredatorApp* app) {
    View* view = view_alloc();
    view_set_context(view, app);
    view_set_draw_callback(view, car_jamming_view_draw_callback);
    view_set_input_callback(view, car_jamming_view_input_callback);
    
    // Initialize model
    CarJammingView* state = malloc(sizeof(CarJammingView));
    state->jamming_time = 0;
    state->animation_frame = 0;
    state->strength = 0;
    state->frequency = 433920000;
    state->hardware_ready = false;
    
    predator_view_set_model(view, state);
    predator_view_set_model_free_callback(view, free);
    
    return view;
}

static void car_jamming_view_free(View* view) {
    view_free(view);
}

void predator_scene_car_jamming_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    // Initialize SubGHz hardware
    predator_subghz_init(app);
    
    // Create custom view
    View* view = car_jamming_view_alloc(app);
    
    // Replace popup view with custom view
    view_dispatcher_remove_view(app->view_dispatcher, PredatorViewPopup);
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewPopup, view);
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
    
    // Start jamming if hardware is ready
    if(app->subghz_txrx) {
        predator_subghz_start_jamming(app, 433920000);
        
        CarJammingView* state = view_get_model(view);
        if(state) {
            state->hardware_ready = true;
        }
    }
    
    app->attack_running = true;
    app->packets_sent = 0;
}

bool predator_scene_car_jamming_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(event.type == SceneManagerEventTypeBack) {
        consumed = true;
        app->attack_running = false;
        predator_subghz_deinit(app);
        scene_manager_previous_scene(app->scene_manager);
    } else if(event.type == SceneManagerEventTypeTick) {
        if(app->attack_running) {
            app->packets_sent++;
            
            // Force view refresh for animations
            view_dispatcher_send_custom_event(app->view_dispatcher, 0xFF);
            consumed = true;
        }
    }
    
    return consumed;
}

void predator_scene_car_jamming_new_on_exit(void* context) {
    PredatorApp* app = context;
    app->attack_running = false;
    
    // Clean up
    predator_subghz_deinit(app);
    
    // Remove and free custom view
    view_dispatcher_remove_view(app->view_dispatcher, PredatorViewPopup);
    View* view = predator_view_dispatcher_get_current_view(app->view_dispatcher);
    if(view) {
        car_jamming_view_free(view);
    }
    
    // Restore standard popup view
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewPopup, popup_get_view(app->popup));
}


