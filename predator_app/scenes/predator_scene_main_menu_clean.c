#include "../predator_i.h"
#include "predator_scene.h"
#include "../helpers/predator_ui_clean.h"
#include <gui/view.h>

// CLEAN MAIN MENU - Professional scrollable interface

static const char* main_menu_items[] = {
    "🚗 Tesla Security",
    "🚗 Universal Car Hacker", 
    "📡 WiFi Attacks",
    "📱 Bluetooth Attacks",
    "💳 RFID Attacks",
    "📻 SubGHz Attacks",
    "🛰️ GPS Tracker",
    "🚙 Wardriving",
    "🎭 Social Engineering",
    "📊 Module Status",
    "🔧 Board Selection",
    "⚙️ Settings",
    "ℹ️ About"
};

static const uint8_t main_menu_count = sizeof(main_menu_items) / sizeof(main_menu_items[0]);
static PredatorScrollableMenu main_menu;

static void main_menu_clean_draw_callback(Canvas* canvas, void* context) {
    if(!canvas) return;
    
    canvas_clear(canvas);
    
    // Draw professional header
    predator_ui_draw_header(canvas, "PREDATOR Security Suite");
    
    // Draw scrollable menu
    predator_ui_draw_scrollable_menu(canvas, &main_menu);
    
    // Draw navigation help
    predator_ui_draw_status_bar(canvas, "↑↓=Navigate  OK=Select  Back=Exit");
}

static bool main_menu_clean_input_callback(InputEvent* event, void* context) {
    PredatorApp* app = context;
    if(!app) return false;
    
    // Handle menu navigation
    if(predator_ui_handle_menu_input(&main_menu, event)) {
        return true;
    }
    
    // Handle selection
    if(event->type == InputTypeShort && event->key == InputKeyOk) {
        uint32_t selected = main_menu.selected_index + 1; // Convert to 1-based index
        view_dispatcher_send_custom_event(app->view_dispatcher, selected);
        return true;
    }
    
    // Handle back button
    if(event->type == InputTypeShort && event->key == InputKeyBack) {
        return false; // Let scene manager handle exit
    }
    
    return true;
}

void predator_scene_main_menu_clean_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app || !app->view_dispatcher) return;
    
    // Initialize clean menu
    main_menu.items = main_menu_items;
    main_menu.item_count = main_menu_count;
    main_menu.selected_index = 0;
    
    // Create custom view
    View* view = view_alloc();
    if(!view) return;
    
    view_set_context(view, app);
    view_set_draw_callback(view, main_menu_clean_draw_callback);
    view_set_input_callback(view, main_menu_clean_input_callback);
    
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewMainMenuClean, view);
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewMainMenuClean);
}

bool predator_scene_main_menu_clean_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    // Handle back button - exit app only from main menu
    if(event.type == SceneManagerEventTypeBack) {
        scene_manager_stop(app->scene_manager);
        view_dispatcher_stop(app->view_dispatcher);
        return true;
    }
    
    if(event.type == SceneManagerEventTypeCustom) {
        consumed = true;
        switch(event.event) {
        case 1: // Tesla Security
            scene_manager_next_scene(app->scene_manager, PredatorSceneCarTeslaUI);
            return true;
        case 2: // Universal Car Hacker
            scene_manager_next_scene(app->scene_manager, PredatorSceneUniversalCarHacker);
            return true;
        case 3: // WiFi Attacks
            scene_manager_next_scene(app->scene_manager, PredatorSceneWifiAttacksUI);
            return true;
        case 4: // Bluetooth Attacks
            scene_manager_next_scene(app->scene_manager, PredatorSceneBluetoothAttacksUI);
            return true;
        case 5: // RFID Attacks
            scene_manager_next_scene(app->scene_manager, PredatorSceneRfidAttacksUI);
            return true;
        case 6: // SubGHz Attacks
            scene_manager_next_scene(app->scene_manager, PredatorSceneSubGhzAttacksUI);
            return true;
        case 7: // GPS Tracker
            scene_manager_next_scene(app->scene_manager, PredatorSceneGpsTrackerUI);
            return true;
        case 8: // Wardriving
            scene_manager_next_scene(app->scene_manager, PredatorSceneWardrivingUI);
            return true;
        case 9: // Social Engineering
            scene_manager_next_scene(app->scene_manager, PredatorSceneSocialEngineeringUI);
            return true;
        case 10: // Module Status
            scene_manager_next_scene(app->scene_manager, PredatorSceneModuleStatusUI);
            return true;
        case 11: // Board Selection
            scene_manager_next_scene(app->scene_manager, PredatorSceneBoardSelectionUI);
            return true;
        case 12: // Settings
            scene_manager_next_scene(app->scene_manager, PredatorSceneSettingsUI);
            return true;
        case 13: // About
            scene_manager_next_scene(app->scene_manager, PredatorSceneAboutUI);
            return true;
        default:
            consumed = false;
            break;
        }
    }
    
    return consumed;
}

void predator_scene_main_menu_clean_on_exit(void* context) {
    PredatorApp* app = context;
    if(app && app->view_dispatcher) {
        view_dispatcher_remove_view(app->view_dispatcher, PredatorViewMainMenuClean);
    }
}
