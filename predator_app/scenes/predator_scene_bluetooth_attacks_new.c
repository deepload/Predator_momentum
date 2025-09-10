#include "../predator_i.h"\n#include "../helpers/predator_view_helpers.h"
#include "../helpers/predator_ui_elements.h"
#include "predator_scene.h"

typedef struct {
    View* view;
    uint8_t selected_index;
    uint8_t scroll_position;
    uint8_t menu_items_count;
} BluetoothAttacksMenuView;

// Menu item definitions
typedef struct {
    const char* name;
    const char* icon;
    uint8_t submenu_index;
} BluetoothMenuItem;

// Menu items array - match the original enum SubmenuIndex order!
static const BluetoothMenuItem menu_items[] = {
    {"BLE Scanner", "📱", SubmenuIndexBleScan},
    {"BLE Spam", "📶", SubmenuIndexBleSpam},
};

#define MENU_ITEMS_COUNT (sizeof(menu_items) / sizeof(menu_items[0]))
#define ITEMS_ON_SCREEN 4

static void bluetooth_attacks_menu_draw_callback(Canvas* canvas, void* context) {
    PredatorApp* app = context;
    
    if(!app) return;
    
    // Get view state
    BluetoothAttacksMenuView* state = PREDATOR_GET_MODEL(app->view_dispatcher, BluetoothAttacksMenuView);
    if(!state) return;
    
    canvas_clear(canvas);
    
    // Draw title
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, 64, 8, AlignCenter, AlignCenter, "Bluetooth Attacks");
    
    // Draw separator
    canvas_draw_line(canvas, 0, 16, 128, 16);
    
    // Draw Bluetooth logo visualization
    uint8_t bt_x = 24;
    uint8_t bt_y = 34;
    uint8_t bt_size = 24;
    
    // Draw BT logo outline
    canvas_draw_box(canvas, bt_x - 4, bt_y - bt_size/2, 8, bt_size);
    
    // Draw the pointed part at the top
    canvas_draw_line(canvas, bt_x, bt_y - bt_size/2, bt_x + bt_size/3, bt_y - bt_size/4);
    canvas_draw_line(canvas, bt_x + bt_size/3, bt_y - bt_size/4, bt_x, bt_y);
    
    // Draw the pointed part at the bottom
    canvas_draw_line(canvas, bt_x, bt_y, bt_x + bt_size/3, bt_y + bt_size/4);
    canvas_draw_line(canvas, bt_x + bt_size/3, bt_y + bt_size/4, bt_x, bt_y + bt_size/2);
    
    // Draw scrolling menu
    canvas_set_font(canvas, FontSecondary);
    
    // Calculate visible range based on selection and scroll position
    for(uint8_t i = 0; i < ITEMS_ON_SCREEN && (i + state->scroll_position) < state->menu_items_count; i++) {
        uint8_t item_idx = i + state->scroll_position;
        const BluetoothMenuItem* item = &menu_items[item_idx];
        
        uint8_t y_pos = 26 + 12 * i;
        
        // Draw selection indicator or space for non-selected items
        if(item_idx == state->selected_index) {
            elements_slightly_rounded_box(canvas, 50, y_pos - 1, 70, 11);
            canvas_set_color(canvas, ColorWhite);
        } else {
            canvas_set_color(canvas, ColorBlack);
        }
        
        // Draw icon
        canvas_draw_str(canvas, 53, y_pos + 4, item->icon);
        
        // Draw menu item text
        canvas_draw_str(canvas, 65, y_pos + 4, item->name);
        
        // Reset color
        canvas_set_color(canvas, ColorBlack);
    }
    
    // Draw attack description box
    predator_ui_draw_status_box(canvas, "Description", 10, 60, 108, 24);
    
    // Show description based on selection
    canvas_set_font(canvas, FontSecondary);
    
    if(state->selected_index == 0) {
        // BLE Scanner
        canvas_draw_str(canvas, 14, 70, "Scan for nearby Bluetooth");
        canvas_draw_str(canvas, 14, 80, "Low Energy devices");
    } else {
        // BLE Spam
        canvas_draw_str(canvas, 14, 70, "Send BLE advertisements to");
        canvas_draw_str(canvas, 14, 80, "appear as multiple devices");
    }
    
    // Draw controls
    elements_button_left(canvas, "Back");
    elements_button_right(canvas, "OK");
}

static bool bluetooth_attacks_menu_input_callback(InputEvent* event, void* context) {
    PredatorApp* app = context;
    bool consumed = false;
    
    // Get view state
    BluetoothAttacksMenuView* state = PREDATOR_GET_MODEL(app->view_dispatcher, BluetoothAttacksMenuView);
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

static View* bluetooth_attacks_menu_view_alloc(PredatorApp* app) {
    View* view = view_alloc();
    
    // Initialize the model
    BluetoothAttacksMenuView* state = malloc(sizeof(BluetoothAttacksMenuView));
    state->selected_index = 0;
    state->scroll_position = 0;
    state->menu_items_count = MENU_ITEMS_COUNT;
    
    // Set view callbacks
    view_set_context(view, app);
    view_set_draw_callback(view, bluetooth_attacks_menu_draw_callback);
    view_set_input_callback(view, bluetooth_attacks_menu_input_callback);
    
    // Set model and free callback
    predator_predator_view_set_model(view, state);
    predator_predator_view_set_model_free_callback(view, free);
    
    return view;
}

static void bluetooth_attacks_menu_view_free(View* view) {
    view_free(view);
}

void predator_scene_bluetooth_attacks_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    // Create custom view
    View* view = bluetooth_attacks_menu_view_alloc(app);
    
    // Replace submenu view with custom view
    view_dispatcher_remove_view(app->view_dispatcher, PredatorViewSubmenu);
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewSubmenu, view);
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

bool predator_scene_bluetooth_attacks_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        // Handle menu selection
        consumed = true;
        switch(event.event) {
        case SubmenuIndexBleScan:
            scene_manager_next_scene(app->scene_manager, PredatorSceneBleScan);
            break;
        case SubmenuIndexBleSpam:
            scene_manager_next_scene(app->scene_manager, PredatorSceneBleScan);
            break;
        default:
            consumed = false;
            break;
        }
    }

    return consumed;
}

void predator_scene_bluetooth_attacks_new_on_exit(void* context) {
    PredatorApp* app = context;
    
    // Remove and free custom view
    view_dispatcher_remove_view(app->view_dispatcher, PredatorViewSubmenu);
    View* view = predator_predator_view_dispatcher_get_current_view(app->view_dispatcher);
    if(view) {
        bluetooth_attacks_menu_view_free(view);
    }
    
    // Restore standard submenu view
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewSubmenu, submenu_get_view(app->submenu));
}



