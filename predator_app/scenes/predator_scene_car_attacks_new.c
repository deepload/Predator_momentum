#include "../predator_i.h"
#include "../helpers/predator_view_helpers.h"
#include "../helpers/predator_subghz.h"
#include "../helpers/predator_ui_elements.h"
#include "predator_scene.h"

typedef enum {
    CarMenuStateList,
    CarMenuStateConfirm
} CarMenuState;

typedef struct {
    View* view;
    CarMenuState state;
    uint8_t selected_index;
    uint8_t items_count;
    char confirm_text[128];
} CarAttacksMenuView;

// Menu items definition
typedef struct {
    const char* name;
    const char* icon;
    uint8_t index;
} CarMenuItem;

// Menu items - must match the original enum SubmenuIndex order
static const CarMenuItem car_items[] = {
    {"All Car Models", "🚙", SubmenuIndexCarModels},
    {"Passive Bag Mode", "👜", SubmenuIndexCarPassiveOpener},
    {"Key Bruteforce", "🔑", SubmenuIndexCarKeyBruteforce},
    {"Signal Jamming", "📡", SubmenuIndexCarJamming},
    {"Tesla Charge Port", "🚗", SubmenuIndexCarTesla},
    {"Rolling Code Attack", "🔄", SubmenuIndexCarRollingCode},
    {"Tire Monitor Spoof", "🛞", SubmenuIndexCarTireMonitor}
};

#define CAR_ITEMS_COUNT (sizeof(car_items) / sizeof(car_items[0]))

static void car_attacks_menu_draw_callback(Canvas* canvas, void* context) {
    PredatorApp* app = context;
    
    if(!app) return;
    
    // Get view state
    CarAttacksMenuView* state = PREDATOR_GET_MODEL(app->view_dispatcher, CarAttacksMenuView);
    if(!state) return;
    
    canvas_clear(canvas);
    
    // Handle different states
    if(state->state == CarMenuStateConfirm) {
        // Draw confirmation screen
        canvas_set_font(canvas, FontPrimary);
        canvas_draw_str_aligned(canvas, 64, 8, AlignCenter, AlignCenter, "Confirm Attack");
        
        // Draw separator
        canvas_draw_line(canvas, 0, 16, 128, 16);
        
        // Draw confirmation box
        predator_ui_draw_status_box(canvas, "Warning", 8, 22, 112, 34);
        
        canvas_set_font(canvas, FontSecondary);
        // Draw multiline text centered in box
        canvas_draw_str_aligned(canvas, 64, 32, AlignCenter, AlignTop, state->confirm_text);
        
        // Draw buttons
        elements_button_left(canvas, "Cancel");
        elements_button_right(canvas, "Start");
        
        return;
    }
    
    // Draw menu title with car icon
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, 64, 8, AlignCenter, AlignCenter, "Car Attacks");
    
    // Draw separator
    canvas_draw_line(canvas, 0, 16, 128, 16);
    
    // Draw car icon as visual indicator
    // Simple car outline
    uint8_t car_x = 20;
    uint8_t car_y = 30;
    uint8_t car_width = 16;
    uint8_t car_height = 8;
    
    canvas_draw_line(canvas, car_x - car_width/2, car_y - car_height/2, 
                     car_x + car_width/2, car_y - car_height/2); // Top
    canvas_draw_line(canvas, car_x - car_width/2, car_y - car_height/2, 
                     car_x - car_width/2, car_y + car_height/2); // Left
    canvas_draw_line(canvas, car_x + car_width/2, car_y - car_height/2, 
                     car_x + car_width/2, car_y + car_height/2); // Right
    canvas_draw_line(canvas, car_x - car_width/2, car_y + car_height/2, 
                     car_x + car_width/2, car_y + car_height/2); // Bottom
    
    // Car wheels
    canvas_draw_circle(canvas, car_x - car_width/4, car_y + car_height/2, 2);
    canvas_draw_circle(canvas, car_x + car_width/4, car_y + car_height/2, 2);
    
    // Draw scrolling menu
    canvas_set_font(canvas, FontSecondary);
    
    // Calculate visible range based on selection
    uint8_t scroll_position = 0;
    if(state->selected_index >= ITEMS_ON_SCREEN) {
        scroll_position = state->selected_index - ITEMS_ON_SCREEN + 1;
        
        // Don't scroll past the end
        if(scroll_position + ITEMS_ON_SCREEN > state->items_count) {
            scroll_position = state->items_count - ITEMS_ON_SCREEN;
        }
    }
    
    // Draw menu frame
    predator_ui_draw_status_box(canvas, "Attack Selection", 38, 22, 82, 48);
    
    // Draw menu items
    for(uint8_t i = 0; i < ITEMS_ON_SCREEN && (i + scroll_position) < state->items_count; i++) {
        uint8_t item_idx = i + scroll_position;
        const CarMenuItem* item = &car_items[item_idx];
        
        uint8_t y_pos = 30 + (i * 8);
        
        // Highlight selected item
        if(item_idx == state->selected_index) {
            canvas_draw_box(canvas, 42, y_pos - 4, 74, 9);
            canvas_set_color(canvas, ColorWhite);
        }
        
        // Draw icon and name
        canvas_draw_str(canvas, 44, y_pos + 2, item->icon);
        canvas_draw_str(canvas, 58, y_pos + 2, item->name);
        
        // Reset color
        canvas_set_color(canvas, ColorBlack);
    }
    
    // Draw scroll indicators with vector chevrons
    predator_ui_draw_scroll_vertical(
        canvas,
        112,
        24,
        62,
        scroll_position > 0,
        (scroll_position + ITEMS_ON_SCREEN) < state->items_count);
    
    // Draw controls
    elements_button_left(canvas, "Back");
    elements_button_right(canvas, "OK");
}

static bool car_attacks_menu_input_callback(InputEvent* event, void* context) {
    PredatorApp* app = context;
    bool consumed = false;
    
    // Get view state
    CarAttacksMenuView* state = PREDATOR_GET_MODEL(app->view_dispatcher, CarAttacksMenuView);
    if(!state) return consumed;
    
    if(event->type == InputTypeShort || event->type == InputTypeRepeat) {
        if(state->state == CarMenuStateList) {
            // Menu navigation
            switch(event->key) {
            case InputKeyUp:
                if(state->selected_index > 0) {
                    state->selected_index--;
                    consumed = true;
                }
                break;
                
            case InputKeyDown:
                if(state->selected_index < state->items_count - 1) {
                    state->selected_index++;
                    consumed = true;
                }
                break;
                
            case InputKeyOk:
            case InputKeyRight:
                // Select menu item
                if(state->selected_index < state->items_count) {
                    uint8_t item_index = car_items[state->selected_index].index;
                    
                    // Special handling for attacks that need confirmation
                    if(item_index == SubmenuIndexCarJamming) {
                        // Show confirmation for jamming attack
                        snprintf(state->confirm_text, sizeof(state->confirm_text),
                            "Signal jamming can disrupt\n"
                            "car key fobs and other\n"
                            "devices in the area.\n\n"
                            "Continue?");
                        state->state = CarMenuStateConfirm;
                    } else {
                        // Directly navigate to the selected scene
                        view_dispatcher_send_custom_event(app->view_dispatcher, item_index);
                    }
                    consumed = true;
                }
                break;
                
            case InputKeyBack:
            case InputKeyLeft:
                // Exit menu
                scene_manager_previous_scene(app->scene_manager);
                consumed = true;
                break;
                
            default:
                break;
            }
        } else if(state->state == CarMenuStateConfirm) {
            // Confirmation screen controls
            switch(event->key) {
            case InputKeyBack:
            case InputKeyLeft:
                // Cancel and return to menu
                state->state = CarMenuStateList;
                consumed = true;
                break;
                
            case InputKeyOk:
            case InputKeyRight:
                // Confirm and proceed
                state->state = CarMenuStateList;
                view_dispatcher_send_custom_event(
                    app->view_dispatcher, car_items[state->selected_index].index);
                consumed = true;
                break;
                
            default:
                break;
            }
        }
    }
    
    return consumed;
}

static View* car_attacks_menu_view_alloc(PredatorApp* app) {
    View* view = view_alloc();
    view_set_context(view, app);
    view_set_draw_callback(view, car_attacks_menu_draw_callback);
    view_set_input_callback(view, car_attacks_menu_input_callback);
    
    // Initialize model
    CarAttacksMenuView* state = malloc(sizeof(CarAttacksMenuView));
    state->state = CarMenuStateList;
    state->selected_index = 0;
    state->items_count = CAR_ITEMS_COUNT;
    memset(state->confirm_text, 0, sizeof(state->confirm_text));
    
    predator_view_set_model(view, state);
    predator_view_set_model_free_callback(view, free);
    
    return view;
}


void predator_scene_car_attacks_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    // Create custom view
    View* view = car_attacks_menu_view_alloc(app);
    
    // Switch to a safe view before replacing to avoid dispatcher crash
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewLoading);
    // Replace submenu view with custom view
    view_dispatcher_remove_view(app->view_dispatcher, PredatorViewSubmenu);
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewSubmenu, view);
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
    
    // Restore previous selection if available
    CarAttacksMenuView* state = PREDATOR_GET_MODEL(app->view_dispatcher, CarAttacksMenuView);
    if(state) {
        state->selected_index = scene_manager_get_scene_state(app->scene_manager, PredatorSceneCarAttacks);
    }
}

bool predator_scene_car_attacks_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    // Get view state safely for Momentum SDK
    CarAttacksMenuView* state = PREDATOR_GET_MODEL(app->view_dispatcher, CarAttacksMenuView);
    
    if(event.type == SceneManagerEventTypeCustom) {
        // Save the selected index for when returning to this scene
        if(state) {
            scene_manager_set_scene_state(app->scene_manager, PredatorSceneCarAttacks, state->selected_index);
        }
        
        // Handle menu selection - these should match the original SubmenuIndex values
        consumed = true;
        switch(event.event) {
        case SubmenuIndexCarModels:
            scene_manager_next_scene(app->scene_manager, PredatorSceneCarModels);
            break;
        case SubmenuIndexCarPassiveOpener:
            scene_manager_next_scene(app->scene_manager, PredatorSceneCarPassiveOpener);
            break;
        case SubmenuIndexCarKeyBruteforce:
            scene_manager_next_scene(app->scene_manager, PredatorSceneCarKey);
            break;
        case SubmenuIndexCarJamming:
            scene_manager_next_scene(app->scene_manager, PredatorSceneCarJamming);
            break;
        case SubmenuIndexCarTesla:
            scene_manager_next_scene(app->scene_manager, PredatorSceneCarTesla);
            break;
        case SubmenuIndexCarRollingCode:
            // Handle rolling code attack
            popup_set_header(app->popup, "Rolling Code Attack", 64, 10, AlignCenter, AlignTop);
            popup_set_text(app->popup, 
                "Capturing rolling codes...\n"
                "Waiting for signal\n"
                "Press Back to stop", 
                64, 25, AlignCenter, AlignTop);
            popup_set_callback(app->popup, predator_scene_car_attacks_popup_callback);
            popup_set_context(app->popup, app);
            popup_set_timeout(app->popup, 0);
            popup_enable_timeout(app->popup);
            
            // Initialize SubGHz
            predator_subghz_init(app);
            app->attack_running = true;
            app->packets_sent = 0;
            
            view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
            break;
        case SubmenuIndexCarTireMonitor:
            // Handle tire monitor spoofing
            popup_set_header(app->popup, "Tire Monitor Spoof", 64, 10, AlignCenter, AlignTop);
            popup_set_text(app->popup, 
                "Spoofing TPMS signals...\n"
                "Pressure: 15 PSI (Low)\n"
                "Press Back to stop", 
                64, 25, AlignCenter, AlignTop);
            popup_set_callback(app->popup, predator_scene_car_attacks_popup_callback);
            popup_set_context(app->popup, app);
            popup_set_timeout(app->popup, 0);
            popup_enable_timeout(app->popup);
            
            // Initialize SubGHz
            predator_subghz_init(app);
            app->attack_running = true;
            app->packets_sent = 0;
            
            view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
            break;
        default:
            consumed = false;
            break;
        }
    } else if(event.type == SceneManagerEventTypeBack) {
        // If in confirmation screen, return to menu instead of leaving scene
        if(state && state->state == CarMenuStateConfirm) {
            state->state = CarMenuStateList;
            consumed = true;
        }
    }
    
    return consumed;
}

void predator_scene_car_attacks_new_on_exit(void* context) {
    PredatorApp* app = context;
    
    // Switch to a safe view before removing to avoid dispatcher crash
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewLoading);
    // Remove custom view and restore default submenu view
    view_dispatcher_remove_view(app->view_dispatcher, PredatorViewSubmenu);
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewSubmenu, submenu_get_view(app->submenu));
}


