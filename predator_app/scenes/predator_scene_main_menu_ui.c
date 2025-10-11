#include "../predator_i.h"
#include "predator_scene.h"
#include "predator_submenu_index.h"
#include "../helpers/predator_workflow_validator.h"  // RE-ENABLED - Only 32 bytes

// Main Menu - Professional UI
// Clean submenu implementation for Tesla demo

static void main_menu_submenu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    if(!app || !app->view_dispatcher) return;
    
    // PROFESSIONAL WORKFLOW VALIDATION (re-enabled - lightweight)
    if(!predator_workflow_validate_submenu_callback(app, index)) {
        return; // Validation failed, error already shown
    }
    
    predator_workflow_add_breadcrumb(app, "Main Menu Selection");
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void predator_scene_main_menu_ui_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app || !app->submenu) return;
    
    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "🏆 PREDATOR MASTER");
    
    // 🏆 MASTER CONTROL CENTER - KEEP ALL FEATURES
    submenu_add_item(app->submenu, "🏆 Master Control", SubmenuIndexMasterControl, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "🎛️ Master Orchestrator", SubmenuIndexMasterOrchestrator, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "📱 App Launcher", SubmenuIndexAppLauncher, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "📊 System Monitor", SubmenuIndexSystemMonitor, main_menu_submenu_callback, app);
    
    // 🚨 EMERGENCY CONTROLS - KEEP ALL
    submenu_add_item(app->submenu, "🚨 Emergency Mode", SubmenuIndexEmergencyMode, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "🇫🇷 Elon Renault Demo", SubmenuIndexElonRenaultDemo, main_menu_submenu_callback, app);
    
    // ⚛️ VIP++ FEATURES - KEEP ALL ELON'S REQUIREMENTS
    submenu_add_item(app->submenu, "⚛️ Tesla VIP++", SubmenuIndexTeslaSecuritySuite, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "🚗 All Car Brands", SubmenuIndexCarBrandSecuritySuite, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "🎯 One-Click Hack", SubmenuIndexUserFriendlyUI, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "🚗 Universal Hacker", SubmenuIndexUniversalCarHacker, main_menu_submenu_callback, app);
    
    // 🏛️ GOVERNMENT SUITE - KEEP ALL CONTRACTS
    submenu_add_item(app->submenu, "🏛️ Government Mode", SubmenuIndexGovernmentMode, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "🚦 Traffic Control", SubmenuIndexTrafficInfrastructure, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "🏭 Infrastructure", SubmenuIndexCriticalInfrastructure, main_menu_submenu_callback, app);
    
    // 🎯 CORE ATTACK CATEGORIES - KEEP ALL
    submenu_add_item(app->submenu, "📡 WiFi Attacks", SubmenuIndexWifiAttacks, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "🚗 Car Attacks", SubmenuIndexCarAttacks, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "📱 Bluetooth", SubmenuIndexBluetoothAttacks, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "📻 SubGHz", SubmenuIndexSubGhzAttacks, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "💳 RFID", SubmenuIndexRfidAttacks, main_menu_submenu_callback, app);
    
    // ⚙️ SYSTEM - KEEP ALL
    submenu_add_item(app->submenu, "📊 Status", SubmenuIndexModuleStatus, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "🔧 Board", SubmenuIndexBoardSelection, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "⚙️ Settings", SubmenuIndexSettings, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "ℹ️ About", SubmenuIndexAbout, main_menu_submenu_callback, app);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

bool predator_scene_main_menu_ui_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(event.type == SceneManagerEventTypeCustom) {
        consumed = true;
        switch(event.event) {
        // 🏆 MASTER CONTROL CENTER - KEEP ALL FEATURES
        case SubmenuIndexMasterControl:
            scene_manager_next_scene(app->scene_manager, PredatorSceneMasterControlUI);
            break;
        case SubmenuIndexMasterOrchestrator:
            scene_manager_next_scene(app->scene_manager, PredatorSceneMasterOrchestratorSimple);
            break;
        case SubmenuIndexAppLauncher:
            scene_manager_next_scene(app->scene_manager, PredatorSceneAppLauncherUI);
            break;
        case SubmenuIndexSystemMonitor:
            scene_manager_next_scene(app->scene_manager, PredatorSceneSystemMonitorUI);
            break;
            
        // 🚨 EMERGENCY CONTROLS - KEEP ALL
        case SubmenuIndexEmergencyMode:
            // Activate emergency mode
            app->region = PredatorRegionUnblock;
            app->vip_mode = true;
            if(app->popup) {
                popup_reset(app->popup);
                popup_set_header(app->popup, "🚨 EMERGENCY MODE", 64, 10, AlignCenter, AlignTop);
                popup_set_text(app->popup, 
                    "⚡ MAX POWER ACTIVATED\n"
                    "🚀 ALL SYSTEMS GO\n"
                    "🏆 GOVERNMENT GRADE", 
                    64, 25, AlignCenter, AlignTop);
                popup_set_timeout(app->popup, 2000);
                popup_enable_timeout(app->popup);
                view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
            }
            break;
            
        case SubmenuIndexElonRenaultDemo:
            scene_manager_next_scene(app->scene_manager, PredatorSceneElonRenaultDemo);
            break;
            
        // ⚛️ VIP++ FEATURES - KEEP ALL ELON'S REQUIREMENTS
        case SubmenuIndexTeslaSecuritySuite:
            scene_manager_next_scene(app->scene_manager, PredatorSceneTeslaSecuritySuite);
            break;
        case SubmenuIndexCarBrandSecuritySuite:
            scene_manager_next_scene(app->scene_manager, PredatorSceneCarBrandSecuritySuite);
            break;
        case SubmenuIndexUserFriendlyUI:
            scene_manager_next_scene(app->scene_manager, PredatorSceneUserFriendlyUI);
            break;
        case SubmenuIndexUniversalCarHacker:
            scene_manager_next_scene(app->scene_manager, PredatorSceneUniversalCarHacker);
            break;
            
        // 🏛️ GOVERNMENT SUITE - KEEP ALL CONTRACTS
        case SubmenuIndexGovernmentMode:
            scene_manager_next_scene(app->scene_manager, PredatorSceneGovernmentContractsUI);
            break;
        case SubmenuIndexTrafficInfrastructure:
            scene_manager_next_scene(app->scene_manager, PredatorSceneTrafficInfrastructureUI);
            break;
        case SubmenuIndexCriticalInfrastructure:
            scene_manager_next_scene(app->scene_manager, PredatorSceneCriticalInfrastructureUI);
            break;
            
        // 🎯 CORE ATTACK CATEGORIES - KEEP ALL
        case SubmenuIndexWifiAttacks:
            scene_manager_next_scene(app->scene_manager, PredatorSceneWifiAttacksUI);
            break;
        case SubmenuIndexCarAttacks:
            scene_manager_next_scene(app->scene_manager, PredatorSceneCarAttacksUI);
            break;
        case SubmenuIndexBluetoothAttacks:
            scene_manager_next_scene(app->scene_manager, PredatorSceneBluetoothAttacksUI);
            break;
        case SubmenuIndexSubGhzAttacks:
            scene_manager_next_scene(app->scene_manager, PredatorSceneSubGhzAttacksUI);
            break;
        case SubmenuIndexRfidAttacks:
            scene_manager_next_scene(app->scene_manager, PredatorSceneRfidAttacksUI);
            break;
            
        // ⚙️ SYSTEM - KEEP ALL
        case SubmenuIndexModuleStatus:
            scene_manager_next_scene(app->scene_manager, PredatorSceneModuleStatusUI);
            break;
        case SubmenuIndexBoardSelection:
            scene_manager_next_scene(app->scene_manager, PredatorSceneBoardSelectionUI);
            break;
        case SubmenuIndexSettings:
            scene_manager_next_scene(app->scene_manager, PredatorSceneSettingsUI);
            break;
        case SubmenuIndexAbout:
            scene_manager_next_scene(app->scene_manager, PredatorSceneAboutUI);
            break;
            
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
