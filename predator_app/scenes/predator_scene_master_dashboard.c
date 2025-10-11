#include "../predator_i.h"
#include "predator_scene.h"
#include "../helpers/predator_app_communication.h"
#include "../helpers/predator_professional_ui.h"

// 🏆 REVOLUTIONARY MASTER DASHBOARD - THE GENIUS DIFFERENTIATOR!
// Real-time communication between ALL specialized apps
// NO COMPETITOR HAS THIS - MARKET DOMINATION GUARANTEED!

typedef struct {
    Canvas* canvas;
    PredatorApp* app;
    uint32_t update_timer;
    PredatorAppInfo apps[12];
    uint32_t app_count;
    bool communication_active;
    uint32_t total_operations;
    uint32_t success_rate;
    char status_message[128];
} MasterDashboard;

static MasterDashboard* dashboard = NULL;

// Professional Dashboard Drawing
static void draw_master_dashboard(Canvas* canvas, void* context) {
    PredatorApp* app = context;
    if(!app || !dashboard) return;
    
    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);
    
    // 🏆 REVOLUTIONARY HEADER - SHOW MARKET DOMINANCE
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 2, 12, "🏆 MASTER CONTROL CENTER");
    
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str(canvas, 2, 24, "Real-Time App Communication");
    
    // 📊 LIVE APP STATUS GRID - GENIUS VISUALIZATION
    uint8_t y_pos = 35;
    uint8_t col1 = 2, col2 = 66;
    
    // Column 1: Core Apps
    canvas_draw_str(canvas, col1, y_pos, "CORE APPS:");
    y_pos += 10;
    
    for(uint32_t i = 0; i < dashboard->app_count && i < 6; i++) {
        PredatorAppInfo* info = &dashboard->apps[i];
        
        // Status icon based on app status
        const char* status_icon = "●";
        if(info->status == PredatorAppStatusRunning) status_icon = "🟢";
        else if(info->status == PredatorAppStatusError) status_icon = "🔴";
        else if(info->status == PredatorAppStatusIdle) status_icon = "🟡";
        
        // App name with status
        char app_line[32];
        snprintf(app_line, sizeof(app_line), "%s %s", status_icon, info->app_name);
        canvas_draw_str(canvas, col1, y_pos, app_line);
        
        // Communication indicator
        if(info->last_activity > 0) {
            canvas_draw_str(canvas, col1 + 50, y_pos, "📡");
        }
        
        y_pos += 8;
    }
    
    // Column 2: Advanced Apps
    y_pos = 45;
    canvas_draw_str(canvas, col2, y_pos, "VIP++ APPS:");
    y_pos += 10;
    
    for(uint32_t i = 6; i < dashboard->app_count && i < 12; i++) {
        PredatorAppInfo* info = &dashboard->apps[i];
        
        const char* status_icon = "●";
        if(info->status == PredatorAppStatusRunning) status_icon = "⚡";
        else if(info->vip_mode) status_icon = "💎";
        else if(info->government_mode) status_icon = "🏛️";
        
        char app_line[32];
        snprintf(app_line, sizeof(app_line), "%s %s", status_icon, info->app_name);
        canvas_draw_str(canvas, col2, y_pos, app_line);
        
        y_pos += 8;
    }
    
    // 📊 REVOLUTIONARY STATISTICS - SHOW SUPERIORITY
    canvas_draw_line(canvas, 0, 100, 128, 100);
    canvas_draw_str(canvas, 2, 112, "ECOSYSTEM STATS:");
    
    char stats[64];
    snprintf(stats, sizeof(stats), "Apps: %lu | Ops: %lu", dashboard->app_count, dashboard->total_operations);
    canvas_draw_str(canvas, 2, 122, stats);
    
    snprintf(stats, sizeof(stats), "Success: %lu%% | Comm: %s", 
             dashboard->success_rate, 
             dashboard->communication_active ? "ACTIVE" : "IDLE");
    canvas_draw_str(canvas, 2, 132, stats);
    
    // 🚀 MARKET DOMINATION INDICATOR
    if(dashboard->app_count >= 8) {
        canvas_draw_str(canvas, 80, 112, "🚀 DOMINATING");
    } else if(dashboard->app_count >= 5) {
        canvas_draw_str(canvas, 80, 112, "🏆 LEADING");
    } else {
        canvas_draw_str(canvas, 80, 112, "📈 GROWING");
    }
    
    // Status message at bottom
    if(strlen(dashboard->status_message) > 0) {
        canvas_draw_str(canvas, 2, 142, dashboard->status_message);
    }
}

// Update dashboard with real-time data
static void update_dashboard_data(PredatorApp* app) {
    if(!dashboard) return;
    
    // Get all app information via IPC
    dashboard->app_count = 0;
    predator_ipc_get_all_apps(dashboard->apps, &dashboard->app_count);
    
    // Calculate total statistics
    dashboard->total_operations = 0;
    uint32_t total_success = 0;
    
    for(uint32_t i = 0; i < dashboard->app_count; i++) {
        dashboard->total_operations += dashboard->apps[i].operations_count;
        total_success += dashboard->apps[i].success_count;
    }
    
    // Calculate success rate
    if(dashboard->total_operations > 0) {
        dashboard->success_rate = (total_success * 100) / dashboard->total_operations;
    } else {
        dashboard->success_rate = 100;
    }
    
    // Check communication activity
    dashboard->communication_active = false;
    for(uint32_t i = 0; i < dashboard->app_count; i++) {
        if(dashboard->apps[i].last_activity > 0 && 
           (furi_get_tick() - dashboard->apps[i].last_activity) < 5000) {
            dashboard->communication_active = true;
            break;
        }
    }
    
    // Update status message based on ecosystem state
    if(dashboard->app_count >= 10) {
        strcpy(dashboard->status_message, "🏆 COMPLETE ECOSYSTEM ACTIVE");
    } else if(dashboard->app_count >= 8) {
        strcpy(dashboard->status_message, "🚀 ADVANCED SUITE RUNNING");
    } else if(dashboard->app_count >= 5) {
        strcpy(dashboard->status_message, "📈 CORE APPS OPERATIONAL");
    } else if(dashboard->app_count >= 3) {
        strcpy(dashboard->status_message, "⚡ BASIC SUITE ACTIVE");
    } else {
        strcpy(dashboard->status_message, "🔄 INITIALIZING ECOSYSTEM");
    }
}

// Timer callback for real-time updates
static void dashboard_timer_callback(void* context) {
    PredatorApp* app = context;
    if(!app || !dashboard) return;
    
    update_dashboard_data(app);
    
    // Trigger view update
    if(app->view_dispatcher) {
        view_dispatcher_send_custom_event(app->view_dispatcher, 0);
    }
}

// Input handler for dashboard
static bool dashboard_input_callback(InputEvent* event, void* context) {
    PredatorApp* app = context;
    if(!app) return false;
    
    if(event->type == InputTypePress) {
        switch(event->key) {
        case InputKeyBack:
            scene_manager_previous_scene(app->scene_manager);
            return true;
            
        case InputKeyOk:
            // Launch app launcher for detailed control
            scene_manager_next_scene(app->scene_manager, PredatorSceneAppLauncherUI);
            return true;
            
        case InputKeyUp:
            // Activate emergency mode across all apps
            predator_ipc_activate_emergency_mode();
            strcpy(dashboard->status_message, "🚨 EMERGENCY MODE ACTIVATED");
            return true;
            
        case InputKeyDown:
            // Activate government mode across all apps
            predator_ipc_activate_government_mode();
            strcpy(dashboard->status_message, "🏛️ GOVERNMENT MODE ACTIVATED");
            return true;
            
        case InputKeyLeft:
            // Previous dashboard view
            return true;
            
        case InputKeyRight:
            // Next dashboard view
            return true;
            
        default:
            break;
        }
    }
    
    return false;
}

void predator_scene_master_dashboard_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    // Initialize dashboard
    dashboard = malloc(sizeof(MasterDashboard));
    if(!dashboard) return;
    
    memset(dashboard, 0, sizeof(MasterDashboard));
    dashboard->app = app;
    
    // Initialize IPC system
    predator_ipc_init();
    
    // Register master app
    predator_ipc_register_app(PredatorAppTypeMaster, "Master Control", "1.0");
    predator_ipc_update_status(PredatorAppTypeMaster, PredatorAppStatusRunning, "Dashboard Active");
    
    // Setup view
    view_allocate_model(app->view, ViewModelTypeLocking, sizeof(MasterDashboard));
    view_set_context(app->view, app);
    view_set_draw_callback(app->view, draw_master_dashboard);
    view_set_input_callback(app->view, dashboard_input_callback);
    
    // Start real-time update timer
    dashboard->update_timer = furi_timer_alloc(dashboard_timer_callback, FuriTimerTypePeriodic, app);
    furi_timer_start(dashboard->update_timer, 1000); // Update every second
    
    // Initial data load
    update_dashboard_data(app);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewView);
}

bool predator_scene_master_dashboard_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(event.type == SceneManagerEventTypeCustom) {
        // Handle real-time updates
        consumed = true;
    }
    
    return consumed;
}

void predator_scene_master_dashboard_on_exit(void* context) {
    PredatorApp* app = context;
    if(!app || !dashboard) return;
    
    // Stop timer
    if(dashboard->update_timer) {
        furi_timer_stop(dashboard->update_timer);
        furi_timer_free(dashboard->update_timer);
    }
    
    // Unregister master app
    predator_ipc_unregister_app(PredatorAppTypeMaster);
    
    // Cleanup
    view_free_model(app->view);
    free(dashboard);
    dashboard = NULL;
}
