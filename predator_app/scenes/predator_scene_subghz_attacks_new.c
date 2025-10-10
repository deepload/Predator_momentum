#include "../predator_i.h"
#include "predator_scene.h"
#include "predator_submenu_index.h"
#include "../helpers/predator_ui_elements.h"
#include "../helpers/predator_compliance.h"
#include "../helpers/predator_subghz.h"
#include "../helpers/predator_ui_status.h"
#include "../helpers/predator_logging.h"
#include <furi.h>

// Submenu callback for navigation
static void subghz_attacks_submenu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    if(!app || !app->view_dispatcher) return;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void predator_scene_subghz_attacks_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    if(!app) {
        FURI_LOG_E("SubGhzAttacks", "App context is NULL on enter");
        return;
    }
    
    if(!app->scene_manager) {
        FURI_LOG_E("SubGhzAttacks", "Scene manager is NULL, cannot proceed");
        return;
    }
    
    if(!app->view_dispatcher) {
        FURI_LOG_E("SubGhzAttacks", "View dispatcher is NULL, cannot switch view");
        return;
    }
    
    if(!app->submenu) {
        return;
    }
    
    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "SubGHz Attacks");
    
    // Add SubGHz attack menu items (Professional UI)
    submenu_add_item(app->submenu, "📡 RF Jamming", 100, subghz_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "📤 Raw Send", 101, subghz_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "📊 Live Monitor", 199, subghz_attacks_submenu_callback, app);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

bool predator_scene_subghz_attacks_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(event.type == SceneManagerEventTypeCustom) {
        consumed = true;
        switch(event.event) {
        case 100: // RF Jamming (Professional UI)
            scene_manager_next_scene(app->scene_manager, PredatorSceneSubGhzJammingUI);
            break;
        case 101: // Raw Send (Professional UI)
            scene_manager_next_scene(app->scene_manager, PredatorSceneSubGhzRawSendUI);
            break;
        case 199: // Live Monitor (Professional UI)
            scene_manager_next_scene(app->scene_manager, PredatorSceneLiveMonitorUI);
            break;
        default:
            consumed = false;
            break;
        }
    } else if(event.type == SceneManagerEventTypeBack) {
        if(app->scene_manager) {
            scene_manager_previous_scene(app->scene_manager);
            consumed = true;
        }
    }
    
    return consumed;
}

void predator_scene_subghz_attacks_new_on_exit(void* context) {
    PredatorApp* app = context;
    
    if(app && app->submenu) {
        submenu_reset(app->submenu);
    }
    if(app) {
        app->attack_running = false;
    }
}
