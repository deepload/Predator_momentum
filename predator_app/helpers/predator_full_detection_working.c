#include "predator_full_detection.h"
#include "../predator_i.h"
#include "predator_boards.h"
#include <furi.h>

// FULL HARDWARE DETECTION SYSTEM - Working implementation

typedef struct {
    bool esp32_available;
    bool gps_available;
    bool subghz_available;
    bool nfc_available;
    char esp32_status[64];
    char gps_status[64];
    char subghz_status[64];
    char nfc_status[64];
    uint32_t last_check_time;
} DetectionState;

static DetectionState detection_state;

bool predator_full_detection_scan(PredatorApp* app) {
    if(!app) return false;
    
    FURI_LOG_I("Detection", "Starting full hardware detection scan...");
    
    // Reset detection state
    memset(&detection_state, 0, sizeof(DetectionState));
    detection_state.last_check_time = furi_get_tick();
    
    const PredatorBoardConfig* config = predator_boards_get_config(app->board_type);
    
    // DETECT ESP32 (WiFi/Bluetooth)
    if(app->board_type == PredatorBoardType3in1AIO || app->board_type == PredatorBoardTypeScreen28) {
        if(app->esp32_uart) {
            detection_state.esp32_available = true;
            snprintf(detection_state.esp32_status, sizeof(detection_state.esp32_status), "✓ ESP32 Ready");
            FURI_LOG_I("Detection", "ESP32 detected and ready");
        } else {
            detection_state.esp32_available = false;
            snprintf(detection_state.esp32_status, sizeof(detection_state.esp32_status), "⚠ ESP32 Not Initialized");
            FURI_LOG_W("Detection", "ESP32 UART not initialized");
        }
    } else {
        detection_state.esp32_available = false;
        snprintf(detection_state.esp32_status, sizeof(detection_state.esp32_status), "✗ Not Available on Board");
    }
    
    // DETECT GPS
    if(app->board_type == PredatorBoardType3in1AIO || app->board_type == PredatorBoardTypeScreen28) {
        if(app->gps_uart) {
            detection_state.gps_available = true;
            snprintf(detection_state.gps_status, sizeof(detection_state.gps_status), "✓ GPS Ready");
            FURI_LOG_I("Detection", "GPS detected and ready");
        } else {
            detection_state.gps_available = false;
            snprintf(detection_state.gps_status, sizeof(detection_state.gps_status), "⚠ GPS Not Initialized");
            FURI_LOG_W("Detection", "GPS UART not initialized");
        }
    } else {
        detection_state.gps_available = false;
        snprintf(detection_state.gps_status, sizeof(detection_state.gps_status), "✗ Not Available on Board");
    }
    
    // DETECT SUBGHZ (Always available on Flipper)
    if(app->subghz_txrx) {
        detection_state.subghz_available = true;
        snprintf(detection_state.subghz_status, sizeof(detection_state.subghz_status), "✓ SubGHz Ready");
        FURI_LOG_I("Detection", "SubGHz detected and ready");
    } else {
        detection_state.subghz_available = false;
        snprintf(detection_state.subghz_status, sizeof(detection_state.subghz_status), "⚠ SubGHz Not Initialized");
        FURI_LOG_W("Detection", "SubGHz not initialized");
    }
    
    // DETECT NFC (Always available on Flipper)
    detection_state.nfc_available = true;
    snprintf(detection_state.nfc_status, sizeof(detection_state.nfc_status), "✓ NFC Ready");
    
    FURI_LOG_I("Detection", "Hardware detection complete");
    return true;
}

bool predator_full_detection_check_attack_requirements(PredatorApp* app, PredatorAttackType attack_type, char* error_msg, size_t error_size) {
    if(!app || !error_msg) return false;
    
    // Ensure we have recent detection data
    uint32_t current_time = furi_get_tick();
    if(current_time - detection_state.last_check_time > 30000) { // 30 seconds old
        predator_full_detection_scan(app);
    }
    
    const PredatorBoardConfig* config = predator_boards_get_config(app->board_type);
    const char* board_name = config ? config->name : "Unknown Board";
    
    switch(attack_type) {
        case PredatorAttackWiFi:
            if(!detection_state.esp32_available) {
                snprintf(error_msg, error_size, 
                    "WiFi Attack Requirements:\n\n"
                    "❌ ESP32 Module Required\n"
                    "Current Board: %s\n\n"
                    "Solution:\n"
                    "• Use 3in1 AIO V1.4 board\n"
                    "• Use 2.8\" Screen board\n"
                    "• Check ESP32 connections", 
                    board_name);
                return false;
            }
            break;
            
        case PredatorAttackBluetooth:
            if(!detection_state.esp32_available) {
                snprintf(error_msg, error_size, 
                    "Bluetooth Attack Requirements:\n\n"
                    "❌ ESP32 Module Required\n"
                    "Current Board: %s\n\n"
                    "Solution:\n"
                    "• Use 3in1 AIO V1.4 board\n"
                    "• Use 2.8\" Screen board", 
                    board_name);
                return false;
            }
            break;
            
        case PredatorAttackGPS:
            if(!detection_state.gps_available) {
                snprintf(error_msg, error_size, 
                    "GPS Attack Requirements:\n\n"
                    "❌ GPS Module Required\n"
                    "Current Board: %s\n\n"
                    "Solution:\n"
                    "• Use 3in1 AIO V1.4 board\n"
                    "• Use 2.8\" Screen board\n"
                    "• Move to open area for signal", 
                    board_name);
                return false;
            }
            break;
            
        case PredatorAttackWardriving:
            if(!detection_state.esp32_available || !detection_state.gps_available) {
                snprintf(error_msg, error_size, 
                    "Wardriving Requirements:\n\n"
                    "ESP32: %s\n"
                    "GPS: %s\n\n"
                    "Solution:\n"
                    "• Use 3in1 AIO V1.4 board\n"
                    "• Ensure both modules working", 
                    detection_state.esp32_status, detection_state.gps_status);
                return false;
            }
            break;
            
        case PredatorAttackSubGHz:
            if(!detection_state.subghz_available) {
                snprintf(error_msg, error_size, 
                    "SubGHz Attack Requirements:\n\n"
                    "❌ SubGHz Radio Required\n\n"
                    "Solution:\n"
                    "• Check SubGHz initialization\n"
                    "• Restart application\n"
                    "• Verify Flipper firmware");
                return false;
            }
            break;
            
        case PredatorAttackRFID:
            if(!detection_state.nfc_available) {
                snprintf(error_msg, error_size, 
                    "RFID Attack Requirements:\n\n"
                    "❌ NFC Module Required\n\n"
                    "Solution:\n"
                    "• Check NFC initialization\n"
                    "• Restart application");
                return false;
            }
            break;
            
        default:
            snprintf(error_msg, error_size, "Unknown attack type requested");
            return false;
    }
    
    return true;
}

void predator_full_detection_get_detailed_status(PredatorApp* app, char* status, size_t status_size) {
    if(!app || !status) return;
    
    // Ensure we have recent detection data
    predator_full_detection_scan(app);
    
    const PredatorBoardConfig* config = predator_boards_get_config(app->board_type);
    const char* board_name = config ? config->name : "Unknown Board";
    
    snprintf(status, status_size,
        "Board: %s\n\n"
        "Hardware Status:\n"
        "ESP32: %s\n"
        "GPS: %s\n"
        "SubGHz: %s\n"
        "NFC: %s",
        board_name,
        detection_state.esp32_status,
        detection_state.gps_status,
        detection_state.subghz_status,
        detection_state.nfc_status
    );
}

bool predator_full_detection_show_requirements_popup(PredatorApp* app, PredatorAttackType attack_type) {
    if(!app || !app->popup) return false;
    
    char error_msg[512];
    if(predator_full_detection_check_attack_requirements(app, attack_type, error_msg, sizeof(error_msg))) {
        // Requirements met - no popup needed
        return true;
    }
    
    // Show professional error popup with detailed requirements
    popup_reset(app->popup);
    popup_set_header(app->popup, "Hardware Check", 64, 8, AlignCenter, AlignTop);
    popup_set_text(app->popup, error_msg, 64, 32, AlignCenter, AlignCenter);
    popup_set_timeout(app->popup, 8000); // 8 seconds to read detailed info
    popup_enable_timeout(app->popup);
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
    
    return false;
}
