#include "../predator_i.h"
#include "predator_scene.h"
#include "predator_submenu_index.h"

// Advanced RF/SubGHz Attacks Menu
static void advanced_rf_submenu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    if(!app || !app->view_dispatcher) return;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void predator_scene_advanced_rf_ui_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app || !app->submenu) return;
    
    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "Advanced RF/SubGHz");
    
    submenu_add_item(app->submenu, "Spectrum Analyzer", SubmenuIndexSpectrumAnalyzer, advanced_rf_submenu_callback, app);
    submenu_add_item(app->submenu, "Protocol Analyzer", SubmenuIndexProtocolAnalyzer, advanced_rf_submenu_callback, app);
    submenu_add_item(app->submenu, "Signal Replay+", SubmenuIndexSignalReplay, advanced_rf_submenu_callback, app);
    submenu_add_item(app->submenu, "Frequency Jammer", SubmenuIndexFrequencyJammer, advanced_rf_submenu_callback, app);
    submenu_add_item(app->submenu, "TPMS Attacks", SubmenuIndexTPMS, advanced_rf_submenu_callback, app);
    submenu_add_item(app->submenu, "LoRa/LoRaWAN", SubmenuIndexLoRa, advanced_rf_submenu_callback, app);
    submenu_add_item(app->submenu, "Zigbee Attacks", SubmenuIndexZigbee, advanced_rf_submenu_callback, app);
    submenu_add_item(app->submenu, "Z-Wave Attacks", SubmenuIndexZWave, advanced_rf_submenu_callback, app);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

bool predator_scene_advanced_rf_ui_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(event.type == SceneManagerEventTypeCustom) {
        consumed = true;
        // Navigation to advanced RF scenes (stub - scenes not yet created)
        FURI_LOG_I("AdvancedRF", "Selected: %lu", event.event);
    }
    
    UNUSED(app);
    return consumed;
}

void predator_scene_advanced_rf_ui_on_exit(void* context) {
    PredatorApp* app = context;
    if(app && app->submenu) {
        submenu_reset(app->submenu);
    }
}
