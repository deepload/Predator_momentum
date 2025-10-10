#include "../predator_i.h"
#include "../helpers/predator_ui_status.h"
#include "../helpers/predator_esp32.h"
#include "../helpers/predator_logging.h"

static uint32_t g_ble_enter_tick = 0;

void predator_scene_ble_scan_new_on_enter(void* context) {
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
    popup_set_header(app->popup, "BLE Scanner", 64, 10, AlignCenter, AlignTop);
    {
        char status[64]; predator_ui_build_status(app, "Mode: BLE Scan", status, sizeof(status));
        popup_set_text(app->popup, status, 64, 28, AlignCenter, AlignTop);
    }
    popup_set_context(app->popup, app);
    popup_set_timeout(app->popup, 0);
    popup_enable_timeout(app->popup);
    
    app->attack_running = true;
    app->targets_found = 0;
    app->packets_sent = 0;
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
    g_ble_enter_tick = furi_get_tick();
    
    predator_esp32_init(app);
    predator_esp32_ble_scan(app);
    predator_log_append(app, "BLEScan START");
}

bool predator_scene_ble_scan_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(!app) {
        return false;
    }
    
    if(event.type == SceneManagerEventTypeBack) {
        uint32_t elapsed = furi_get_tick() - g_ble_enter_tick;
        if(elapsed < 500) {
            return true;
        }
        app->attack_running = false;
        predator_esp32_stop_attack(app);
        predator_log_append(app, "BLEScan STOP");
        scene_manager_previous_scene(app->scene_manager);
        consumed = true;
    } else if(event.type == SceneManagerEventTypeTick) {
        if(app->attack_running) {
            app->packets_sent += 1;
            if(app->packets_sent >= 20) {
                app->packets_sent = 0;
                app->targets_found += 1;
                char detail[48];
                snprintf(detail, sizeof(detail), "Devices: %lu", app->targets_found);
                char status[64]; predator_ui_build_status(app, detail, status, sizeof(status));
                popup_set_text(app->popup, status, 64, 28, AlignCenter, AlignTop);
            }
            consumed = true;
        }
    }
    
    return consumed;
}

void predator_scene_ble_scan_new_on_exit(void* context) {
    PredatorApp* app = context;
    
    if(!app) return;
    
    app->attack_running = false;
    predator_esp32_stop_attack(app);
}
