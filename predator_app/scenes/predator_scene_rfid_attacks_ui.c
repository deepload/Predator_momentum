#include "../predator_i.h"
#include "predator_scene.h"
#include "predator_submenu_index.h"

// RFID Attacks Submenu - Professional UI
static void rfid_attacks_submenu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    if(!app || !app->view_dispatcher) return;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void predator_scene_rfid_attacks_ui_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app || !app->submenu) return;
    
    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "💳 RFID/NFC Attacks");
    
    // AUTOMATED CLONING
    submenu_add_item(app->submenu, "🤖 Auto Card Clone", 10, rfid_attacks_submenu_callback, app);
    
    // GOVERNMENT-GRADE CARD SUPPORT
    submenu_add_item(app->submenu, "🎫 Calypso Cards", 1, rfid_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "🚇 MIFARE Classic", 2, rfid_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "🏢 MIFARE DESFire", 3, rfid_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "🏦 EMV Payment Cards", 4, rfid_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "🆔 HID Prox Cards", 5, rfid_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "🔑 EM4100/4102", 6, rfid_attacks_submenu_callback, app);
    
    // GENERIC ATTACKS
    submenu_add_item(app->submenu, "💳 Generic Clone", SubmenuIndexRfidClone, rfid_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "🔓 Bruteforce Attack", SubmenuIndexRfidBruteforce, rfid_attacks_submenu_callback, app);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

bool predator_scene_rfid_attacks_ui_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    // Handle back button - return to main menu
    if(event.type == SceneManagerEventTypeBack) {
        scene_manager_previous_scene(app->scene_manager);
        return true;  // Consumed - prevents framework bug
    }
    
    if(event.type == SceneManagerEventTypeCustom) {
        consumed = true;
        switch(event.event) {
        case 10: // Auto Card Clone
            scene_manager_next_scene(app->scene_manager, PredatorSceneAutoCardCloneUI);
            break;
        case 1: // Calypso Cards
            scene_manager_next_scene(app->scene_manager, PredatorSceneCalypsoCardsUI);
            break;
        case 2: // MIFARE Classic
            scene_manager_next_scene(app->scene_manager, PredatorSceneRfidCloneUI);
            break;
        case 3: // MIFARE DESFire
            scene_manager_next_scene(app->scene_manager, PredatorSceneRfidCloneUI);
            break;
        case 4: // EMV Payment Cards
            scene_manager_next_scene(app->scene_manager, PredatorSceneRfidCloneUI);
            break;
        case 5: // HID Prox Cards
            scene_manager_next_scene(app->scene_manager, PredatorSceneRfidCloneUI);
            break;
        case 6: // EM4100/4102
            scene_manager_next_scene(app->scene_manager, PredatorSceneRfidCloneUI);
            break;
        case SubmenuIndexRfidClone: // Generic Clone
            scene_manager_next_scene(app->scene_manager, PredatorSceneRfidCloneUI);
            break;
        case SubmenuIndexRfidBruteforce: // Bruteforce Attack
            scene_manager_next_scene(app->scene_manager, PredatorSceneRfidBruteforceUI);
            break;
        default:
            consumed = false;
            break;
        }
    }
    
    return consumed;
}

void predator_scene_rfid_attacks_ui_on_exit(void* context) {
    PredatorApp* app = context;
    if(app && app->submenu) {
        submenu_reset(app->submenu);
    }
}
