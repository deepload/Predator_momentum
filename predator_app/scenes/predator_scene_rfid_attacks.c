#include "../predator_i.h"

enum SubmenuIndex {
    SubmenuIndexRfidClone,
    SubmenuIndexRfidBruteforce,
    SubmenuIndexRfidFuzzing,
    SubmenuIndexNfcClone,
    SubmenuIndexMifareHack,
};

void predator_scene_rfid_attacks_submenu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

// Popup callback for when an attack is running
static void predator_scene_rfid_attacks_popup_callback(void* context) {
    PredatorApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, PredatorCustomEventPopupBack);
}

void predator_scene_rfid_attacks_on_enter(void* context) {
    PredatorApp* app = context;
    Submenu* submenu = app->submenu;

    submenu_add_item(
        submenu, "💳 RFID Clone", SubmenuIndexRfidClone, predator_scene_rfid_attacks_submenu_callback, app);
    submenu_add_item(
        submenu, "🔓 RFID Bruteforce", SubmenuIndexRfidBruteforce, predator_scene_rfid_attacks_submenu_callback, app);
    submenu_add_item(
        submenu, "🎲 RFID Fuzzing", SubmenuIndexRfidFuzzing, predator_scene_rfid_attacks_submenu_callback, app);
    submenu_add_item(
        submenu, "📱 NFC Clone", SubmenuIndexNfcClone, predator_scene_rfid_attacks_submenu_callback, app);
    submenu_add_item(
        submenu, "🔐 Mifare Hack", SubmenuIndexMifareHack, predator_scene_rfid_attacks_submenu_callback, app);

    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

bool predator_scene_rfid_attacks_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        // Handle popup back button press
        if(event.event == PredatorCustomEventPopupBack) {
            // Return to submenu view
            view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
            
            // Clean up any running attacks
            app->attack_running = false;
            
            consumed = true;
        } else {
            // Handle menu selection
            consumed = true;
            switch(event.event) {
            case SubmenuIndexRfidClone:
                scene_manager_next_scene(app->scene_manager, PredatorSceneRfidClone);
                break;
            case SubmenuIndexRfidBruteforce:
                scene_manager_next_scene(app->scene_manager, PredatorSceneRfidBruteforce);
                break;
            case SubmenuIndexRfidFuzzing:
                // RFID Fuzzing attack
                popup_set_header(app->popup, "RFID Fuzzing", 64, 10, AlignCenter, AlignTop);
                popup_set_text(app->popup, "Fuzzing RFID protocols...\nTesting variations: 127\nPress Back to stop", 64, 25, AlignCenter, AlignTop);
                popup_set_context(app->popup, app);
                popup_set_callback(app->popup, predator_scene_rfid_attacks_popup_callback);
                popup_set_timeout(app->popup, 0);
                popup_enable_timeout(app->popup);
                
                // Start attack
                app->attack_running = true;
                app->packets_sent = 0;
                
                view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
                break;
            case SubmenuIndexNfcClone:
                // NFC Clone
                popup_set_header(app->popup, "NFC Clone", 64, 10, AlignCenter, AlignTop);
                popup_set_text(app->popup, "Place NFC card on Flipper...\nReading NFC data\nPress Back to stop", 64, 25, AlignCenter, AlignTop);
                popup_set_context(app->popup, app);
                popup_set_callback(app->popup, predator_scene_rfid_attacks_popup_callback);
                popup_set_timeout(app->popup, 0);
                popup_enable_timeout(app->popup);
                
                // Start attack
                app->attack_running = true;
                app->packets_sent = 0;
                
                view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
                break;
            case SubmenuIndexMifareHack:
                // Mifare Attack
                popup_set_header(app->popup, "Mifare Attack", 64, 10, AlignCenter, AlignTop);
                popup_set_text(app->popup, "Attacking Mifare Classic...\nUsing known keys\nPress Back to stop", 64, 25, AlignCenter, AlignTop);
                popup_set_context(app->popup, app);
                popup_set_callback(app->popup, predator_scene_rfid_attacks_popup_callback);
                popup_set_timeout(app->popup, 0);
                popup_enable_timeout(app->popup);
                
                // Start attack
                app->attack_running = true;
                app->packets_sent = 0;
                
                view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
                break;
            default:
                break;
            }
        }
    }

    return consumed;
}

void predator_scene_rfid_attacks_on_exit(void* context) {
    PredatorApp* app = context;
    submenu_reset(app->submenu);
}
