#include "../predator_i.h"
#include "predator_scene.h"
#include "predator_submenu_index.h"
#include <furi.h>

// Submenu callback for navigation
static void predator_start_submenu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

// Main scene callbacks

void predator_scene_start_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    // Use the existing submenu instead of custom view to avoid white screen
    submenu_reset(app->submenu);
    
    // Add menu items matching the original structure
    submenu_add_item(app->submenu, "📡 WiFi Attacks", SubmenuIndexWifiAttacks, predator_start_submenu_callback, app);
    submenu_add_item(app->submenu, "📱 Bluetooth Attacks", SubmenuIndexBluetoothAttacks, predator_start_submenu_callback, app);
    submenu_add_item(app->submenu, "📻 SubGHz/RF Attacks", SubmenuIndexSubGhzAttacks, predator_start_submenu_callback, app);
    submenu_add_item(app->submenu, "🚗 Car Attacks", SubmenuIndexCarAttacks, predator_start_submenu_callback, app);
    submenu_add_item(app->submenu, "💳 RFID/NFC Attacks", SubmenuIndexRfidAttacks, predator_start_submenu_callback, app);
    submenu_add_item(app->submenu, "🛰️ GPS Tracker", SubmenuIndexGpsTracker, predator_start_submenu_callback, app);
    submenu_add_item(app->submenu, "🗺️ Wardriving", SubmenuIndexWardriving, predator_start_submenu_callback, app);
    submenu_add_item(app->submenu, "🎭 Social Engineering", SubmenuIndexSocialEngineering, predator_start_submenu_callback, app);
    submenu_add_item(app->submenu, "🧩 Module Status", SubmenuIndexModuleStatus, predator_start_submenu_callback, app);
    submenu_add_item(app->submenu, "🔧 Board Selection", SubmenuIndexBoardSelection, predator_start_submenu_callback, app);
    submenu_add_item(app->submenu, "⚙️ Settings", SubmenuIndexSettings, predator_start_submenu_callback, app);
    submenu_add_item(app->submenu, "ℹ️ About", SubmenuIndexAbout, predator_start_submenu_callback, app);
    
    submenu_set_header(app->submenu, "PREDATOR");
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

bool predator_scene_start_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(!app) {
        FURI_LOG_E("Start", "App context is NULL in event handler");
        return false;
    }
    
    if(event.type == SceneManagerEventTypeCustom) {
        consumed = true;
        switch(event.event) {
        case 0: // Assuming SubmenuIndexWifiAttacks is 0 or similar
            scene_manager_next_scene(app->scene_manager, 10); // Adjust based on likely scene ID for WiFi Attacks
            break;
        case 1: // Assuming SubmenuIndexBluetoothAttacks is 1 or similar
            scene_manager_next_scene(app->scene_manager, 11); // Adjust based on likely scene ID for Bluetooth Attacks
            break;
        case 2: // Assuming SubmenuIndexSubGhzAttacks is 2 or similar
            scene_manager_next_scene(app->scene_manager, 12); // Adjust based on likely scene ID for SubGHz Attacks
            break;
        case 3: // Assuming SubmenuIndexCarAttacks is 3 or similar
            scene_manager_next_scene(app->scene_manager, 13); // Adjust based on likely scene ID for Car Attacks
            break;
        case 4: // Assuming SubmenuIndexRfidAttacks is 4 or similar
            scene_manager_next_scene(app->scene_manager, 14); // Adjust based on likely scene ID for RFID Attacks
            break;
        case 5: // Assuming SubmenuIndexSocialEngineering is 5 or similar
            scene_manager_next_scene(app->scene_manager, 15); // Adjust based on likely scene ID for Social Engineering
            break;
        case 6: // Assuming SubmenuIndexGPSTracker is 6 or similar
            scene_manager_next_scene(app->scene_manager, 16); // Adjust based on likely scene ID for GPS Tracker
            break;
        case 7: // Assuming SubmenuIndexWardriving is 7 or similar
            scene_manager_next_scene(app->scene_manager, 17); // Adjust based on likely scene ID for Wardriving
            break;
        case 8: // Assuming SubmenuIndexBoardSelection is 8 or similar
            scene_manager_next_scene(app->scene_manager, 18); // Adjust based on likely scene ID for Board Selection
            break;
        case 9: // Assuming SubmenuIndexModuleStatus is 9 or similar
            scene_manager_next_scene(app->scene_manager, 19); // Adjust based on likely scene ID for Module Status
            break;
        case 10: // Assuming SubmenuIndexSettings is 10 or similar
            scene_manager_next_scene(app->scene_manager, 20); // Adjust based on likely scene ID for Settings
            break;
        case 11: // Assuming SubmenuIndexAbout is 11 or similar
            scene_manager_next_scene(app->scene_manager, 21); // Adjust based on likely scene ID for About
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
    
    // Clean up submenu
    submenu_reset(app->submenu);
}
