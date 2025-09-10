#include "../predator_i.h"\n#include "../helpers/predator_view_helpers.h"
#include "../helpers/predator_ui_elements.h"
#include "predator_scene.h"

typedef struct {
    View* view;
    uint8_t selected_index;
    uint8_t scroll_position;
    uint8_t menu_items_count;
} RfidAttacksMenuView;

// Menu item definitions
typedef struct {
    const char* name;
    const char* icon;
    uint8_t submenu_index;
} RfidMenuItem;

// Menu items array - match the original enum SubmenuIndex order!
static const RfidMenuItem menu_items[] = {
    {"RFID Clone", "💳", SubmenuIndexRfidClone},
    {"RFID Bruteforce", "🔓", SubmenuIndexRfidBruteforce},
    {"RFID Fuzzing", "🎲", SubmenuIndexRfidFuzzing},
    {"NFC Clone", "📱", SubmenuIndexNfcClone},
    {"Mifare Hack", "🔐", SubmenuIndexMifareHack},
};

#define MENU_ITEMS_COUNT (sizeof(menu_items) / sizeof(menu_items[0]))
#define ITEMS_ON_SCREEN 4

static void rfid_attacks_menu_draw_callback(Canvas* canvas, void* context) {
    PredatorApp* app = context;
    
    if(!app) return;
    
    // Get view state
    RfidAttacksMenuView* state = PREDATOR_GET_MODEL(app->view_dispatcher, RfidAttacksMenuView);
    if(!state) return;
    
    canvas_clear(canvas);
    
    // Draw title
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, 64, 8, AlignCenter, AlignCenter, "RFID/NFC Attacks");
    
    // Draw separator
    canvas_draw_line(canvas, 0, 16, 128, 16);
    
    // Draw RFID card visualization
    uint8_t card_x = 20;
    uint8_t card_y = 35;
    uint8_t card_width = 20;
    uint8_t card_height = 30;
    
    canvas_draw_rbox(canvas, card_x - card_width/2, card_y - card_height/2, 
                   card_width, card_height, 3);
    
    // Draw RFID chip on card
    canvas_draw_circle(canvas, card_x, card_y, 6);
    canvas_draw_circle(canvas, card_x, card_y, 4);
    canvas_draw_circle(canvas, card_x, card_y, 2);
    
    // Draw scrolling menu
    canvas_set_font(canvas, FontSecondary);
    
    // Calculate visible range based on selection and scroll position
    for(uint8_t i = 0; i < ITEMS_ON_SCREEN && (i + state->scroll_position) < state->menu_items_count; i++) {
        uint8_t item_idx = i + state->scroll_position;
        const RfidMenuItem* item = &menu_items[item_idx];
        
        uint8_t y_pos = 25 + 10 * i;
        
        // Draw selection indicator or space for non-selected items
        if(item_idx == state->selected_index) {
            elements_slightly_rounded_box(canvas, 45, y_pos - 1, 78, 11);
            canvas_set_color(canvas, ColorWhite);
        } else {
            canvas_set_color(canvas, ColorBlack);
        }
        
        // Draw icon
        canvas_draw_str(canvas, 48, y_pos + 4, item->icon);
        
        // Draw menu item text
        canvas_draw_str(canvas, 60, y_pos + 4, item->name);
        
        // Reset color
        canvas_set_color(canvas, ColorBlack);
    }
    
    // Draw scroll indicators if needed
    if(state->scroll_position > 0) {
        canvas_draw_icon(canvas, 116, 20, &I_ButtonUp_4x7);
    }
    
    if(state->scroll_position + ITEMS_ON_SCREEN < state->menu_items_count) {
        canvas_draw_icon(canvas, 116, 60, &I_ButtonDown_4x7);
    }
    
    // Draw attack description box
    predator_ui_draw_status_box(canvas, "Description", 10, 65, 108, 15);
    
    // Show description based on selection
    canvas_set_font(canvas, FontSecondary);
    
    const char* description = "Select attack type";
    switch(state->selected_index) {
        case 0: description = "Clone RFID cards (125kHz)"; break;
        case 1: description = "Bruteforce common RFID codes"; break;
        case 2: description = "Test RFID protocols & variations"; break;
        case 3: description = "Clone NFC tags (13.56MHz)"; break;
        case 4: description = "Attack Mifare Classic cards"; break;
    }
    
    canvas_draw_str_aligned(canvas, 64, 73, AlignCenter, AlignCenter, description);
    
    // Draw controls
    elements_button_left(canvas, "Back");
    elements_button_right(canvas, "OK");
}

static bool rfid_attacks_menu_input_callback(InputEvent* event, void* context) {
    PredatorApp* app = context;
    bool consumed = false;
    
    // Get view state
    RfidAttacksMenuView* state = PREDATOR_GET_MODEL(app->view_dispatcher, RfidAttacksMenuView);
    if(!state) return consumed;
    
    // Navigation controls
    if(event->type == InputTypeShort || event->type == InputTypeRepeat) {
        switch(event->key) {
        case InputKeyUp:
            if(state->selected_index > 0) {
                state->selected_index--;
                
                // Adjust scroll position if needed
                if(state->selected_index < state->scroll_position) {
                    state->scroll_position = state->selected_index;
                }
                consumed = true;
            }
            break;
            
        case InputKeyDown:
            if(state->selected_index < state->menu_items_count - 1) {
                state->selected_index++;
                
                // Adjust scroll position if needed
                if(state->selected_index >= state->scroll_position + ITEMS_ON_SCREEN) {
                    state->scroll_position = state->selected_index - ITEMS_ON_SCREEN + 1;
                }
                consumed = true;
            }
            break;
            
        case InputKeyRight:
        case InputKeyOk:
            // Select the current item
            view_dispatcher_send_custom_event(
                app->view_dispatcher, menu_items[state->selected_index].submenu_index);
            consumed = true;
            break;
            
        case InputKeyLeft:
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

static View* rfid_attacks_menu_view_alloc(PredatorApp* app) {
    View* view = view_alloc();
    
    // Initialize the model
    RfidAttacksMenuView* state = malloc(sizeof(RfidAttacksMenuView));
    state->selected_index = 0;
    state->scroll_position = 0;
    state->menu_items_count = MENU_ITEMS_COUNT;
    
    // Set view callbacks
    view_set_context(view, app);
    view_set_draw_callback(view, rfid_attacks_menu_draw_callback);
    view_set_input_callback(view, rfid_attacks_menu_input_callback);
    
    // Set model and free callback
    predator_view_set_model(view, state);
    predator_view_set_model_free_callback(view, free);
    
    return view;
}

static void rfid_attacks_menu_view_free(View* view) {
    view_free(view);
}

void predator_scene_rfid_attacks_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    // Create custom view
    View* view = rfid_attacks_menu_view_alloc(app);
    
    // Replace submenu view with custom view
    view_dispatcher_remove_view(app->view_dispatcher, PredatorViewSubmenu);
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewSubmenu, view);
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

bool predator_scene_rfid_attacks_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        // Handle popup back button press
        if(event.event == PredatorCustomEventPopupBack) {
            // Return to submenu view
            view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
            
            // Clean up any running attacks
            app->attack_running = false;
            
            consumed = true;
        } else {
            // Handle menu selection
            consumed = true;
            switch(event.event) {
            case SubmenuIndexRfidClone:
                scene_manager_next_scene(app->scene_manager, PredatorSceneRfidClone);
                break;
            case SubmenuIndexRfidBruteforce:
                scene_manager_next_scene(app->scene_manager, PredatorSceneRfidBruteforce);
                break;
            case SubmenuIndexRfidFuzzing:
                // RFID Fuzzing attack - use popup for now
                popup_set_header(app->popup, "RFID Fuzzing", 64, 10, AlignCenter, AlignTop);
                popup_set_text(app->popup, "Fuzzing RFID protocols...\nTesting variations: 127\nPress Back to stop", 64, 25, AlignCenter, AlignTop);
                popup_set_context(app->popup, app);
                popup_set_callback(app->popup, predator_scene_rfid_attacks_popup_callback);
                popup_set_timeout(app->popup, 0);
                popup_enable_timeout(app->popup);
                
                // Start attack
                app->attack_running = true;
                app->packets_sent = 0;
                
                view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
                break;
            case SubmenuIndexNfcClone:
                // NFC Clone - use popup for now
                popup_set_header(app->popup, "NFC Clone", 64, 10, AlignCenter, AlignTop);
                popup_set_text(app->popup, "Place NFC card on Flipper...\nReading NFC data\nPress Back to stop", 64, 25, AlignCenter, AlignTop);
                popup_set_context(app->popup, app);
                popup_set_callback(app->popup, predator_scene_rfid_attacks_popup_callback);
                popup_set_timeout(app->popup, 0);
                popup_enable_timeout(app->popup);
                
                // Start attack
                app->attack_running = true;
                app->packets_sent = 0;
                
                view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
                break;
            case SubmenuIndexMifareHack:
                // Mifare Attack - use popup for now
                popup_set_header(app->popup, "Mifare Attack", 64, 10, AlignCenter, AlignTop);
                popup_set_text(app->popup, "Attacking Mifare Classic...\nUsing known keys\nPress Back to stop", 64, 25, AlignCenter, AlignTop);
                popup_set_context(app->popup, app);
                popup_set_callback(app->popup, predator_scene_rfid_attacks_popup_callback);
                popup_set_timeout(app->popup, 0);
                popup_enable_timeout(app->popup);
                
                // Start attack
                app->attack_running = true;
                app->packets_sent = 0;
                
                view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
                break;
            default:
                consumed = false;
                break;
            }
        }
    }

    return consumed;
}

void predator_scene_rfid_attacks_new_on_exit(void* context) {
    PredatorApp* app = context;
    
    // Remove and free custom view
    view_dispatcher_remove_view(app->view_dispatcher, PredatorViewSubmenu);
    View* view = predator_view_dispatcher_get_current_view(app->view_dispatcher);
    if(view) {
        rfid_attacks_menu_view_free(view);
    }
    
    // Restore standard submenu view
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewSubmenu, submenu_get_view(app->submenu));
}


