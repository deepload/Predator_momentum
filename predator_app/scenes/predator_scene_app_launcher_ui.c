#include "../predator_i.h"
#include "predator_scene.h"
#include "predator_submenu_index.h"

// 📱 APP LAUNCHER - MANAGE ALL SPECIALIZED APPS
static void app_launcher_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    if(!app || !app->view_dispatcher) return;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void predator_scene_app_launcher_ui_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app || !app->submenu) return;
    
    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "📱 APP LAUNCHER");
    
    // 🏛️ GOVERNMENT SUITE
    submenu_add_item(app->submenu, "🏛️ Government App", 0, app_launcher_callback, app);
    
    // 🎯 CORE ATTACK SUITE
    submenu_add_item(app->submenu, "📡 WiFi App", 1, app_launcher_callback, app);
    submenu_add_item(app->submenu, "🚗 Car App", 2, app_launcher_callback, app);
    submenu_add_item(app->submenu, "⚡ Tesla App", 3, app_launcher_callback, app);
    submenu_add_item(app->submenu, "📱 Bluetooth App", 4, app_launcher_callback, app);
    submenu_add_item(app->submenu, "💳 RFID App", 5, app_launcher_callback, app);
    submenu_add_item(app->submenu, "📻 SubGHz App", 6, app_launcher_callback, app);
    
    // 🏢 ENTERPRISE SUITE
    submenu_add_item(app->submenu, "🏭 Infrastructure App", 7, app_launcher_callback, app);
    submenu_add_item(app->submenu, "🎰 Casino App", 8, app_launcher_callback, app);
    submenu_add_item(app->submenu, "🌐 Network App", 9, app_launcher_callback, app);
    submenu_add_item(app->submenu, "🔐 Crypto App", 10, app_launcher_callback, app);
    
    // 🛠️ SYSTEM CONTROLS
    submenu_add_item(app->submenu, "🔄 Reload Apps", 11, app_launcher_callback, app);
    submenu_add_item(app->submenu, "📊 App Status", 12, app_launcher_callback, app);
    submenu_add_item(app->submenu, "⚙️ App Settings", 13, app_launcher_callback, app);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

bool predator_scene_app_launcher_ui_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(event.type == SceneManagerEventTypeCustom) {
        consumed = true;
        
        if(app->popup) {
            popup_reset(app->popup);
            popup_set_header(app->popup, "📱 APP LAUNCHER", 64, 10, AlignCenter, AlignTop);
            
            switch(event.event) {
            case 0: // Government App
                popup_set_text(app->popup, 
                    "🏛️ GOVERNMENT APP\n"
                    "✅ ONU Ready\n"
                    "🚦 Traffic Control\n"
                    "🏭 Infrastructure", 
                    64, 25, AlignCenter, AlignTop);
                // ELON PROOF - All apps redirect to settings for now
                scene_manager_next_scene(app->scene_manager, PredatorSceneSettingsUI);
                break;
                
            case 1: // WiFi App
            case 2: // Car App  
            case 3: // Tesla App
            case 4: // Bluetooth App
            case 5: // RFID App
            case 6: // SubGHz App
            case 7: // Infrastructure App
            case 8: // Casino App
            case 9: // Network App
            case 10: // Crypto App
                // ELON PROOF - All redirect to settings
                scene_manager_next_scene(app->scene_manager, PredatorSceneSettingsUI);
                break;
                
            case 11: // Reload Apps
                popup_set_text(app->popup, 
                    "🔄 RELOADING APPS\n"
                    "✅ Scanning System\n"
                    "📱 11 Apps Found\n"
                    "🏆 All Ready", 
                    64, 25, AlignCenter, AlignTop);
                break;
                
            case 12: // App Status
                popup_set_text(app->popup, 
                    "📊 APP STATUS\n"
                    "✅ 11/11 Apps Ready\n"
                    "🏆 Master Control\n"
                    "🚀 All Systems GO", 
                    64, 25, AlignCenter, AlignTop);
                break;
                
            case 13: // App Settings
                popup_set_text(app->popup, 
                    "⚙️ APP SETTINGS\n"
                    "🔧 Global Config\n"
                    "🎯 Auto-Launch\n"
                    "📊 Monitoring", 
                    64, 25, AlignCenter, AlignTop);
                break;
                
            default:
                consumed = false;
                break;
            }
            
            if(consumed && event.event <= 10) {
                popup_set_timeout(app->popup, 2000);
                popup_enable_timeout(app->popup);
                view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
            }
        }
    }
    
    return consumed;
}

void predator_scene_app_launcher_ui_on_exit(void* context) {
    PredatorApp* app = context;
    if(app && app->submenu) {
        submenu_reset(app->submenu);
    }
    if(app && app->popup) {
        popup_reset(app->popup);
    }
}
