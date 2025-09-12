#include "../predator_i.h"
#include "../helpers/predator_view_helpers.h"
#include "../helpers/predator_subghz.h"
#include "../helpers/predator_ui_elements.h"

// Custom view for Tesla charge port opener
typedef struct {
    View* view;
    uint32_t signals_sent;
    uint8_t animation_frame;
} TeslaChargePortView;

static void tesla_charge_port_view_draw_callback(Canvas* canvas, void* context) {
    PredatorApp* app = context;
    
    if(!app) return;
    
    uint32_t signals_sent = app->packets_sent / 20;
    uint8_t animation_frame = (app->packets_sent / 5) % 4;
    
    canvas_clear(canvas);
    
    // Draw title
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, 64, 8, AlignCenter, AlignCenter, "Tesla Charge Port");
    
    // Draw separator
    canvas_draw_line(canvas, 0, 16, 128, 16);
    
    // Draw Tesla car outline with animated charge port
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
    
    // Charge port with animation
    uint8_t port_x = 85;
    uint8_t port_y = 35;
    
    if(signals_sent > 0) {
        // Draw animated charge port opening
        uint8_t port_state = animation_frame;
        
        if(port_state == 0) {
            // Closed
            canvas_draw_line(canvas, port_x, port_y, port_x, port_y + 3);
        } else {
            // Open with different angles
            canvas_draw_line(canvas, port_x, port_y, port_x + port_state, port_y + 3 - port_state);
        }
        
        // Draw charge signal waves when transmitting
        for(uint8_t i = 0; i < animation_frame + 1; i++) {
            canvas_draw_circle(canvas, port_x, port_y, 8 + (i * 3));
        }
    } else {
        // Closed port
        canvas_draw_line(canvas, port_x, port_y, port_x, port_y + 3);
    }
    
    // Draw status box
    predator_ui_draw_status_box(canvas, "Status", 14, 56, 100, 24);
    
    canvas_set_font(canvas, FontSecondary);
    
    // Status information
    canvas_draw_str(canvas, 20, 66, "Frequency:");
    canvas_draw_str(canvas, 72, 66, "315 MHz");
    
    // Draw transmission counter with signals animation
    canvas_draw_str(canvas, 20, 76, "Signals sent:");
    
    char counter_text[16];
    snprintf(counter_text, sizeof(counter_text), "%lu", signals_sent);
    canvas_draw_str(canvas, 72, 76, counter_text);
}

static bool tesla_charge_port_view_input_callback(InputEvent* event, void* context) {
    PredatorApp* app = context;
    bool consumed = false;
    
    // Null safety check
    if(!app || !event) return false;
    
    if(event->type == InputTypeShort) {
        if(event->key == InputKeyBack) {
            if(app->scene_manager) {
                scene_manager_previous_scene(app->scene_manager);
            }
            consumed = true;
        }
    }
    
    return consumed;
}

static View* tesla_charge_port_view_alloc(PredatorApp* app) {
    View* view = view_alloc();
    view_set_context(view, app);
    view_set_draw_callback(view, tesla_charge_port_view_draw_callback);
    view_set_input_callback(view, tesla_charge_port_view_input_callback);
    
    return view;
}


void predator_scene_car_tesla_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    // Comprehensive null safety check
    if(!app || !app->view_dispatcher) {
        return;
    }
    
    // Create custom view
    View* view = tesla_charge_port_view_alloc(app);
    if(!view) {
        return;
    }
    
    // Replace popup view with custom view
    view_dispatcher_remove_view(app->view_dispatcher, PredatorViewPopup);
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewPopup, view);
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
    
    // Initialize SubGHz and hardware
    predator_subghz_init(app);
    
    // Check for hardware errors
    if(!app->subghz_txrx) {
        // Show error message on custom view
        app->has_error = true;
        strncpy(app->error_message, "Failed to initialize SubGHz hardware.", sizeof(app->error_message));
    } else {
        // Start the attack
        app->attack_running = true;
        app->packets_sent = 0;
    }
}

bool predator_scene_car_tesla_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    // Null safety check
    if(!app) return false;
    
    if(event.type == SceneManagerEventTypeBack) {
        // Handle back button press
        consumed = true;
        app->attack_running = false;
        predator_subghz_deinit(app);
        if(app->scene_manager) {
            scene_manager_previous_scene(app->scene_manager);
        }
    } else if(event.type == SceneManagerEventTypeTick) {
        if(app->attack_running) {
            app->packets_sent++;
            
            // Send Tesla charge port signal every 2 seconds
            if(app->packets_sent % 20 == 0) {
                predator_subghz_send_tesla_charge_port(app);
            }
            
            // Force a redraw for animations
            if(app->view_dispatcher) {
                view_dispatcher_send_custom_event(app->view_dispatcher, 0xFF);
            }
            consumed = true;
        }
    }
    
    return consumed;
}

void predator_scene_car_tesla_new_on_exit(void* context) {
    PredatorApp* app = context;
    
    // Null safety check
    if(!app) return;
    
    // Clean up
    app->attack_running = false;
    predator_subghz_deinit(app);
    
    // Remove custom view and restore default popup view
    if(app->view_dispatcher && app->popup) {
        view_dispatcher_remove_view(app->view_dispatcher, PredatorViewPopup);
        view_dispatcher_add_view(app->view_dispatcher, PredatorViewPopup, popup_get_view(app->popup));
    }
}


