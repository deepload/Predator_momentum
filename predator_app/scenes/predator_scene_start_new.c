#include "../predator_i.h"
#include "predator_scene.h"
#include "predator_submenu_index.h"
#include <furi.h>

// Submenu callback to route selection to scene via custom event
static void start_submenu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    if(!app || !app->view_dispatcher) return;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void predator_scene_start_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    if(!app) {
        FURI_LOG_E("StartMenu", "App context is NULL on enter");
        return;
    }
    
    if(!app->scene_manager) {
        FURI_LOG_E("StartMenu", "Scene manager is NULL, cannot proceed");
        return;
    }

    if(!app->submenu) {
        FURI_LOG_E("StartMenu", "Submenu is NULL, cannot initialize UI");
        return;
    }

    // Build main menu using enum-based navigation
    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "Predator");

    submenu_add_item(app->submenu, "WiFi Attacks", SubmenuIndexWifiAttacks, start_submenu_callback, app);
    submenu_add_item(app->submenu, "Bluetooth Attacks", SubmenuIndexBluetoothAttacks, start_submenu_callback, app);
    submenu_add_item(app->submenu, "SubGHz Attacks", SubmenuIndexSubGhzAttacks, start_submenu_callback, app);
    submenu_add_item(app->submenu, "Car Attacks", SubmenuIndexCarAttacks, start_submenu_callback, app);
    submenu_add_item(app->submenu, "RFID Attacks", SubmenuIndexRfidAttacks, start_submenu_callback, app);
    submenu_add_item(app->submenu, "GPS Tracker", SubmenuIndexGpsTracker, start_submenu_callback, app);
    submenu_add_item(app->submenu, "Wardriving", SubmenuIndexWardriving, start_submenu_callback, app);
    submenu_add_item(app->submenu, "Social Engineering", SubmenuIndexSocialEngineering, start_submenu_callback, app);
    submenu_add_item(app->submenu, "Quick Tests", SubmenuIndexQuickPoc, start_submenu_callback, app);
    submenu_add_item(app->submenu, "Module Status", SubmenuIndexModuleStatus, start_submenu_callback, app);
    submenu_add_item(app->submenu, "Board Selection", SubmenuIndexBoardSelection, start_submenu_callback, app);
    submenu_add_item(app->submenu, "Settings", SubmenuIndexSettings, start_submenu_callback, app);
    submenu_add_item(app->submenu, "About", SubmenuIndexAbout, start_submenu_callback, app);

    submenu_set_selected_item(app->submenu, SubmenuIndexWifiAttacks);
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
    FURI_LOG_I("StartMenu", "Main menu initialized");
}

bool predator_scene_start_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(!app) {
        FURI_LOG_E("StartMenu", "App context is NULL in event handler");
        return false;
    }

    if(event.type == SceneManagerEventTypeBack) {
        // Let scene manager handle back (will exit app if no previous scene)
        FURI_LOG_I("StartMenu", "Back event received");
        consumed = false;
    } else if(event.type == SceneManagerEventTypeCustom) {
        consumed = true;
        switch(event.event) {
        case SubmenuIndexWifiAttacks:
            scene_manager_next_scene(app->scene_manager, PredatorSceneWifiAttacks);
            break;
        case SubmenuIndexBluetoothAttacks:
            scene_manager_next_scene(app->scene_manager, PredatorSceneBluetoothAttacks);
            break;
        case SubmenuIndexSubGhzAttacks:
            scene_manager_next_scene(app->scene_manager, PredatorSceneSubghzAttacks);
            break;
        case SubmenuIndexCarAttacks:
            scene_manager_next_scene(app->scene_manager, PredatorSceneCarAttacks);
            break;
        case SubmenuIndexRfidAttacks:
            scene_manager_next_scene(app->scene_manager, PredatorSceneRfidAttacks);
            break;
        case SubmenuIndexGpsTracker:
            scene_manager_next_scene(app->scene_manager, PredatorSceneGpsTracker);
            break;
        case SubmenuIndexWardriving:
            scene_manager_next_scene(app->scene_manager, PredatorSceneWardriving);
            break;
        case SubmenuIndexSocialEngineering:
            scene_manager_next_scene(app->scene_manager, PredatorSceneSocialEngineering);
            break;
        case SubmenuIndexQuickPoc:
            scene_manager_next_scene(app->scene_manager, PredatorSceneQuickPoc);
            break;
        case SubmenuIndexModuleStatus:
            scene_manager_next_scene(app->scene_manager, PredatorSceneModuleStatus);
            break;
        case SubmenuIndexBoardSelection:
            scene_manager_next_scene(app->scene_manager, PredatorSceneBoardSelection);
            break;
        case SubmenuIndexSettings:
            scene_manager_next_scene(app->scene_manager, PredatorSceneSettings);
            break;
        case SubmenuIndexAbout:
            scene_manager_next_scene(app->scene_manager, PredatorSceneAbout);
            break;
        default:
            FURI_LOG_W("StartMenu", "Unknown menu index: %lu", event.event);
            consumed = false;
            break;
        }
    }
    
    return consumed;
}

void predator_scene_start_new_on_exit(void* context) {
    PredatorApp* app = context;
    
    if(!app) {
        FURI_LOG_E("StartMenu", "App context is NULL on exit");
        return;
    }
    
    FURI_LOG_I("StartMenu", "Exited main menu scene");
}

