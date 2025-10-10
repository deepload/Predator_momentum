#include "../predator_i.h"
#include "predator_scene.h"
#include "predator_submenu_index.h"

// Advanced NFC/RFID Attacks Menu
static void advanced_nfc_submenu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    if(!app || !app->view_dispatcher) return;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void predator_scene_advanced_nfc_ui_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app || !app->submenu) return;
    
    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "Advanced NFC/RFID");
    
    submenu_add_item(app->submenu, "MIFARE Nested", SubmenuIndexMifareNested, advanced_nfc_submenu_callback, app);
    submenu_add_item(app->submenu, "MIFARE Hardnested", SubmenuIndexMifareHardnested, advanced_nfc_submenu_callback, app);
    submenu_add_item(app->submenu, "MIFARE Darkside", SubmenuIndexMifareDarkside, advanced_nfc_submenu_callback, app);
    submenu_add_item(app->submenu, "DESFire Attacks", SubmenuIndexDESFire, advanced_nfc_submenu_callback, app);
    submenu_add_item(app->submenu, "NTAG Password Crack", SubmenuIndexNTAGCrack, advanced_nfc_submenu_callback, app);
    submenu_add_item(app->submenu, "EMV Relay Attack", SubmenuIndexEMVRelay, advanced_nfc_submenu_callback, app);
    submenu_add_item(app->submenu, "HID iCLASS Attack", SubmenuIndexHIDiCLASS, advanced_nfc_submenu_callback, app);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

bool predator_scene_advanced_nfc_ui_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(event.type == SceneManagerEventTypeCustom) {
        consumed = true;
        FURI_LOG_I("AdvancedNFC", "Selected: %lu", event.event);
    }
    
    UNUSED(app);
    return consumed;
}

void predator_scene_advanced_nfc_ui_on_exit(void* context) {
    PredatorApp* app = context;
    if(app && app->submenu) {
        submenu_reset(app->submenu);
    }
}
