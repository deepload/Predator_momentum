#include "../predator_i.h"
#include "predator_scene.h"
#include "predator_submenu_index.h"

// Advanced WiFi Attacks Menu
static void advanced_wifi_submenu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    if(!app || !app->view_dispatcher) return;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void predator_scene_advanced_wifi_ui_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app || !app->submenu) return;
    
    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "Advanced WiFi");
    
    submenu_add_item(app->submenu, "WPA3 Attacks", SubmenuIndexWPA3, advanced_wifi_submenu_callback, app);
    submenu_add_item(app->submenu, "WPS Pixie Dust", SubmenuIndexWPSPixie, advanced_wifi_submenu_callback, app);
    submenu_add_item(app->submenu, "KARMA Attack", SubmenuIndexKARMA, advanced_wifi_submenu_callback, app);
    submenu_add_item(app->submenu, "MANA Attack", SubmenuIndexMANA, advanced_wifi_submenu_callback, app);
    submenu_add_item(app->submenu, "Beacon Flood", SubmenuIndexBeaconFlood, advanced_wifi_submenu_callback, app);
    submenu_add_item(app->submenu, "Hidden SSID Enum", SubmenuIndexHiddenSSID, advanced_wifi_submenu_callback, app);
    submenu_add_item(app->submenu, "Rogue AP Detect", SubmenuIndexRogueAP, advanced_wifi_submenu_callback, app);
    submenu_add_item(app->submenu, "Client Tracking", SubmenuIndexClientTrack, advanced_wifi_submenu_callback, app);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

bool predator_scene_advanced_wifi_ui_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(event.type == SceneManagerEventTypeCustom) {
        consumed = true;
        FURI_LOG_I("AdvancedWiFi", "Selected: %lu", event.event);
    }
    
    UNUSED(app);
    return consumed;
}

void predator_scene_advanced_wifi_ui_on_exit(void* context) {
    PredatorApp* app = context;
    if(app && app->submenu) {
        submenu_reset(app->submenu);
    }
}
