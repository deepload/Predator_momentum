#include "../predator_i.h"
#include "predator_scene.h"
#include "predator_submenu_index.h"

// Bluetooth Attacks Submenu - Professional UI
static void bluetooth_attacks_submenu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    if(!app || !app->view_dispatcher) return;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void predator_scene_bluetooth_attacks_ui_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app || !app->submenu) return;
    
    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "📱 Bluetooth Attacks");
    
    submenu_add_item(app->submenu, "📱 BLE Scan", SubmenuIndexBleScan, bluetooth_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "💥 BLE Spam", SubmenuIndexBleSpam, bluetooth_attacks_submenu_callback, app);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

bool predator_scene_bluetooth_attacks_ui_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    // Handle back button - return to main menu
    if(event.type == SceneManagerEventTypeBack) {
        // Return false to let scene manager navigate back
        return false;
    }
    
    if(event.type == SceneManagerEventTypeCustom) {
        consumed = true;
        switch(event.event) {
        case SubmenuIndexBleScan:
            scene_manager_next_scene(app->scene_manager, PredatorSceneBleScanUI);
            break;
        case SubmenuIndexBleSpam:
            scene_manager_next_scene(app->scene_manager, PredatorSceneBleSpamUI);
            break;
        default:
            consumed = false;
            break;
        }
    }
    
    return consumed;
}

void predator_scene_bluetooth_attacks_ui_on_exit(void* context) {
    PredatorApp* app = context;
    if(app && app->submenu) {
        submenu_reset(app->submenu);
    }
}
