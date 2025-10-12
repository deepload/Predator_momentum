#include "../predator_i.h"
#include "predator_scene.h"
#include "predator_submenu_index.h"

// Main Menu - Professional UI
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
    submenu_add_item(app->submenu, "🚗 Universal Car Hacker", 2, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "📡 WiFi Attacks", 3, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "📱 Bluetooth Attacks", 4, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "💳 RFID Attacks", 5, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "📻 SubGHz Attacks", 6, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "🛰️ GPS Tracker", 7, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "🚙 Wardriving", 8, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "🎭 Social Engineering", 9, main_menu_submenu_callback, app);
    // submenu_add_item(app->submenu, "🚦 Traffic Light Security", 10, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "📊 Module Status", 10, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "🔧 Board Selection", 11, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "⚙️ Settings", 12, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "ℹ️ About", 13, main_menu_submenu_callback, app);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

bool predator_scene_main_menu_ui_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    // Handle back button - exit app only from main menu
    if(event.type == SceneManagerEventTypeBack) {
        // From main menu, we exit the app
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
        // case 10: // Traffic Light Security - Temporarily disabled
        //     scene_manager_next_scene(app->scene_manager, PredatorSceneTrafficLightSecurityUI);
        //     return true;
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

void predator_scene_main_menu_ui_on_exit(void* context) {
    PredatorApp* app = context;
    if(app && app->submenu) {
        submenu_reset(app->submenu);
    }
}
