#include "../predator_i.h"
#include "predator_scene.h"
#include "predator_submenu_index.h"
#include "../helpers/predator_ui_elements.h"
#include <furi.h>

// Submenu callback for navigation
static void subghz_attacks_submenu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void predator_scene_subghz_attacks_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    // Use standard submenu to avoid NULL pointer issues
    submenu_reset(app->submenu);
    
    // Add SubGHz attack menu items (using simple indices)
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
        case 100: // RF Jamming
            consumed = true;
            break;
        case 101: // Raw Send
            consumed = true;
            break;
        case 102: // Garage Door
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
    submenu_reset(app->submenu);
}


