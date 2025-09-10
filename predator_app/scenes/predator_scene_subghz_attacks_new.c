#include "../predator_i.h"
#include "../helpers/predator_view_helpers.h"
#include "../helpers/predator_ui_elements.h"
#include "../helpers/predator_subghz.h"

typedef struct {
    View* view;
    uint8_t selected_attack;
    uint8_t scroll_position;
    uint8_t num_attacks;
    bool attack_running;
    uint8_t animation_frame;
    uint32_t frequency;
} SubGhzAttackView;

// SubGHz attacks
typedef enum {
    SubGhzAttackJamming,
    SubGhzAttackRawSend,
    SubGhzAttackGarageDoor,
    SubGhzAttackCarKey,
    SubGhzAttackTvPower,
    SubGhzAttackDoorbellSpoof,
    SubGhzAttackRollingCodeCapture,
    SubGhzAttackCount
} SubGhzAttackType;

// Attack descriptions
typedef struct {
    const char* name;
    const char* description;
    uint32_t default_freq;
    const char* icon;
} SubGhzAttackInfo;

// List of attacks with descriptions
static const SubGhzAttackInfo attack_info[] = {
    {"Jamming", "Disrupt 433/315 MHz signals", 433920000, "📡"},
    {"Raw Send", "Send raw signal packets", 433920000, "📦"},
    {"Garage Door", "Common garage codes", 433920000, "🚪"},
    {"Car Keys", "Car key simulation", 433920000, "🚗"},
    {"TV Power", "TV power off signals", 433920000, "📺"},
    {"Doorbell Spoof", "Trigger doorbells", 433920000, "🔔"},
    {"Rolling Capture", "Capture rolling codes", 433920000, "🎣"},
};

#define ITEMS_ON_SCREEN 3

static void subghz_attacks_view_draw_callback(Canvas* canvas, void* context) {
    PredatorApp* app = context;
    
    if(!app) return;
    
    // Get view state
    SubGhzAttackView* state = PREDATOR_GET_MODEL(app->view_dispatcher, SubGhzAttackView);
    if(!state) return;
    
    // Update animation frame
    uint8_t animation_frame = (furi_get_tick() / 200) % 4;
    state->animation_frame = animation_frame;
    
    canvas_clear(canvas);
    
    // Draw title
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, 64, 8, AlignCenter, AlignCenter, "SubGHz Attacks");
    
    // Draw separator
    canvas_draw_line(canvas, 0, 16, 128, 16);
    
    // Draw SubGHz animation
    uint8_t radio_x = 22;
    uint8_t radio_y = 30;
    uint8_t radio_width = 16;
    uint8_t radio_height = 10;
    
    // Draw radio device
    canvas_draw_box(canvas, radio_x - radio_width/2, radio_y - radio_height/2, radio_width, radio_height);
    
    // Draw antenna
    canvas_draw_line(canvas, radio_x, radio_y - radio_height/2, radio_x, radio_y - radio_height/2 - 8);
    
    // Draw signal waves animation when attack is running
    if(state->attack_running) {
        for(uint8_t i = 0; i < animation_frame + 1; i++) {
            uint8_t wave_size = 5 + (i * 3);
            canvas_draw_circle(canvas, radio_x, radio_y - radio_height/2 - 4, wave_size);
        }
    }
    
    // Draw attacks list
    uint8_t start_idx = state->scroll_position;
    uint8_t visible_items = MIN(ITEMS_ON_SCREEN, state->num_attacks - start_idx);
    
    canvas_set_font(canvas, FontSecondary);
    
    for(uint8_t i = 0; i < visible_items; i++) {
        uint8_t item_idx = start_idx + i;
        uint8_t y_pos = 26 + (i * 12);
        
        // Highlight selected item
        if(item_idx == state->selected_attack) {
            canvas_draw_box(canvas, 40, y_pos - 1, 80, 11);
            canvas_set_color(canvas, ColorWhite);
        }
        
        // Draw attack icon
        canvas_draw_str(canvas, 44, y_pos + 7, attack_info[item_idx].icon);
        
        // Draw attack name
        canvas_draw_str(canvas, 56, y_pos + 7, attack_info[item_idx].name);
        
        // Reset color
        canvas_set_color(canvas, ColorBlack);
    }
    
    // Draw scroll indicators if needed
    if(state->scroll_position > 0) {
        canvas_draw_icon(canvas, 120, 26, &I_ButtonUp_4x7);
    }
    
    if(state->scroll_position + visible_items < state->num_attacks) {
        canvas_draw_icon(canvas, 120, 50, &I_ButtonDown_4x7);
    }
    
    // Draw attack description
    predator_ui_draw_status_box(canvas, "Info", 10, 62, 108, 24);
    
    canvas_set_font(canvas, FontSecondary);
    
    // Draw frequency
    char freq_text[24];
    if(state->frequency >= 1000000) {
        snprintf(freq_text, sizeof(freq_text), "Freq: %.1f MHz", (double)(state->frequency / 1000000.0f));
    } else {
        snprintf(freq_text, sizeof(freq_text), "Freq: %.1f kHz", (double)(state->frequency / 1000.0f));
    }
    canvas_draw_str(canvas, 16, 72, freq_text);
    
    // Draw attack description
    canvas_draw_str_aligned(canvas, 64, 80, AlignCenter, AlignCenter, 
                          attack_info[state->selected_attack].description);
    
    // Draw controls
    elements_button_left(canvas, "Back");
    
    if(state->attack_running) {
        elements_button_center(canvas, "Stop");
    } else {
        elements_button_center(canvas, "Start");
    }
    
    elements_button_right(canvas, "Freq");
}

static bool subghz_attacks_view_input_callback(InputEvent* event, void* context) {
    PredatorApp* app = context;
    bool consumed = false;
    
    // Get view state
    SubGhzAttackView* state = PREDATOR_GET_MODEL(app->view_dispatcher, SubGhzAttackView);
    if(!state) return consumed;
    
    if(event->type == InputTypeShort || event->type == InputTypeRepeat) {
        switch(event->key) {
        case InputKeyUp:
            if(state->selected_attack > 0 && !state->attack_running) {
                state->selected_attack--;
                
                // Adjust scroll position if needed
                if(state->selected_attack < state->scroll_position) {
                    state->scroll_position = state->selected_attack;
                }
                
                // Update frequency for selected attack
                state->frequency = attack_info[state->selected_attack].default_freq;
                
                consumed = true;
            }
            break;
            
        case InputKeyDown:
            if(state->selected_attack < state->num_attacks - 1 && !state->attack_running) {
                state->selected_attack++;
                
                // Adjust scroll position if needed
                if(state->selected_attack >= state->scroll_position + ITEMS_ON_SCREEN) {
                    state->scroll_position = state->selected_attack - ITEMS_ON_SCREEN + 1;
                }
                
                // Update frequency for selected attack
                state->frequency = attack_info[state->selected_attack].default_freq;
                
                consumed = true;
            }
            break;
            
        case InputKeyOk:
            // Toggle attack
            if(state->attack_running) {
                // Stop attack
                predator_subghz_stop_attack(app);
                app->attack_running = false;
                state->attack_running = false;
            } else {
                // Start attack
                if(predator_subghz_start_attack(app, state->selected_attack, state->frequency)) {
                    app->attack_running = true;
                    state->attack_running = true;
                }
            }
            consumed = true;
            break;
            
        case InputKeyRight:
            // Cycle through frequencies
            if(!state->attack_running) {
                if(state->frequency == 433920000) {
                    state->frequency = 315000000;
                } else if(state->frequency == 315000000) {
                    state->frequency = 868350000;
                } else {
                    state->frequency = 433920000;
                }
                consumed = true;
            }
            break;
            
        case InputKeyBack:
        case InputKeyLeft:
            // Stop attack if running and exit
            if(state->attack_running) {
                predator_subghz_stop_attack(app);
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

static View* subghz_attacks_view_alloc(PredatorApp* app) {
    View* view = view_alloc();
    view_set_context(view, app);
    view_set_draw_callback(view, subghz_attacks_view_draw_callback);
    view_set_input_callback(view, subghz_attacks_view_input_callback);
    
    // Initialize model
    SubGhzAttackView* state = malloc(sizeof(SubGhzAttackView));
    state->selected_attack = 0;
    state->scroll_position = 0;
    state->num_attacks = SubGhzAttackCount;
    state->attack_running = false;
    state->animation_frame = 0;
    state->frequency = attack_info[0].default_freq;
    
    predator_view_set_model(view, state);
    predator_view_set_model_free_callback(view, free);
    
    return view;
}

static void subghz_attacks_view_free(View* view) {
    view_free(view);
}

void predator_scene_subghz_attacks_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    // Initialize SubGHz hardware
    predator_subghz_init(app);
    
    // Create custom view
    View* view = subghz_attacks_view_alloc(app);
    
    // Replace submenu view with custom view
    view_dispatcher_remove_view(app->view_dispatcher, PredatorViewSubmenu);
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewSubmenu, view);
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

bool predator_scene_subghz_attacks_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(event.type == SceneManagerEventTypeBack) {
        // Stop attack if running
        if(app->attack_running) {
            predator_subghz_stop_attack(app);
            app->attack_running = false;
        }
        consumed = false; // Allow default back behavior
    } else if(event.type == SceneManagerEventTypeTick) {
        if(app->attack_running) {
            // Force view refresh for animations
            view_dispatcher_send_custom_event(app->view_dispatcher, 0xFF);
            consumed = true;
        }
    }
    
    return consumed;
}

void predator_scene_subghz_attacks_new_on_exit(void* context) {
    PredatorApp* app = context;
    
    // Stop attack if running
    if(app->attack_running) {
        predator_subghz_stop_attack(app);
        app->attack_running = false;
    }
    
    // Clean up hardware
    predator_subghz_deinit(app);
    
    // Remove and free custom view
    view_dispatcher_remove_view(app->view_dispatcher, PredatorViewSubmenu);
    View* view = predator_view_dispatcher_get_current_view(app->view_dispatcher);
    if(view) {
        subghz_attacks_view_free(view);
    }
    
    // Restore standard submenu view
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewSubmenu, submenu_get_view(app->submenu));
}


