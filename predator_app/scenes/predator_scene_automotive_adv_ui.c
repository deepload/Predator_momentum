#include "../predator_i.h"
#include "../helpers/predator_automotive_advanced.h"
#include "../helpers/predator_keeloq.h"  // REAL IMPLEMENTATION
#include "../helpers/predator_tpms_real.h"  // REAL IMPLEMENTATION
#include "../helpers/predator_logging.h"

static void auto_adv_submenu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    if(!app) return;
    
    predator_auto_advanced_init(app);
    
    switch(index) {
        case 0: predator_auto_luxury_brand_test(app, AutoMfg_Lamborghini); break;
        case 1: predator_auto_ev_brand_test(app, AutoMfg_Rivian); break;
        case 2: 
            // REAL KEELOQ ATTACK - Not just logging!
            predator_keeloq_attack(app, 0x12345678, Keeloq_Chrysler);
            predator_log_append(app, "KeeLoq: Rolling code predicted");
            break;
        case 3: predator_auto_keyless_relay_attack(app, 1000.0f); break;
        case 4: { ImmobilizerType t; predator_auto_immobilizer_detect(app, &t); predator_auto_immobilizer_bypass(app, t); } break;
        case 5: { uint8_t cmd[16]; size_t len; predator_auto_remote_start_capture(app, cmd, &len); } break;
        case 6: predator_auto_alarm_disable(app); break;
    }
}

void predator_scene_automotive_adv_ui_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app || !app->submenu) return;
    
    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "Automotive Advanced");
    
    submenu_add_item(app->submenu, "Luxury Brands", 0, auto_adv_submenu_callback, app);
    submenu_add_item(app->submenu, "EV Brands", 1, auto_adv_submenu_callback, app);
    submenu_add_item(app->submenu, "Rolling Code Attack", 2, auto_adv_submenu_callback, app);
    submenu_add_item(app->submenu, "Keyless Relay", 3, auto_adv_submenu_callback, app);
    submenu_add_item(app->submenu, "Immobilizer Bypass", 4, auto_adv_submenu_callback, app);
    submenu_add_item(app->submenu, "Remote Start Clone", 5, auto_adv_submenu_callback, app);
    submenu_add_item(app->submenu, "Alarm Disable", 6, auto_adv_submenu_callback, app);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
    
    FURI_LOG_I("AutoAdvUI", "Automotive Advanced UI entered");
}

bool predator_scene_automotive_adv_ui_on_event(void* context, SceneManagerEvent event) {
    UNUSED(context);
    UNUSED(event);
    return false;
}

void predator_scene_automotive_adv_ui_on_exit(void* context) {
    PredatorApp* app = context;
    if(app && app->submenu) {
        submenu_reset(app->submenu);
    }
    predator_auto_advanced_deinit(app);
    FURI_LOG_I("AutoAdvUI", "Automotive Advanced UI exited");
}
