#include "../predator_i.h"
#include "predator_scene.h"
#include "../helpers/predator_boards.h"
#include "../helpers/predator_regional_crypto_keys.h"  // Regional crypto keys
#include "../helpers/predator_global_surveillance.h"   // Global surveillance
#include "../helpers/predator_car_domination.h"       // Car domination
#include "../helpers/predator_logging.h"              // Enhanced logging
#include "predator_submenu_index.h"

// Main Menu - Professional UI (Memory Optimized, No Guards)
// Clean submenu implementation for Tesla demo
// ANTI-ACCIDENTAL-EXIT: Double-press Back to exit app
// 🎮 KONAMI CODE: Up Up Down Down Left Right Left Right OK (Easter Egg!)

static uint32_t last_back_press_time = 0;
static uint32_t menu_enter_time = 0;
static bool allow_exit = false;  // CRITICAL: Flag to control exit
#define DOUBLE_PRESS_TIMEOUT_MS 2000 // 2 seconds to press Back again
#define GRACE_PERIOD_MS 1000 // Ignore back presses for first 1000ms after entering menu (was 500ms)

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
    FURI_LOG_E("MainMenu", "MENU ENTER");
    FURI_LOG_E("MainMenu", "v2.0 EXIT BLOCK");
    
    if(!app || !app->submenu) {
        FURI_LOG_E("MainMenu", "NULL ERROR");
        return;
    }
    
    // CRITICAL FIX: Reset ALL exit protection flags
    menu_enter_time = furi_get_tick();
    last_back_press_time = 0;  // Reset back press timer
    allow_exit = false;  // BLOCK all exits until double-press
    
    FURI_LOG_I("MainMenu", "ENTER t=%lu BLOCK", menu_enter_time);
    
    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "PREDATOR v2.0 NUCLEAR");
    
    // GOVERNMENT APPROVED - ALL CAPABILITIES ACCESSIBLE
    submenu_add_item(app->submenu, "Car Attacks", 1, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "Tesla Security", 2, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "Car Hack", 21, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "Traffic Lights", 20, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "SCADA", 22, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "Social Eng", 23, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "Satellite", 24, main_menu_submenu_callback, app);
#ifndef GLOBAL_SURVEILLANCE_DISABLED
    submenu_add_item(app->submenu, "Surveillance", 25, main_menu_submenu_callback, app);
#endif
    submenu_add_item(app->submenu, "GPS", 17, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "Wardriving", 18, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "Barriers", 15, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "WiFi", 4, main_menu_submenu_callback, app);  // STEP 1: RESTORED
    // submenu_add_item(app->submenu, "📱 Bluetooth Attacks", 5, main_menu_submenu_callback, app);  // REVERTED: TOO MUCH MEMORY
    submenu_add_item(app->submenu, "RFID", 6, main_menu_submenu_callback, app);
    // submenu_add_item(app->submenu, "📻 SubGHz Attacks", 7, main_menu_submenu_callback, app);  // COMMENTED FOR MEMORY
    submenu_add_item(app->submenu, "Monitor", 19, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "Status", 8, main_menu_submenu_callback, app);
    
    // Simple card/board selection menu item
    submenu_add_item(app->submenu, "Boards", 12, main_menu_submenu_callback, app);
    
    submenu_add_item(app->submenu, "Settings", 13, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "About", 14, main_menu_submenu_callback, app);
    
    // 🎮 KONAMI CODE NOTE: Custom input callback breaks submenu navigation
    // TODO: Implement via view_dispatcher input callback instead of view-level override
    // For now, Konami code logic is disabled to maintain menu functionality
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

bool predator_scene_main_menu_ui_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    
    FURI_LOG_D("MainMenu", "Event %d", event.type);
    
    // CRITICAL: Handle back button FIRST, before anything else
    // ALWAYS return true to prevent app from exiting
    if(event.type == SceneManagerEventTypeBack) {
        uint32_t current_time = furi_get_tick();
        uint32_t time_since_enter = current_time - menu_enter_time;
        uint32_t time_since_last_back = (last_back_press_time == 0) ? 99999 : (current_time - last_back_press_time);
        
        FURI_LOG_E("MainMenu", "BACK t=%lu last=%lu exit=%d", time_since_enter, time_since_last_back, allow_exit);
        
        // ABSOLUTE PROTECTION: Grace period - ignore ALL back presses for first 1000ms
        if(time_since_enter < GRACE_PERIOD_MS) {
            FURI_LOG_W("MainMenu", "BLOCK grace %lu<%d", time_since_enter, GRACE_PERIOD_MS);
            return true;  // CONSUME - NEVER exit
        }
        
        // Check if this is a double press (second press within timeout)
        if(last_back_press_time != 0 && time_since_last_back < DOUBLE_PRESS_TIMEOUT_MS) {
            // DOUBLE PRESS DETECTED - Allow exit
            FURI_LOG_E("MainMenu", "DOUBLE BACK - EXIT");
            allow_exit = true;  // Allow the exit
            PREDATOR_SAFE_EXIT_APP(app);
            return true;
        } else {
            // FIRST PRESS - Show warning and start timer
            FURI_LOG_W("MainMenu", "FIRST BACK - 2s to exit");
            last_back_press_time = current_time;
            allow_exit = false;  // Ensure exit is still blocked
            
            // Vibrate to confirm first press
            if(app->notifications) {
                notification_message(app->notifications, &sequence_single_vibro);
            }
            
            return true; // CRITICAL: ALWAYS return true - IMPOSSIBLE to return false
        }
    }
    
    // Handle menu item selections
    if(event.type == SceneManagerEventTypeCustom) {
        FURI_LOG_D("MainMenu", "Item %lu", event.event);
        switch(event.event) {
        case 1: // Car Attacks (Complete)
            scene_manager_next_scene(app->scene_manager, PredatorSceneCarAttacksMainUI);
            break;
        case 2: // Tesla Security Suite
            scene_manager_next_scene(app->scene_manager, PredatorSceneCarTeslaUI);
            break;
        case 21: // Advanced Car Hacking
            scene_manager_next_scene(app->scene_manager, PredatorSceneAdvancedCarHackingUI);
            break;
        case 20: // Traffic Lights - Swiss Government
            scene_manager_next_scene(app->scene_manager, PredatorSceneTrafficLightSecurityUI);
            break;
        case 22: // Industrial SCADA
            scene_manager_next_scene(app->scene_manager, PredatorSceneIndustrialScadaUI);
            break;
        case 23: // Social Engineering
            scene_manager_next_scene(app->scene_manager, PredatorSceneSocialEngineeringToolkitUI);
            break;
        case 24: // Satellite Communication
            scene_manager_next_scene(app->scene_manager, PredatorSceneSatelliteCommunicationUI);
            break;
#ifndef GLOBAL_SURVEILLANCE_DISABLED
        case 25: // Global Surveillance
            scene_manager_next_scene(app->scene_manager, PredatorSceneGlobalSurveillanceUI);
            break;
#endif
        case 17: // GPS Tracker - Government Approved
            scene_manager_next_scene(app->scene_manager, PredatorSceneGpsTrackerUI);
            break;
        case 18: // Wardriving - Government Approved
            scene_manager_next_scene(app->scene_manager, PredatorSceneWardrivingUI);
            break;
        case 15: // Parking Barriers - Swiss Government
            scene_manager_next_scene(app->scene_manager, PredatorSceneParkingBarriersUI);
            break;
        case 19: // Live Monitor
            scene_manager_next_scene(app->scene_manager, PredatorSceneLiveMonitorUI);
            break;
        case 4: // WiFi Attacks - STEP 1: RESTORED
            scene_manager_next_scene(app->scene_manager, PredatorSceneWifiAttacksUI);
            break;
        case 5: // Bluetooth Attacks - REVERTED: TOO MUCH MEMORY
            // scene_manager_next_scene(app->scene_manager, PredatorSceneBluetoothAttacksUI);
            break;
        case 6: // RFID Attacks
            scene_manager_next_scene(app->scene_manager, PredatorSceneRfidAttacksUI);
            break;
        case 7: // SubGHz Attacks - TEMPORARILY DISABLED FOR MEMORY
            // scene_manager_next_scene(app->scene_manager, PredatorSceneSubGhzAttacksUI);
            break;
        case 8: // Module Status
            scene_manager_next_scene(app->scene_manager, PredatorSceneModuleStatusUI);
            break;
        case 12: // Board Selection
            scene_manager_next_scene(app->scene_manager, PredatorSceneBoardSelectionUI);
            break;
        case 13: // Settings
            scene_manager_next_scene(app->scene_manager, PredatorSceneSettingsUI);
            break;
        case 14: // About
            scene_manager_next_scene(app->scene_manager, PredatorSceneAboutUI);
            break;
        default:
            FURI_LOG_W("MainMenu", "Unknown %lu", event.event);
            break;
        }
        return true;  // Always consume custom events
    }
    
    // CRITICAL: Default return - consume ALL events to prevent app exit
    FURI_LOG_D("MainMenu", "Consume %d", event.type);
    return true;
}

void predator_scene_main_menu_ui_on_exit(void* context) {
    PredatorApp* app = context;
    FURI_LOG_E("MainMenu", "EXIT allow=%d", allow_exit);
    if(!allow_exit) {
        FURI_LOG_E("MainMenu", "EXIT BUG - no double press");
    }
    if(app && app->submenu) {
        submenu_reset(app->submenu);
    }
}
