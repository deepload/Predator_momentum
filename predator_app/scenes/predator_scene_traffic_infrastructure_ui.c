#include "../predator_i.h"
#include "../helpers/predator_traffic.h"
#include "../helpers/predator_logging.h"

// Traffic Infrastructure UI - User-friendly workflow
// California State contract: 4 accident analysis
// Switzerland contract: Traffic security assessment

static void traffic_submenu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    if(!app) return;
    
    predator_traffic_init(app);
    
    switch(index) {
        case 0: // Scan
            predator_traffic_scan_signals(app, 915000000);
            break;
        case 1: // Opticom
            predator_traffic_test_opticom(app, 0x12345678);
            break;
        case 2: // Railroad
            predator_traffic_railroad_detect(app);
            break;
        case 3: // Toll
            predator_traffic_toll_scan(app);
            break;
        case 4: // Timing
            {
                uint32_t red_ms, green_ms;
                predator_traffic_analyze_timing(app, 0x87654321, &red_ms, &green_ms);
            }
            break;
        case 5: // CA Contract
            FURI_LOG_I("Traffic", "CA State 4-accident study activated");
            break;
    }
}

void predator_scene_traffic_infrastructure_ui_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app || !app->submenu) return;
    
    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "Traffic Infrastructure");
    
    submenu_add_item(app->submenu, "Scan Traffic Signals", 0, traffic_submenu_callback, app);
    submenu_add_item(app->submenu, "Test Opticom", 1, traffic_submenu_callback, app);
    submenu_add_item(app->submenu, "Railroad Crossing", 2, traffic_submenu_callback, app);
    submenu_add_item(app->submenu, "Toll Booth DSRC", 3, traffic_submenu_callback, app);
    submenu_add_item(app->submenu, "Timing Analysis", 4, traffic_submenu_callback, app);
    submenu_add_item(app->submenu, "CA 4-Accident Study", 5, traffic_submenu_callback, app);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
    
    FURI_LOG_I("TrafficUI", "Traffic Infrastructure UI entered");
}

bool predator_scene_traffic_infrastructure_ui_on_event(void* context, SceneManagerEvent event) {
    UNUSED(context);
    UNUSED(event);
    return false;
}

void predator_scene_traffic_infrastructure_ui_on_exit(void* context) {
    PredatorApp* app = context;
    if(app && app->submenu) {
        submenu_reset(app->submenu);
    }
    predator_traffic_deinit(app);
    FURI_LOG_I("TrafficUI", "Traffic Infrastructure UI exited");
}
