#include "../predator_i.h"
#include "../helpers/predator_view_helpers.h"
#include "../helpers/predator_ui_elements.h"

typedef struct {
    View* view;
    uint32_t keys_tried;
    uint32_t current_key;
    uint8_t animation_frame;
    uint8_t progress;
    bool key_found;
    char card_type[32];
} RfidBruteforceView;

static void rfid_bruteforce_view_draw_callback(Canvas* canvas, void* context) {
    PredatorApp* app = context;
    
    if(!app) return;
    
    // Get view state
    RfidBruteforceView* state = PREDATOR_GET_MODEL(app->view_dispatcher, RfidBruteforceView);
    if(!state) return;
    
    // Update animation frame
    uint8_t animation_frame = (furi_get_tick() / 200) % 4;
    state->animation_frame = animation_frame;
    
    // Update state from app
    state->keys_tried = app->packets_sent;
    state->progress = (app->packets_sent % 100);
    
    // Update current key being tried
    if(app->attack_running) {
        state->current_key = 0x10000000 + (app->packets_sent * 137);
    }
    
    // Check if we've "found" a key (for demo purposes)
    if(app->packets_sent > 0 && app->packets_sent % 100 == 0) {
        state->key_found = true;
    }
    
    canvas_clear(canvas);
    
    // Draw title
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, 64, 8, AlignCenter, AlignCenter, "RFID Bruteforce");
    
    // Draw separator
    canvas_draw_line(canvas, 0, 16, 128, 16);
    
    // Draw RFID card visualization
    uint8_t card_x = 30;
    uint8_t card_y = 35;
    uint8_t card_width = 16;
    uint8_t card_height = 26;
    
    canvas_draw_rbox(canvas, card_x - card_width/2, card_y - card_height/2, 
                    card_width, card_height, 3);
    
    // Draw RFID chip on card
    canvas_draw_circle(canvas, card_x, card_y, 4);
    
    // Draw signal waves emanating from card when bruteforcing
    if(app->attack_running && !state->key_found) {
        for(uint8_t i = 0; i < animation_frame + 1; i++) {
            uint8_t wave_size = 6 + (i * 4);
            canvas_draw_circle(canvas, card_x, card_y, wave_size);
        }
    }
    
    // Draw RFID reader
    uint8_t reader_x = 80;
    uint8_t reader_y = 35;
    uint8_t reader_width = 20;
    uint8_t reader_height = 12;
    
    canvas_draw_box(canvas, reader_x - reader_width/2, reader_y - reader_height/2, 
                   reader_width, reader_height);
    
    // Draw reader indicator light
    if(state->key_found) {
        // Green light when key found
        canvas_draw_disc(canvas, reader_x + reader_width/2 - 3, 
                       reader_y - reader_height/2 + 3, 2);
    } else if(app->attack_running) {
        // Blinking light when bruteforcing
        if(animation_frame % 2 == 0) {
            canvas_draw_disc(canvas, reader_x + reader_width/2 - 3, 
                           reader_y - reader_height/2 + 3, 2);
        }
    } else {
        // Empty circle when inactive
        canvas_draw_circle(canvas, reader_x + reader_width/2 - 3, 
                         reader_y - reader_height/2 + 3, 2);
    }
    
    // Draw signal waves between card and reader when key is found
    if(state->key_found) {
        // Show successful connection animation
        canvas_draw_line(canvas, card_x + card_width/2, card_y, 
                       reader_x - reader_width/2, reader_y);
        
        if(animation_frame % 2 == 0) {
            uint8_t mid_x = (card_x + card_width/2 + reader_x - reader_width/2) / 2;
            uint8_t mid_y = (card_y + reader_y) / 2;
            canvas_draw_disc(canvas, mid_x, mid_y, 2);
        }
    }
    
    // Status information
    predator_ui_draw_status_box(canvas, "Bruteforce Status", 10, 50, 108, 28);
    
    canvas_set_font(canvas, FontSecondary);
    
    // Keys tried counter
    char keys_text[24];
    snprintf(keys_text, sizeof(keys_text), "Keys tried: %lu", state->keys_tried);
    canvas_draw_str(canvas, 16, 60, keys_text);
    
    // Current key in hex
    char key_text[24];
    snprintf(key_text, sizeof(key_text), "Key: 0x%08lX", state->current_key);
    canvas_draw_str(canvas, 16, 70, key_text);
    
    // Status text
    if(state->key_found) {
        canvas_draw_str(canvas, 92, 60, "SUCCESS");
    } else if(app->attack_running) {
        // Show animated searching text
        char search_text[16] = "RUNNING";
        canvas_draw_str(canvas, 92, 60, search_text);
    } else {
        canvas_draw_str(canvas, 92, 60, "READY");
    }
    
    // Draw progress bar
    predator_ui_draw_progress_bar(canvas, 92, 70, 20, 5, 
                                state->key_found ? 100 : state->progress, 
                                ProgressBarStyleBordered);
}

static bool rfid_bruteforce_view_input_callback(InputEvent* event, void* context) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(event->type == InputTypeShort) {
        switch(event->key) {
        case InputKeyBack:
            scene_manager_previous_scene(app->scene_manager);
            consumed = true;
            break;
            
        case InputKeyOk:
            // Toggle bruteforce
            app->attack_running = !app->attack_running;
            if(app->attack_running) {
                app->packets_sent = 0;
            }
            consumed = true;
            break;
            
        default:
            break;
        }
    }
    
    return consumed;
}

static View* rfid_bruteforce_view_alloc(PredatorApp* app) {
    View* view = view_alloc();
    view_set_context(view, app);
    view_set_draw_callback(view, rfid_bruteforce_view_draw_callback);
    view_set_input_callback(view, rfid_bruteforce_view_input_callback);
    
    // Initialize model
    RfidBruteforceView* state = malloc(sizeof(RfidBruteforceView));
    state->keys_tried = 0;
    state->current_key = 0;
    state->animation_frame = 0;
    state->progress = 0;
    state->key_found = false;
    strncpy(state->card_type, "EM4100", sizeof(state->card_type));
    
    predator_view_set_model(view, state);
    predator_view_set_model_free_callback(view, free);
    
    return view;
}


void predator_scene_rfid_bruteforce_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    // Create custom view
    View* view = rfid_bruteforce_view_alloc(app);
    
    // Replace popup view with custom view
    view_dispatcher_remove_view(app->view_dispatcher, PredatorViewPopup);
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewPopup, view);
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
}

bool predator_scene_rfid_bruteforce_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(event.type == SceneManagerEventTypeBack) {
        consumed = true;
        app->attack_running = false;
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

void predator_scene_rfid_bruteforce_new_on_exit(void* context) {
    PredatorApp* app = context;
    app->attack_running = false;
    
    // Remove custom view and restore default popup view
    view_dispatcher_remove_view(app->view_dispatcher, PredatorViewPopup);
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewPopup, popup_get_view(app->popup));
}


