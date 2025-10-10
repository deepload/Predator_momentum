#include "../predator_i.h"
#include "predator_scene.h"
#include "predator_submenu_index.h"

// Advanced Bluetooth Attacks Menu
static void advanced_bt_submenu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    if(!app || !app->view_dispatcher) return;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void predator_scene_advanced_bt_ui_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app || !app->submenu) return;
    
    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "Advanced Bluetooth");
    
    submenu_add_item(app->submenu, "BLE GATT Fuzzing", SubmenuIndexBLEGATTFuzz, advanced_bt_submenu_callback, app);
    submenu_add_item(app->submenu, "BLE MITM Attack", SubmenuIndexBLEMITM, advanced_bt_submenu_callback, app);
    submenu_add_item(app->submenu, "BLE Relay Attack", SubmenuIndexBLERelay, advanced_bt_submenu_callback, app);
    submenu_add_item(app->submenu, "Pairing Bypass", SubmenuIndexBLEPairingBypass, advanced_bt_submenu_callback, app);
    submenu_add_item(app->submenu, "HID Injection", SubmenuIndexBLEHIDInject, advanced_bt_submenu_callback, app);
    submenu_add_item(app->submenu, "Bluebug Attack", SubmenuIndexBluebug, advanced_bt_submenu_callback, app);
    submenu_add_item(app->submenu, "Bluesnarf Attack", SubmenuIndexBluesnarf, advanced_bt_submenu_callback, app);
    submenu_add_item(app->submenu, "Device Fingerprint", SubmenuIndexBTFingerprint, advanced_bt_submenu_callback, app);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

bool predator_scene_advanced_bt_ui_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(event.type == SceneManagerEventTypeCustom) {
        consumed = true;
        FURI_LOG_I("AdvancedBT", "Selected: %lu", event.event);
    }
    
    UNUSED(app);
    return consumed;
}

void predator_scene_advanced_bt_ui_on_exit(void* context) {
    PredatorApp* app = context;
    if(app && app->submenu) {
        submenu_reset(app->submenu);
    }
}
