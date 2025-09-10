#include "../predator_i.h"\n#include "../helpers/predator_view_helpers.h"
#include "../helpers/predator_subghz.h"
#include "../helpers/predator_ui_elements.h"
#include "predator_scene.h"

typedef struct {
    View* view;
    uint32_t cars_attempted;
    uint8_t animation_frame;
    bool low_power_mode;
    uint32_t screen_idle_time;
} PassiveOpenerView;

// Screen timeout and power management
#define SCREEN_TIMEOUT_MS 10000 // Turn off screen after 10 seconds
#define LOW_POWER_INTERVAL 5 // Only transmit every 5 ticks in low power mode

static void passive_opener_view_draw_callback(Canvas* canvas, void* context) {
    PredatorApp* app = context;
    
    if(!app) return;
    
    uint32_t cars_attempted = app->packets_sent;
    uint8_t animation_frame = (app->packets_sent / 5) % 4;
    
    // Extract state information
    PassiveOpenerView* state = PREDATOR_GET_MODEL(app->view_dispatcher, PassiveOpenerView);
    bool low_power_mode = state ? state->low_power_mode : false;
    
    canvas_clear(canvas);
    
    // Draw title
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, 64, 8, AlignCenter, AlignCenter, "Passive Car Opener");
    
    // Draw separator
    canvas_draw_line(canvas, 0, 16, 128, 16);
    
    if(low_power_mode) {
        // Low power mode display
        predator_ui_draw_status_box(canvas, "POWER SAVE", 12, 24, 104, 30);
        
        canvas_set_font(canvas, FontSecondary);
        char cars_text[16];
        snprintf(cars_text, sizeof(cars_text), "Cars: %lu", cars_attempted);
        canvas_draw_str_aligned(canvas, 64, 35, AlignCenter, AlignCenter, cars_text);
        
        // Determine active frequency based on animation frame
        const char* freq_text = animation_frame == 0 ? "433.9MHz" : 
                              (animation_frame == 1 ? "315MHz" : "868MHz");
        canvas_draw_str_aligned(canvas, 64, 45, AlignCenter, AlignCenter, freq_text);
        
        // Wake instruction
        canvas_draw_str_aligned(canvas, 64, 62, AlignCenter, AlignBottom, "Press button to wake");
    } else {
        // Normal mode display
        
        // Draw car icon with signal animation
        // Simple car outline
        canvas_draw_line(canvas, 35, 30, 93, 30); // Top
        canvas_draw_line(canvas, 30, 30, 30, 45); // Left front
        canvas_draw_line(canvas, 30, 45, 40, 45); // Bottom front
        canvas_draw_line(canvas, 98, 30, 98, 45); // Right back
        canvas_draw_line(canvas, 88, 45, 98, 45); // Bottom back
        
        // Car body
        canvas_draw_line(canvas, 40, 45, 42, 50); // Front wheel well
        canvas_draw_line(canvas, 42, 50, 86, 50); // Bottom
        canvas_draw_line(canvas, 86, 50, 88, 45); // Rear wheel well
        
        // Wheels
        canvas_draw_circle(canvas, 50, 50, 5);
        canvas_draw_circle(canvas, 78, 50, 5);
        
        // Draw signal waves emanating from the car based on animation frame
        for(uint8_t i = 0; i <= animation_frame; i++) {
            canvas_draw_circle(canvas, 64, 30, 20 + (i * 5));
        }
        
        // Status information
        predator_ui_draw_status_box(canvas, "Status", 12, 56, 104, 24);
        
        canvas_set_font(canvas, FontSecondary);
        canvas_draw_str(canvas, 18, 66, "Mode:");
        canvas_draw_str(canvas, 72, 66, "COVERT");
        
        char cars_text[32];
        snprintf(cars_text, sizeof(cars_text), "Cars: %lu", cars_attempted);
        canvas_draw_str(canvas, 18, 76, cars_text);
        
        // Determine active frequency based on animation frame
        const char* freq_text = animation_frame == 0 ? "433.9MHz" : 
                              (animation_frame == 1 ? "315MHz" : "868MHz");
        canvas_draw_str(canvas, 72, 76, freq_text);
    }
}

static bool passive_opener_view_input_callback(InputEvent* event, void* context) {
    PredatorApp* app = context;
    bool consumed = false;
    
    PassiveOpenerView* state = PREDATOR_GET_MODEL(app->view_dispatcher, PassiveOpenerView);
    
    if(event->type == InputTypeShort || event->type == InputTypeRepeat) {
        switch(event->key) {
        case InputKeyBack:
            // Special handling if in low power mode - wake up first
            if(state && state->low_power_mode) {
                state->low_power_mode = false;
                state->screen_idle_time = 0;
                notification_message(app->notifications, &sequence_display_backlight_on);
                consumed = true;
            } else {
                // Normal back button handling
                scene_manager_previous_scene(app->scene_manager);
                consumed = true;
            }
            break;
        default:
            break;
        }
    }
    
    return consumed;
}

static View* passive_opener_view_alloc(PredatorApp* app) {
    View* view = view_alloc();
    view_set_context(view, app);
    view_set_draw_callback(view, passive_opener_view_draw_callback);
    view_set_input_callback(view, passive_opener_view_input_callback);
    
    // Allocate and initialize model
    PassiveOpenerView* state = malloc(sizeof(PassiveOpenerView));
    state->cars_attempted = 0;
    state->animation_frame = 0;
    state->low_power_mode = false;
    state->screen_idle_time = 0;
    
    predator_view_set_model(view, state);
    predator_view_set_model_free_callback(view, free);
    
    return view;
}

static void passive_opener_view_free(View* view) {
    view_free(view);
}

void predator_scene_car_passive_opener_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    // Create custom view
    View* view = passive_opener_view_alloc(app);
    
    // Replace popup view with custom view
    view_dispatcher_remove_view(app->view_dispatcher, PredatorViewPopup);
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewPopup, view);
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
    
    // Start passive car opener mode with power optimization
    predator_subghz_init(app);
    
    // Add error handling for SubGHz initialization
    if(!app->subghz_txrx) {
        // Notify user if hardware initialization failed
        PassiveOpenerView* state = view_get_model(view);
        state->low_power_mode = true; // Force low power to display error
        return;
    }
    
    predator_subghz_start_passive_car_opener(app);
    app->attack_running = true;
    app->packets_sent = 0;
    
    // Set notification pattern to confirm activation
    notification_message(app->notifications, &sequence_success);
}

bool predator_scene_car_passive_opener_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    // Get view and state
    View* view = predator_view_dispatcher_get_current_view(app->view_dispatcher);
    PassiveOpenerView* state = view_get_model(view);
    
    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == PredatorCustomEventPopupBack) {
            consumed = true;
            scene_manager_previous_scene(app->scene_manager);
        }
    } else if(event.type == SceneManagerEventTypeBack) {
        // Wake up on back button if in low power mode
        if(state && state->low_power_mode) {
            state->low_power_mode = false;
            state->screen_idle_time = 0;
            notification_message(app->notifications, &sequence_display_backlight_on);
            consumed = true;
        }
    } else if(event.type == SceneManagerEventTypeTick) {
        if(app->attack_running) {
            // Cycle through common car frequencies for better success rate
            static uint8_t freq_index = 0;
            
            static uint32_t counter = 0;
            // Cycle frequency every 50 ticks (5 seconds)
            if(counter++ % 50 == 0) {
                // Change frequency for wider coverage
                predator_subghz_stop_passive_car_opener(app);
                predator_subghz_start_passive_car_opener(app);
                freq_index = (freq_index + 1) % 3;
                
                // Increment attempted car count on frequency change
                app->packets_sent++;
            }
            
            if(state) {
                // Screen power management - only increment in normal mode
                if(!state->low_power_mode) {
                    state->screen_idle_time += 100; // Tick is approximately 100ms
                    
                    // Enter low power mode after timeout
                    if(state->screen_idle_time >= SCREEN_TIMEOUT_MS) {
                        state->low_power_mode = true;
                        
                        // Dim backlight significantly to save power
                        notification_message(app->notifications, &sequence_display_backlight_off);
                    }
                }
                
                // In low power mode, only process every few ticks
                if(!state->low_power_mode || (counter % LOW_POWER_INTERVAL == 0)) {
                    // Process passive car opener tick with error handling
                    predator_subghz_passive_car_opener_tick(app);
                }
                
                // Update animation frame
                state->animation_frame = (counter / 5) % 4;
                state->cars_attempted = app->packets_sent;
            }
            
            // Force view redraw for animations
            view_dispatcher_send_custom_event(app->view_dispatcher, 0xFF);
            consumed = true;
        }
    }
    
    return consumed;
}

void predator_scene_car_passive_opener_new_on_exit(void* context) {
    PredatorApp* app = context;
    app->attack_running = false;
    
    // Get view and state
    View* view = predator_view_dispatcher_get_current_view(app->view_dispatcher);
    PassiveOpenerView* state = view_get_model(view);
    
    // Restore normal display brightness if we were in low power
    if(state && state->low_power_mode) {
        notification_message(app->notifications, &sequence_display_backlight_on);
    }
    
    // Stop passive car opener mode
    predator_subghz_stop_passive_car_opener(app);
    predator_subghz_deinit(app);
    
    // Play exit tone
    notification_message(app->notifications, &sequence_blink_stop);
    
    // Remove and free custom view
    view_dispatcher_remove_view(app->view_dispatcher, PredatorViewPopup);
    passive_opener_view_free(view);
    
    // Restore standard popup view
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewPopup, popup_get_view(app->popup));
}


