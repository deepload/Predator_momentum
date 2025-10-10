#include "../predator_i.h"
#include "predator_scene.h"
#include "predator_submenu_index.h"

// Advanced Automotive Attacks Menu
static void automotive_advanced_submenu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    if(!app || !app->view_dispatcher) return;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void predator_scene_automotive_advanced_ui_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app || !app->submenu) return;
    
    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "Automotive Advanced");
    
    submenu_add_item(app->submenu, "CAN Bus Sniffer", SubmenuIndexCANSniffer, automotive_advanced_submenu_callback, app);
    submenu_add_item(app->submenu, "CAN Bus Fuzzing", SubmenuIndexCANFuzz, automotive_advanced_submenu_callback, app);
    submenu_add_item(app->submenu, "CAN Replay Attack", SubmenuIndexCANReplay, automotive_advanced_submenu_callback, app);
    submenu_add_item(app->submenu, "UDS Diagnostics", SubmenuIndexUDS, automotive_advanced_submenu_callback, app);
    submenu_add_item(app->submenu, "OBD-II Scanner", SubmenuIndexOBD, automotive_advanced_submenu_callback, app);
    submenu_add_item(app->submenu, "TPMS Spoofing", SubmenuIndexTPMSSpoof, automotive_advanced_submenu_callback, app);
    submenu_add_item(app->submenu, "Immobilizer Crack", SubmenuIndexImmobilizer, automotive_advanced_submenu_callback, app);
    submenu_add_item(app->submenu, "ECU Identification", SubmenuIndexECUIdent, automotive_advanced_submenu_callback, app);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

bool predator_scene_automotive_advanced_ui_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(event.type == SceneManagerEventTypeCustom) {
        consumed = true;
        FURI_LOG_I("AutomotiveAdv", "Selected: %lu", event.event);
    }
    
    UNUSED(app);
    return consumed;
}

void predator_scene_automotive_advanced_ui_on_exit(void* context) {
    PredatorApp* app = context;
    if(app && app->submenu) {
        submenu_reset(app->submenu);
    }
}
