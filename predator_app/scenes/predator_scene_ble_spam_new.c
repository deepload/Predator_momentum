#include "../predator_i.h"
// Live integration
#include "../helpers/predator_esp32.h"
#include "../helpers/predator_compliance.h"
#include "../helpers/predator_ui_status.h"
#include "../helpers/predator_logging.h"

void predator_scene_ble_spam_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    if(!app) {
        return;
    }
    
    if(!app->scene_manager) {
        return;
    }
    
    if(!app->view_dispatcher) {
        return;
    }
    
    popup_reset(app->popup);
    popup_set_header(app->popup, "BLE Spam", 64, 10, AlignCenter, AlignTop);
    bool live_allowed = predator_compliance_is_feature_allowed(app, PredatorFeatureBleSpam, app->authorized);
    if(live_allowed) {
        predator_esp32_init(app);
        bool started = predator_esp32_ble_spam(app, 0);
        if(started) {
            char status[64]; predator_ui_build_status(app, "Mode: BLE Spam", status, sizeof(status));
            popup_set_text(app->popup, status, 64, 28, AlignCenter, AlignTop);
            predator_log_append(app, "BleSpam START");
        } else {
            popup_set_text(app->popup, "ESP32 not ready — Falling back to Demo\nPress Back to return", 64, 28, AlignCenter, AlignTop);
        }
    } else {
        popup_set_text(app->popup, "Demo Mode — Authorization required\nPress Back to return", 64, 28, AlignCenter, AlignTop);
    }
    popup_set_context(app->popup, app);
    popup_set_timeout(app->popup, 0);
    popup_enable_timeout(app->popup);
    
    app->attack_running = true;
    app->packets_sent = 0;
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
}

bool predator_scene_ble_spam_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(!app) {
        return false;
    }
    
    if(event.type == SceneManagerEventTypeBack) {
        app->attack_running = false;
        if(predator_compliance_is_feature_allowed(app, PredatorFeatureBleSpam, app->authorized)) {
            predator_esp32_stop_attack(app);
        }
        predator_log_append(app, "BleSpam STOP");
        scene_manager_previous_scene(app->scene_manager);
        consumed = true;
    } else if(event.type == SceneManagerEventTypeTick) {
        if(app->attack_running) {
            app->packets_sent += 10;
            consumed = true;
        }
    }
    
    return consumed;
}

void predator_scene_ble_spam_new_on_exit(void* context) {
    PredatorApp* app = context;
    
    if(!app) return;
    
    app->attack_running = false;
    if(predator_compliance_is_feature_allowed(app, PredatorFeatureBleSpam, app->authorized)) {
        predator_esp32_stop_attack(app);
    }
}
