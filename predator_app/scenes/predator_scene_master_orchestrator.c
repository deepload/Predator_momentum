#include "../predator_i.h"
#include "../helpers/predator_app_communication.h"
#include "../helpers/predator_ecosystem_manager.h"

// 🏆 MASTER ORCHESTRATOR - THE GENIUS DIFFERENTIATOR!
// Professional Inter-App Communication and Coordination

typedef struct {
    Submenu* submenu;
    uint32_t selected_apps_count;
    PredatorAppType selected_apps[12];
    bool orchestration_active;
    char status_message[128];
    uint32_t active_workflows;
    PredatorEcosystemState ecosystem_state;
} PredatorMasterOrchestrator;

typedef enum {
    SubmenuIndexViewAllApps,
    SubmenuIndexLaunchCoordinatedAttack,
    SubmenuIndexParallelScan,
    SubmenuIndexSequentialExploit,
    SubmenuIndexGovernmentWorkflow,
    SubmenuIndexTeslaWorkflow,
    SubmenuIndexComprehensiveScan,
    SubmenuIndexEmergencyCoordination,
    SubmenuIndexVipCoordination,
    SubmenuIndexRealTimeCommunication,
    SubmenuIndexEcosystemStatus,
    SubmenuIndexWorkflowManager,
    SubmenuIndexDataSharing,
    SubmenuIndexPerformanceMetrics,
    SubmenuIndexDemoSuperiority
} SubmenuIndex;

static PredatorMasterOrchestrator* orchestrator = NULL;

static void master_orchestrator_submenu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    furi_assert(app);

    switch(index) {
    case SubmenuIndexViewAllApps:
        scene_manager_next_scene(app->scene_manager, PredatorSceneAppStatus);
        break;
    case SubmenuIndexLaunchCoordinatedAttack:
        scene_manager_next_scene(app->scene_manager, PredatorSceneCoordinatedAttack);
        break;
    case SubmenuIndexParallelScan:
        scene_manager_next_scene(app->scene_manager, PredatorSceneParallelScan);
        break;
    case SubmenuIndexSequentialExploit:
        scene_manager_next_scene(app->scene_manager, PredatorSceneSequentialExploit);
        break;
    case SubmenuIndexGovernmentWorkflow:
        scene_manager_next_scene(app->scene_manager, PredatorSceneGovernmentWorkflow);
        break;
    case SubmenuIndexTeslaWorkflow:
        scene_manager_next_scene(app->scene_manager, PredatorSceneTeslaWorkflow);
        break;
    case SubmenuIndexComprehensiveScan:
        scene_manager_next_scene(app->scene_manager, PredatorSceneComprehensiveScan);
        break;
    case SubmenuIndexEmergencyCoordination:
        scene_manager_next_scene(app->scene_manager, PredatorSceneEmergencyCoordination);
        break;
    case SubmenuIndexVipCoordination:
        scene_manager_next_scene(app->scene_manager, PredatorSceneVipCoordination);
        break;
    case SubmenuIndexRealTimeCommunication:
        scene_manager_next_scene(app->scene_manager, PredatorSceneLiveCommunicationDemo);
        break;
    case SubmenuIndexEcosystemStatus:
        scene_manager_next_scene(app->scene_manager, PredatorSceneEcosystemStatus);
        break;
    case SubmenuIndexWorkflowManager:
        scene_manager_next_scene(app->scene_manager, PredatorSceneWorkflowManager);
        break;
    case SubmenuIndexDataSharing:
        scene_manager_next_scene(app->scene_manager, PredatorSceneDataSharing);
        break;
    case SubmenuIndexPerformanceMetrics:
        scene_manager_next_scene(app->scene_manager, PredatorScenePerformanceMetrics);
        break;
    case SubmenuIndexDemoSuperiority:
        scene_manager_next_scene(app->scene_manager, PredatorSceneDemoSuperiority);
        break;
    }
}

void predator_scene_master_orchestrator_on_enter(void* context) {
    PredatorApp* app = context;
    
    // Initialize orchestrator
    orchestrator = malloc(sizeof(PredatorMasterOrchestrator));
    memset(orchestrator, 0, sizeof(PredatorMasterOrchestrator));
    
    // Initialize ecosystem manager
    if(!predator_ecosystem_init()) {
        FURI_LOG_E("Orchestrator", "Failed to initialize ecosystem manager");
        strncpy(orchestrator->status_message, "❌ Ecosystem initialization failed", sizeof(orchestrator->status_message) - 1);
    } else {
        predator_ecosystem_start_monitoring();
        strncpy(orchestrator->status_message, "✅ Ecosystem ready for orchestration", sizeof(orchestrator->status_message) - 1);
    }
    
    // Get current ecosystem state
    predator_ecosystem_get_state(&orchestrator->ecosystem_state);
    
    orchestrator->submenu = submenu_alloc();
    
    // Professional orchestration header
    submenu_set_header(orchestrator->submenu, "🏆 MASTER ORCHESTRATOR");
    
    // 🔍 APP MANAGEMENT
    submenu_add_item(orchestrator->submenu, "📱 View All Apps", SubmenuIndexViewAllApps, master_orchestrator_submenu_callback, app);
    
    // 🚀 COORDINATED OPERATIONS
    submenu_add_item(orchestrator->submenu, "⚔️ Coordinated Attack", SubmenuIndexLaunchCoordinatedAttack, master_orchestrator_submenu_callback, app);
    submenu_add_item(orchestrator->submenu, "🔄 Parallel Scan", SubmenuIndexParallelScan, master_orchestrator_submenu_callback, app);
    submenu_add_item(orchestrator->submenu, "📋 Sequential Exploit", SubmenuIndexSequentialExploit, master_orchestrator_submenu_callback, app);
    
    // 🏛️ PROFESSIONAL WORKFLOWS
    submenu_add_item(orchestrator->submenu, "🏛️ Government Workflow", SubmenuIndexGovernmentWorkflow, master_orchestrator_submenu_callback, app);
    submenu_add_item(orchestrator->submenu, "🚗 Tesla Workflow", SubmenuIndexTeslaWorkflow, master_orchestrator_submenu_callback, app);
    submenu_add_item(orchestrator->submenu, "🔍 Comprehensive Scan", SubmenuIndexComprehensiveScan, master_orchestrator_submenu_callback, app);
    
    // 🚨 EMERGENCY & VIP MODES
    submenu_add_item(orchestrator->submenu, "🚨 Emergency Coordination", SubmenuIndexEmergencyCoordination, master_orchestrator_submenu_callback, app);
    submenu_add_item(orchestrator->submenu, "💎 VIP Coordination", SubmenuIndexVipCoordination, master_orchestrator_submenu_callback, app);
    
    // 📡 COMMUNICATION & MONITORING
    submenu_add_item(orchestrator->submenu, "📡 Real-Time Communication", SubmenuIndexRealTimeCommunication, master_orchestrator_submenu_callback, app);
    submenu_add_item(orchestrator->submenu, "📊 Ecosystem Status", SubmenuIndexEcosystemStatus, master_orchestrator_submenu_callback, app);
    submenu_add_item(orchestrator->submenu, "⚙️ Workflow Manager", SubmenuIndexWorkflowManager, master_orchestrator_submenu_callback, app);
    submenu_add_item(orchestrator->submenu, "🔄 Data Sharing", SubmenuIndexDataSharing, master_orchestrator_submenu_callback, app);
    submenu_add_item(orchestrator->submenu, "📈 Performance Metrics", SubmenuIndexPerformanceMetrics, master_orchestrator_submenu_callback, app);
    
    // 🏆 MARKET DOMINATION
    submenu_add_item(orchestrator->submenu, "🏆 Demo Superiority", SubmenuIndexDemoSuperiority, master_orchestrator_submenu_callback, app);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
    view_set_previous_callback(submenu_get_view(orchestrator->submenu), predator_navigation_submenu_callback);
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
    
    // Register master app with IPC system
    predator_ipc_register_app(PredatorAppTypeMaster, "Predator Master Orchestrator", "2.0");
    predator_ipc_update_status(PredatorAppTypeMaster, PredatorAppStatusRunning, "Master Orchestration Active");
    
    FURI_LOG_I("Orchestrator", "Master Orchestrator initialized - %s", orchestrator->status_message);
}

bool predator_scene_master_orchestrator_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        consumed = true;
    }

    return consumed;
}

void predator_scene_master_orchestrator_on_exit(void* context) {
    PredatorApp* app = context;
    
    if(orchestrator) {
        // Stop ecosystem monitoring
        predator_ecosystem_stop_monitoring();
        
        // Update master app status
        predator_ipc_update_status(PredatorAppTypeMaster, PredatorAppStatusIdle, "Orchestration Stopped");
        
        // Clean up submenu
        if(orchestrator->submenu) {
            submenu_free(orchestrator->submenu);
        }
        
        // Clean up orchestrator
        free(orchestrator);
        orchestrator = NULL;
        
        FURI_LOG_I("Orchestrator", "Master Orchestrator stopped");
    }
}
