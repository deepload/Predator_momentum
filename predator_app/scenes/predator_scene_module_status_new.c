#include "../predator_i.h"
#include "../helpers/predator_view_helpers.h"
#include "../helpers/predator_esp32.h"
#include "../helpers/predator_boards.h"
#include "../helpers/predator_ui_elements.h"
#include "predator_scene.h"
#include <gui/elements.h>
#include <stdio.h>

typedef struct {
    View* view;
    bool marauder_switch_on;
    bool gps_switch_on;
    bool esp32_connected;
    bool gps_connected;
    uint32_t satellites;
    PredatorBoardType board_type;
    uint8_t animation_frame;
} ModuleStatusView;

void predator_scene_module_status_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    if(!app) {
        FURI_LOG_E("ModuleStatus", "App context is NULL on enter");
        return;
    }
    
    if(!app->scene_manager) {
        FURI_LOG_E("ModuleStatus", "Scene manager is NULL, cannot proceed");
        return;
    }
    
    if(!app->view_dispatcher) {
        FURI_LOG_E("ModuleStatus", "View dispatcher is NULL, cannot switch view");
        return;
    }
    
    if(!app->popup) {
        FURI_LOG_E("ModuleStatus", "Popup is NULL, cannot initialize UI");
        return;
    }
    
    popup_reset(app->popup);
    popup_set_header(app->popup, "Module Status", 64, 10, AlignCenter, AlignTop);
    popup_set_text(app->popup, "Checking module status...\nPress Back to return", 64, 28, AlignCenter, AlignTop);
    popup_set_context(app->popup, app);
    popup_set_timeout(app->popup, 0);
    popup_enable_timeout(app->popup);
    
    app->module_connected = false;
    app->attack_running = true;
    app->packets_sent = 0;
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
    FURI_LOG_I("ModuleStatus", "Module Status scene entered with simulation mode");
}

bool predator_scene_module_status_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(!app) {
        FURI_LOG_E("ModuleStatus", "App context is NULL in event handler");
        return false;
    }
    
    if(event.type == SceneManagerEventTypeBack) {
        FURI_LOG_I("ModuleStatus", "Back event received, navigating to previous scene");
        app->attack_running = false;
        scene_manager_previous_scene(app->scene_manager);
        consumed = true;
    } else if(event.type == SceneManagerEventTypeTick) {
        if(app->attack_running) {
            app->packets_sent += 1;
            if(app->packets_sent >= 10) {
                app->module_connected = true;
                if(app->popup) {
                    popup_set_text(app->popup, "Module Detected\nESP32: Connected\nGPS: Active\nPress Back to return", 64, 28, AlignCenter, AlignTop);
                    FURI_LOG_I("ModuleStatus", "Module status updated to detected");
                } else {
                    FURI_LOG_W("ModuleStatus", "Popup is NULL, cannot update text");
                }
                app->attack_running = false;
            }
            consumed = true;
        }
    }
    
    return consumed;
}

void predator_scene_module_status_new_on_exit(void* context) {
    PredatorApp* app = context;
    
    if(!app) {
        FURI_LOG_E("ModuleStatus", "App context is NULL on exit");
        return;
    }
    
    app->attack_running = false;
    FURI_LOG_I("ModuleStatus", "Exited Module Status scene");
}
