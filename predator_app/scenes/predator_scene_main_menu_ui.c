#include "../predator_i.h"
#include "predator_scene.h"
#include "predator_submenu_index.h"
#include "../helpers/predator_workflow_validator.h"

// Main Menu - Professional UI
// Clean submenu implementation for Tesla demo

static void main_menu_submenu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    if(!app || !app->view_dispatcher) return;
    
    // PROFESSIONAL WORKFLOW VALIDATION
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
    submenu_set_header(app->submenu, "🚗 PREDATOR SECURITY SUITE");
    
    // VIP++ QUANTUM-READY - ELON'S TESLA SECURITY SUITE
    submenu_add_item(app->submenu, "⚛️ VIP++ QUANTUM TESLA SUITE", SubmenuIndexTeslaSecuritySuite, main_menu_submenu_callback, app);
    
    // VIP++ ALL CAR BRANDS - DEMONSTRATE SUPERIORITY
    submenu_add_item(app->submenu, "🚗 VIP++ ALL CAR BRANDS", SubmenuIndexCarBrandSecuritySuite, main_menu_submenu_callback, app);
    
    // USER-FRIENDLY FLIPPER ZERO UI - READY FOR YESTERDAY
    submenu_add_item(app->submenu, "🎯 ONE-CLICK CAR HACK", SubmenuIndexUserFriendlyUI, main_menu_submenu_callback, app);
    
    // UNIVERSAL CAR HACKER - STANDALONE APP THAT CAN DO EVERYTHING
    submenu_add_item(app->submenu, "🚗 UNIVERSAL CAR HACKER", SubmenuIndexUniversalCarHacker, main_menu_submenu_callback, app);
    
    // === NEW GOVERNMENT CONTRACT FEATURES ===
    submenu_add_item(app->submenu, "🚦 Traffic Infrastructure", SubmenuIndexTrafficInfrastructure, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "⚡ Tesla Advanced 2024", SubmenuIndexTeslaAdvanced, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "🏎️ Automotive Advanced", SubmenuIndexAutomotiveAdvancedNew, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "🎰 Casino/RFID Advanced", SubmenuIndexCasinoRFID, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "🏛️ CRITICAL INFRASTRUCTURE", SubmenuIndexCriticalInfrastructure, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "🌍 GLOBAL GOVERNMENT CONTRACTS", SubmenuIndexGovernmentContracts, main_menu_submenu_callback, app);
    
    // Add main menu items (Professional UI only)
    submenu_add_item(app->submenu, "📡 WiFi Attacks", SubmenuIndexWifiAttacks, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "📱 Bluetooth Attacks", SubmenuIndexBluetoothAttacks, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "🚗 Car Attacks", SubmenuIndexCarAttacks, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "📻 SubGHz Attacks", SubmenuIndexSubGhzAttacks, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "💳 RFID Attacks", SubmenuIndexRfidAttacks, main_menu_submenu_callback, app);
    
    // Advanced attack categories
    submenu_add_item(app->submenu, "🔬 Advanced RF/SubGHz", SubmenuIndexAdvancedRF, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "💎 Advanced NFC/RFID", SubmenuIndexAdvancedNFC, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "🌐 Advanced WiFi", SubmenuIndexAdvancedWiFi, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "📲 Advanced Bluetooth", SubmenuIndexAdvancedBT, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "🚙 Automotive Advanced", SubmenuIndexAutomotiveAdvanced, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "📡 Infrared Remote", SubmenuIndexInfrared, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "🔌 USB Attacks", SubmenuIndexUSBAttacks, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "🌍 Network Attacks", SubmenuIndexNetworkAttacks, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "🔐 Crypto Tools", SubmenuIndexCryptoTools, main_menu_submenu_callback, app);
    
    submenu_add_item(app->submenu, "🛰️ GPS Tracker", SubmenuIndexGpsTracker, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "🚙 Wardriving", SubmenuIndexWardriving, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "🎭 Social Engineering", SubmenuIndexSocialEngineering, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "📊 Module Status", SubmenuIndexModuleStatus, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "🔧 Board Selection", SubmenuIndexBoardSelection, main_menu_submenu_callback, app);
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
        // Disabled scenes - show professional feedback
        case SubmenuIndexTeslaSecuritySuite:
        case SubmenuIndexCarBrandSecuritySuite:
        case SubmenuIndexUserFriendlyUI:
        case SubmenuIndexUniversalCarHacker:
        case SubmenuIndexGpsTracker:
        case SubmenuIndexWardriving:
        case SubmenuIndexSocialEngineering:
            // Show professional "Coming Soon" popup
            if(app->popup) {
                popup_reset(app->popup);
                popup_set_header(app->popup, "🚧 FEATURE DEVELOPMENT", 64, 10, AlignCenter, AlignTop);
                popup_set_text(app->popup, 
                    "This advanced feature is\ncurrently under development\n\n"
                    "🏛️ Available in next update\n"
                    "📧 Contact support for ETA", 
                    64, 25, AlignCenter, AlignTop);
                popup_set_timeout(app->popup, 3000);
                popup_enable_timeout(app->popup);
                view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
            }
            break;
        // Advanced attack categories
        case SubmenuIndexAdvancedRF:
            predator_workflow_safe_scene_transition(app, PredatorSceneAdvancedRFUI);
            break;
        case SubmenuIndexAdvancedNFC:
            scene_manager_next_scene(app->scene_manager, PredatorSceneAdvancedNFCUI);
            break;
        case SubmenuIndexAdvancedWiFi:
            scene_manager_next_scene(app->scene_manager, PredatorSceneAdvancedWiFiUI);
            break;
        case SubmenuIndexAdvancedBT:
            scene_manager_next_scene(app->scene_manager, PredatorSceneAdvancedBTUI);
            break;
        case SubmenuIndexAutomotiveAdvanced:
            scene_manager_next_scene(app->scene_manager, PredatorSceneAutomotiveAdvancedUI);
            break;
        case SubmenuIndexInfrared:
            scene_manager_next_scene(app->scene_manager, PredatorSceneInfraredUI);
            break;
        case SubmenuIndexUSBAttacks:
            scene_manager_next_scene(app->scene_manager, PredatorSceneUSBAttacksUI);
            break;
        case SubmenuIndexNetworkAttacks:
            scene_manager_next_scene(app->scene_manager, PredatorSceneNetworkAttacksUI);
            break;
        case SubmenuIndexCryptoTools:
            scene_manager_next_scene(app->scene_manager, PredatorSceneCryptoToolsUI);
            break;
        case SubmenuIndexWifiAttacks:
            scene_manager_next_scene(app->scene_manager, PredatorSceneWifiAttacksUI);
            break;
        case SubmenuIndexBluetoothAttacks:
            scene_manager_next_scene(app->scene_manager, PredatorSceneBluetoothAttacksUI);
            break;
        case SubmenuIndexCarAttacks:
            scene_manager_next_scene(app->scene_manager, PredatorSceneCarAttacksUI);
            break;
        case SubmenuIndexSubGhzAttacks:
            scene_manager_next_scene(app->scene_manager, PredatorSceneSubGhzAttacksUI);
            break;
        case SubmenuIndexRfidAttacks:
            scene_manager_next_scene(app->scene_manager, PredatorSceneRfidAttacksUI);
            break;
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
        // NEW GOVERNMENT CONTRACT FEATURES
        case SubmenuIndexTrafficInfrastructure:
            scene_manager_next_scene(app->scene_manager, PredatorSceneTrafficInfrastructureUI);
            break;
        case SubmenuIndexTeslaAdvanced:
            scene_manager_next_scene(app->scene_manager, PredatorSceneTeslaAdvancedUI);
            break;
        case SubmenuIndexAutomotiveAdvancedNew:
            scene_manager_next_scene(app->scene_manager, PredatorSceneAutomotiveAdvUI);
            break;
        case SubmenuIndexCasinoRFID:
            scene_manager_next_scene(app->scene_manager, PredatorSceneCasinoRFIDUI);
            break;
        case SubmenuIndexCriticalInfrastructure:
            scene_manager_next_scene(app->scene_manager, PredatorSceneCriticalInfrastructureUI);
            break;
        case SubmenuIndexGovernmentContracts:
            scene_manager_next_scene(app->scene_manager, PredatorSceneGovernmentContractsUI);
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
