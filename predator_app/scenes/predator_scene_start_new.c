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
    
    if(event.type == SceneManagerEventTypeCustom) {
        // Selection state is handled by static variables in callbacks
        // Navigate to the selected scene - same logic as original
        consumed = true;
        switch(event.event) {
        case SubmenuIndexWifiAttacks:
            scene_manager_next_scene(app->scene_manager, 1); // WifiAttacks
            break;
        case SubmenuIndexBluetoothAttacks:
            scene_manager_next_scene(app->scene_manager, 5); // BluetoothAttacks
            break;
        case SubmenuIndexSubGhzAttacks:
            scene_manager_next_scene(app->scene_manager, 8); // SubghzAttacks
            break;
        case SubmenuIndexCarAttacks:
            scene_manager_next_scene(app->scene_manager, 9); // CarAttacks
            break;
        case SubmenuIndexRfidAttacks:
            scene_manager_next_scene(app->scene_manager, 15); // RfidAttacks
            break;
        case SubmenuIndexGpsTracker:
            scene_manager_next_scene(app->scene_manager, 18); // GpsTracker
            break;
        case SubmenuIndexWardriving:
            scene_manager_next_scene(app->scene_manager, 20); // Wardriving
            break;
        case SubmenuIndexSocialEngineering:
            scene_manager_next_scene(app->scene_manager, 21); // SocialEngineering
            break;
        case SubmenuIndexModuleStatus:
            scene_manager_next_scene(app->scene_manager, 22); // ModuleStatus
            break;
        case SubmenuIndexBoardSelection:
            scene_manager_next_scene(app->scene_manager, 24); // BoardSelection
            break;
        case SubmenuIndexSettings:
            scene_manager_next_scene(app->scene_manager, 23); // Settings
            break;
        case SubmenuIndexAbout:
            scene_manager_next_scene(app->scene_manager, 25); // About
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
