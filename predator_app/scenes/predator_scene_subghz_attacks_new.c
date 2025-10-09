#include "../predator_i.h"
#include "predator_scene.h"
#include "predator_submenu_index.h"
#include "../helpers/predator_ui_elements.h"
#include "../helpers/predator_compliance.h"
#include "../helpers/predator_subghz.h"
#include <furi.h>

// Popup callback for SubGHz attacks
static void predator_scene_subghz_attacks_popup_callback(void* context) {
    PredatorApp* app = context;
    if(!app || !app->view_dispatcher) return;
    view_dispatcher_send_custom_event(app->view_dispatcher, 999);
}

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
    
    submenu_add_item(app->submenu, "RF Jamming", 100, subghz_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "Raw Send", 101, subghz_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "Garage Door (TBD)", 102, subghz_attacks_submenu_callback, app);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

bool predator_scene_subghz_attacks_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(event.type == SceneManagerEventTypeCustom) {
        consumed = true;
        switch(event.event) {
        case 100: { // RF Jamming
            if(app->popup && app->view_dispatcher) {
                popup_set_header(app->popup, "RF Jamming", 64, 10, AlignCenter, AlignTop);
                // Choose region-aware frequency and feature for compliance
                uint32_t freq = 433920000; PredatorFeature feat = PredatorFeatureSubGhz433Tx;
                PredatorRegion r = app->region;
                if(r == PredatorRegionUS || r == PredatorRegionJP) { freq = 315000000; feat = PredatorFeatureSubGhz315Tx; }
                else if(r == PredatorRegionEU || r == PredatorRegionCH) { freq = 433920000; feat = PredatorFeatureSubGhz433Tx; }
                else if(r == PredatorRegionCN) { freq = 433920000; feat = PredatorFeatureSubGhz433Tx; }
                bool live_allowed = predator_compliance_is_feature_allowed(app, feat, app->authorized);
                if(live_allowed) {
                    predator_subghz_init(app);
                    if(predator_subghz_start_jamming(app, freq)) {
                        char buf[64]; snprintf(buf, sizeof(buf), "Live — Jamming %lu Hz\nPress Back to stop", freq);
                        popup_set_text(app->popup, buf, 64, 25, AlignCenter, AlignTop);
                    } else {
                        popup_set_text(app->popup, "RF not ready — Falling back to Demo\nPress Back to return", 64, 25, AlignCenter, AlignTop);
                    }
                } else {
                    popup_set_text(app->popup, "Demo Mode — Authorization required\nPress Back to return", 64, 25, AlignCenter, AlignTop);
                }
                popup_set_callback(app->popup, predator_scene_subghz_attacks_popup_callback);
                popup_set_context(app->popup, app);
                popup_set_timeout(app->popup, 0);
                popup_enable_timeout(app->popup);
                app->attack_running = true;
                app->packets_sent = 0;
                view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
            }
            break;
        }
        case 101: { // Raw Send
            if(app->popup && app->view_dispatcher) {
                popup_set_header(app->popup, "Raw Send", 64, 10, AlignCenter, AlignTop);
                // Use simple car key frame as a raw-send sample and region-aware feature gate
                PredatorRegion r = app->region; PredatorFeature feat = PredatorFeatureSubGhz433Tx;
                if(r == PredatorRegionUS || r == PredatorRegionJP) feat = PredatorFeatureSubGhz315Tx;
                bool live_allowed = predator_compliance_is_feature_allowed(app, feat, app->authorized);
                if(live_allowed) {
                    predator_subghz_init(app);
                    predator_subghz_send_car_key(app, 0xA5B6C7D8);
                    popup_set_text(app->popup, "Live — Raw frame sent\nPress Back to return", 64, 25, AlignCenter, AlignTop);
                } else {
                    popup_set_text(app->popup, "Demo Mode — Authorization required\nPress Back to return", 64, 25, AlignCenter, AlignTop);
                }
                popup_set_callback(app->popup, predator_scene_subghz_attacks_popup_callback);
                popup_set_context(app->popup, app);
                popup_set_timeout(app->popup, 0);
                popup_enable_timeout(app->popup);
                app->attack_running = false; // one-shot send
                app->packets_sent = 0;
                view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
            }
            break;
        }
        case 102: // Garage Door placeholder
            if(app->popup && app->view_dispatcher) {
                popup_set_header(app->popup, "Garage Door", 64, 10, AlignCenter, AlignTop);
                popup_set_text(app->popup, "Feature not implemented yet.\nCheck back later.\nPress Back to return", 64, 25, AlignCenter, AlignTop);
                popup_set_callback(app->popup, predator_scene_subghz_attacks_popup_callback);
                popup_set_context(app->popup, app);
                popup_set_timeout(app->popup, 0);
                popup_enable_timeout(app->popup);
                view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
            }
            break;
        case 999: // Custom event for popup back
            if(app->view_dispatcher) {
                app->attack_running = false;
                // Stop any live RF activity if running
                predator_subghz_stop_attack(app);
                view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
            }
            consumed = true;
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
    } else if(event.type == SceneManagerEventTypeTick) {
        if(app->attack_running) {
            app->packets_sent += 1;
            if(app->packets_sent >= 20) {
                app->packets_sent = 0;
                if(app->popup) {
                    popup_set_text(app->popup, "Signal interference active\nPress Back to stop", 64, 25, AlignCenter, AlignTop);
                }
            }
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
