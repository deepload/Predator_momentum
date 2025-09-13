#include "../predator_i.h"
#include "predator_scene.h"
#include <furi.h>

static void start_submenu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    if(!app) {
        FURI_LOG_E("StartMenu", "App context is NULL in submenu callback");
        return;
    }
    if(!app->view_dispatcher) {
        FURI_LOG_E("StartMenu", "View dispatcher is NULL in submenu callback");
        return;
    }
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
    
    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "Predator Momentum");
    submenu_add_item(app->submenu, "Car Attacks", 1, start_submenu_callback, app);
    submenu_add_item(app->submenu, "WiFi Attacks", 2, start_submenu_callback, app);
    submenu_add_item(app->submenu, "Bluetooth Attacks", 3, start_submenu_callback, app);
    submenu_add_item(app->submenu, "SubGHz Attacks", 4, start_submenu_callback, app);
    submenu_add_item(app->submenu, "RFID Attacks", 5, start_submenu_callback, app);
    submenu_add_item(app->submenu, "GPS Tracker", 6, start_submenu_callback, app);
    submenu_add_item(app->submenu, "Social Engineering", 7, start_submenu_callback, app);
    submenu_add_item(app->submenu, "Settings", 8, start_submenu_callback, app);
    submenu_add_item(app->submenu, "Module Status", 9, start_submenu_callback, app);
    submenu_add_item(app->submenu, "About", 10, start_submenu_callback, app);
    
    submenu_set_selected_item(app->submenu, 0);
    FURI_LOG_I("StartMenu", "Main menu initialized");
}

bool predator_scene_start_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(!app) {
        FURI_LOG_E("StartMenu", "App context is NULL in event handler");
        return false;
    }
    
    if(event.type == SceneManagerEventTypeCustom) {
        FURI_LOG_I("StartMenu", "Custom event received: %lu", event.event);
        consumed = true;
        switch(event.event) {
        case 1: // Car Attacks
            scene_manager_next_scene(app->scene_manager, PredatorSceneCarAttacks);
            break;
        case 2: // WiFi Attacks
            scene_manager_next_scene(app->scene_manager, PredatorSceneWifiAttacks);
            break;
        case 3: // Bluetooth Attacks
            scene_manager_next_scene(app->scene_manager, PredatorSceneBluetoothAttacks);
            break;
        case 4: // SubGHz Attacks
            scene_manager_next_scene(app->scene_manager, PredatorSceneSubGhzAttacks);
            break;
        case 5: // RFID Attacks
            scene_manager_next_scene(app->scene_manager, PredatorSceneRfidAttacks);
            break;
        case 6: // GPS Tracker
            scene_manager_next_scene(app->scene_manager, PredatorSceneGPSTracker);
            break;
        case 7: // Social Engineering
            scene_manager_next_scene(app->scene_manager, PredatorSceneSocialEngineering);
            break;
        case 8: // Settings
            scene_manager_next_scene(app->scene_manager, PredatorSceneSettings);
            break;
        case 9: // Module Status
            scene_manager_next_scene(app->scene_manager, PredatorSceneModuleStatus);
            break;
        case 10: // About
            scene_manager_next_scene(app->scene_manager, PredatorSceneAbout);
            break;
        default:
            FURI_LOG_W("StartMenu", "Unknown custom event: %lu", event.event);
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
    
    if(app->submenu) {
        submenu_reset(app->submenu);
    }
    FURI_LOG_I("StartMenu", "Exited main menu scene");
}
