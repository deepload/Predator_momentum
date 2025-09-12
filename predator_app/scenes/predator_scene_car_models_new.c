#include "../predator_i.h"
#include "../helpers/predator_view_helpers.h"
#include "../helpers/predator_subghz.h"
#include "../helpers/predator_models.h"
#include "../helpers/predator_models_seed.h"
#include "../helpers/predator_ui_elements.h"
#include "predator_scene.h"
#include "predator_submenu_index.h"

// Popup callback for car models
static void predator_scene_car_models_popup_callback(void* context) {
    PredatorApp* app = context;
    if(!app || !app->view_dispatcher) return;
    view_dispatcher_send_custom_event(app->view_dispatcher, PredatorCustomEventPopupBack);
}

// Submenu callback for car models
static void car_models_submenu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    if(!app || !app->view_dispatcher) return;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}



void predator_scene_car_models_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    // Comprehensive null safety check
    if(!app || !app->submenu || !app->view_dispatcher) {
        FURI_LOG_E("CarModels", "NULL pointer in car models scene entry");
        return;
    }
    
    // Validate and ensure board type is properly set
    if(app->board_type == PredatorBoardTypeUnknown || app->board_type >= PredatorBoardTypeCount) {
        FURI_LOG_W("CarModels", "Invalid board type %d, defaulting to Original", app->board_type);
        app->board_type = PredatorBoardTypeOriginal;
    }
    
    // Verify board configuration is accessible before SubGHz init
    const PredatorBoardConfig* board_config = predator_boards_get_config(app->board_type);
    if(!board_config) {
        FURI_LOG_E("CarModels", "Failed to get board config, using Original board");
        app->board_type = PredatorBoardTypeOriginal;
    } else {
        FURI_LOG_I("CarModels", "Using board: %s", predator_boards_get_name(app->board_type));
    }
    
    // Initialize SubGHz with validated board type
    predator_subghz_init(app);
    
    // Additional safety checks for critical components
    if(!app->notifications || !app->scene_manager) {
        FURI_LOG_E("CarModels", "Missing critical app components");
        return;
    }
    
    // Safely reset submenu with null check
    if(app->submenu) {
        submenu_reset(app->submenu);
        
        // Add car model menu items - test with minimal set first
        submenu_add_item(app->submenu, "Toyota", 0, car_models_submenu_callback, app);
        submenu_add_item(app->submenu, "Honda", 1, car_models_submenu_callback, app);
        submenu_add_item(app->submenu, "Tesla", 4, car_models_submenu_callback, app);
        
        submenu_set_header(app->submenu, "Car Models");
    } else {
        FURI_LOG_E("CarModels", "Submenu is NULL, cannot add items");
        return;
    }
    
    if(app->view_dispatcher) {
        view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
    }
}

bool predator_scene_car_models_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    // Null safety check
    if(!app || !app->popup || !app->view_dispatcher) {
        return false;
    }
    
    if(event.type == SceneManagerEventTypeCustom) {
        consumed = true;
        switch(event.event) {
        case 0: // Toyota Camry
        case 1: // Honda Civic
        case 2: // Ford F-150
        case 3: // BMW 5-Series
        case 4: // Tesla Model 3
        case 5: // Mercedes C-Class
        case 6: // Audi Q5
            // Show attack popup for selected car model with extra safety
            if(app->popup) {
                popup_set_header(app->popup, "Car Attack", 64, 10, AlignCenter, AlignTop);
                popup_set_text(app->popup, "Sending unlock signal...\nPress Back to stop", 64, 25, AlignCenter, AlignTop);
                popup_set_context(app->popup, app);
                popup_set_callback(app->popup, predator_scene_car_models_popup_callback);
                popup_set_timeout(app->popup, 0);
                popup_enable_timeout(app->popup);
                
                // Start attack
                app->attack_running = true;
                app->packets_sent = 0;
                
                if(app->view_dispatcher) {
                    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
                }
            } else {
                FURI_LOG_E("CarModels", "Popup is NULL, cannot show attack");
            }
            break;
        case 99: // More Models
            // Show info popup with safety checks
            if(app->popup) {
                popup_set_header(app->popup, "More Models", 64, 10, AlignCenter, AlignTop);
                popup_set_text(app->popup, "Additional car models\navailable in CSV files\nPress Back to return", 64, 25, AlignCenter, AlignTop);
                popup_set_context(app->popup, app);
                popup_set_callback(app->popup, predator_scene_car_models_popup_callback);
                popup_set_timeout(app->popup, 0);
                popup_enable_timeout(app->popup);
                
                if(app->view_dispatcher) {
                    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
                }
            } else {
                FURI_LOG_E("CarModels", "Popup is NULL for More Models");
            }
            break;
        case PredatorCustomEventPopupBack:
            // Return to submenu from popup with safety checks
            app->attack_running = false;
            if(app->view_dispatcher) {
                view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
            }
            consumed = true;
            break;
        default:
            consumed = false;
            break;
        }
    } else if(event.type == SceneManagerEventTypeTick) {
        if(app->attack_running && app->popup) {
            // Update attack progress every 5 ticks
            if(app->packets_sent % 5 == 0) {
                char attack_text[64];
                snprintf(attack_text, sizeof(attack_text), "Sending signals...\nAttempts: %lu\nPress Back", app->packets_sent / 5);
                popup_set_text(app->popup, attack_text, 64, 25, AlignCenter, AlignTop);
            }
            
            app->packets_sent++;
            consumed = true;
        }
    }
    
    return consumed;
}

void predator_scene_car_models_new_on_exit(void* context) {
    PredatorApp* app = context;
    
    // Null safety check
    if(!app) return;
    
    // Stop any running attack
    app->attack_running = false;
    
    // Clean up submenu with null check
    if(app->submenu) {
        submenu_reset(app->submenu);
    }
    
    // Clean up SubGHz
    predator_subghz_deinit(app);
}


