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
    
    if(!app) {
        FURI_LOG_E("RfidAttacks", "App context is NULL on enter");
        return;
    }
    
    // Ensure scene_manager and view_dispatcher are valid to prevent crashes
    if(!app->scene_manager) {
        FURI_LOG_E("RfidAttacks", "Scene manager is NULL, cannot proceed");
        return;
    }
    
    if(!app->view_dispatcher) {
        FURI_LOG_E("RfidAttacks", "View dispatcher is NULL, cannot switch view");
        return;
    }
    
    // Set up submenu for RFID/NFC Attacks
    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "RFID/NFC Attacks");
    
    // Add RFID/NFC attack menu items
    submenu_add_item(app->submenu, "💳 RFID Clone", 0, rfid_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "🔓 RFID Bruteforce", 1, rfid_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "🎲 RFID Fuzzing", 2, rfid_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "📱 NFC Clone", 3, rfid_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "🔐 Mifare Hack", 4, rfid_attacks_submenu_callback, app);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

bool predator_scene_rfid_attacks_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(!app) {
        FURI_LOG_E("RfidAttacks", "App context is NULL in event handler");
        return false;
    }
    
    if(event.type == SceneManagerEventTypeCustom) {
        consumed = true;
        switch(event.event) {
        case 0: 
            scene_manager_next_scene(app->scene_manager, 30);
            break;
        case 1: 
            scene_manager_next_scene(app->scene_manager, 31);
            break;
        case 2:
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
        case 3:
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
        case 4:
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
    } else if(event.type == SceneManagerEventTypeBack) {
        scene_manager_previous_scene(app->scene_manager);
        consumed = true;
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
