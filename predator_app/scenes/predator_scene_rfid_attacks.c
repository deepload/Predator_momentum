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
        consumed = true;
        switch(event.event) {
        case SubmenuIndexRfidBruteforce:
            popup_set_header(app->popup, "RFID Bruteforce", 64, 10, AlignCenter, AlignTop);
            popup_set_text(app->popup, "Bruteforcing RFID cards...\nTrying common IDs\nPress Back to stop", 64, 25, AlignCenter, AlignTop);
            view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
            break;
        case SubmenuIndexMifareHack:
            popup_set_header(app->popup, "Mifare Attack", 64, 10, AlignCenter, AlignTop);
            popup_set_text(app->popup, "Attacking Mifare Classic...\nUsing known keys\nPress Back to stop", 64, 25, AlignCenter, AlignTop);
            view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
            break;
        default:
            break;
        }
    }

    return consumed;
}

void predator_scene_rfid_attacks_on_exit(void* context) {
    PredatorApp* app = context;
    submenu_reset(app->submenu);
}
