#include "../predator_i.h"
#include "../helpers/predator_app_communication.h"
#include "../helpers/predator_ecosystem_manager.h"

// ⚔️ COORDINATED ATTACK - Multi-App Orchestration

typedef struct {
    Submenu* submenu;
    PredatorAppType selected_apps[12];
    uint32_t selected_count;
    bool attack_active;
    char target_info[64];
    uint32_t attack_progress;
} PredatorCoordinatedAttack;

typedef enum {
    SubmenuIndexSelectWifiCar,
    SubmenuIndexSelectAllCore,
    SubmenuIndexSelectGovernment,
    SubmenuIndexSelectTeslaFocused,
    SubmenuIndexSelectInfrastructure,
    SubmenuIndexSelectComprehensive,
    SubmenuIndexCustomSelection,
    SubmenuIndexLaunchAttack,
    SubmenuIndexAttackStatus,
    SubmenuIndexStopAttack
} SubmenuIndex;

static PredatorCoordinatedAttack* coord_attack = NULL;

static void coordinated_attack_submenu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    furi_assert(app);

    if(!coord_attack) return;

    switch(index) {
    case SubmenuIndexSelectWifiCar:
        // WiFi + Car combo attack
        coord_attack->selected_count = 2;
        coord_attack->selected_apps[0] = PredatorAppTypeWifi;
        coord_attack->selected_apps[1] = PredatorAppTypeCar;
        strncpy(coord_attack->target_info, "WiFi + Car Combo Attack", sizeof(coord_attack->target_info) - 1);
        break;
        
    case SubmenuIndexSelectAllCore:
        // All core attack apps
        coord_attack->selected_count = 6;
        coord_attack->selected_apps[0] = PredatorAppTypeWifi;
        coord_attack->selected_apps[1] = PredatorAppTypeCar;
        coord_attack->selected_apps[2] = PredatorAppTypeBluetooth;
        coord_attack->selected_apps[3] = PredatorAppTypeRfid;
        coord_attack->selected_apps[4] = PredatorAppTypeSubghz;
        coord_attack->selected_apps[5] = PredatorAppTypeNetwork;
        strncpy(coord_attack->target_info, "All Core Attack Apps", sizeof(coord_attack->target_info) - 1);
        break;
        
    case SubmenuIndexSelectGovernment:
        // Government-grade comprehensive attack
        coord_attack->selected_count = 7;
        coord_attack->selected_apps[0] = PredatorAppTypeGovernment;
        coord_attack->selected_apps[1] = PredatorAppTypeWifi;
        coord_attack->selected_apps[2] = PredatorAppTypeInfrastructure;
        coord_attack->selected_apps[3] = PredatorAppTypeNetwork;
        coord_attack->selected_apps[4] = PredatorAppTypeCrypto;
        coord_attack->selected_apps[5] = PredatorAppTypeRfid;
        coord_attack->selected_apps[6] = PredatorAppTypeSubghz;
        strncpy(coord_attack->target_info, "Government-Grade Assessment", sizeof(coord_attack->target_info) - 1);
        break;
        
    case SubmenuIndexSelectTeslaFocused:
        // Tesla-focused attack coordination
        coord_attack->selected_count = 4;
        coord_attack->selected_apps[0] = PredatorAppTypeTesla;
        coord_attack->selected_apps[1] = PredatorAppTypeCar;
        coord_attack->selected_apps[2] = PredatorAppTypeSubghz;
        coord_attack->selected_apps[3] = PredatorAppTypeRfid;
        strncpy(coord_attack->target_info, "Tesla Security Assessment", sizeof(coord_attack->target_info) - 1);
        break;
        
    case SubmenuIndexSelectInfrastructure:
        // Infrastructure security assessment
        coord_attack->selected_count = 5;
        coord_attack->selected_apps[0] = PredatorAppTypeInfrastructure;
        coord_attack->selected_apps[1] = PredatorAppTypeNetwork;
        coord_attack->selected_apps[2] = PredatorAppTypeWifi;
        coord_attack->selected_apps[3] = PredatorAppTypeCrypto;
        coord_attack->selected_apps[4] = PredatorAppTypeSubghz;
        strncpy(coord_attack->target_info, "Infrastructure Security", sizeof(coord_attack->target_info) - 1);
        break;
        
    case SubmenuIndexSelectComprehensive:
        // All available apps - maximum firepower
        coord_attack->selected_count = 11;
        coord_attack->selected_apps[0] = PredatorAppTypeGovernment;
        coord_attack->selected_apps[1] = PredatorAppTypeWifi;
        coord_attack->selected_apps[2] = PredatorAppTypeCar;
        coord_attack->selected_apps[3] = PredatorAppTypeTesla;
        coord_attack->selected_apps[4] = PredatorAppTypeBluetooth;
        coord_attack->selected_apps[5] = PredatorAppTypeRfid;
        coord_attack->selected_apps[6] = PredatorAppTypeSubghz;
        coord_attack->selected_apps[7] = PredatorAppTypeInfrastructure;
        coord_attack->selected_apps[8] = PredatorAppTypeCasino;
        coord_attack->selected_apps[9] = PredatorAppTypeNetwork;
        coord_attack->selected_apps[10] = PredatorAppTypeCrypto;
        strncpy(coord_attack->target_info, "COMPREHENSIVE - All Apps", sizeof(coord_attack->target_info) - 1);
        break;
        
    case SubmenuIndexLaunchAttack:
        if(coord_attack->selected_count > 0) {
            // Launch coordinated attack
            coord_attack->attack_active = true;
            coord_attack->attack_progress = 0;
            
            // Execute coordinated attack via ecosystem manager
            predator_ecosystem_launch_coordinated_attack(
                coord_attack->selected_apps, 
                coord_attack->selected_count, 
                coord_attack->target_info
            );
            
            // Update master app status
            predator_ipc_update_status(PredatorAppTypeMaster, PredatorAppStatusRunning, "Coordinated Attack Active");
            
            FURI_LOG_I("CoordAttack", "Launched coordinated attack with %lu apps: %s", 
                      coord_attack->selected_count, coord_attack->target_info);
        }
        break;
        
    case SubmenuIndexAttackStatus:
        scene_manager_next_scene(app->scene_manager, PredatorSceneAttackStatus);
        break;
        
    case SubmenuIndexStopAttack:
        if(coord_attack->attack_active) {
            // Stop all coordinated apps
            for(uint32_t i = 0; i < coord_attack->selected_count; i++) {
                predator_ipc_send_command(PredatorCommandStopApp, coord_attack->selected_apps[i], "stop_coordinated");
                predator_ipc_update_status(coord_attack->selected_apps[i], PredatorAppStatusIdle, "Stopped by Master");
            }
            
            coord_attack->attack_active = false;
            coord_attack->attack_progress = 0;
            
            predator_ipc_update_status(PredatorAppTypeMaster, PredatorAppStatusCompleted, "Coordinated Attack Stopped");
            
            FURI_LOG_I("CoordAttack", "Stopped coordinated attack");
        }
        break;
    }
}

void predator_scene_coordinated_attack_on_enter(void* context) {
    PredatorApp* app = context;
    
    // Initialize coordinated attack
    coord_attack = malloc(sizeof(PredatorCoordinatedAttack));
    memset(coord_attack, 0, sizeof(PredatorCoordinatedAttack));
    
    coord_attack->submenu = submenu_alloc();
    
    // Professional coordinated attack header
    submenu_set_header(coord_attack->submenu, "⚔️ COORDINATED ATTACK");
    
    // 🎯 PRE-CONFIGURED ATTACK COMBINATIONS
    submenu_add_item(coord_attack->submenu, "📶🚗 WiFi + Car Combo", SubmenuIndexSelectWifiCar, coordinated_attack_submenu_callback, app);
    submenu_add_item(coord_attack->submenu, "🚀 All Core Apps (6)", SubmenuIndexSelectAllCore, coordinated_attack_submenu_callback, app);
    submenu_add_item(coord_attack->submenu, "🏛️ Government Suite (7)", SubmenuIndexSelectGovernment, coordinated_attack_submenu_callback, app);
    submenu_add_item(coord_attack->submenu, "⚡ Tesla Focused (4)", SubmenuIndexSelectTeslaFocused, coordinated_attack_submenu_callback, app);
    submenu_add_item(coord_attack->submenu, "🏗️ Infrastructure (5)", SubmenuIndexSelectInfrastructure, coordinated_attack_submenu_callback, app);
    submenu_add_item(coord_attack->submenu, "💥 COMPREHENSIVE (11)", SubmenuIndexSelectComprehensive, coordinated_attack_submenu_callback, app);
    
    // 🎮 ATTACK CONTROL
    submenu_add_item(coord_attack->submenu, "🚀 LAUNCH ATTACK", SubmenuIndexLaunchAttack, coordinated_attack_submenu_callback, app);
    submenu_add_item(coord_attack->submenu, "📊 Attack Status", SubmenuIndexAttackStatus, coordinated_attack_submenu_callback, app);
    submenu_add_item(coord_attack->submenu, "🛑 STOP ATTACK", SubmenuIndexStopAttack, coordinated_attack_submenu_callback, app);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
    view_set_previous_callback(submenu_get_view(coord_attack->submenu), predator_navigation_submenu_callback);
    
    // Initialize with WiFi + Car combo as default
    coord_attack->selected_count = 2;
    coord_attack->selected_apps[0] = PredatorAppTypeWifi;
    coord_attack->selected_apps[1] = PredatorAppTypeCar;
    strncpy(coord_attack->target_info, "WiFi + Car Combo Attack", sizeof(coord_attack->target_info) - 1);
    
    FURI_LOG_I("CoordAttack", "Coordinated Attack scene initialized");
}

bool predator_scene_coordinated_attack_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        consumed = true;
    }

    return consumed;
}

void predator_scene_coordinated_attack_on_exit(void* context) {
    PredatorApp* app = context;
    
    if(coord_attack) {
        // Stop any active attack
        if(coord_attack->attack_active) {
            for(uint32_t i = 0; i < coord_attack->selected_count; i++) {
                predator_ipc_send_command(PredatorCommandStopApp, coord_attack->selected_apps[i], "cleanup");
            }
        }
        
        // Clean up submenu
        if(coord_attack->submenu) {
            submenu_free(coord_attack->submenu);
        }
        
        // Clean up coordinated attack
        free(coord_attack);
        coord_attack = NULL;
        
        FURI_LOG_I("CoordAttack", "Coordinated Attack scene cleaned up");
    }
}
