#include "../predator_i.h"
#include "predator_scene.h"
#include "predator_submenu_index.h"
#include "../helpers/predator_ui_status.h"
#include "../helpers/predator_logging.h"
#include "../helpers/predator_compliance.h"
#include "../helpers/predator_subghz.h"
#include "../helpers/predator_esp32.h"
#include "../helpers/predator_gps.h"
#include <furi.h>

// Professional stabilization - back debounce tracking
static uint32_t last_back_press = 0;

// Submenu callback with professional error handling
static void car_model_tests_submenu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    if(!app) {
        FURI_LOG_E("CarModelTests", "NULL app context in callback");
        return;
    }
    if(!app->view_dispatcher) {
        FURI_LOG_E("CarModelTests", "NULL view_dispatcher in callback");
        return;
    }
    FURI_LOG_I("CarModelTests", "Menu selection: %lu", index);
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void predator_scene_car_model_tests_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app) {
        FURI_LOG_E("CarModelTests", "NULL app context on enter");
        return;
    }

    // Professional validation
    if(!app->scene_manager || !app->view_dispatcher || !app->submenu) {
        FURI_LOG_E("CarModelTests", "Critical components NULL - scene_manager:%p view_dispatcher:%p submenu:%p", 
                   app->scene_manager, app->view_dispatcher, app->submenu);
        return;
    }

    // Board validation for hardware initialization
    if(app->board_type == PredatorBoardTypeUnknown) {
        FURI_LOG_W("CarModelTests", "Board type unknown, defaulting to Original");
        app->board_type = PredatorBoardTypeOriginal;
    }

    // Initialize hardware power rails
    predator_esp32_power_on(app);
    predator_gps_power_on(app);

    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "Tesla Security Tests");

    // Show selected model with enhanced display
    char hdr[80];
    if(strlen(app->selected_model_make) > 0 && strlen(app->selected_model_name) > 0) {
        snprintf(hdr, sizeof(hdr), "Target: %.12s %.20s", app->selected_model_make, app->selected_model_name);
    } else {
        snprintf(hdr, sizeof(hdr), "Target: All Models (Government Test)");
    }
    submenu_add_item(app->submenu, hdr, 0, car_model_tests_submenu_callback, app);

    // Government-grade security tests
    submenu_add_item(app->submenu, "🔋 Tesla Charge Port Security", 1, car_model_tests_submenu_callback, app);
    submenu_add_item(app->submenu, "🔑 Rolling Code Vulnerability", 2, car_model_tests_submenu_callback, app);
    submenu_add_item(app->submenu, "📡 RF Jamming Resistance", 3, car_model_tests_submenu_callback, app);
    submenu_add_item(app->submenu, "🚗 Passive Key Exploitation", 4, car_model_tests_submenu_callback, app);
    submenu_add_item(app->submenu, "🚦 Traffic Light Integration", 5, car_model_tests_submenu_callback, app);
    submenu_add_item(app->submenu, "🌐 Swiss Government Test", 6, car_model_tests_submenu_callback, app);
    submenu_add_item(app->submenu, "🇺🇸 California State Test", 7, car_model_tests_submenu_callback, app);
    submenu_add_item(app->submenu, "🚶 Walking Open (Elon Demo)", 8, car_model_tests_submenu_callback, app);
    submenu_add_item(app->submenu, "📊 Live Monitor (Logs)", 99, car_model_tests_submenu_callback, app);

    // Log initialization
    predator_log_append(app, "CarModelTests: Professional security testing initialized");
    char model_msg[100];
    snprintf(model_msg, sizeof(model_msg), "Target: %s %s (Freq: %luHz)", 
             app->selected_model_make, app->selected_model_name, app->selected_model_freq);
    predator_log_append(app, model_msg);

    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
    FURI_LOG_I("CarModelTests", "Professional car security testing scene initialized");
}

bool predator_scene_car_model_tests_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    if(!app) {
        FURI_LOG_E("CarModelTests", "NULL app context in event handler");
        return false;
    }

    // Professional back-debounce using navigation safety helper
    if(event.type == SceneManagerEventTypeBack) {
        if(predator_navigation_back_debounce(&last_back_press, 500)) {
            FURI_LOG_D("CarModelTests", "Back press debounced");
            return true;
        }
        
        predator_log_append(app, "CarModelTests: Exiting to car attacks menu");
        PREDATOR_SAFE_PREVIOUS_SCENE(app);
        return true;
    }

    if(event.type == SceneManagerEventTypeCustom) {
        FURI_LOG_I("CarModelTests", "Processing custom event: %lu", event.event);
        
        switch(event.event) {
        case 0:
            // Non-selectable header; ignore
            return true;
            
        case 1: { // Tesla Charge Port Security
            predator_log_append(app, "TESLA SECURITY: Charge port vulnerability test");
            predator_log_append(app, "Government-grade Tesla security assessment");
            scene_manager_next_scene(app->scene_manager, PredatorSceneCarTesla);
            return true;
        }
        
        case 2: { // Rolling Code Vulnerability
            // Professional frequency selection with region awareness
            uint32_t freq = app->selected_model_freq ? app->selected_model_freq : 433920000;
            if(app->region == PredatorRegionUS || app->region == PredatorRegionJP) {
                freq = 315000000;
            } else if(app->region == PredatorRegionEU) {
                freq = 433920000;
            }
            app->selected_model_freq = freq;
            
            predator_log_append(app, "ROLLING CODE: Advanced cryptographic attack");
            char freq_msg[80];
            snprintf(freq_msg, sizeof(freq_msg), "Frequency: %.3fMHz (Region: %s)", 
                    freq / 1000000.0f, 
                    (app->region == PredatorRegionUS) ? "US" : 
                    (app->region == PredatorRegionEU) ? "EU" : "JP");
            predator_log_append(app, freq_msg);
            
            FURI_LOG_I("CarModelTests", "Rolling code attack at %luHz", freq);
            scene_manager_next_scene(app->scene_manager, PredatorSceneCarKeyBruteforce);
            return true;
        }
        
        case 3: { // RF Jamming Resistance
            predator_log_append(app, "RF JAMMING: Signal interference testing");
            predator_log_append(app, "Testing vehicle resistance to RF attacks");
            scene_manager_next_scene(app->scene_manager, PredatorSceneCarJamming);
            return true;
        }
        
        case 4: { // Passive Key Exploitation
            predator_log_append(app, "PASSIVE KEY: Proximity-based vulnerability");
            predator_log_append(app, "Testing keyless entry security");
            scene_manager_next_scene(app->scene_manager, PredatorSceneCarPassiveOpener);
            return true;
        }
        
        case 5: { // Traffic Light Integration
            predator_log_append(app, "TRAFFIC LIGHTS: Government infrastructure test");
            predator_log_append(app, "California State & Switzerland contracts");
            
            if(app->popup) {
                popup_reset(app->popup);
                popup_set_header(app->popup, "Traffic Light Security", 64, 10, AlignCenter, AlignTop);
                popup_set_text(app->popup, "Government contracts:\nCalifornia State (4 accidents)\nSwitzerland (Infrastructure)\n\nTesting traffic signal security", 
                              64, 25, AlignCenter, AlignTop);
                popup_set_context(app->popup, app);
                popup_set_timeout(app->popup, 0);
                popup_enable_timeout(app->popup);
                view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
            }
            return true;
        }
        
        case 6: { // Swiss Government Test
            predator_log_append(app, "SWISS GOV: Official government security test");
            predator_log_append(app, "Switzerland infrastructure assessment");
            app->vip_mode = true; // Enable VIP mode for government testing
            
            if(app->popup) {
                popup_reset(app->popup);
                popup_set_header(app->popup, "Swiss Government", 64, 10, AlignCenter, AlignTop);
                popup_set_text(app->popup, "🇨🇭 SWITZERLAND CONTRACT\n\nOfficial government testing\nTraffic light security\nInfrastructure assessment\n\nVIP MODE ACTIVATED", 
                              64, 25, AlignCenter, AlignTop);
                popup_set_context(app->popup, app);
                popup_set_timeout(app->popup, 0);
                popup_enable_timeout(app->popup);
                view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
            }
            return true;
        }
        
        case 7: { // California State Test
            predator_log_append(app, "CALIFORNIA: State government security contract");
            predator_log_append(app, "Post-accident traffic light analysis (4 incidents)");
            app->vip_mode = true; // Enable VIP mode for government testing
            
            if(app->popup) {
                popup_reset(app->popup);
                popup_set_header(app->popup, "California State", 64, 10, AlignCenter, AlignTop);
                popup_set_text(app->popup, "🇺🇸 CALIFORNIA CONTRACT\n\n4 traffic accidents analysis\nSignal response testing\nSafety verification\n\nVIP MODE ACTIVATED", 
                              64, 25, AlignCenter, AlignTop);
                popup_set_context(app->popup, app);
                popup_set_timeout(app->popup, 0);
                popup_enable_timeout(app->popup);
                view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
            }
            return true;
        }
        
        case 8: { // Walking Open (Elon Demo)
            predator_log_append(app, "ELON DEMO: Walking Open for Paris Forum");
            predator_log_append(app, "Tesla transparency demonstration");
            scene_manager_next_scene(app->scene_manager, PredatorSceneWalkingOpenUI);
            return true;
        }
        
        case 99: { // Live Monitor
            predator_log_append(app, "MONITORING: Real-time security analysis");
            scene_manager_next_scene(app->scene_manager, PredatorSceneLiveMonitor);
            return true;
        }
        
        default:
            FURI_LOG_W("CarModelTests", "Unknown event: %lu", event.event);
            return false;
        }
    }

    return false;
}

void predator_scene_car_model_tests_on_exit(void* context) {
    PredatorApp* app = context;
    if(!app) {
        FURI_LOG_E("CarModelTests", "NULL app context on exit");
        return;
    }
    
    // Professional cleanup
    predator_log_append(app, "CarModelTests: Professional security testing session ended");
    
    // Reset back debounce
    last_back_press = 0;
    
    // Clean up submenu
    if(app->submenu) {
        submenu_reset(app->submenu);
    } else {
        FURI_LOG_W("CarModelTests", "Submenu is NULL on exit");
    }
    
    FURI_LOG_I("CarModelTests", "Car model tests scene exited cleanly");
}
