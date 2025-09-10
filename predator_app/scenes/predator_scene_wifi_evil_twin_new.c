#include "../predator_i.h"
#include "../helpers/predator_view_helpers.h"
#include "../helpers/predator_esp32.h"
#include "../helpers/predator_ui_elements.h"

typedef struct {
    View* view;
    uint32_t clients_connected;
    bool ap_active;
    uint8_t animation_frame;
    char ssid[32];
} EvilTwinView;

static void evil_twin_view_draw_callback(Canvas* canvas, void* context) {
    PredatorApp* app = context;
    
    if(!app) return;
    
    // Get view state
    EvilTwinView* state = PREDATOR_GET_MODEL(app->view_dispatcher, EvilTwinView);
    if(!state) return;
    
    // Update animation frame
    uint8_t animation_frame = (furi_get_tick() / 200) % 4;
    state->animation_frame = animation_frame;
    
    // Update AP status from app state
    state->ap_active = app->attack_running;
    
    canvas_clear(canvas);
    
    // Draw title
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, 64, 8, AlignCenter, AlignCenter, "Evil Twin AP");
    
    // Draw separator
    canvas_draw_line(canvas, 0, 16, 128, 16);
    
    // Draw WiFi router visualization
    uint8_t router_x = 30;
    uint8_t router_y = 30;
    
    // Router base
    canvas_draw_box(canvas, router_x - 10, router_y - 5, 20, 10);
    
    // Router antennas
    canvas_draw_line(canvas, router_x - 5, router_y - 5, router_x - 5, router_y - 10);
    canvas_draw_line(canvas, router_x + 5, router_y - 5, router_x + 5, router_y - 10);
    
    // Router activity lights
    for(uint8_t i = 0; i < 3; i++) {
        uint8_t light_x = router_x - 6 + (i * 5);
        
        // Animate the lights based on AP status and animation frame
        if(state->ap_active && (i == animation_frame % 3)) {
            canvas_draw_box(canvas, light_x, router_y, 2, 2);
        } else {
            canvas_draw_frame(canvas, light_x, router_y, 2, 2);
        }
    }
    
    // Draw WiFi signal waves when AP is active
    if(state->ap_active) {
        for(uint8_t i = 0; i < animation_frame + 1; i++) {
            uint8_t wave_size = 6 + (i * 5);
            canvas_draw_circle(canvas, router_x, router_y - 5, wave_size);
        }
    }
    
    // Draw client device
    uint8_t device_x = 85;
    uint8_t device_y = 30;
    
    // Phone outline
    canvas_draw_box(canvas, device_x - 5, device_y - 8, 10, 16);
    
    // Phone screen
    canvas_set_color(canvas, ColorWhite);
    canvas_draw_box(canvas, device_x - 3, device_y - 6, 6, 10);
    canvas_set_color(canvas, ColorBlack);
    
    // Connection status indicator
    if(state->ap_active) {
        // Show connecting animation or connected status
        if(state->clients_connected > 0) {
            // Show connected - solid signal bars
            for(uint8_t i = 0; i < 3; i++) {
                uint8_t bar_height = 2 + i;
                canvas_draw_box(canvas, device_x - 2 + (i * 2), device_y + 6, 1, bar_height);
            }
            
            // Show lock icon to indicate captive portal
            canvas_draw_box(canvas, device_x, device_y + 10, 1, 2);
        } else {
            // Show connecting - animated signal bars
            uint8_t active_bar = animation_frame % 3;
            for(uint8_t i = 0; i < 3; i++) {
                uint8_t bar_height = 2 + i;
                if(i == active_bar) {
                    canvas_draw_box(canvas, device_x - 2 + (i * 2), device_y + 6, 1, bar_height);
                } else {
                    canvas_draw_frame(canvas, device_x - 2 + (i * 2), device_y + 6, 1, bar_height);
                }
            }
        }
    }
    
    // Status information
    predator_ui_draw_status_box(canvas, "AP Status", 10, 45, 108, 30);
    
    canvas_set_font(canvas, FontSecondary);
    
    // SSID display
    canvas_draw_str(canvas, 16, 55, "SSID:");
    canvas_draw_str(canvas, 55, 55, state->ssid);
    
    // Client count
    char client_text[24];
    snprintf(client_text, sizeof(client_text), "Clients: %lu", state->clients_connected);
    canvas_draw_str(canvas, 16, 65, client_text);
    
    // AP status text
    canvas_draw_str(canvas, 16, 75, "Status:");
    if(state->ap_active) {
        canvas_draw_str(canvas, 55, 75, "ACTIVE");
    } else {
        canvas_draw_str(canvas, 55, 75, "Starting...");
    }
    
    // Draw controls
    elements_button_left(canvas, "Back");
}

static bool evil_twin_view_input_callback(InputEvent* event, void* context) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(event->type == InputTypeShort) {
        switch(event->key) {
        case InputKeyBack:
        case InputKeyLeft:
            scene_manager_previous_scene(app->scene_manager);
            consumed = true;
            break;
        default:
            break;
        }
    }
    
    return consumed;
}

static View* evil_twin_view_alloc(PredatorApp* app) {
    View* view = view_alloc();
    view_set_context(view, app);
    view_set_draw_callback(view, evil_twin_view_draw_callback);
    view_set_input_callback(view, evil_twin_view_input_callback);
    
    // Initialize model
    EvilTwinView* state = malloc(sizeof(EvilTwinView));
    state->clients_connected = 0;
    state->ap_active = false;
    state->animation_frame = 0;
    strncpy(state->ssid, "Free_WiFi_Login", sizeof(state->ssid));
    
    predator_view_set_model(view, state);
    predator_view_set_model_free_callback(view, free);
    
    return view;
}

static void evil_twin_view_free(View* view) {
    view_free(view);
}

void predator_scene_wifi_evil_twin_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    // Initialize ESP32 hardware
    predator_esp32_init(app);
    
    // Create custom view
    View* view = evil_twin_view_alloc(app);
    
    // Replace popup view with custom view
    view_dispatcher_remove_view(app->view_dispatcher, PredatorViewPopup);
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewPopup, view);
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
    
    // Start attack
    if(app->esp32_uart) {
        // Start evil twin attack
        predator_esp32_wifi_evil_twin(app);
        app->attack_running = true;
    }
}

bool predator_scene_wifi_evil_twin_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == PredatorCustomEventPopupBack) {
            // Return to submenu view
            consumed = true;
            scene_manager_previous_scene(app->scene_manager);
        }
    } else if(event.type == SceneManagerEventTypeBack) {
        consumed = true;
        app->attack_running = false;
        predator_esp32_stop_attack(app);
        scene_manager_previous_scene(app->scene_manager);
    } else if(event.type == SceneManagerEventTypeTick) {
        if(app->attack_running) {
            EvilTwinView* state = PREDATOR_GET_MODEL(app->view_dispatcher, EvilTwinView);
            
            // Simulate clients connecting every ~10 seconds
            if(app->packets_sent % 100 == 0 && app->packets_sent > 0) {
                if(state) {
                    state->clients_connected++;
                }
            }
            
            // Update packets counter for timed events
            app->packets_sent++;
            
            // Force view refresh for animations
            view_dispatcher_send_custom_event(app->view_dispatcher, 0xFF);
            consumed = true;
        }
    }
    
    return consumed;
}

void predator_scene_wifi_evil_twin_new_on_exit(void* context) {
    PredatorApp* app = context;
    app->attack_running = false;
    
    // Stop attack
    predator_esp32_stop_attack(app);
    
    // Remove and free custom view
    view_dispatcher_remove_view(app->view_dispatcher, PredatorViewPopup);
    View* view = predator_view_dispatcher_get_current_view(app->view_dispatcher);
    if(view) {
        evil_twin_view_free(view);
    }
    
    // Restore standard popup view
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewPopup, popup_get_view(app->popup));
}


