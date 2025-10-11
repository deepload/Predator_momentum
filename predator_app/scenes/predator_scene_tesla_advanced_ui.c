#include "../predator_i.h"
#include "../helpers/predator_tesla_advanced.h"
#include "../helpers/predator_tesla_tpms_rce.h"  // REAL EXPLOIT
#include "../helpers/predator_logging.h"

static void tesla_adv_submenu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    if(!app) return;
    
    predator_tesla_advanced_init(app);
    
    switch(index) {
        case 0: predator_tesla_vcsec_scan(app); break;
        case 1: 
            // REAL TPMS RCE EXPLOIT - Not just logging!
            predator_tesla_tpms_execute_rce(app);
            predator_log_append(app, "Tesla TPMS RCE: EXPLOIT EXECUTED");
            break;
        case 2: predator_tesla_ble_relay_attack(app, true); break;
        case 3: predator_tesla_fsd_camera_jam(app, true); break;
        case 4: predator_tesla_supercharger_scan(app); break;
        case 5: predator_tesla_bms_diagnostic(app); break;
    }
}

void predator_scene_tesla_advanced_ui_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app || !app->submenu) return;
    
    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "Tesla Advanced 2024");
    
    submenu_add_item(app->submenu, "VCSEC Scan", 0, tesla_adv_submenu_callback, app);
    submenu_add_item(app->submenu, "TPMS RCE (Pwn2Own)", 1, tesla_adv_submenu_callback, app);
    submenu_add_item(app->submenu, "BLE Relay Attack", 2, tesla_adv_submenu_callback, app);
    submenu_add_item(app->submenu, "FSD Jamming", 3, tesla_adv_submenu_callback, app);
    submenu_add_item(app->submenu, "Supercharger Bypass", 4, tesla_adv_submenu_callback, app);
    submenu_add_item(app->submenu, "BMS Diagnostics", 5, tesla_adv_submenu_callback, app);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
    
    FURI_LOG_I("TeslaAdvUI", "Tesla Advanced UI entered");
}

bool predator_scene_tesla_advanced_ui_on_event(void* context, SceneManagerEvent event) {
    UNUSED(context);
    UNUSED(event);
    return false;
}

void predator_scene_tesla_advanced_ui_on_exit(void* context) {
    PredatorApp* app = context;
    if(app && app->submenu) {
        submenu_reset(app->submenu);
    }
    predator_tesla_advanced_deinit(app);
    FURI_LOG_I("TeslaAdvUI", "Tesla Advanced UI exited");
}
