#include "../predator_i.h"
#include "predator_scene.h"
#include "predator_submenu_index.h"
#include "../helpers/predator_ui_elements.h"
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
    
    // Ensure scene_manager and view_dispatcher are valid to prevent crashes
    if(!app->scene_manager) {
        FURI_LOG_E("SubGhzAttacks", "Scene manager is NULL, cannot proceed");
        return;
    }
    
    if(!app->view_dispatcher) {
        FURI_LOG_E("SubGhzAttacks", "View dispatcher is NULL, cannot switch view");
        return;
    }
    
    // Comprehensive null safety check
    if(!app->submenu) {
        return;
    }
    
    // Use standard submenu to avoid NULL pointer issues
    submenu_reset(app->submenu);
    
    // Add SubGHz attack menu items with placeholders
    submenu_add_item(app->submenu, "📡 RF Jamming", 100, subghz_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "📦 Raw Send", 101, subghz_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "🚪 Garage Door", 102, subghz_attacks_submenu_callback, app);
    
    submenu_set_header(app->submenu, "SubGHz Attacks");
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

bool predator_scene_subghz_attacks_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(event.type == SceneManagerEventTypeCustom) {
        consumed = true;
        switch(event.event) {
        case 100: // RF Jamming placeholder
            if(app->popup && app->view_dispatcher) {
                popup_set_header(app->popup, "RF Jamming", 64, 10, AlignCenter, AlignTop);
                popup_set_text(app->popup, "Feature not implemented yet.\nCheck back later.\nPress Back to return", 64, 25, AlignCenter, AlignTop);
                popup_set_callback(app->popup, predator_scene_subghz_attacks_popup_callback);
                popup_set_context(app->popup, app);
                popup_set_timeout(app->popup, 0);
                popup_enable_timeout(app->popup);
                view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
            }
            break;
        case 101: // Raw Send placeholder
            if(app->popup && app->view_dispatcher) {
                popup_set_header(app->popup, "Raw Send", 64, 10, AlignCenter, AlignTop);
                popup_set_text(app->popup, "Feature not implemented yet.\nCheck back later.\nPress Back to return", 64, 25, AlignCenter, AlignTop);
                popup_set_callback(app->popup, predator_scene_subghz_attacks_popup_callback);
                popup_set_context(app->popup, app);
                popup_set_timeout(app->popup, 0);
                popup_enable_timeout(app->popup);
                view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
            }
            break;
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
        case 999: // Custom event for popup back, avoiding conflict with placeholder values
            // Handle popup back button - return to submenu
            if(app->view_dispatcher) {
                view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
            }
            consumed = true;
            break;
        default:
            consumed = false;
            break;
        }
    }
    
    return consumed;
}

void predator_scene_subghz_attacks_new_on_exit(void* context) {
    PredatorApp* app = context;
    
    // Clean up submenu
    if(app->submenu) {
        submenu_reset(app->submenu);
    }
}
