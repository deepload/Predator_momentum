#include "../predator_i.h"
#include "../helpers/predator_view_helpers.h"
#include "../helpers/predator_ui_elements.h"
#include "predator_scene.h"
#include "predator_submenu_index.h"
#include <furi.h>
#include <gui/canvas.h>
#include <gui/elements.h>

typedef struct {
    uint8_t selected_menu_index;
    uint8_t scroll_position;
    uint8_t menu_items_count;
} PredatorStartModel;

// Static data for the custom menu screen
static PredatorStartModel* model = NULL;

// Menu item definitions - match the original ones but with better styling
typedef struct {
    const char* name;
    const char* icon;
    uint8_t submenu_index;
} PredatorMenuItem;

// Menu items array - must match the original enum SubmenuIndex order!
static const PredatorMenuItem menu_items[] = {
    {"WiFi Attacks", "📡", SubmenuIndexWifiAttacks},
    {"Bluetooth Attacks", "📱", SubmenuIndexBluetoothAttacks},
    {"SubGHz/RF Attacks", "📻", SubmenuIndexSubGhzAttacks},
    {"Car Attacks", "🚗", SubmenuIndexCarAttacks},
    {"RFID/NFC Attacks", "💳", SubmenuIndexRfidAttacks},
    {"GPS Tracker", "🛰️", SubmenuIndexGpsTracker},
    {"Wardriving", "🗺️", SubmenuIndexWardriving},
    {"Social Engineering", "🎭", SubmenuIndexSocialEngineering},
    {"Module Status", "🧩", SubmenuIndexModuleStatus},
    {"Settings", "⚙️", SubmenuIndexSettings},
    {"About", "ℹ️", SubmenuIndexAbout},
};

#define MENU_ITEMS_COUNT (sizeof(menu_items) / sizeof(menu_items[0]))
#define MENU_Y_OFFSET 16

// Custom view callbacks
static void predator_start_draw_callback(Canvas* canvas, void* context) {
    PredatorApp* app = context;
    (void)app;
    canvas_clear(canvas);
    
    // Draw logo at the top
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, 64, 8, AlignCenter, AlignCenter, "PREDATOR");

    // Draw styled menu items with selection indicator
    canvas_set_font(canvas, FontSecondary);
    
    // Draw separator line
    elements_slightly_rounded_frame(canvas, 0, 16, 128, 48);

    // Calculate visible menu range based on selection and scroll position
    for(uint8_t i = 0; i < ITEMS_ON_SCREEN && (i + model->scroll_position) < model->menu_items_count; i++) {
        uint8_t item_idx = i + model->scroll_position;
        const PredatorMenuItem* item = &menu_items[item_idx];
        
        uint8_t y_pos = MENU_Y_OFFSET + 10 * i;
        
        // Draw selection indicator or space for non-selected items
        if(item_idx == model->selected_menu_index) {
            elements_slightly_rounded_box(canvas, 1, y_pos - 1, 126, 11);
            canvas_set_color(canvas, ColorWhite);
        } else {
            canvas_set_color(canvas, ColorBlack);
        }
        
        // Draw icon
        canvas_draw_str_aligned(canvas, 4, y_pos + 4, AlignLeft, AlignCenter, item->icon);
        
        // Draw menu item text
        canvas_draw_str_aligned(canvas, 14, y_pos + 4, AlignLeft, AlignCenter, item->name);
        
        // Reset color
        canvas_set_color(canvas, ColorBlack);
    }
    
    // Draw scroll indicators with vector chevrons
    predator_ui_draw_scroll_vertical(
        canvas,
        120,
        MENU_Y_OFFSET,
        MENU_Y_OFFSET + 40,
        model->scroll_position > 0,
        (model->scroll_position + ITEMS_ON_SCREEN) < model->menu_items_count);
    
    // Draw status/help text at the bottom
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str_aligned(
        canvas, 64, 63, AlignCenter, AlignBottom, "⬆️⬇️ to select, ➡️ to choose");
}

static bool predator_start_input_callback(InputEvent* event, void* context) {
    PredatorApp* app = context;
    bool consumed = false;
    
    // Navigation controls
    if(event->type == InputTypeShort || event->type == InputTypeRepeat) {
        switch(event->key) {
        case InputKeyUp:
            if(model->selected_menu_index > 0) {
                model->selected_menu_index--;
                
                // Adjust scroll position if needed
                if(model->selected_menu_index < model->scroll_position) {
                    model->scroll_position = model->selected_menu_index;
                }
                consumed = true;
            }
            break;
            
        case InputKeyDown:
            if(model->selected_menu_index < model->menu_items_count - 1) {
                model->selected_menu_index++;
                
                // Adjust scroll position if needed
                if(model->selected_menu_index >= model->scroll_position + ITEMS_ON_SCREEN) {
                    model->scroll_position = model->selected_menu_index - ITEMS_ON_SCREEN + 1;
                }
                consumed = true;
            }
            break;
            
        case InputKeyRight:
        case InputKeyOk:
            // Select the current item
            view_dispatcher_send_custom_event(
                app->view_dispatcher, menu_items[model->selected_menu_index].submenu_index);
            consumed = true;
            break;
            
        default:
            break;
        }
    }
    
    return consumed;
}

// View allocation and initialization
static View* predator_start_view_alloc(PredatorApp* app) {
    View* view = view_alloc();
    
    // Initialize the model
    if(!model) {
        model = malloc(sizeof(PredatorStartModel));
        model->selected_menu_index = 0;
        model->scroll_position = 0;
        model->menu_items_count = MENU_ITEMS_COUNT;
    }
    
    // Set view callbacks
    view_set_context(view, app);
    view_set_draw_callback(view, predator_start_draw_callback);
    view_set_input_callback(view, predator_start_input_callback);
    
    return view;
}

static void predator_start_view_free(View* view) {
    if(model) {
        free(model);
        model = NULL;
    }
    view_free(view);
}

// Main scene callbacks

void predator_scene_start_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    // Create a custom view instead of submenu
    View* view = predator_start_view_alloc(app);
    
    // Add it with a special view ID
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewWidget, view);
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewWidget);
    
    // If returning to this scene, restore previous selection
    if(model) {
        model->selected_menu_index = scene_manager_get_scene_state(
            app->scene_manager, PredatorSceneStart);
    }
}

bool predator_scene_start_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(event.type == SceneManagerEventTypeCustom) {
        // Save selected menu item for when we return to this scene
        scene_manager_set_scene_state(app->scene_manager, PredatorSceneStart, event.event);
        
        // Navigate to the selected scene - same logic as original
        consumed = true;
        switch(event.event) {
        case SubmenuIndexWifiAttacks:
            scene_manager_next_scene(app->scene_manager, PredatorSceneWifiAttacks);
            break;
        case SubmenuIndexBluetoothAttacks:
            scene_manager_next_scene(app->scene_manager, PredatorSceneBluetoothAttacks);
            break;
        case SubmenuIndexSubGhzAttacks:
            scene_manager_next_scene(app->scene_manager, PredatorSceneSubGhzAttacks);
            break;
        case SubmenuIndexCarAttacks:
            scene_manager_next_scene(app->scene_manager, PredatorSceneCarAttacks);
            break;
        case SubmenuIndexRfidAttacks:
            scene_manager_next_scene(app->scene_manager, PredatorSceneRfidAttacks);
            break;
        case SubmenuIndexGpsTracker:
            scene_manager_next_scene(app->scene_manager, PredatorSceneGPSTracker);
            break;
        case SubmenuIndexWardriving:
            scene_manager_next_scene(app->scene_manager, PredatorSceneWardriving);
            break;
        case SubmenuIndexSocialEngineering:
            scene_manager_next_scene(app->scene_manager, PredatorSceneAbout);
            break;
        case SubmenuIndexModuleStatus:
            scene_manager_next_scene(app->scene_manager, PredatorSceneModuleStatus);
            break;
        case SubmenuIndexSettings:
            scene_manager_next_scene(app->scene_manager, PredatorSceneSettings);
            break;
        case SubmenuIndexAbout:
            scene_manager_next_scene(app->scene_manager, PredatorSceneAbout);
            break;
        default:
            consumed = false;
            break;
        }
    }
    
    return consumed;
}

void predator_scene_start_new_on_exit(void* context) {
    PredatorApp* app = context;
    
    // Remove and free custom view
    view_dispatcher_remove_view(app->view_dispatcher, PredatorViewWidget);
    View* view = predator_view_dispatcher_get_current_view(app->view_dispatcher);
    if(view) {
        predator_start_view_free(view);
    }
}


