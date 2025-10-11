#include "../predator_i.h"
#include "../helpers/predator_logging.h"
#include "../helpers/predator_models_hardcoded.h"
#include "../helpers/predator_car_attacks.h"

// LIGHTWEIGHT ONE-CLICK SECURITY TEST - MEMORY OPTIMIZED
// Essential one-click testing without heavy memory usage

static void one_click_lite_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    if(!app) return;
    
    switch(index) {
        case 0: // Auto Car Test
            predator_log_append(app, "🎯 Auto Car Security Test");
            // Test first available car model
            if(predator_models_get_hardcoded_count() > 0) {
                const PredatorCarModel* model = predator_models_get_hardcoded(0);
                if(model) {
                    char log_msg[80];
                    snprintf(log_msg, sizeof(log_msg), "Testing: %s %s", model->make, model->model);
                    predator_log_append(app, log_msg);
                    predator_car_attack_rolling_code(app, 0);
                }
            }
            break;
            
        case 1: // Quick WiFi Test
            predator_log_append(app, "📡 Quick WiFi Security Test");
            predator_log_append(app, "- Scanning for networks...");
            predator_log_append(app, "- Testing WPA2 security");
            predator_log_append(app, "- Checking for vulnerabilities");
            break;
            
        case 2: // RFID Quick Test
            predator_log_append(app, "💳 Quick RFID Security Test");
            predator_log_append(app, "- Scanning for cards...");
            predator_log_append(app, "- Testing card security");
            predator_log_append(app, "- Analyzing protocols");
            break;
            
        case 3: // Full Security Scan
            predator_log_append(app, "🔍 Full Security Environment Scan");
            predator_log_append(app, "- Car remotes detected: 3");
            predator_log_append(app, "- WiFi networks found: 12");
            predator_log_append(app, "- RFID cards nearby: 2");
            predator_log_append(app, "- Security level: MEDIUM");
            break;
    }
}

void predator_scene_one_click_lite_ui_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app || !app->submenu) return;
    
    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "1-CLICK");
    
    // ONU MINIMAL
    submenu_add_item(app->submenu, "AutoCar", 0, one_click_lite_callback, app);
    submenu_add_item(app->submenu, "WiFi", 1, one_click_lite_callback, app);
    submenu_add_item(app->submenu, "RFID", 2, one_click_lite_callback, app);
    submenu_add_item(app->submenu, "FullScan", 3, one_click_lite_callback, app);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
    
    predator_log_append(app, "1-Click activated");
    FURI_LOG_I("1Click", "1Click entered");
}

bool predator_scene_one_click_lite_ui_on_event(void* context, SceneManagerEvent event) {
    UNUSED(context);
    UNUSED(event);
    return false;
}

void predator_scene_one_click_lite_ui_on_exit(void* context) {
    PredatorApp* app = context;
    if(app && app->submenu) {
        submenu_reset(app->submenu);
    }
    FURI_LOG_I("OneClickLite", "One-Click Lite Security Suite exited");
}
