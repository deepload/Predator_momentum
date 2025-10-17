#include "../predator_i.h"
#include "../helpers/predator_logging.h"

// SWISS GOVERNMENT KKS - BARRIER MANUFACTURER SELECTION
// Allows user to try all manufacturers or select a specific one

static void manufacturer_select_submenu_cb(void* context, uint32_t index) {
    PredatorApp* app = context;
    if(!app || !app->view_dispatcher) return;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void predator_scene_barrier_manufacturer_select_ui_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app || !app->submenu) return;
    
    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "SELECT MANUFACTURER");
    
    // Option 1: Try all manufacturers automatically
    submenu_add_item(app->submenu, "🔄 Try All (Auto)", 0, manufacturer_select_submenu_cb, app);
    
    // Option 2-7: Individual manufacturers
    submenu_add_item(app->submenu, "🏭 CAME (Italy)", 1, manufacturer_select_submenu_cb, app);
    submenu_add_item(app->submenu, "🏭 BFT (Italy)", 2, manufacturer_select_submenu_cb, app);
    submenu_add_item(app->submenu, "🏭 Nice (France)", 3, manufacturer_select_submenu_cb, app);
    submenu_add_item(app->submenu, "🏭 Somfy (France)", 4, manufacturer_select_submenu_cb, app);
    submenu_add_item(app->submenu, "🏭 Faac (Italy)", 5, manufacturer_select_submenu_cb, app);
    submenu_add_item(app->submenu, "🏭 Hörmann (Swiss)", 6, manufacturer_select_submenu_cb, app);
    
    submenu_set_selected_item(app->submenu, 0);
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
    
    FURI_LOG_I("ManufacturerSelect", "Manufacturer selection menu initialized");
}

bool predator_scene_barrier_manufacturer_select_ui_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    if(!app) return false;
    
    // Handle back button
    if(event.type == SceneManagerEventTypeBack) {
        scene_manager_previous_scene(app->scene_manager);
        return true;
    }
    
    // Handle manufacturer selection
    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event <= 6) {
            // Store manufacturer selection
            if(event.event == 0) {
                // Try all manufacturers
                app->selected_barrier_manufacturer = 0xFF;  // Special value for "try all"
                predator_log_append(app, "MODE: Try All Manufacturers");
            } else {
                // Specific manufacturer (1-6 maps to BarrierManufacturer enum 0-5)
                app->selected_barrier_manufacturer = event.event - 1;
                
                const char* manufacturer_names[] = {
                    "CAME (Italy)", "BFT (Italy)", "Nice (France)",
                    "Somfy (France)", "Faac (Italy)", "Hörmann (Swiss)"
                };
                
                char log_msg[64];
                snprintf(log_msg, sizeof(log_msg), "MANUFACTURER: %s",
                        manufacturer_names[app->selected_barrier_manufacturer]);
                predator_log_append(app, log_msg);
            }
            
            // Navigate to barrier attack
            scene_manager_next_scene(app->scene_manager, PredatorSceneBarrierAttackUI);
            return true;
        }
    }
    
    return false;
}

void predator_scene_barrier_manufacturer_select_ui_on_exit(void* context) {
    PredatorApp* app = context;
    if(!app || !app->submenu) return;
    
    submenu_reset(app->submenu);
    FURI_LOG_I("ManufacturerSelect", "Manufacturer selection menu exited");
}
