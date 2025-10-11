#include "../predator_i.h"
#include "../helpers/predator_casino.h"
#include "../helpers/predator_logging.h"

static void casino_submenu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    if(!app) return;
    
    predator_casino_init(app);
    
    switch(index) {
        case 0: predator_casino_chip_scan(app, CasinoChip_HF_13_56MHz); break;
        case 1: predator_casino_slot_sas_sniff(app); break;
        case 2: predator_casino_access_scan(app); break;
        case 3: predator_casino_vip_card_test(app); break;
        case 4: predator_casino_desfire_full_dump(app); break;
        case 5: predator_casino_hid_iclass_elite(app); break;
        case 6: predator_casino_hotel_keycard_analyze(app); break;
    }
}

void predator_scene_casino_rfid_ui_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app || !app->submenu) return;
    
    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "Casino/RFID Advanced");
    
    submenu_add_item(app->submenu, "Scan Casino Chips", 0, casino_submenu_callback, app);
    submenu_add_item(app->submenu, "Slot Machine SAS", 1, casino_submenu_callback, app);
    submenu_add_item(app->submenu, "Access Cards", 2, casino_submenu_callback, app);
    submenu_add_item(app->submenu, "VIP Card Clone", 3, casino_submenu_callback, app);
    submenu_add_item(app->submenu, "DESFire Advanced", 4, casino_submenu_callback, app);
    submenu_add_item(app->submenu, "HID iCLASS Elite", 5, casino_submenu_callback, app);
    submenu_add_item(app->submenu, "Hotel Keycards", 6, casino_submenu_callback, app);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
    
    FURI_LOG_I("CasinoUI", "Casino RFID UI entered");
}

bool predator_scene_casino_rfid_ui_on_event(void* context, SceneManagerEvent event) {
    UNUSED(context);
    UNUSED(event);
    return false;
}

void predator_scene_casino_rfid_ui_on_exit(void* context) {
    PredatorApp* app = context;
    if(app && app->submenu) {
        submenu_reset(app->submenu);
    }
    predator_casino_deinit(app);
    FURI_LOG_I("CasinoUI", "Casino RFID UI exited");
}
