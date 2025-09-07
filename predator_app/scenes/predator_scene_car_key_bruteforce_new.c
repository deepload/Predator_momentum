#include "../predator_i.h"
#include "../helpers/predator_subghz.h"
#include "../helpers/predator_ui_elements.h"

typedef struct {
    View* view;
    uint32_t key_attempts;
    uint32_t current_key;
    uint8_t animation_frame;
    uint8_t progress;
    bool key_found;
    char car_model[32];
    char frequency_str[16];
} CarKeyBruteforceView;

static void car_key_bruteforce_view_draw_callback(Canvas* canvas, void* context) {
    PredatorApp* app = context;
    
    if(!app) return;
    
    // Get view state
    CarKeyBruteforceView* state = view_get_model(app->view_dispatcher->current_view);
    if(!state) return;
    
    // Update animation frame
    uint8_t animation_frame = (furi_get_tick() / 200) % 4;
    state->animation_frame = animation_frame;
    
    // Update state from app
    state->key_attempts = app->packets_sent;
    state->progress = (app->packets_sent % 100);
    
    // Update current key being tried
    if(app->attack_running) {
        state->current_key = 0x10000000 + (app->packets_sent * 573);
    }
    
    // Check if we've "found" a key (for demo purposes)
    if(app->packets_sent > 0 && app->packets_sent % 100 == 0) {
        state->key_found = true;
    }
    
    canvas_clear(canvas);
    
    // Draw title
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, 64, 8, AlignCenter, AlignCenter, "Car Key Bruteforce");
    
    // Draw separator
    canvas_draw_line(canvas, 0, 16, 128, 16);
    
    // Draw car key visualization
    uint8_t key_x = 32;
    uint8_t key_y = 32;
    
    // Key base
    canvas_draw_box(canvas, key_x - 10, key_y - 10, 20, 20);
    
    // Key teeth - animate when trying keys
    for(uint8_t i = 0; i < 4; i++) {
        uint8_t tooth_height = ((state->current_key >> (i * 8)) & 0x0F) % 5 + 2;
        uint8_t tooth_x = key_x - 8 + (i * 5);
        
        // Draw teeth with animation offset when actively bruteforcing
        if(app->attack_running && !state->key_found) {
            tooth_height = (tooth_height + animation_frame) % 5 + 2;
        }
        
        canvas_draw_line(canvas, tooth_x, key_y - 10, tooth_x, key_y - 10 - tooth_height);
    }
    
    // Key transmission animation
    if(app->attack_running) {
        // Draw signal waves based on animation frame
        for(uint8_t i = 0; i < animation_frame; i++) {
            canvas_draw_circle(canvas, key_x + 15, key_y, 5 + (i * 4));
        }
    }
    
    // Draw car outline
    uint8_t car_x = 80;
    uint8_t car_y = 32;
    
    // Simple car outline
    canvas_draw_line(canvas, car_x - 15, car_y - 5, car_x + 15, car_y - 5); // Top
    canvas_draw_line(canvas, car_x - 15, car_y - 5, car_x - 15, car_y + 5); // Left
    canvas_draw_line(canvas, car_x + 15, car_y - 5, car_x + 15, car_y + 5); // Right
    canvas_draw_line(canvas, car_x - 15, car_y + 5, car_x - 10, car_y + 5); // Bottom left
    canvas_draw_line(canvas, car_x + 10, car_y + 5, car_x + 15, car_y + 5); // Bottom right
    
    // Car wheels
    canvas_draw_circle(canvas, car_x - 10, car_y + 5, 3);
    canvas_draw_circle(canvas, car_x + 10, car_y + 5, 3);
    
    // Show unlock animation if key found
    if(state->key_found) {
        // Draw unlocked indicator
        canvas_draw_box(canvas, car_x - 5, car_y - 2, 10, 4);
        
        // Draw door slightly open based on animation
        canvas_draw_line(canvas, car_x + 15, car_y - 5, car_x + 15 + animation_frame, car_y - 5 - animation_frame);
        canvas_draw_line(canvas, car_x + 15 + animation_frame, car_y - 5 - animation_frame, car_x + 15 + animation_frame, car_y + 5 - animation_frame);
        canvas_draw_line(canvas, car_x + 15 + animation_frame, car_y + 5 - animation_frame, car_x + 15, car_y + 5);
    }
    
    // Status information
    predator_ui_draw_status_box(canvas, "Status", 10, 48, 108, 30);
    
    canvas_set_font(canvas, FontSecondary);
    
    // Car model and frequency
    canvas_draw_str(canvas, 16, 58, "Model:");
    canvas_draw_str(canvas, 58, 58, state->key_found ? "Honda Civic" : "Scanning...");
    
    canvas_draw_str(canvas, 16, 68, "Freq:");
    canvas_draw_str(canvas, 58, 68, "433.92 MHz");
    
    // Key counter
    char attempts_text[16];
    snprintf(attempts_text, sizeof(attempts_text), "%lu", state->key_attempts);
    canvas_draw_str(canvas, 16, 78, "Keys:");
    canvas_draw_str(canvas, 58, 78, attempts_text);
    
    // Draw progress bar
    uint8_t progress = state->key_found ? 100 : state->progress;
    predator_ui_draw_progress_bar(canvas, 95, 78, 20, 5, progress, ProgressBarStyleBordered);
}

static bool car_key_bruteforce_view_input_callback(InputEvent* event, void* context) {
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

static View* car_key_bruteforce_view_alloc(PredatorApp* app) {
    View* view = view_alloc();
    view_set_context(view, app);
    view_set_draw_callback(view, car_key_bruteforce_view_draw_callback);
    view_set_input_callback(view, car_key_bruteforce_view_input_callback);
    
    // Initialize model
    CarKeyBruteforceView* state = malloc(sizeof(CarKeyBruteforceView));
    state->key_attempts = 0;
    state->current_key = 0;
    state->animation_frame = 0;
    state->progress = 0;
    state->key_found = false;
    strlcpy(state->car_model, "Scanning...", sizeof(state->car_model));
    strlcpy(state->frequency_str, "433.92 MHz", sizeof(state->frequency_str));
    
    view_set_model(view, state);
    view_set_model_free_callback(view, free);
    
    return view;
}

static void car_key_bruteforce_view_free(View* view) {
    view_free(view);
}

void predator_scene_car_key_bruteforce_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    // Initialize SubGHz hardware
    predator_subghz_init(app);
    
    // Create custom view
    View* view = car_key_bruteforce_view_alloc(app);
    
    // Replace popup view with custom view
    view_dispatcher_remove_view(app->view_dispatcher, PredatorViewPopup);
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewPopup, view);
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
    
    if(app->subghz_txrx) {
        // Start car bruteforce on 433.92 MHz
        predator_subghz_start_car_bruteforce(app, 433920000);
    }
    
    app->attack_running = true;
    app->packets_sent = 0;
}

bool predator_scene_car_key_bruteforce_new_on_event(void* context, SceneManagerEvent event) {
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
            
            // Every 10 ticks, send a car key
            if(app->packets_sent % 10 == 0) {
                uint32_t key_code = 0x10000000 + (app->packets_sent * 573);
                predator_subghz_send_car_key(app, key_code);
            }
            
            // Force view refresh for animations
            view_dispatcher_send_custom_event(app->view_dispatcher, 0xFF);
            consumed = true;
        }
    }
    
    return consumed;
}

void predator_scene_car_key_bruteforce_new_on_exit(void* context) {
    PredatorApp* app = context;
    app->attack_running = false;
    
    // Clean up
    predator_subghz_deinit(app);
    
    // Remove and free custom view
    view_dispatcher_remove_view(app->view_dispatcher, PredatorViewPopup);
    View* view = view_dispatcher_get_current_view(app->view_dispatcher);
    if(view) {
        car_key_bruteforce_view_free(view);
    }
    
    // Restore standard popup view
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewPopup, popup_get_view(app->popup));
}
