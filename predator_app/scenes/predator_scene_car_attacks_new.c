#include "../predator_i.h"
#include "predator_scene.h"
#include "predator_submenu_index.h"
#include "../helpers/predator_ui_elements.h"
#include "../helpers/predator_subghz.h"
#include <furi.h>

// Submenu callback for navigation
static void car_attacks_submenu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    if(!app) {
        FURI_LOG_E("CarAttacks", "App context is NULL in submenu callback");
        return;
    }
    if(!app->view_dispatcher) {
        FURI_LOG_E("CarAttacks", "View dispatcher is NULL in submenu callback");
        return;
    }
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void predator_scene_car_attacks_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    if(!app) {
        FURI_LOG_E("CarAttacks", "App context is NULL on enter");
        return;
    }
    
    // Ensure scene_manager and view_dispatcher are valid to prevent crashes
    if(!app->scene_manager) {
        FURI_LOG_E("CarAttacks", "Scene manager is NULL, cannot proceed");
        return;
    }
    
    if(!app->view_dispatcher) {
        FURI_LOG_E("CarAttacks", "View dispatcher is NULL, cannot switch view");
        return;
    }
    
    // Validate board type before any hardware initialization
    if(app->board_type == 0) {
        FURI_LOG_W("CarAttacks", "Board type is Unknown, defaulting to Original");
        app->board_type = 0; // Keep as Original
    }
    
    // Set up submenu for Car Attacks
    if(!app->submenu) {
        FURI_LOG_E("CarAttacks", "Submenu is NULL, cannot initialize");
        return;
    }
    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "Car Attacks");

    // Add submenu items for car attack scenes with error logging
    submenu_add_item(app->submenu, "Tesla", 1, car_attacks_submenu_callback, app);
    FURI_LOG_I("CarAttacks", "Added Tesla submenu item");
    submenu_add_item(app->submenu, "Car Models", 2, car_attacks_submenu_callback, app);
    FURI_LOG_I("CarAttacks", "Added Car Models submenu item");
    submenu_add_item(app->submenu, "Jamming", 3, car_attacks_submenu_callback, app);
    FURI_LOG_I("CarAttacks", "Added Jamming submenu item");
    submenu_add_item(app->submenu, "Key Bruteforce", 4, car_attacks_submenu_callback, app);
    FURI_LOG_I("CarAttacks", "Added Key Bruteforce submenu item");
    submenu_add_item(app->submenu, "Passive Opener", 5, car_attacks_submenu_callback, app);
    FURI_LOG_I("CarAttacks", "Added Passive Opener submenu item");
    submenu_add_item(app->submenu, "VIP: Tesla Swiss Unlock", 6, car_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "Live Monitor (logs)", 99, car_attacks_submenu_callback, app);
    FURI_LOG_I("CarAttacks", "Added VIP Tesla Swiss Unlock submenu item");

    submenu_set_selected_item(app->submenu, 0);
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
    FURI_LOG_I("CarAttacks", "Car Attacks submenu initialized successfully");
}

bool predator_scene_car_attacks_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(!app) {
        FURI_LOG_E("CarAttacks", "App context is NULL in event handler");
        return false;
    }
    
    if(event.type == SceneManagerEventTypeBack) {
        FURI_LOG_I("CarAttacks", "Back event received, navigating to previous scene");
        scene_manager_previous_scene(app->scene_manager);
        consumed = true;
    } else if(event.type == SceneManagerEventTypeCustom) {
        FURI_LOG_I("CarAttacks", "Custom event received: %lu", event.event);
        consumed = true;
        switch(event.event) {
        case 1: // Tesla
            FURI_LOG_I("CarAttacks", "Navigating to Tesla scene");
            scene_manager_next_scene(app->scene_manager, PredatorSceneCarTesla);
            break;
        case 2: // Car Models
            FURI_LOG_I("CarAttacks", "Navigating to Car Models scene");
            scene_manager_next_scene(app->scene_manager, PredatorSceneCarModels);
            break;
        case 3: // Jamming
            FURI_LOG_I("CarAttacks", "Navigating to Jamming scene");
            scene_manager_next_scene(app->scene_manager, PredatorSceneCarJamming);
            break;
        case 4: // Key Bruteforce
            FURI_LOG_I("CarAttacks", "Navigating to Key Bruteforce scene");
            scene_manager_next_scene(app->scene_manager, PredatorSceneCarKeyBruteforce);
            break;
        case 5: // Passive Opener
            FURI_LOG_I("CarAttacks", "Navigating to Passive Opener scene");
            scene_manager_next_scene(app->scene_manager, PredatorSceneCarPassiveOpener);
            break;
        case 6: // VIP: Tesla Swiss Unlock
            FURI_LOG_I("CarAttacks", "Activating VIP Mode for Tesla Swiss Unlock and navigating to Tesla scene");
            app->vip_mode = true; // Set VIP mode
            scene_manager_next_scene(app->scene_manager, PredatorSceneCarTesla);
            // Send custom event to Tesla scene to trigger VIP mode (placeholder)
            view_dispatcher_send_custom_event(app->view_dispatcher, 999);
            break;
        case 99: // Live Monitor
            FURI_LOG_I("CarAttacks", "Navigating to Live Monitor scene");
            scene_manager_next_scene(app->scene_manager, PredatorSceneLiveMonitor);
            break;
        default:
            FURI_LOG_W("CarAttacks", "Unknown custom event: %lu", event.event);
            consumed = false;
            break;
        }
    }
    
    return consumed;
}

void predator_scene_car_attacks_new_on_exit(void* context) {
    PredatorApp* app = context;
    
    if(!app) {
        FURI_LOG_E("CarAttacks", "App context is NULL on exit");
        return;
    }
    
    // Stop any running attacks
    if(app->attack_running) {
        FURI_LOG_I("CarAttacks", "Stopping running attack on exit");
        app->attack_running = false;
    }
    
    // Clean up submenu
    if(app->submenu) {
        submenu_reset(app->submenu);
        FURI_LOG_I("CarAttacks", "Submenu reset on exit");
    } else {
        FURI_LOG_W("CarAttacks", "Submenu is NULL on exit, skipping reset");
    }
    FURI_LOG_I("CarAttacks", "Exited Car Attacks scene");
}
