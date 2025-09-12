#include "../predator_i.h"
#include "predator_scene.h"
#include "predator_submenu_index.h"
#include "../helpers/predator_ui_elements.h"
#include <furi.h>

// Popup callback for RFID attacks
static void predator_scene_rfid_attacks_popup_callback(void* context) {
    PredatorApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, PredatorCustomEventPopupBack);
}

// Submenu callback for navigation
static void rfid_attacks_submenu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void predator_scene_rfid_attacks_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    // Use standard submenu to avoid NULL pointer issues
    submenu_reset(app->submenu);
    
    // Add RFID/NFC attack menu items
    submenu_add_item(app->submenu, "💳 RFID Clone", SubmenuIndexRfidClone, rfid_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "🔓 RFID Bruteforce", SubmenuIndexRfidBruteforce, rfid_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "🎲 RFID Fuzzing", SubmenuIndexRfidFuzzing, rfid_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "📱 NFC Clone", SubmenuIndexNfcClone, rfid_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "🔐 Mifare Hack", SubmenuIndexMifareHack, rfid_attacks_submenu_callback, app);
    
    submenu_set_header(app->submenu, "RFID/NFC Attacks");
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

bool predator_scene_rfid_attacks_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        consumed = true;
        switch(event.event) {
        case SubmenuIndexRfidClone:
            scene_manager_next_scene(app->scene_manager, PredatorSceneRfidClone);
            break;
        case SubmenuIndexRfidBruteforce:
            scene_manager_next_scene(app->scene_manager, PredatorSceneRfidBruteforce);
            break;
        case SubmenuIndexRfidFuzzing:
            // RFID Fuzzing attack - use popup
            popup_set_header(app->popup, "RFID Fuzzing", 64, 10, AlignCenter, AlignTop);
            popup_set_text(app->popup, "Fuzzing RFID protocols...\nTesting variations: 127\nPress Back to stop", 64, 25, AlignCenter, AlignTop);
            popup_set_context(app->popup, app);
            popup_set_callback(app->popup, predator_scene_rfid_attacks_popup_callback);
            popup_set_timeout(app->popup, 0);
            popup_enable_timeout(app->popup);
            
            app->attack_running = true;
            app->packets_sent = 0;
            
            view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
            break;
        case SubmenuIndexNfcClone:
            // NFC Clone - use popup
            popup_set_header(app->popup, "NFC Clone", 64, 10, AlignCenter, AlignTop);
            popup_set_text(app->popup, "Place NFC card on Flipper...\nReading NFC data\nPress Back to stop", 64, 25, AlignCenter, AlignTop);
            popup_set_context(app->popup, app);
            popup_set_callback(app->popup, predator_scene_rfid_attacks_popup_callback);
            popup_set_timeout(app->popup, 0);
            popup_enable_timeout(app->popup);
            
            app->attack_running = true;
            app->packets_sent = 0;
            
            view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
            break;
        case SubmenuIndexMifareHack:
            // Mifare Attack - use popup
            popup_set_header(app->popup, "Mifare Attack", 64, 10, AlignCenter, AlignTop);
            popup_set_text(app->popup, "Attacking Mifare Classic...\nUsing known keys\nPress Back to stop", 64, 25, AlignCenter, AlignTop);
            popup_set_context(app->popup, app);
            popup_set_callback(app->popup, predator_scene_rfid_attacks_popup_callback);
            popup_set_timeout(app->popup, 0);
            popup_enable_timeout(app->popup);
            
            app->attack_running = true;
            app->packets_sent = 0;
            
            view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
            break;
        case PredatorCustomEventPopupBack:
            // Handle popup back button - stop attack and return to menu
            app->attack_running = false;
            view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
            consumed = true;
            break;
        default:
            consumed = false;
            break;
        }
    }

    return consumed;
}

void predator_scene_rfid_attacks_new_on_exit(void* context) {
    PredatorApp* app = context;
    
    // Stop any running attacks
    app->attack_running = false;
    
    // Clean up submenu
    submenu_reset(app->submenu);
}


