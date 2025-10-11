#include "../predator_i.h"
#include "../helpers/predator_infrastructure.h"
#include "../helpers/predator_scada_real.h"
#include "../helpers/predator_logging.h"

static void infra_submenu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    if(!app) return;
    
    // Professional logging for government demonstration
    FURI_LOG_I("InfraUI", "🇨🇭🇺🇸 Government representative accessing: Index %lu", index);
    
    predator_infra_init(app);
    
    switch(index) {
        case 0: 
            FURI_LOG_I("InfraUI", "🇨🇭🇺🇸 Power Grid Analysis Initiated");
            predator_infra_power_grid_scan(app); 
            break;
        case 1: 
            FURI_LOG_I("InfraUI", "🇨🇭🇺🇸 Water SCADA Analysis Initiated");
            predator_infra_water_system_scan(app); 
            break;
        case 2: 
            FURI_LOG_I("InfraUI", "🇨🇭🇺🇸 Industrial Control Analysis Initiated");
            predator_infra_industrial_scan(app); 
            break;
        case 3: predator_infra_eas_scan(app); break;
        case 4: predator_infra_gas_pipeline_scan(app); break;
        case 5: predator_infra_dam_control_scan(app); break;
        case 6: predator_infra_airport_scan(app); break;
        case 7: predator_infra_hospital_scan(app); break;
        case 8: 
            FURI_LOG_E("InfraUI", "🇨🇭🇺🇸 LIVE GOVERNMENT DEMONSTRATION STARTING");
            predator_log_append(app, "🇨🇭🇺🇸 Government Demo: SCADA/Grid Hacking");
            scada_government_demonstration(app); 
            break;
    }
}

void predator_scene_critical_infrastructure_ui_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app || !app->submenu) return;
    
    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "🏛️ CRITICAL INFRASTRUCTURE SECURITY");
    
    // Professional government-grade menu items
    submenu_add_item(app->submenu, "⚡ Power Grid (IEC 61850)", 0, infra_submenu_callback, app);
    submenu_add_item(app->submenu, "💧 Water SCADA (DNP3)", 1, infra_submenu_callback, app);
    submenu_add_item(app->submenu, "🏭 Industrial (Modbus TCP)", 2, infra_submenu_callback, app);
    submenu_add_item(app->submenu, "📢 Emergency Alert (EAS)", 3, infra_submenu_callback, app);
    submenu_add_item(app->submenu, "🛢️ Gas Pipeline Control", 4, infra_submenu_callback, app);
    submenu_add_item(app->submenu, "🌊 Dam Control Systems", 5, infra_submenu_callback, app);
    submenu_add_item(app->submenu, "✈️ Airport Traffic Control", 6, infra_submenu_callback, app);
    submenu_add_item(app->submenu, "🏥 Hospital Life Support", 7, infra_submenu_callback, app);
    submenu_add_item(app->submenu, "🇨🇭🇺🇸 LIVE GOVERNMENT DEMO", 8, infra_submenu_callback, app);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
    
    FURI_LOG_I("InfraUI", "Critical Infrastructure UI entered");
}

bool predator_scene_critical_infrastructure_ui_on_event(void* context, SceneManagerEvent event) {
    UNUSED(context);
    UNUSED(event);
    return false;
}

void predator_scene_critical_infrastructure_ui_on_exit(void* context) {
    PredatorApp* app = context;
    if(app && app->submenu) {
        submenu_reset(app->submenu);
    }
    predator_infra_deinit(app);
    FURI_LOG_I("InfraUI", "Critical Infrastructure UI exited");
}
