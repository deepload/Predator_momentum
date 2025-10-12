#include "predator_hardware_manager.h"
#include "../predator_i.h"
#include "predator_boards.h"
#include <furi.h>

// SIMPLIFIED PROFESSIONAL HARDWARE MANAGER

bool predator_hardware_validate_board(PredatorApp* app) {
    if(!app) return false;
    
    const PredatorBoardConfig* config = predator_boards_get_config(app->board_type);
    if(!config) {
        FURI_LOG_E("HardwareManager", "Invalid board configuration");
        return false;
    }
    
    FURI_LOG_I("HardwareManager", "Validating hardware for board: %s", config->name);
    return true; // Simplified - always return true for now
}

bool predator_hardware_check_requirements(PredatorApp* app, PredatorAttackType attack_type, char* error_msg, size_t error_size) {
    if(!app || !error_msg) return false;
    
    const PredatorBoardConfig* config = predator_boards_get_config(app->board_type);
    if(!config) {
        snprintf(error_msg, error_size, "Invalid board configuration");
        return false;
    }
    
    switch(attack_type) {
        case PredatorAttackWiFi:
        case PredatorAttackBluetooth:
            if(app->board_type != PredatorBoardType3in1AIO && app->board_type != PredatorBoardTypeScreen28) {
                snprintf(error_msg, error_size, 
                    "WiFi/Bluetooth requires ESP32!\n\n"
                    "Current: %s\n\n"
                    "Solution: Use 3in1 AIO or 2.8\" Screen board", 
                    config->name);
                return false;
            }
            break;
            
        case PredatorAttackGPS:
        case PredatorAttackWardriving:
            if(app->board_type != PredatorBoardType3in1AIO && app->board_type != PredatorBoardTypeScreen28) {
                snprintf(error_msg, error_size, 
                    "GPS requires GPS module!\n\n"
                    "Current: %s\n\n"
                    "Solution: Use 3in1 AIO or 2.8\" Screen board", 
                    config->name);
                return false;
            }
            break;
            
        case PredatorAttackSubGHz:
        case PredatorAttackRFID:
            // Always available on Flipper
            break;
            
        default:
            snprintf(error_msg, error_size, "Unknown attack type");
            return false;
    }
    
    return true;
}

void predator_hardware_get_status_summary(PredatorApp* app, char* summary, size_t summary_size) {
    if(!app || !summary) return;
    
    const PredatorBoardConfig* config = predator_boards_get_config(app->board_type);
    const char* board_name = config ? config->name : "Unknown";
    
    bool has_esp32 = (app->board_type == PredatorBoardType3in1AIO || app->board_type == PredatorBoardTypeScreen28);
    bool has_gps = has_esp32; // Same boards have both
    
    snprintf(summary, summary_size,
        "Board: %s\n\n"
        "WiFi/BT: %s\n"
        "GPS: %s\n"
        "SubGHz: ✓ Ready\n"
        "NFC: ✓ Ready",
        board_name,
        has_esp32 ? "✓ Ready" : "✗ Missing",
        has_gps ? "✓ Ready" : "✗ Missing"
    );
}

bool predator_hardware_show_requirements_popup(PredatorApp* app, PredatorAttackType attack_type) {
    if(!app || !app->popup) return false;
    
    char error_msg[256];
    if(predator_hardware_check_requirements(app, attack_type, error_msg, sizeof(error_msg))) {
        // Requirements met - no popup needed
        return true;
    }
    
    // Show professional error popup
    popup_reset(app->popup);
    popup_set_header(app->popup, "Hardware Required", 64, 10, AlignCenter, AlignTop);
    popup_set_text(app->popup, error_msg, 64, 32, AlignCenter, AlignCenter);
    popup_set_timeout(app->popup, 5000); // 5 seconds to read
    popup_enable_timeout(app->popup);
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
    
    return false;
}
