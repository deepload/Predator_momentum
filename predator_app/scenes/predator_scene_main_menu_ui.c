#include "../predator_i.h"
#include "predator_scene.h"
#include "../helpers/predator_ui_clean.h"
#include "predator_submenu_index.h"

// Main Menu - Professional UI (Memory Optimized, No Guards)
// Clean submenu implementation for Tesla demo

static void main_menu_submenu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    if(!app || !app->view_dispatcher) return;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void predator_scene_main_menu_ui_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app || !app->submenu) return;
    
    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "🔧 PREDATOR Optimized");
    
    // PROFESSIONAL - OPTIMIZED FOR MEMORY & FUNCTIONALITY
    submenu_add_item(app->submenu, "🚗 Tesla Security", 1, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "🚗 Car Models", 2, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "🚧 Parking Barriers", 3, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "📡 WiFi Attacks", 4, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "📱 Bluetooth Attacks", 5, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "💳 RFID Attacks", 6, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "📻 SubGHz Attacks", 7, main_menu_submenu_callback, app);
    // submenu_add_item(app->submenu, "🛰️ GPS Tracker", 8, main_menu_submenu_callback, app);  // Temp disabled
    // submenu_add_item(app->submenu, "🚙 Wardriving", 9, main_menu_submenu_callback, app);  // Temp disabled
    // submenu_add_item(app->submenu, "🎭 Social Engineering", 10, main_menu_submenu_callback, app);  // Temp disabled
    submenu_add_item(app->submenu, "📊 Module Status", 8, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "🔧 Board Selection", 12, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "⚙️ Settings", 13, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "ℹ️ About", 14, main_menu_submenu_callback, app);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

bool predator_scene_main_menu_ui_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    // Handle back button - SAFE exit app only from main menu
    if(event.type == SceneManagerEventTypeBack) {
        // CRITICAL: Only exit from main menu - use safe exit pattern
        PREDATOR_SAFE_EXIT_APP(app);
        return true;
    }
    
    if(event.type == SceneManagerEventTypeCustom) {
        consumed = true;
        switch(event.event) {
        case 1: // Tesla Security
            scene_manager_next_scene(app->scene_manager, PredatorSceneCarTeslaUI);
            return true;
        case 2: // Car Models
            scene_manager_next_scene(app->scene_manager, PredatorSceneCarModelsUI);
            return true;
        case 3: // Parking Barriers
            scene_manager_next_scene(app->scene_manager, PredatorSceneParkingBarriersUI);
            return true;
        case 4: // WiFi Attacks
            scene_manager_next_scene(app->scene_manager, PredatorSceneWifiAttacksUI);
            return true;
        case 5: // Bluetooth Attacks
            scene_manager_next_scene(app->scene_manager, PredatorSceneBluetoothAttacksUI);
            return true;
        case 6: // RFID Attacks
            scene_manager_next_scene(app->scene_manager, PredatorSceneRfidAttacksUI);
            return true;
        case 7: // SubGHz Attacks
            scene_manager_next_scene(app->scene_manager, PredatorSceneSubGhzAttacksUI);
            return true;
        case 8: // Module Status
            scene_manager_next_scene(app->scene_manager, PredatorSceneModuleStatusUI);
            return true;
        case 12: // Board Selection
            scene_manager_next_scene(app->scene_manager, PredatorSceneBoardSelectionUI);
            return true;
        case 13: // Settings
            scene_manager_next_scene(app->scene_manager, PredatorSceneSettingsUI);
            return true;
        case 14: // About
            scene_manager_next_scene(app->scene_manager, PredatorSceneAboutUI);
            return true;
        default:
            consumed = false;
            break;
        }
    }
    
    return consumed;
}

void predator_scene_main_menu_ui_on_exit(void* context) {
    PredatorApp* app = context;
    if(app && app->submenu) {
        submenu_reset(app->submenu);
    }
}
