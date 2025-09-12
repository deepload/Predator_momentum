#include "../predator_i.h"
#include "predator_scene.h"
#include "predator_submenu_index.h"
#include "../helpers/predator_ui_elements.h"
#include <furi.h>

// Submenu callback for navigation
static void bluetooth_attacks_submenu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void predator_scene_bluetooth_attacks_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    // Use standard submenu to avoid NULL pointer issues
    submenu_reset(app->submenu);
    
    // Add Bluetooth attack menu items
    submenu_add_item(app->submenu, "📱 BLE Scanner", SubmenuIndexBleScan, bluetooth_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "📶 BLE Spam", SubmenuIndexBleSpam, bluetooth_attacks_submenu_callback, app);
    
    submenu_set_header(app->submenu, "Bluetooth Attacks");
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

bool predator_scene_bluetooth_attacks_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(event.type == SceneManagerEventTypeCustom) {
        consumed = true;
        switch(event.event) {
        case SubmenuIndexBleScan:
            scene_manager_next_scene(app->scene_manager, 6); // BleScan
            break;
        case SubmenuIndexBleSpam:
            scene_manager_next_scene(app->scene_manager, 7); // BleSpam
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
    
    // Clean up submenu
    submenu_reset(app->submenu);
}



