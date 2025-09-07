#include "../predator_i.h"
#include "../helpers/predator_esp32.h"
#include "../helpers/predator_ui_elements.h"
#include "predator_scene.h"

typedef enum {
    WifiMenuStateList,
    WifiMenuStateConfirm
} WifiMenuState;

typedef struct {
    View* view;
    WifiMenuState state;
    uint8_t selected_index;
    uint8_t items_count;
    char confirm_text[128];
} WifiAttacksMenuView;

// Menu items definition
typedef struct {
    const char* name;
    const char* icon;
    uint8_t index;
} WifiMenuItem;

// Menu items
static const WifiMenuItem wifi_items[] = {
    {"WiFi Scanner", "📡", SubmenuIndexWifiScan},
    {"Deauth Attack", "💥", SubmenuIndexWifiDeauth},
    {"Evil Twin AP", "👥", SubmenuIndexWifiEvilTwin},
    {"Handshake Capture", "🔑", SubmenuIndexWifiHandshakeCapture},
    {"Pwnagotchi Mode", "🤖", SubmenuIndexWifiPwnagotchi}
};

#define WIFI_ITEMS_COUNT (sizeof(wifi_items) / sizeof(wifi_items[0]))
#define ITEMS_ON_SCREEN 4

static void wifi_attacks_menu_draw_callback(Canvas* canvas, void* context) {
    PredatorApp* app = context;
    
    if(!app) return;
    
    // Get view state
    WifiAttacksMenuView* state = view_get_model(app->view_dispatcher->current_view);
    if(!state) return;
    
    canvas_clear(canvas);
    
    // Handle different states
    if(state->state == WifiMenuStateConfirm) {
        // Draw confirmation screen
        canvas_set_font(canvas, FontPrimary);
        canvas_draw_str_aligned(canvas, 64, 8, AlignCenter, AlignCenter, "Confirm Action");
        
        // Draw separator
        canvas_draw_line(canvas, 0, 16, 128, 16);
        
        // Draw confirmation box
        predator_ui_draw_status_box(canvas, "Warning", 8, 22, 112, 34);
        
        canvas_set_font(canvas, FontSecondary);
        // Draw multiline text centered in box
        canvas_draw_str_aligned(canvas, 64, 32, AlignCenter, AlignTop, state->confirm_text);
        
        // Draw buttons
        elements_button_left(canvas, "Cancel");
        elements_button_right(canvas, "OK");
        
        return;
    }
    
    // Draw menu title
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, 64, 8, AlignCenter, AlignCenter, "WiFi Attacks");
    
    // Draw separator
    canvas_draw_line(canvas, 0, 16, 128, 16);
    
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
    predator_ui_draw_status_box(canvas, "Attack Type", 8, 22, 112, 48);
    
    // Draw menu items
    for(uint8_t i = 0; i < ITEMS_ON_SCREEN && (i + scroll_position) < state->items_count; i++) {
        uint8_t item_idx = i + scroll_position;
        const WifiMenuItem* item = &wifi_items[item_idx];
        
        uint8_t y_pos = 32 + (i * 10);
        
        // Highlight selected item
        if(item_idx == state->selected_index) {
            canvas_draw_box(canvas, 12, y_pos - 1, 104, 10);
            canvas_set_color(canvas, ColorWhite);
        }
        
        // Draw icon and name
        canvas_draw_str(canvas, 14, y_pos + 6, item->icon);
        canvas_draw_str(canvas, 28, y_pos + 6, item->name);
        
        // Reset color
        canvas_set_color(canvas, ColorBlack);
    }
    
    // Draw scroll indicators if needed
    if(scroll_position > 0) {
        canvas_draw_icon(canvas, 112, 24, &I_ButtonUp_4x7);
    }
    
    if(scroll_position + ITEMS_ON_SCREEN < state->items_count) {
        canvas_draw_icon(canvas, 112, 60, &I_ButtonDown_4x7);
    }
    
    // Draw controls
    elements_button_left(canvas, "Back");
    elements_button_right(canvas, "OK");
}

static bool wifi_attacks_menu_input_callback(InputEvent* event, void* context) {
    PredatorApp* app = context;
    bool consumed = false;
    
    // Get view state
    WifiAttacksMenuView* state = view_get_model(app->view_dispatcher->current_view);
    if(!state) return consumed;
    
    if(event->type == InputTypeShort || event->type == InputTypeRepeat) {
        if(state->state == WifiMenuStateList) {
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
                    uint8_t item_index = wifi_items[state->selected_index].index;
                    
                    // Special handling for attacks that need confirmation
                    if(item_index == SubmenuIndexWifiDeauth) {
                        // Show confirmation for deauth attack
                        snprintf(state->confirm_text, sizeof(state->confirm_text),
                            "Deauth attack will disconnect\n"
                            "all clients from nearby\n"
                            "WiFi networks.\n\n"
                            "Continue?");
                        state->state = WifiMenuStateConfirm;
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
        } else if(state->state == WifiMenuStateConfirm) {
            // Confirmation screen controls
            switch(event->key) {
            case InputKeyBack:
            case InputKeyLeft:
                // Cancel and return to menu
                state->state = WifiMenuStateList;
                consumed = true;
                break;
                
            case InputKeyOk:
            case InputKeyRight:
                // Confirm and proceed
                state->state = WifiMenuStateList;
                view_dispatcher_send_custom_event(
                    app->view_dispatcher, wifi_items[state->selected_index].index);
                consumed = true;
                break;
                
            default:
                break;
            }
        }
    }
    
    return consumed;
}

static View* wifi_attacks_menu_view_alloc(PredatorApp* app) {
    View* view = view_alloc();
    view_set_context(view, app);
    view_set_draw_callback(view, wifi_attacks_menu_draw_callback);
    view_set_input_callback(view, wifi_attacks_menu_input_callback);
    
    // Initialize model
    WifiAttacksMenuView* state = malloc(sizeof(WifiAttacksMenuView));
    state->state = WifiMenuStateList;
    state->selected_index = 0;
    state->items_count = WIFI_ITEMS_COUNT;
    memset(state->confirm_text, 0, sizeof(state->confirm_text));
    
    view_set_model(view, state);
    view_set_model_free_callback(view, free);
    
    return view;
}

static void wifi_attacks_menu_view_free(View* view) {
    view_free(view);
}

void predator_scene_wifi_attacks_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    // Create custom view
    View* view = wifi_attacks_menu_view_alloc(app);
    
    // Replace submenu view with custom view
    view_dispatcher_remove_view(app->view_dispatcher, PredatorViewSubmenu);
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewSubmenu, view);
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
    
    // Restore previous selection if available
    WifiAttacksMenuView* state = view_get_model(view);
    if(state) {
        state->selected_index = scene_manager_get_scene_state(app->scene_manager, PredatorSceneWifiAttacks);
    }
}

bool predator_scene_wifi_attacks_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    // Get view state
    View* view = view_dispatcher_get_current_view(app->view_dispatcher);
    WifiAttacksMenuView* state = view_get_model(view);
    
    if(event.type == SceneManagerEventTypeCustom) {
        // Save the selected index for when returning to this scene
        if(state) {
            scene_manager_set_scene_state(app->scene_manager, PredatorSceneWifiAttacks, state->selected_index);
        }
        
        // Handle menu selection - these should match the original scene indexes
        consumed = true;
        switch(event.event) {
        case SubmenuIndexWifiScan:
            scene_manager_next_scene(app->scene_manager, PredatorSceneWifiScan);
            break;
        case SubmenuIndexWifiDeauth:
            scene_manager_next_scene(app->scene_manager, PredatorSceneWifiDeauth);
            break;
        case SubmenuIndexWifiEvilTwin:
            scene_manager_next_scene(app->scene_manager, PredatorSceneWifiEvilTwin);
            break;
        case SubmenuIndexWifiHandshakeCapture:
            // Show handshake capture popup
            scene_manager_next_scene(app->scene_manager, PredatorSceneWifiHandshakeCapture);
            break;
        case SubmenuIndexWifiPwnagotchi:
            // Show pwnagotchi mode popup
            scene_manager_next_scene(app->scene_manager, PredatorSceneWifiPwnagotchi);
            break;
        default:
            consumed = false;
            break;
        }
    } else if(event.type == SceneManagerEventTypeBack) {
        // If in confirmation screen, return to menu instead of leaving scene
        if(state && state->state == WifiMenuStateConfirm) {
            state->state = WifiMenuStateList;
            consumed = true;
        }
    }
    
    return consumed;
}

void predator_scene_wifi_attacks_new_on_exit(void* context) {
    PredatorApp* app = context;
    
    // Remove and free custom view
    view_dispatcher_remove_view(app->view_dispatcher, PredatorViewSubmenu);
    View* view = view_dispatcher_get_current_view(app->view_dispatcher);
    if(view) {
        wifi_attacks_menu_view_free(view);
    }
    
    // Restore standard submenu view
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewSubmenu, submenu_get_view(app->submenu));
}
