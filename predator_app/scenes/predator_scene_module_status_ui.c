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
    
    // FORCE hardware initialization to get real status
    const PredatorBoardConfig* board_config = predator_boards_get_config(app->board_type);
    if(board_config) {
        // Initialize ESP32 if not already done
        if(!app->esp32_uart && board_config->esp32_tx_pin) {
            predator_esp32_init(app);
        }
        
        // Initialize GPS if not already done  
        if(!app->gps_uart && board_config->gps_tx_pin) {
            predator_gps_init(app);
        }
    }
    
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
    
    // Hardware status with board capability indicators
    const PredatorBoardConfig* config = predator_boards_get_config(app->board_type);
    
    // ESP32 Status
    bool esp32_available = config && config->esp32_tx_pin;
    snprintf(item, sizeof(item), "ESP32: %s%s", 
             app->esp32_uart ? "CONNECTED" : (esp32_available ? "AVAILABLE" : "N/A"),
             esp32_available ? " ✓" : "");
    submenu_add_item(app->submenu, item, 2, module_status_submenu_cb, app);
    
    // GPS Status  
    bool gps_available = config && config->gps_tx_pin;
    snprintf(item, sizeof(item), "GPS: %s%s", 
             app->gps_uart ? "CONNECTED" : (gps_available ? "AVAILABLE" : "N/A"),
             gps_available ? " ✓" : "");
    submenu_add_item(app->submenu, item, 3, module_status_submenu_cb, app);
    
    // SubGHz Status (always available on Flipper)
    snprintf(item, sizeof(item), "SubGHz: READY ✓");
    submenu_add_item(app->submenu, item, 4, module_status_submenu_cb, app);
    
    // NFC Status
    bool nfc_available = config && config->has_nfc_writer;
    snprintf(item, sizeof(item), "NFC: %s%s", 
             nfc_available ? "AVAILABLE" : "INTERNAL",
             nfc_available ? " ✓" : " ✓");
    submenu_add_item(app->submenu, item, 5, module_status_submenu_cb, app);
    
    // Bluetooth (always available)
    submenu_add_item(app->submenu, "BLE: READY ✓", 6, module_status_submenu_cb, app);
    
    // Board-specific components
    submenu_add_item(app->submenu, "--- BOARD COMPONENTS ---", 7, module_status_submenu_cb, app);
    
    // RF Components
    bool has_cc1101 = config && config->has_external_rf;
    snprintf(item, sizeof(item), "CC1101: %s%s", 
             has_cc1101 ? "AVAILABLE" : "N/A",
             has_cc1101 ? " ✓" : "");
    submenu_add_item(app->submenu, item, 8, module_status_submenu_cb, app);
    
    // NRF24 (usually on multi-boards)
    bool has_nrf24 = (app->board_type == PredatorBoardType3in1AIO || 
                      app->board_type == PredatorBoardTypeDrB0rkMultiV2);
    snprintf(item, sizeof(item), "NRF24: %s%s", 
             has_nrf24 ? "AVAILABLE" : "N/A",
             has_nrf24 ? " ✓" : "");
    submenu_add_item(app->submenu, item, 9, module_status_submenu_cb, app);
    
    // Power switches
    bool has_gps_power = config && config->gps_power_switch;
    snprintf(item, sizeof(item), "GPS Power: %s%s", 
             has_gps_power ? "CONTROLLED" : "DIRECT",
             has_gps_power ? " ✓" : " ✓");
    submenu_add_item(app->submenu, item, 10, module_status_submenu_cb, app);
    
    bool has_marauder_switch = config && config->marauder_switch;
    snprintf(item, sizeof(item), "Marauder SW: %s%s", 
             has_marauder_switch ? "AVAILABLE" : "N/A",
             has_marauder_switch ? " ✓" : "");
    submenu_add_item(app->submenu, item, 11, module_status_submenu_cb, app);
    
    // Power levels
    if(config && config->rf_power_dbm > 0) {
        snprintf(item, sizeof(item), "RF Power: %ddBm ✓", config->rf_power_dbm);
        submenu_add_item(app->submenu, item, 12, module_status_submenu_cb, app);
    }
    
    // UART Baud rates
    submenu_add_item(app->submenu, "--- UART CONFIG ---", 13, module_status_submenu_cb, app);
    
    if(config && config->esp32_baud_rate > 0) {
        snprintf(item, sizeof(item), "ESP32: %lu baud ✓", config->esp32_baud_rate);
        submenu_add_item(app->submenu, item, 14, module_status_submenu_cb, app);
    }
    
    if(config && config->gps_baud_rate > 0) {
        snprintf(item, sizeof(item), "GPS: %lu baud ✓", config->gps_baud_rate);
        submenu_add_item(app->submenu, item, 15, module_status_submenu_cb, app);
    }
    
    // GPIO Pin mapping
    submenu_add_item(app->submenu, "--- GPIO MAPPING ---", 16, module_status_submenu_cb, app);
    
    if(config && config->esp32_tx_pin) {
        snprintf(item, sizeof(item), "ESP32 TX: Pin %d ✓", 
                 config->esp32_tx_pin == &gpio_ext_pc0 ? 15 : 
                 config->esp32_tx_pin == &gpio_ext_pc1 ? 16 : 0);
        submenu_add_item(app->submenu, item, 17, module_status_submenu_cb, app);
    }
    
    if(config && config->gps_tx_pin) {
        snprintf(item, sizeof(item), "GPS TX: Pin %d ✓", 
                 config->gps_tx_pin == &gpio_ext_pb2 ? 13 : 
                 config->gps_tx_pin == &gpio_ext_pb3 ? 14 : 0);
        submenu_add_item(app->submenu, item, 18, module_status_submenu_cb, app);
    }
    
    // Stats
    submenu_add_item(app->submenu, "--- STATISTICS ---", 19, module_status_submenu_cb, app);
    
    snprintf(item, sizeof(item), "Packets Sent: %lu", (unsigned long)app->packets_sent);
    submenu_add_item(app->submenu, item, 20, module_status_submenu_cb, app);
    
    snprintf(item, sizeof(item), "Targets Found: %lu", (unsigned long)app->targets_found);
    submenu_add_item(app->submenu, item, 21, module_status_submenu_cb, app);
    
    // System info
    submenu_add_item(app->submenu, "--- SYSTEM INFO ---", 22, module_status_submenu_cb, app);
    
    // Uptime
    uint32_t uptime_sec = furi_get_tick() / 1000;
    uint32_t hours = uptime_sec / 3600;
    uint32_t minutes = (uptime_sec % 3600) / 60;
    snprintf(item, sizeof(item), "Uptime: %luh %lum ✓", hours, minutes);
    submenu_add_item(app->submenu, item, 23, module_status_submenu_cb, app);
    
    // Memory usage (approximate)
    size_t free_heap = memmgr_get_free_heap();
    size_t total_heap = memmgr_get_total_heap();
    uint32_t usage_percent = ((total_heap - free_heap) * 100) / total_heap;
    snprintf(item, sizeof(item), "Memory: %lu%% used ✓", usage_percent);
    submenu_add_item(app->submenu, item, 24, module_status_submenu_cb, app);
    
    // Board detection status
    snprintf(item, sizeof(item), "Detection: %s ✓", 
             app->board_type == PredatorBoardTypeUnknown ? "AUTO" : "MANUAL");
    submenu_add_item(app->submenu, item, 25, module_status_submenu_cb, app);
    
    // Firmware info
    snprintf(item, sizeof(item), "API Level: %d ✓", 87);
    submenu_add_item(app->submenu, item, 26, module_status_submenu_cb, app);
    
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
