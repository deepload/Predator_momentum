#include "../predator_i.h"\n#include "../helpers/predator_view_helpers.h"
#include "../helpers/predator_esp32.h"
#include "../helpers/predator_ui_elements.h"

typedef struct {
    View* view;
    uint8_t selected_mode;
    uint8_t animation_frame;
    uint32_t devices_spoofed;
    bool attack_running;
} BleSpamView;

// BLE Spam Modes
typedef enum {
    BleSpamModeApple,
    BleSpamModeSamsung,
    BleSpamModeRandom,
    BleSpamModeSwiftPair,
    BleSpamModeAirpods,
    BleSpamModeCount
} BleSpamMode;

// BLE Spam mode descriptions
static const char* ble_spam_mode_names[] = {
    "Apple Devices",
    "Samsung Devices",
    "Random MAC",
    "Swift Pair",
    "Airpods"
};

// BLE Spam mode descriptions
static const char* ble_spam_mode_desc[] = {
    "Simulate multiple Apple devices",
    "Simulate Samsung Galaxy devices",
    "Generate random MAC addresses",
    "MS Swift Pair notifications",
    "Airpods connection popups"
};

static void ble_spam_view_draw_callback(Canvas* canvas, void* context) {
    PredatorApp* app = context;
    
    if(!app) return;
    
    // Get view state
    BleSpamView* state = PREDATOR_GET_MODEL(app->view_dispatcher, BleSpamView);
    if(!state) return;
    
    // Update animation frame
    uint8_t animation_frame = (furi_get_tick() / 200) % 4;
    state->animation_frame = animation_frame;
    
    // Update attack status from app
    state->attack_running = app->attack_running;
    state->devices_spoofed = app->packets_sent;
    
    canvas_clear(canvas);
    
    // Draw title
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, 64, 8, AlignCenter, AlignCenter, "BLE Spammer");
    
    // Draw separator
    canvas_draw_line(canvas, 0, 16, 128, 16);
    
    // Draw BLE icon with animation
    uint8_t bt_x = 20;
    uint8_t bt_y = 32;
    uint8_t bt_size = 16;
    
    // Draw Bluetooth logo
    canvas_draw_box(canvas, bt_x - 3, bt_y - bt_size/2, 6, bt_size);
    
    // Draw the pointed part at the top
    canvas_draw_line(canvas, bt_x, bt_y - bt_size/2, bt_x + bt_size/3, bt_y - bt_size/4);
    canvas_draw_line(canvas, bt_x + bt_size/3, bt_y - bt_size/4, bt_x, bt_y);
    
    // Draw the pointed part at the bottom
    canvas_draw_line(canvas, bt_x, bt_y, bt_x + bt_size/3, bt_y + bt_size/4);
    canvas_draw_line(canvas, bt_x + bt_size/3, bt_y + bt_size/4, bt_x, bt_y + bt_size/2);
    
    // Draw signal waves when attack is running
    if(state->attack_running) {
        for(uint8_t i = 0; i < animation_frame + 1; i++) {
            uint8_t wave_size = 5 + (i * 3);
            canvas_draw_circle(canvas, bt_x, bt_y, wave_size);
        }
    }
    
    // Draw mode selection box
    predator_ui_draw_status_box(canvas, "Spam Mode", 40, 22, 80, 24);
    
    // Draw selected mode
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str_aligned(canvas, 80, 34, AlignCenter, AlignCenter, 
                           ble_spam_mode_names[state->selected_mode]);
    
    // Draw up/down indicators
    if(state->selected_mode > 0) {
        canvas_draw_str_aligned(canvas, 115, 30, AlignCenter, AlignCenter, "▲");
    }
    if(state->selected_mode < BleSpamModeCount - 1) {
        canvas_draw_str_aligned(canvas, 115, 38, AlignCenter, AlignCenter, "▼");
    }
    
    // Draw mode description
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str_aligned(canvas, 80, 48, AlignCenter, AlignTop, 
                           ble_spam_mode_desc[state->selected_mode]);
    
    // Draw status box
    predator_ui_draw_status_box(canvas, "Attack Status", 10, 58, 108, 20);
    
    // Status line
    canvas_set_font(canvas, FontSecondary);
    
    // Draw status or instruction text
    if(state->attack_running) {
        // Show spoofed devices counter
        char devices_text[32];
        snprintf(devices_text, sizeof(devices_text), 
                "Devices spoofed: %lu", state->devices_spoofed);
        canvas_draw_str_aligned(canvas, 64, 68, AlignCenter, AlignCenter, devices_text);
        
        // Show animated text for attack in progress
        canvas_draw_str(canvas, 64 - 24, 76, "ACTIVE");
        
        // Draw animated dots
        for(uint8_t i = 0; i < animation_frame + 1; i++) {
            canvas_draw_box(canvas, 64 - 24 + 40 + (i * 4), 74, 2, 2);
        }
    } else {
        // Show instruction
        canvas_draw_str_aligned(canvas, 64, 68, AlignCenter, AlignCenter, 
                              "Press OK to start spam attack");
    }
    
    // Draw controls
    elements_button_left(canvas, "Back");
    
    if(state->attack_running) {
        elements_button_center(canvas, "Stop");
    } else {
        elements_button_center(canvas, "Start");
    }
}

static bool ble_spam_view_input_callback(InputEvent* event, void* context) {
    PredatorApp* app = context;
    bool consumed = false;
    
    // Get view state
    BleSpamView* state = PREDATOR_GET_MODEL(app->view_dispatcher, BleSpamView);
    if(!state) return consumed;
    
    if(event->type == InputTypeShort || event->type == InputTypeRepeat) {
        switch(event->key) {
        case InputKeyUp:
            if(!state->attack_running && state->selected_mode > 0) {
                state->selected_mode--;
                consumed = true;
            }
            break;
            
        case InputKeyDown:
            if(!state->attack_running && state->selected_mode < BleSpamModeCount - 1) {
                state->selected_mode++;
                consumed = true;
            }
            break;
            
        case InputKeyOk:
        case InputKeyCenter:
            if(state->attack_running) {
                // Stop attack
                predator_esp32_stop_attack(app);
                app->attack_running = false;
            } else {
                // Start attack with selected mode
                predator_esp32_ble_spam(app, state->selected_mode);
                app->attack_running = true;
                app->packets_sent = 0;
            }
            consumed = true;
            break;
            
        case InputKeyBack:
        case InputKeyLeft:
            // Stop attack if running and return
            if(state->attack_running) {
                predator_esp32_stop_attack(app);
                app->attack_running = false;
            }
            scene_manager_previous_scene(app->scene_manager);
            consumed = true;
            break;
            
        default:
            break;
        }
    }
    
    return consumed;
}

static View* ble_spam_view_alloc(PredatorApp* app) {
    View* view = view_alloc();
    view_set_context(view, app);
    view_set_draw_callback(view, ble_spam_view_draw_callback);
    view_set_input_callback(view, ble_spam_view_input_callback);
    
    // Initialize model
    BleSpamView* state = malloc(sizeof(BleSpamView));
    state->selected_mode = 0;
    state->animation_frame = 0;
    state->devices_spoofed = 0;
    state->attack_running = false;
    
    predator_view_set_model(view, state);
    predator_view_set_model_free_callback(view, free);
    
    return view;
}

static void ble_spam_view_free(View* view) {
    view_free(view);
}

void predator_scene_ble_spam_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    // Initialize ESP32 hardware
    predator_esp32_init(app);
    
    // Create custom view
    View* view = ble_spam_view_alloc(app);
    
    // Replace popup view with custom view
    view_dispatcher_remove_view(app->view_dispatcher, PredatorViewPopup);
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewPopup, view);
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
}

bool predator_scene_ble_spam_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(event.type == SceneManagerEventTypeBack) {
        // Stop attack if running and return
        if(app->attack_running) {
            predator_esp32_stop_attack(app);
            app->attack_running = false;
        }
        scene_manager_previous_scene(app->scene_manager);
        consumed = true;
    } else if(event.type == SceneManagerEventTypeTick) {
        if(app->attack_running) {
            // Increment packet counter for animation
            app->packets_sent += (app->packets_sent % 5 == 0) ? 1 : 0;
            
            // Force view refresh for animations
            view_dispatcher_send_custom_event(app->view_dispatcher, 0xFF);
            consumed = true;
        }
    }
    
    return consumed;
}

void predator_scene_ble_spam_new_on_exit(void* context) {
    PredatorApp* app = context;
    
    // Stop attack if running
    if(app->attack_running) {
        predator_esp32_stop_attack(app);
        app->attack_running = false;
    }
    
    // Remove and free custom view
    view_dispatcher_remove_view(app->view_dispatcher, PredatorViewPopup);
    View* view = predator_view_dispatcher_get_current_view(app->view_dispatcher);
    if(view) {
        ble_spam_view_free(view);
    }
    
    // Restore standard popup view
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewPopup, popup_get_view(app->popup));
}


