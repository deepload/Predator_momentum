#include "../predator_i.h"
#include "../helpers/predator_crypto1.h"  // REAL IMPLEMENTATION
#include "../helpers/predator_logging.h"
#include "predator_scene.h"
#include "predator_submenu_index.h"

// Advanced NFC/RFID Attacks Menu
static void advanced_nfc_submenu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    if(!app) return;
    
    switch(index) {
        case SubmenuIndexMifareNested:
            // REAL MIFARE NESTED ATTACK
            predator_crypto1_nested_attack(app, 1);
            predator_log_append(app, "MIFARE: Nested attack executed");
            break;
        case SubmenuIndexMifareDarkside:
            // REAL MIFARE DARKSIDE ATTACK
            predator_crypto1_darkside_attack(app);
            predator_log_append(app, "MIFARE: Darkside attack executed");
            break;
        case SubmenuIndexMifareHardnested:
        case SubmenuIndexDESFire:
        case SubmenuIndexNTAGCrack:
        case SubmenuIndexEMVRelay:
        case SubmenuIndexHIDiCLASS:
            FURI_LOG_I("AdvancedNFC", "Selected: %lu (not yet implemented)", index);
            break;
    }
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
    UNUSED(context);
    UNUSED(event);
    return false;
}

void predator_scene_advanced_nfc_ui_on_exit(void* context) {
    PredatorApp* app = context;
    if(app && app->submenu) {
        submenu_reset(app->submenu);
    }
}
