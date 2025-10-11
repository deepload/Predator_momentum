#include "../predator_i.h"
#include "predator_scene.h"
#include "predator_submenu_index.h"

// 🏆 MASTER CONTROL CENTER - ULTIMATE COMMAND CENTER
static void master_control_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    if(!app || !app->view_dispatcher) return;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void predator_scene_master_control_ui_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app || !app->submenu) return;
    
    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "🏆 MASTER CONTROL");
    
    // 🚀 SPECIALIZED APP LAUNCHER
    submenu_add_item(app->submenu, "📱 App Launcher", 0, master_control_callback, app);
    submenu_add_item(app->submenu, "📊 System Monitor", 1, master_control_callback, app);
    submenu_add_item(app->submenu, "⚙️ Global Settings", 2, master_control_callback, app);
    
    // 🚨 EMERGENCY CONTROLS
    submenu_add_item(app->submenu, "🚨 Emergency Mode", 3, master_control_callback, app);
    submenu_add_item(app->submenu, "🇫🇷 Elon Renault Demo", 4, master_control_callback, app);
    
    // 🏛️ GOVERNMENT SUITE
    submenu_add_item(app->submenu, "🏛️ Government Mode", 5, master_control_callback, app);
    submenu_add_item(app->submenu, "🚦 Traffic Control", 6, master_control_callback, app);
    submenu_add_item(app->submenu, "🏭 Infrastructure", 7, master_control_callback, app);
    
    // 🎯 SPECIALIZED ATTACKS
    submenu_add_item(app->submenu, "📡 WiFi Suite", 8, master_control_callback, app);
    submenu_add_item(app->submenu, "🚗 Car Suite", 9, master_control_callback, app);
    submenu_add_item(app->submenu, "⚡ Tesla Suite", 10, master_control_callback, app);
    submenu_add_item(app->submenu, "📱 Bluetooth Suite", 11, master_control_callback, app);
    submenu_add_item(app->submenu, "💳 RFID Suite", 12, master_control_callback, app);
    submenu_add_item(app->submenu, "📻 SubGHz Suite", 13, master_control_callback, app);
    submenu_add_item(app->submenu, "🎰 Casino Suite", 14, master_control_callback, app);
    submenu_add_item(app->submenu, "🔐 Crypto Suite", 15, master_control_callback, app);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

bool predator_scene_master_control_ui_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(event.type == SceneManagerEventTypeCustom) {
        consumed = true;
        
        switch(event.event) {
        case 0: // App Launcher
            scene_manager_next_scene(app->scene_manager, PredatorSceneAppLauncherUI);
            break;
            
        case 1: // System Monitor
            scene_manager_next_scene(app->scene_manager, PredatorSceneSystemMonitorUI);
            break;
            
        case 2: // Global Settings
            scene_manager_next_scene(app->scene_manager, PredatorSceneSettingsUI);
            break;
            
        case 3: // Emergency Mode
            if(app->popup) {
                popup_reset(app->popup);
                popup_set_header(app->popup, "🚨 EMERGENCY MODE", 64, 10, AlignCenter, AlignTop);
                popup_set_text(app->popup, 
                    "ALL SYSTEMS ACTIVE\n"
                    "MAXIMUM POWER\n"
                    "UNBLOCK REGION\n"
                    "GOVERNMENT GRADE", 
                    64, 25, AlignCenter, AlignTop);
                popup_set_timeout(app->popup, 2000);
                popup_enable_timeout(app->popup);
                view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
                
                // Activate emergency mode
                app->region = PredatorRegionUnblock;
                app->vip_mode = true;
            }
            break;
            
        case 4: // Elon Renault Demo
            scene_manager_next_scene(app->scene_manager, PredatorSceneElonRenaultDemo);
            break;
            
        case 5: // Government Mode
        case 6: // Traffic Control
        case 7: // Infrastructure
        case 8: // WiFi Suite
        case 9: // Car Suite
        case 10: // Tesla Suite
        case 11: // Bluetooth Suite
        case 12: // RFID Suite
        case 13: // SubGHz Suite
        case 14: // Casino Suite
        case 15: // Crypto Suite
            // All redirect to settings for now - ELON PROOF
            scene_manager_next_scene(app->scene_manager, PredatorSceneSettingsUI);
            break;
            
        default:
            consumed = false;
            break;
        }
    }
    
    return consumed;
}

void predator_scene_master_control_ui_on_exit(void* context) {
    PredatorApp* app = context;
    if(app && app->submenu) {
        submenu_reset(app->submenu);
    }
    if(app && app->popup) {
        popup_reset(app->popup);
    }
}
