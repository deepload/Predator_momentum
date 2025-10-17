#include "../predator_i.h"
#include "predator_scene.h"
#include "../helpers/predator_boards.h"
#include "predator_submenu_index.h"

// Main Menu - Professional UI (Memory Optimized, No Guards)
// Clean submenu implementation for Tesla demo
// ANTI-ACCIDENTAL-EXIT: Double-press Back to exit app
// 🎮 KONAMI CODE: Up Up Down Down Left Right Left Right OK (Easter Egg!)

static uint32_t last_back_press_time = 0;
#define DOUBLE_PRESS_TIMEOUT_MS 2000 // 2 seconds to press Back again

// 🎮 KONAMI CODE DISABLED - Breaks submenu navigation when enabled
// TODO: Re-implement using view_dispatcher input callback instead of view-level override
/*
// Konami Code State Tracker
typedef enum {
    KonamiStep0_Start = 0,
    KonamiStep1_Up,
    KonamiStep2_Up2,
    KonamiStep3_Down,
    KonamiStep4_Down2,
    KonamiStep5_Left,
    KonamiStep6_Right,
    KonamiStep7_Left2,
    KonamiStep8_Right2,
    KonamiStep9_Complete
} KonamiCodeStep;

static KonamiCodeStep konami_state = KonamiStep0_Start;
static uint32_t last_konami_input_time = 0;
#define KONAMI_TIMEOUT_MS 5000
*/

// Safe exit macro
#define PREDATOR_SAFE_EXIT_APP(app) \
    do { \
        if(app && app->view_dispatcher) { \
            view_dispatcher_stop(app->view_dispatcher); \
        } \
    } while(0)

static void main_menu_submenu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    if(!app || !app->view_dispatcher) return;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void predator_scene_main_menu_ui_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app || !app->submenu) return;
    
    // Reset back button timer when entering main menu
    last_back_press_time = 0;
    
    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "🔧 PREDATOR Optimized");
    
    // PROFESSIONAL - OPTIMIZED FOR MEMORY & FUNCTIONALITY
    submenu_add_item(app->submenu, "🚗 Tesla Security", 1, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "🚗 Car Models", 2, main_menu_submenu_callback, app);
    // 🎮 Walking Open removed - replaced by Konami code easter egg!
    submenu_add_item(app->submenu, "🚧 Parking Barriers", 15, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "📡 WiFi Attacks", 4, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "📱 Bluetooth Attacks", 5, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "💳 RFID Attacks", 6, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "📻 SubGHz Attacks", 7, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "📊 Module Status", 8, main_menu_submenu_callback, app);
    
    // Simple card/board selection menu item
    submenu_add_item(app->submenu, "🔧 Card Selection", 12, main_menu_submenu_callback, app);
    
    submenu_add_item(app->submenu, "⚙️ Settings", 13, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "ℹ️ About", 14, main_menu_submenu_callback, app);
    
    // 🎮 KONAMI CODE NOTE: Custom input callback breaks submenu navigation
    // TODO: Implement via view_dispatcher input callback instead of view-level override
    // For now, Konami code logic is disabled to maintain menu functionality
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

bool predator_scene_main_menu_ui_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    // ANTI-ACCIDENTAL-EXIT: Double-press Back to exit app
    if(event.type == SceneManagerEventTypeBack) {
        uint32_t current_time = furi_get_tick();
        uint32_t time_since_last = current_time - last_back_press_time;
        
        // Check if this is a double-press (within 2 seconds)
        if(time_since_last < DOUBLE_PRESS_TIMEOUT_MS && last_back_press_time != 0) {
            // DOUBLE PRESS DETECTED - Exit app
            FURI_LOG_I("MainMenu", "Double Back press detected - exiting app");
            PREDATOR_SAFE_EXIT_APP(app);
            return true;
        } else {
            // FIRST PRESS - Show warning and wait for second press
            FURI_LOG_I("MainMenu", "Press Back again to exit app");
            last_back_press_time = current_time;
            
            // Optional: Show a popup or notification
            if(app->notifications) {
                notification_message(app->notifications, &sequence_single_vibro);
            }
            
            return true; // Consume the event but don't exit
        }
    }
    
    if(event.type == SceneManagerEventTypeCustom) {
        consumed = true;
        switch(event.event) {
        case 1: // Tesla Security
            scene_manager_next_scene(app->scene_manager, PredatorSceneCarTeslaUI);
            return true;
        case 2: // Car Models
            scene_manager_next_scene(app->scene_manager, PredatorSceneCarContinentUI);
            return true;
        // case 3: Walking Open - REMOVED (replaced by Konami code)
        case 15: // Parking Barriers - Swiss Government
            scene_manager_next_scene(app->scene_manager, PredatorSceneParkingBarriersUI);
            return true;
        case 4: // WiFi Attacks
            scene_manager_next_scene(app->scene_manager, PredatorSceneWifiAttacksUI);
            return true;
        case 5: // Bluetooth Attacks
            scene_manager_next_scene(app->scene_manager, PredatorSceneBluetoothAttacksUI);
            return true;
        case 6: // RFID Attacks
            scene_manager_next_scene(app->scene_manager, PredatorSceneRfidAttacksUI);
            return true;
        case 7: // SubGHz Attacks
            scene_manager_next_scene(app->scene_manager, PredatorSceneSubGhzAttacksUI);
            return true;
        case 8: // Module Status
            scene_manager_next_scene(app->scene_manager, PredatorSceneModuleStatusUI);
            return true;
        case 12: // Board Selection
            scene_manager_next_scene(app->scene_manager, PredatorSceneBoardSelectionUI);
            return true;
        case 13: // Settings
            scene_manager_next_scene(app->scene_manager, PredatorSceneSettingsUI);
            return true;
        case 14: // About
            scene_manager_next_scene(app->scene_manager, PredatorSceneAboutUI);
            return true;
        default:
            consumed = false;
            break;
        }
    }
    
    return consumed;
}

void predator_scene_main_menu_ui_on_exit(void* context) {
    PredatorApp* app = context;
    if(app && app->submenu) {
        submenu_reset(app->submenu);
    }
}
