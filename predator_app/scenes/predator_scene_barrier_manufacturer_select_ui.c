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
    
    // Option 1: Try all manufacturers automatically (now tries all 10!)
    submenu_add_item(app->submenu, "🔄 Try All 10 (Auto)", 0, manufacturer_select_submenu_cb, app);
    
    // Option 2-11: Individual manufacturers (10 total)
    submenu_add_item(app->submenu, "🏭 CAME (Italy)", 1, manufacturer_select_submenu_cb, app);
    submenu_add_item(app->submenu, "🏭 BFT (Italy)", 2, manufacturer_select_submenu_cb, app);
    submenu_add_item(app->submenu, "🏭 Nice (France)", 3, manufacturer_select_submenu_cb, app);
    submenu_add_item(app->submenu, "🏭 Somfy (France)", 4, manufacturer_select_submenu_cb, app);
    submenu_add_item(app->submenu, "🏭 Faac (Italy)", 5, manufacturer_select_submenu_cb, app);
    submenu_add_item(app->submenu, "🏭 Hörmann (Swiss)", 6, manufacturer_select_submenu_cb, app);
    submenu_add_item(app->submenu, "🏭 Benincà (Italy)", 7, manufacturer_select_submenu_cb, app);
    submenu_add_item(app->submenu, "🏭 DEA System (Italy)", 8, manufacturer_select_submenu_cb, app);
    submenu_add_item(app->submenu, "🏭 Ditec (Italy)", 9, manufacturer_select_submenu_cb, app);
    submenu_add_item(app->submenu, "🏭 Roger Tech (Italy)", 10, manufacturer_select_submenu_cb, app);
    
    submenu_set_selected_item(app->submenu, 0);
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
    
    FURI_LOG_I("ManufacturerSelect", "Manufacturer selection menu initialized");
}

bool predator_scene_barrier_manufacturer_select_ui_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    if(!app) return false;
    
    // Handle back button - CRITICAL: Match CarModelsUI pattern exactly
    if(event.type == SceneManagerEventTypeBack) {
        // Go back to parking barriers selection
        scene_manager_previous_scene(app->scene_manager);
        return true;  // Must return true when explicitly calling scene_manager_previous_scene()
    }
    
    // Handle manufacturer selection
    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event <= 10) {
            // Store manufacturer selection
            if(event.event == 0) {
                // Try all manufacturers (now 10 total!)
                app->selected_barrier_manufacturer = 0xFF;  // Special value for "try all"
                predator_log_append(app, "MODE: Try All 10 Manufacturers");
            } else {
                // Specific manufacturer (1-10 maps to BarrierManufacturer enum 0-9)
                app->selected_barrier_manufacturer = event.event - 1;
                
                const char* manufacturer_names[] = {
                    "CAME (Italy)", "BFT (Italy)", "Nice (France)",
                    "Somfy (France)", "Faac (Italy)", "Hörmann (Swiss)",
                    "Benincà (Italy)", "DEA System (Italy)", "Ditec (Italy)", "Roger Tech (Italy)"
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
    if(!app) return;
    // Nothing to cleanup: using shared submenu (match CarModelsUI pattern)
}
