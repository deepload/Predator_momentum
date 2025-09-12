#include "../predator_i.h"
#include "predator_scene.h"
#include "predator_submenu_index.h"
#include "../helpers/predator_ui_elements.h"
#include <furi.h>

// Submenu callback for navigation
static void wifi_attacks_submenu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void predator_scene_wifi_attacks_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    // Use standard submenu to avoid NULL pointer issues
    submenu_reset(app->submenu);
    
    // Add WiFi attack menu items
    submenu_add_item(app->submenu, "📡 WiFi Scanner", SubmenuIndexWifiScan, wifi_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "💥 Deauth Attack", SubmenuIndexWifiDeauth, wifi_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "👥 Evil Twin AP", SubmenuIndexWifiEvilTwin, wifi_attacks_submenu_callback, app);
    
    submenu_set_header(app->submenu, "WiFi Attacks");
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

bool predator_scene_wifi_attacks_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(event.type == SceneManagerEventTypeCustom) {
        consumed = true;
        switch(event.event) {
        case SubmenuIndexWifiScan:
            scene_manager_next_scene(app->scene_manager, 2); // WifiScan
            break;
        case SubmenuIndexWifiDeauth:
            scene_manager_next_scene(app->scene_manager, 3); // WifiDeauth
            break;
        case SubmenuIndexWifiEvilTwin:
            scene_manager_next_scene(app->scene_manager, 4); // WifiEvilTwin
            break;
        default:
            consumed = false;
            break;
        }
    }
    
    return consumed;
}

void predator_scene_wifi_attacks_new_on_exit(void* context) {
    PredatorApp* app = context;
    
    // Clean up submenu
    submenu_reset(app->submenu);
}

