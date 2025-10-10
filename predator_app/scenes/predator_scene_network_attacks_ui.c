#include "../predator_i.h"
#include "predator_scene.h"
#include "predator_submenu_index.h"

// Network Attacks Menu (via ESP32)
static void network_attacks_submenu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    if(!app || !app->view_dispatcher) return;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void predator_scene_network_attacks_ui_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app || !app->submenu) return;
    
    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "Network Attacks");
    
    submenu_add_item(app->submenu, "ARP Spoofing", SubmenuIndexARPSpoof, network_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "DNS Spoofing", SubmenuIndexDNSSpoof, network_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "MITM Attack", SubmenuIndexMITM, network_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "SSL Strip", SubmenuIndexSSLStrip, network_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "Port Scanner", SubmenuIndexPortScan, network_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "Packet Capture", SubmenuIndexPacketCapture, network_attacks_submenu_callback, app);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

bool predator_scene_network_attacks_ui_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(event.type == SceneManagerEventTypeCustom) {
        consumed = true;
        FURI_LOG_I("NetworkAttacks", "Selected: %lu", event.event);
    }
    
    UNUSED(app);
    return consumed;
}

void predator_scene_network_attacks_ui_on_exit(void* context) {
    PredatorApp* app = context;
    if(app && app->submenu) {
        submenu_reset(app->submenu);
    }
}
