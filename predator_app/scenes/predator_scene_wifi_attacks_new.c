#include "../predator_i.h"
#include "predator_scene.h"
#include "predator_submenu_index.h"
#include "../helpers/predator_ui_elements.h"
#include <furi.h>

// Submenu callback for navigation
static void wifi_attacks_submenu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    if(!app || !app->view_dispatcher) {
        FURI_LOG_E("WiFiAttacks", "App or view dispatcher is NULL in submenu callback");
        return;
    }
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void predator_scene_wifi_attacks_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    if(!app) {
        FURI_LOG_E("WiFiAttacks", "App context is NULL on enter");
        return;
    }
    
    // Ensure scene_manager and view_dispatcher are valid to prevent crashes
    if(!app->scene_manager) {
        FURI_LOG_E("WiFiAttacks", "Scene manager is NULL, cannot proceed");
        return;
    }
    
    if(!app->view_dispatcher) {
        FURI_LOG_E("WiFiAttacks", "View dispatcher is NULL, cannot switch view");
        return;
    }
    
    if(!app->submenu) {
        FURI_LOG_E("WiFiAttacks", "Submenu is NULL, cannot initialize UI");
        return;
    }
    
    // Validate board type before any hardware initialization
    if(app->board_type == PredatorBoardTypeUnknown) {
        FURI_LOG_W("WiFiAttacks", "Board type is Unknown, defaulting to Original");
        app->board_type = PredatorBoardTypeOriginal;
    }
    
    // Set up submenu for WiFi Attacks
    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "WiFi Attacks");

    // Add submenu items for WiFi attack scenes
    submenu_add_item(app->submenu, "Scan (Classic)", 1, wifi_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "Scan (Pro UI)", 6, wifi_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "Deauth (Classic)", 2, wifi_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "Deauth (Pro UI)", 7, wifi_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "Evil Twin", 3, wifi_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "Handshake Capture (PSK)", 4, wifi_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "PMKID Capture", 5, wifi_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "Live Monitor (logs)", 99, wifi_attacks_submenu_callback, app);
    
    submenu_set_selected_item(app->submenu, 0);
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
    FURI_LOG_I("WiFiAttacks", "WiFi Attacks submenu initialized with professional layout");
}

bool predator_scene_wifi_attacks_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(!app) {
        FURI_LOG_E("WiFiAttacks", "App context is NULL in event handler");
        return false;
    }
    
    if(event.type == SceneManagerEventTypeBack) {
        FURI_LOG_I("WiFiAttacks", "Back event received, navigating to previous scene");
        scene_manager_previous_scene(app->scene_manager);
        consumed = true;
    } else if(event.type == SceneManagerEventTypeCustom) {
        FURI_LOG_I("WiFiAttacks", "Custom event received: %lu", event.event);
        consumed = true;
        switch(event.event) {
        case 1: // Scan (Classic)
            scene_manager_next_scene(app->scene_manager, PredatorSceneWifiScan);
            break;
        case 6: // Scan (Pro UI)
            scene_manager_next_scene(app->scene_manager, PredatorSceneWifiScanUI);
            break;
        case 2: // Deauth (Classic)
            scene_manager_next_scene(app->scene_manager, PredatorSceneWifiDeauth);
            break;
        case 7: // Deauth (Pro UI)
            scene_manager_next_scene(app->scene_manager, PredatorSceneWifiDeauthUI);
            break;
        case 3: // Evil Twin
            scene_manager_next_scene(app->scene_manager, PredatorSceneWifiEvilTwin);
            break;
        case 4: // Handshake Capture (PSK)
            scene_manager_next_scene(app->scene_manager, PredatorSceneWifiHandshake);
            break;
        case 5: // PMKID Capture
            scene_manager_next_scene(app->scene_manager, PredatorSceneWifiPmkid);
            break;
        case 99: // Live Monitor
            scene_manager_next_scene(app->scene_manager, PredatorSceneLiveMonitor);
            break;
        default:
            FURI_LOG_W("WiFiAttacks", "Unknown custom event: %lu", event.event);
            consumed = false;
            break;
        }
    }
    
    return consumed;
}

void predator_scene_wifi_attacks_new_on_exit(void* context) {
    PredatorApp* app = context;
    
    if(!app) {
        FURI_LOG_E("WiFiAttacks", "App context is NULL on exit");
        return;
    }
    
    if(app->submenu) {
        submenu_reset(app->submenu);
        FURI_LOG_I("WiFiAttacks", "Submenu reset on exit");
    } else {
        FURI_LOG_W("WiFiAttacks", "Submenu is NULL on exit, skipping reset");
    }
    FURI_LOG_I("WiFiAttacks", "Exited WiFi Attacks submenu scene");
}
