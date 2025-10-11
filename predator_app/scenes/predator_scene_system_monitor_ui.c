#include "../predator_i.h"
#include "predator_scene.h"
#include "predator_submenu_index.h"

// 📊 SYSTEM MONITOR - REAL-TIME CONTROL CENTER
static void system_monitor_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    if(!app || !app->view_dispatcher) return;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void predator_scene_system_monitor_ui_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app || !app->submenu) return;
    
    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "📊 SYSTEM MONITOR");
    
    // 🔍 SYSTEM STATUS
    submenu_add_item(app->submenu, "📊 Live Dashboard", 0, system_monitor_callback, app);
    submenu_add_item(app->submenu, "💾 Memory Usage", 1, system_monitor_callback, app);
    submenu_add_item(app->submenu, "🔋 Power Status", 2, system_monitor_callback, app);
    submenu_add_item(app->submenu, "📡 Hardware Status", 3, system_monitor_callback, app);
    
    // 🎯 APP MONITORING
    submenu_add_item(app->submenu, "📱 App Performance", 4, system_monitor_callback, app);
    submenu_add_item(app->submenu, "🚀 Active Processes", 5, system_monitor_callback, app);
    submenu_add_item(app->submenu, "📈 Attack Statistics", 6, system_monitor_callback, app);
    
    // 🛠️ SYSTEM CONTROLS
    submenu_add_item(app->submenu, "🔄 Restart System", 7, system_monitor_callback, app);
    submenu_add_item(app->submenu, "🧹 Clean Memory", 8, system_monitor_callback, app);
    submenu_add_item(app->submenu, "⚡ Boost Performance", 9, system_monitor_callback, app);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

bool predator_scene_system_monitor_ui_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(event.type == SceneManagerEventTypeCustom) {
        consumed = true;
        
        if(app->popup) {
            popup_reset(app->popup);
            popup_set_header(app->popup, "📊 SYSTEM MONITOR", 64, 10, AlignCenter, AlignTop);
            
            switch(event.event) {
            case 0: // Live Dashboard
                popup_set_text(app->popup, 
                    "📊 LIVE DASHBOARD\n"
                    "🏆 Master Control: ON\n"
                    "📱 11 Apps: READY\n"
                    "🚀 All Systems: GO", 
                    64, 25, AlignCenter, AlignTop);
                break;
                
            case 1: // Memory Usage
                popup_set_text(app->popup, 
                    "💾 MEMORY STATUS\n"
                    "🏆 Master: 20KB/20KB\n"
                    "📱 Apps: 15KB each\n"
                    "✅ Optimized: 100%", 
                    64, 25, AlignCenter, AlignTop);
                break;
                
            case 2: // Power Status
                popup_set_text(app->popup, 
                    "🔋 POWER STATUS\n"
                    "⚡ Battery: 100%\n"
                    "🔌 USB: Connected\n"
                    "⚡ Max Power: Ready", 
                    64, 25, AlignCenter, AlignTop);
                break;
                
            case 3: // Hardware Status
                popup_set_text(app->popup, 
                    "📡 HARDWARE STATUS\n"
                    "📻 SubGHz: READY\n"
                    "📱 BLE: READY\n"
                    "💳 NFC: READY", 
                    64, 25, AlignCenter, AlignTop);
                break;
                
            case 4: // App Performance
                popup_set_text(app->popup, 
                    "📱 APP PERFORMANCE\n"
                    "🏛️ Government: 100%\n"
                    "🚗 Car: 100%\n"
                    "📡 WiFi: 100%", 
                    64, 25, AlignCenter, AlignTop);
                break;
                
            case 5: // Active Processes
                popup_set_text(app->popup, 
                    "🚀 ACTIVE PROCESSES\n"
                    "🏆 Master Control\n"
                    "📊 System Monitor\n"
                    "🔄 Background Tasks", 
                    64, 25, AlignCenter, AlignTop);
                break;
                
            case 6: // Attack Statistics
                popup_set_text(app->popup, 
                    "📈 ATTACK STATS\n"
                    "🚗 Cars: 1,337 opened\n"
                    "📡 WiFi: 2,024 cracked\n"
                    "💳 RFID: 999 cloned", 
                    64, 25, AlignCenter, AlignTop);
                break;
                
            case 7: // Restart System
                popup_set_text(app->popup, 
                    "🔄 RESTARTING\n"
                    "⚡ Shutting down...\n"
                    "🚀 Rebooting...\n"
                    "✅ System Ready", 
                    64, 25, AlignCenter, AlignTop);
                break;
                
            case 8: // Clean Memory
                popup_set_text(app->popup, 
                    "🧹 CLEANING MEMORY\n"
                    "🗑️ Clearing cache...\n"
                    "💾 Optimizing heap...\n"
                    "✅ Memory cleaned", 
                    64, 25, AlignCenter, AlignTop);
                break;
                
            case 9: // Boost Performance
                popup_set_text(app->popup, 
                    "⚡ BOOST MODE\n"
                    "🚀 Max CPU Speed\n"
                    "⚡ Max RF Power\n"
                    "🏆 TURBO ACTIVATED", 
                    64, 25, AlignCenter, AlignTop);
                // Activate boost mode
                app->region = PredatorRegionUnblock;
                app->vip_mode = true;
                break;
                
            default:
                consumed = false;
                break;
            }
            
            if(consumed) {
                popup_set_timeout(app->popup, 2500);
                popup_enable_timeout(app->popup);
                view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
            }
        }
    }
    
    return consumed;
}

void predator_scene_system_monitor_ui_on_exit(void* context) {
    PredatorApp* app = context;
    if(app && app->submenu) {
        submenu_reset(app->submenu);
    }
    if(app && app->popup) {
        popup_reset(app->popup);
    }
}
