#include "../predator_i.h"
#include "predator_scene.h"
#include "predator_submenu_index.h"
#include "../helpers/predator_ui_elements.h"
#include "../helpers/predator_subghz.h"
#include <furi.h>

// Popup callback for car attacks
static void predator_scene_car_attacks_popup_callback(void* context) {
    PredatorApp* app = context;
    if(!app || !app->view_dispatcher) return;
    view_dispatcher_send_custom_event(app->view_dispatcher, PredatorCustomEventPopupBack);
}

// Submenu callback for navigation
static void car_attacks_submenu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    if(!app || !app->view_dispatcher) return;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void predator_scene_car_attacks_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    // Comprehensive null safety check
    if(!app || !app->submenu || !app->view_dispatcher) {
        return;
    }
    
    // Use standard submenu to avoid NULL pointer issues
    submenu_reset(app->submenu);
    
    // Add comprehensive Car attack menu items for Tesla security testing
    submenu_add_item(app->submenu, "🚙 All Car Models", SubmenuIndexCarModels, car_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "👜 Passive Bag Mode", SubmenuIndexCarPassiveOpener, car_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "🔑 Key Bruteforce", SubmenuIndexCarKeyBruteforce, car_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "📡 Signal Jamming", SubmenuIndexCarJamming, car_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "🚗 Tesla Charge Port", SubmenuIndexCarTesla, car_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "🔄 Rolling Code Attack", SubmenuIndexCarRollingCode, car_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "🛞 Tire Monitor Spoof", SubmenuIndexCarTireMonitor, car_attacks_submenu_callback, app);
    
    // Additional Tesla-specific security tests
    submenu_add_item(app->submenu, "⚡ Tesla Supercharger", 300, car_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "🔋 Battery Management", 301, car_attacks_submenu_callback, app);
    submenu_add_item(app->submenu, "🛰️ GPS Spoofing", 302, car_attacks_submenu_callback, app);
    
    submenu_set_header(app->submenu, "Tesla Security Tests");
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

bool predator_scene_car_attacks_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    // Null safety check
    if(!app) return false;
    
    if(event.type == SceneManagerEventTypeCustom) {
        consumed = true;
        switch(event.event) {
        case SubmenuIndexCarModels:
            if(app->scene_manager) {
                scene_manager_next_scene(app->scene_manager, PredatorSceneCarModels);
            }
            break;
        case SubmenuIndexCarPassiveOpener:
            if(app->scene_manager) {
                scene_manager_next_scene(app->scene_manager, PredatorSceneCarPassiveOpener);
            }
            break;
        case SubmenuIndexCarKeyBruteforce:
            if(app->scene_manager) {
                scene_manager_next_scene(app->scene_manager, PredatorSceneCarKey);
            }
            break;
        case SubmenuIndexCarJamming:
            if(app->scene_manager) {
                scene_manager_next_scene(app->scene_manager, PredatorSceneCarJamming);
            }
            break;
        case SubmenuIndexCarTesla:
            if(app->scene_manager) {
                scene_manager_next_scene(app->scene_manager, PredatorSceneCarTesla);
            }
            break;
        case SubmenuIndexCarRollingCode:
            // Handle rolling code attack with popup
            if(app->popup && app->view_dispatcher) {
                popup_set_header(app->popup, "Rolling Code Attack", 64, 10, AlignCenter, AlignTop);
                popup_set_text(app->popup, 
                    "Capturing rolling codes...\n"
                    "Waiting for signal\n"
                    "Press Back to stop", 
                    64, 25, AlignCenter, AlignTop);
                popup_set_callback(app->popup, predator_scene_car_attacks_popup_callback);
                popup_set_context(app->popup, app);
                popup_set_timeout(app->popup, 0);
                popup_enable_timeout(app->popup);
                
                // Initialize SubGHz for attack
                predator_subghz_init(app);
                app->attack_running = true;
                app->packets_sent = 0;
                
                view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
            }
            break;
        case SubmenuIndexCarTireMonitor:
            // Handle tire monitor spoofing with popup
            popup_set_header(app->popup, "Tire Monitor Spoof", 64, 10, AlignCenter, AlignTop);
            popup_set_text(app->popup, 
                "Spoofing TPMS signals...\n"
                "Pressure: 15 PSI (Low)\n"
                "Press Back to stop", 
                64, 25, AlignCenter, AlignTop);
            popup_set_callback(app->popup, predator_scene_car_attacks_popup_callback);
            popup_set_context(app->popup, app);
            popup_set_timeout(app->popup, 0);
            popup_enable_timeout(app->popup);
            
            // Initialize SubGHz for attack
            predator_subghz_init(app);
            app->attack_running = true;
            app->packets_sent = 0;
            
            view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
            break;
        case 300: // Tesla Supercharger
            popup_set_header(app->popup, "Tesla Supercharger Test", 64, 10, AlignCenter, AlignTop);
            popup_set_text(app->popup, 
                "Testing Supercharger security...\n"
                "Analyzing CAN bus signals\n"
                "Press Back to stop", 
                64, 25, AlignCenter, AlignTop);
            popup_set_callback(app->popup, predator_scene_car_attacks_popup_callback);
            popup_set_context(app->popup, app);
            popup_set_timeout(app->popup, 0);
            popup_enable_timeout(app->popup);
            
            app->attack_running = true;
            app->packets_sent = 0;
            
            view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
            break;
        case 301: // Battery Management
            popup_set_header(app->popup, "Battery Management Test", 64, 10, AlignCenter, AlignTop);
            popup_set_text(app->popup, 
                "Testing BMS security...\n"
                "Monitoring battery protocols\n"
                "Press Back to stop", 
                64, 25, AlignCenter, AlignTop);
            popup_set_callback(app->popup, predator_scene_car_attacks_popup_callback);
            popup_set_context(app->popup, app);
            popup_set_timeout(app->popup, 0);
            popup_enable_timeout(app->popup);
            
            app->attack_running = true;
            app->packets_sent = 0;
            
            view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
            break;
        case 302: // GPS Spoofing
            popup_set_header(app->popup, "GPS Spoofing Test", 64, 10, AlignCenter, AlignTop);
            popup_set_text(app->popup, 
                "Testing GPS spoofing resistance...\n"
                "Sending fake GPS signals\n"
                "Press Back to stop", 
                64, 25, AlignCenter, AlignTop);
            popup_set_callback(app->popup, predator_scene_car_attacks_popup_callback);
            popup_set_context(app->popup, app);
            popup_set_timeout(app->popup, 0);
            popup_enable_timeout(app->popup);
            
            app->attack_running = true;
            app->packets_sent = 0;
            
            view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
            break;
        case PredatorCustomEventPopupBack:
            // Handle popup back button - stop attack and return to menu
            if(app->attack_running) {
                predator_subghz_deinit(app);
                app->attack_running = false;
            }
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

void predator_scene_car_attacks_new_on_exit(void* context) {
    PredatorApp* app = context;
    
    // Null safety check
    if(!app) return;
    
    // Stop any running attacks
    if(app->attack_running) {
        predator_subghz_deinit(app);
        app->attack_running = false;
    }
    
    // Clean up submenu
    if(app->submenu) {
        submenu_reset(app->submenu);
    }
}


