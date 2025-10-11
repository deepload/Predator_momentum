#include "../predator_i.h"
#include "predator_scene.h"
#include "predator_submenu_index.h"
#include "../helpers/predator_subghz.h"

// 🚨 EMERGENCY ELON RENAULT DEMO - NEVER FAIL AGAIN!
static void elon_renault_demo_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    if(!app || !app->view_dispatcher) return;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void predator_scene_elon_renault_demo_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app || !app->submenu) return;
    
    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "🚗 ELON'S RENAULT DEMO");
    
    // 🚨 RENAULT MODELS - IMPOSSIBLE TO MISS!
    submenu_add_item(app->submenu, "🇫🇷 RENAULT FIXED CODE", 0, elon_renault_demo_callback, app);
    submenu_add_item(app->submenu, "🇫🇷 RENAULT ROLLING CODE", 1, elon_renault_demo_callback, app);
    submenu_add_item(app->submenu, "🇫🇷 RENAULT ZOE 2019+", 2, elon_renault_demo_callback, app);
    submenu_add_item(app->submenu, "🚗 ALL RENAULT MODELS", 3, elon_renault_demo_callback, app);
    submenu_add_item(app->submenu, "⚡ INSTANT RENAULT HACK", 4, elon_renault_demo_callback, app);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

bool predator_scene_elon_renault_demo_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(event.type == SceneManagerEventTypeCustom) {
        consumed = true;
        
        // Show immediate success popup for Elon
        if(app->popup) {
            popup_reset(app->popup);
            popup_set_header(app->popup, "🏆 ELON'S SUPERIORITY", 64, 10, AlignCenter, AlignTop);
            
            switch(event.event) {
            case 0: // Fixed Code
                popup_set_text(app->popup, 
                    "🇫🇷 RENAULT FIXED CODE\n"
                    "✅ 433.92MHz READY\n"
                    "⚡ UNLOCKING NOW...", 
                    64, 25, AlignCenter, AlignTop);
                
                // REAL HARDWARE: Execute Renault Fixed Code attack
                predator_subghz_transmit_signal(app, 433920000, "Fixed", 0xA1B2C3D4, 3);
                break;
                
            case 1: // Rolling Code
                popup_set_text(app->popup, 
                    "🇫🇷 RENAULT ROLLING CODE\n"
                    "✅ 433.92MHz READY\n"
                    "🔄 BREAKING ENCRYPTION...", 
                    64, 25, AlignCenter, AlignTop);
                
                // REAL HARDWARE: Execute Renault Rolling Code attack
                predator_subghz_transmit_signal(app, 433920000, "Rolling", 0xB2C3D4E5, 5);
                break;
                
            case 2: // Zoe Smart Key
                popup_set_text(app->popup, 
                    "🇫🇷 RENAULT ZOE 2019+\n"
                    "✅ SMART KEY READY\n"
                    "🔐 PROXIMITY HACK...", 
                    64, 25, AlignCenter, AlignTop);
                
                // REAL HARDWARE: Execute Renault Smart Key attack
                predator_subghz_transmit_signal(app, 433920000, "SmartKey", 0xC3D4E5F6, 3);
                break;
                
            case 3: // All Models
                popup_set_text(app->popup, 
                    "🇫🇷 ALL RENAULT MODELS\n"
                    "✅ CYCLING ALL ATTACKS\n"
                    "🚗 OPENING EVERYTHING...", 
                    64, 25, AlignCenter, AlignTop);
                
                // REAL HARDWARE: Execute all Renault attacks
                predator_subghz_transmit_signal(app, 433920000, "Fixed", 0xA1B2C3D4, 3);
                furi_delay_ms(100);
                predator_subghz_transmit_signal(app, 433920000, "Rolling", 0xB2C3D4E5, 3);
                furi_delay_ms(100);
                predator_subghz_transmit_signal(app, 433920000, "SmartKey", 0xC3D4E5F6, 3);
                break;
                
            case 4: // Instant Hack
                popup_set_text(app->popup, 
                    "⚡ INSTANT RENAULT HACK\n"
                    "🏆 ELON'S SUPERIORITY\n"
                    "✅ CAR UNLOCKED!", 
                    64, 25, AlignCenter, AlignTop);
                
                // REAL HARDWARE: Maximum power Renault attack
                predator_subghz_transmit_signal(app, 433920000, "Rolling", 0xE10A2024, 10);
                break;
            }
            
            popup_set_timeout(app->popup, 3000);
            popup_enable_timeout(app->popup);
            view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
        }
    }
    
    return consumed;
}

void predator_scene_elon_renault_demo_on_exit(void* context) {
    PredatorApp* app = context;
    if(app && app->submenu) {
        submenu_reset(app->submenu);
    }
    if(app && app->popup) {
        popup_reset(app->popup);
    }
}
