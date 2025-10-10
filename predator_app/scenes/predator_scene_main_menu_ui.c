#include "../predator_i.h"
#include "predator_scene.h"
#include "predator_submenu_index.h"

// Main Menu - Professional UI
// Clean submenu implementation for Tesla demo

static void main_menu_submenu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    if(!app || !app->view_dispatcher) return;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void predator_scene_main_menu_ui_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app || !app->submenu) return;
    
    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "🔧 PREDATOR Security Testing");
    
    // Professional workflow-oriented menus
    submenu_add_item(app->submenu, "🚗 Tesla Security Testing", SubmenuIndexTeslaSecuritySuite, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "🚙 Multi-Brand Car Testing", SubmenuIndexCarBrandSecuritySuite, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "⚡ Quick Car Assessment", SubmenuIndexUserFriendlyUI, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "🔍 Comprehensive Car Audit", SubmenuIndexUniversalCarHacker, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "🤖 AI Attack Coordinator", SubmenuIndexAIAttackCoordinator, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "⚡ Maximum Power Mode", SubmenuIndexMaximumPower, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "🔍 System Audit", SubmenuIndexSystemAudit, main_menu_submenu_callback, app);
    
    // Add main menu items (Professional UI only)
    submenu_add_item(app->submenu, "📡 WiFi Attacks", SubmenuIndexWifiAttacks, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "📱 Bluetooth Attacks", SubmenuIndexBluetoothAttacks, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "🚗 Car Attacks", SubmenuIndexCarAttacks, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "📻 SubGHz Attacks", SubmenuIndexSubGhzAttacks, main_menu_submenu_callback, app);
    submenu_add_item(app->submenu, "💳 RFID Attacks", SubmenuIndexRfidAttacks, main_menu_submenu_callback, app);
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
        case SubmenuIndexAIAttackCoordinator:
            scene_manager_next_scene(app->scene_manager, PredatorSceneAIAttackCoordinator);
            break;
        case SubmenuIndexMaximumPower:
            scene_manager_next_scene(app->scene_manager, PredatorSceneMaximumPower);
            break;
        case SubmenuIndexSystemAudit:
            scene_manager_next_scene(app->scene_manager, PredatorSceneSystemAudit);
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
        case SubmenuIndexGpsTracker:
            scene_manager_next_scene(app->scene_manager, PredatorSceneGpsTrackerUI);
            break;
        case SubmenuIndexWardriving:
            scene_manager_next_scene(app->scene_manager, PredatorSceneWardrivingUI);
            break;
        case SubmenuIndexSocialEngineering:
            scene_manager_next_scene(app->scene_manager, PredatorSceneSocialEngineeringUI);
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
