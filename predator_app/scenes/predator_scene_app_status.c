#include "../predator_i.h"
#include "../helpers/predator_app_communication.h"
#include "../helpers/predator_ecosystem_manager.h"

// 📱 APP STATUS MONITOR - Real-time view of all specialized apps

typedef struct {
    Widget* widget;
    FuriTimer* update_timer;
    PredatorAppInfo apps[12];
    uint32_t app_count;
    uint32_t refresh_count;
    bool monitoring_active;
} PredatorAppStatusMonitor;

static PredatorAppStatusMonitor* status_monitor = NULL;

static void app_status_update_callback(void* context) {
    UNUSED(context);
    
    if(!status_monitor) return;
    
    // Get current app information
    status_monitor->app_count = 0;
    predator_ipc_get_all_apps(status_monitor->apps, &status_monitor->app_count);
    status_monitor->refresh_count++;
    
    // Update widget display
    widget_reset(status_monitor->widget);
    
    // Header
    widget_add_string_element(status_monitor->widget, 64, 2, AlignCenter, AlignTop, FontPrimary, "📱 APP STATUS MONITOR");
    
    char status_line[64];
    snprintf(status_line, sizeof(status_line), "🔄 Apps: %lu | Refresh: %lu", status_monitor->app_count, status_monitor->refresh_count);
    widget_add_string_element(status_monitor->widget, 64, 12, AlignCenter, AlignTop, FontSecondary, status_line);
    
    // App list
    uint32_t y_pos = 22;
    for(uint32_t i = 0; i < status_monitor->app_count && i < 6; i++) {
        PredatorAppInfo* app = &status_monitor->apps[i];
        
        // App icon based on type
        const char* app_icon = "📱";
        switch(app->app_type) {
            case PredatorAppTypeMaster: app_icon = "🏆"; break;
            case PredatorAppTypeGovernment: app_icon = "🏛️"; break;
            case PredatorAppTypeWifi: app_icon = "📶"; break;
            case PredatorAppTypeCar: app_icon = "🚗"; break;
            case PredatorAppTypeTesla: app_icon = "⚡"; break;
            case PredatorAppTypeBluetooth: app_icon = "📘"; break;
            case PredatorAppTypeRfid: app_icon = "💳"; break;
            case PredatorAppTypeSubghz: app_icon = "📡"; break;
            case PredatorAppTypeInfrastructure: app_icon = "🏗️"; break;
            case PredatorAppTypeCasino: app_icon = "🎰"; break;
            case PredatorAppTypeNetwork: app_icon = "🌐"; break;
            case PredatorAppTypeCrypto: app_icon = "🔐"; break;
            default: app_icon = "❓"; break;
        }
        
        // Status indicator
        const char* status_icon = "⚪";
        switch(app->status) {
            case PredatorAppStatusRunning: status_icon = "🟢"; break;
            case PredatorAppStatusCompleted: status_icon = "✅"; break;
            case PredatorAppStatusError: status_icon = "🔴"; break;
            case PredatorAppStatusWaiting: status_icon = "🟡"; break;
            case PredatorAppStatusCommunicating: status_icon = "📡"; break;
            default: status_icon = "⚪"; break;
        }
        
        // App line
        char app_line[64];
        snprintf(app_line, sizeof(app_line), "%s %s %s", app_icon, status_icon, predator_ipc_app_type_to_string(app->app_type));
        widget_add_string_element(status_monitor->widget, 2, y_pos, AlignLeft, AlignTop, FontSecondary, app_line);
        
        // Operation info
        if(strlen(app->current_operation) > 0) {
            char op_line[32];
            snprintf(op_line, sizeof(op_line), "%.28s", app->current_operation);
            widget_add_string_element(status_monitor->widget, 126, y_pos, AlignRight, AlignTop, FontSecondary, op_line);
        }
        
        y_pos += 8;
    }
    
    // Footer with ecosystem info
    PredatorEcosystemState ecosystem_state;
    if(predator_ecosystem_get_state(&ecosystem_state)) {
        char footer_line[64];
        snprintf(footer_line, sizeof(footer_line), "🔄 Ops: %lu | 📡 Comm: %lu | 📊 Data: %lu bytes", 
                ecosystem_state.total_operations, 
                ecosystem_state.communication_events,
                ecosystem_state.data_shared_bytes);
        widget_add_string_element(status_monitor->widget, 64, 56, AlignCenter, AlignTop, FontSecondary, footer_line);
    }
    
    // Control instructions
    widget_add_string_element(status_monitor->widget, 64, 64, AlignCenter, AlignBottom, FontSecondary, "🔄 Auto-refresh | ← Back");
}

static bool app_status_input_callback(InputEvent* event, void* context) {
    PredatorApp* app = context;
    bool consumed = false;

    if(event->type == InputTypeShort) {
        switch(event->key) {
        case InputKeyBack:
            scene_manager_previous_scene(app->scene_manager);
            consumed = true;
            break;
        case InputKeyOk:
            // Force refresh
            if(status_monitor) {
                app_status_update_callback(NULL);
            }
            consumed = true;
            break;
        case InputKeyLeft:
            // Toggle monitoring
            if(status_monitor) {
                status_monitor->monitoring_active = !status_monitor->monitoring_active;
                if(status_monitor->monitoring_active) {
                    furi_timer_start(status_monitor->update_timer, 2000);
                } else {
                    furi_timer_stop(status_monitor->update_timer);
                }
            }
            consumed = true;
            break;
        case InputKeyRight:
            // Manual refresh all apps
            if(status_monitor) {
                // Send status request to all apps
                for(uint32_t i = 0; i < status_monitor->app_count; i++) {
                    predator_ipc_send_command(PredatorCommandGetStatus, status_monitor->apps[i].app_type, "refresh");
                }
                app_status_update_callback(NULL);
            }
            consumed = true;
            break;
        default:
            break;
        }
    }

    return consumed;
}

void predator_scene_app_status_on_enter(void* context) {
    PredatorApp* app = context;
    
    // Initialize status monitor
    status_monitor = malloc(sizeof(PredatorAppStatusMonitor));
    memset(status_monitor, 0, sizeof(PredatorAppStatusMonitor));
    
    status_monitor->widget = widget_alloc();
    status_monitor->monitoring_active = true;
    status_monitor->refresh_count = 0;
    
    // Create update timer
    status_monitor->update_timer = furi_timer_alloc(app_status_update_callback, FuriTimerTypePeriodic, NULL);
    
    // Set up widget
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewWidget);
    widget_add_string_element(status_monitor->widget, 64, 32, AlignCenter, AlignCenter, FontPrimary, "📱 Initializing App Monitor...");
    
    // Set input callback
    view_set_input_callback(widget_get_view(status_monitor->widget), app_status_input_callback);
    view_set_context(widget_get_view(status_monitor->widget), app);
    
    // Initial update
    app_status_update_callback(NULL);
    
    // Start monitoring timer
    furi_timer_start(status_monitor->update_timer, 2000); // Update every 2 seconds
    
    FURI_LOG_I("AppStatus", "App Status Monitor started");
}

bool predator_scene_app_status_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        consumed = true;
    }

    return consumed;
}

void predator_scene_app_status_on_exit(void* context) {
    PredatorApp* app = context;
    
    if(status_monitor) {
        // Stop timer
        if(status_monitor->update_timer) {
            furi_timer_stop(status_monitor->update_timer);
            furi_timer_free(status_monitor->update_timer);
        }
        
        // Clean up widget
        if(status_monitor->widget) {
            widget_free(status_monitor->widget);
        }
        
        // Clean up monitor
        free(status_monitor);
        status_monitor = NULL;
        
        FURI_LOG_I("AppStatus", "App Status Monitor stopped");
    }
}
