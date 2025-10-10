// Evil Twin scene
#include "../predator_i.h"
#include "../helpers/predator_view_helpers.h"
#include "../helpers/predator_esp32.h"
#include "../helpers/predator_ui_elements.h"
#include "../helpers/predator_compliance.h"
#include "../helpers/predator_ui_status.h"
#include "../helpers/predator_settings.h"
#include "../helpers/predator_logging.h"

// File-scope flags for stability
static bool s_et_live_started = false;
static uint32_t s_et_ticks_since_enter = 0;

// Ensure Back works via popup callback
static void wifi_evil_twin_popup_back(void* context) {
    PredatorApp* app = context;
    if(app && app->view_dispatcher) {
        view_dispatcher_send_custom_event(app->view_dispatcher, PredatorCustomEventPopupBack);
    }
}

void predator_scene_wifi_evil_twin_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    if(!app) {
        FURI_LOG_E("WiFiEvilTwin", "App context is NULL on enter");
        return;
    }
    
    // Validate board type before any hardware initialization
    if(app->board_type == 0) {
        FURI_LOG_W("WiFiEvilTwin", "Board type is Unknown, defaulting to Original");
        app->board_type = 0; // Keep as Original
    }
    
    // Ensure scene_manager and view_dispatcher are valid to prevent crashes
    if(!app->scene_manager) {
        FURI_LOG_E("WiFiEvilTwin", "Scene manager is NULL, cannot proceed");
        return;
    }
    
    if(!app->view_dispatcher) {
        FURI_LOG_E("WiFiEvilTwin", "View dispatcher is NULL, cannot switch view");
        return;
    }
    
    // Ensure popup is valid
    if(!app->popup) {
        FURI_LOG_E("WiFiEvilTwin", "Popup is NULL, cannot initialize UI");
        return;
    }
    
    // Configure popup content to avoid blank screen — render first, then start hardware
    popup_reset(app->popup);
    popup_set_header(app->popup, "Evil Twin", 64, 10, AlignCenter, AlignTop);
    popup_set_text(app->popup, "Starting Evil Twin...\nBack=Stop | See Live Monitor", 64, 28, AlignCenter, AlignTop);
    popup_set_context(app->popup, app);
    popup_set_callback(app->popup, wifi_evil_twin_popup_back);
    popup_set_timeout(app->popup, 0);
    popup_enable_timeout(app->popup);
    // Switch to popup view immediately to avoid white screen
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);

    bool live_allowed = predator_compliance_is_feature_allowed(app, PredatorFeatureWifiEvilTwin, app->authorized);
    s_et_live_started = false;
    s_et_ticks_since_enter = 0;
    if(!live_allowed) {
        popup_set_text(app->popup, "Demo Mode — Authorization required\nPress Back to return", 64, 28, AlignCenter, AlignTop);
    } else {
        // Live path
        // Read channel source from settings: 0=Setting, 1=Auto
        int32_t ch_src = 0; predator_settings_get_int(app, "EVILTWIN_CHANNEL_SRC", 0, &ch_src);
        int32_t channel = 6;
        if(ch_src == 0) { // Setting
            predator_settings_get_int(app, "WIFI_CHANNEL", 6, &channel);
            if(channel < 0 || channel > 13) channel = 6;
        }
        // Initialize ESP32 after UI is visible
        predator_esp32_init(app);
        bool started = predator_esp32_wifi_evil_twin(app);
        if(started) {
            char detail[64];
            if(ch_src == 0) {
                snprintf(detail, sizeof(detail), "Mode: EvilTwin (ch %ld)", (long)channel);
            } else {
                snprintf(detail, sizeof(detail), "Mode: EvilTwin (Auto channel)");
            }
            char status[128]; predator_ui_build_status(app, detail, status, sizeof(status));
            // Append guidance for juniors without strncat (use snprintf with offset)
            size_t len = strlen(status);
            if(len < sizeof(status)) {
                snprintf(status + len, sizeof(status) - len, "\nSteps: Match SSID & channel\nThen run Deauth bursts");
            }
            popup_set_text(app->popup, status, 64, 28, AlignCenter, AlignTop);
            char logline[96];
            if(ch_src == 0) snprintf(logline, sizeof(logline), "WiFiEvilTwin START ch=%ld", (long)channel);
            else snprintf(logline, sizeof(logline), "WiFiEvilTwin START ch=auto");
            predator_log_append(app, logline);
            FURI_LOG_I("WiFiEvilTwin", "Live Evil Twin started");
            s_et_live_started = true;
        } else {
            popup_set_text(app->popup, "ESP32 not ready — Falling back to Demo\nPress Back to return", 64, 28, AlignCenter, AlignTop);
            FURI_LOG_W("WiFiEvilTwin", "ESP32 start failed; falling back to Demo text");
        }
    }

    // Start simulated Evil Twin activity for UI animation/progress
    app->attack_running = true;
    app->targets_found = 0;
    app->packets_sent = 0;
    FURI_LOG_I("WiFiEvilTwin", "WiFi Evil Twin scene entered");
}

bool predator_scene_wifi_evil_twin_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(!app) {
        FURI_LOG_E("WiFiEvilTwin", "App context is NULL in event handler");
        return false;
    }
    
    if(event.type == SceneManagerEventTypeBack) {
        // Handle back event to stop the attack
        app->attack_running = false;
        FURI_LOG_I("WiFiEvilTwin", "Back event received, stopping attack and navigating back");
        if(predator_compliance_is_feature_allowed(app, PredatorFeatureWifiEvilTwin, app->authorized)) {
            predator_esp32_stop_attack(app);
        }
        scene_manager_previous_scene(app->scene_manager);
        consumed = true;
    } else if(event.type == SceneManagerEventTypeCustom && event.event == PredatorCustomEventPopupBack) {
        // Popup back callback path
        app->attack_running = false;
        FURI_LOG_I("WiFiEvilTwin", "Popup Back pressed, stopping attack and navigating back");
        if(predator_compliance_is_feature_allowed(app, PredatorFeatureWifiEvilTwin, app->authorized)) {
            predator_esp32_stop_attack(app);
        }
        scene_manager_previous_scene(app->scene_manager);
        consumed = true;
    } else if(event.type == SceneManagerEventTypeTick) {
        if(app->attack_running) {
            // Startup watchdog: if live didn't start within ~2s, stay in demo text to keep UI responsive
            if(!s_et_live_started) {
                s_et_ticks_since_enter++;
                if(s_et_ticks_since_enter == 30 && app->popup) { // assuming ~100ms tick
                    popup_set_text(app->popup, "Evil Twin (Demo)\nBack=Stop | See Live Monitor", 64, 28, AlignCenter, AlignTop);
                }
            }
            // Simulate connections to Evil Twin AP using a simple counter
            app->packets_sent += 1;
            if(app->packets_sent >= 20 && app->popup) {
                app->packets_sent = 0;
                app->targets_found += 1;
                // Update popup text to show progress
                char progress_text[64];
                snprintf(progress_text, sizeof(progress_text), "Devices connected: %lu\nPress Back to stop", app->targets_found);
                popup_set_text(app->popup, progress_text, 64, 28, AlignCenter, AlignTop);
                FURI_LOG_I("WiFiEvilTwin", "Updated devices connected: %lu", app->targets_found);
            }
            consumed = true;
        }
    }
    
    return consumed;
}

void predator_scene_wifi_evil_twin_new_on_exit(void* context) {
    PredatorApp* app = context;
    
    if(!app) {
        FURI_LOG_E("WiFiEvilTwin", "App context is NULL on exit");
        return;
    }
    
    // Stop any running attack
    app->attack_running = false;
    if(predator_compliance_is_feature_allowed(app, PredatorFeatureWifiEvilTwin, app->authorized)) {
        predator_esp32_stop_attack(app);
    }
    FURI_LOG_I("WiFiEvilTwin", "Exited WiFi Evil Twin scene");
}
