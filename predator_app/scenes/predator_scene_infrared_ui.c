#include "../predator_i.h"
#include "predator_scene.h"
#include "predator_submenu_index.h"

// Infrared Universal Remote Menu
static void infrared_submenu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    if(!app || !app->view_dispatcher) return;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void predator_scene_infrared_ui_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app || !app->submenu) return;
    
    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "IR Universal Remote");
    
    submenu_add_item(app->submenu, "TV Power Brute", SubmenuIndexIRTVPower, infrared_submenu_callback, app);
    submenu_add_item(app->submenu, "AC Control", SubmenuIndexIRAC, infrared_submenu_callback, app);
    submenu_add_item(app->submenu, "Projector Control", SubmenuIndexIRProjector, infrared_submenu_callback, app);
    submenu_add_item(app->submenu, "IR Capture", SubmenuIndexIRCapture, infrared_submenu_callback, app);
    submenu_add_item(app->submenu, "IR Replay", SubmenuIndexIRReplay, infrared_submenu_callback, app);
    submenu_add_item(app->submenu, "IR Database", SubmenuIndexIRDatabase, infrared_submenu_callback, app);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

bool predator_scene_infrared_ui_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(event.type == SceneManagerEventTypeCustom) {
        consumed = true;
        FURI_LOG_I("Infrared", "Selected: %lu", event.event);
    }
    
    UNUSED(app);
    return consumed;
}

void predator_scene_infrared_ui_on_exit(void* context) {
    PredatorApp* app = context;
    if(app && app->submenu) {
        submenu_reset(app->submenu);
    }
}
