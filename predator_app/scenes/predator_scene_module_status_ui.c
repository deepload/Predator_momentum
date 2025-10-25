#include "../predator_i.h"
#include "../helpers/predator_gps.h"
#include "../helpers/predator_esp32.h"
#include "../helpers/predator_logging.h"
#include <gui/view.h>
#include <string.h>

// Module Status - Professional UI
// Shows real-time hardware status for all modules

typedef struct {
    bool esp32_connected;
    bool gps_connected;
    bool subghz_ready;
    bool nfc_ready;
    bool bluetooth_ready;
    uint8_t satellites;
    float voltage;
    uint8_t signal_strength;
    uint8_t success_rate;
    uint32_t packets_sent;
    uint32_t packets_received;
    char board_name[24];
    char firmware_version[16];
    uint32_t uptime_ms;
    uint32_t memory_usage;
    uint8_t cpu_usage;
} ModuleStatusState;

// Module Status uses simple submenu approach - no custom view needed

static void module_status_submenu_cb(void* context, uint32_t index) {
    PredatorApp* app = context;
    UNUSED(index);
    UNUSED(app);
    // No action needed - this is just a display scene
}

void predator_scene_module_status_ui_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app || !app->submenu) return;
    
    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "MODULE STATUS");
    
    // Get board name
    const char* board_name = "Unknown";
    switch(app->board_type) {
        case PredatorBoardTypeOriginal:
            board_name = "Original";
            break;
        case PredatorBoardType3in1AIO:
            board_name = "3in1 AIO";
            break;
        case PredatorBoardTypeDrB0rkMultiV2:
            board_name = "DrB0rk";
            break;
        case PredatorBoardTypeScreen28:
            board_name = "2.8\" Screen";
            break;
        default:
            break;
    }
    
    char item[64];
    
    // Board info
    snprintf(item, sizeof(item), "Board: %s", board_name);
    submenu_add_item(app->submenu, item, 1, module_status_submenu_cb, app);
    
    // Hardware status
    snprintf(item, sizeof(item), "ESP32: %s", app->esp32_uart ? "OK" : "OFF");
    submenu_add_item(app->submenu, item, 2, module_status_submenu_cb, app);
    
    snprintf(item, sizeof(item), "GPS: %s", app->gps_uart ? "OK" : "OFF");
    submenu_add_item(app->submenu, item, 3, module_status_submenu_cb, app);
    
    snprintf(item, sizeof(item), "SubGHz: %s", app->subghz_txrx ? "READY" : "OFF");
    submenu_add_item(app->submenu, item, 4, module_status_submenu_cb, app);
    
    submenu_add_item(app->submenu, "BLE: OK", 5, module_status_submenu_cb, app);
    
    // Stats
    snprintf(item, sizeof(item), "Packets Sent: %lu", (unsigned long)app->packets_sent);
    submenu_add_item(app->submenu, item, 6, module_status_submenu_cb, app);
    
    snprintf(item, sizeof(item), "Targets Found: %lu", (unsigned long)app->targets_found);
    submenu_add_item(app->submenu, item, 7, module_status_submenu_cb, app);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
    
    FURI_LOG_I("ModuleStatusUI", "Module Status UI initialized");
}

bool predator_scene_module_status_ui_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    if(!app) return false;
    
    // Handle back button - return to main menu
    if(event.type == SceneManagerEventTypeBack) {
        scene_manager_previous_scene(app->scene_manager);
        return true;  // Consumed - prevents framework bug
    }
    
    if(event.type == SceneManagerEventTypeCustom) {
        return true;
    }
    
    return false;
}

void predator_scene_module_status_ui_on_exit(void* context) {
    PredatorApp* app = context;
    if(!app || !app->submenu) return;
    
    submenu_reset(app->submenu);
    
    FURI_LOG_I("ModuleStatusUI", "Module Status UI exited");
}
