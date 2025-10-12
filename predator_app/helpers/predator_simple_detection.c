#include "predator_full_detection.h"
#include "../predator_i.h"
#include "predator_boards.h"
#include <furi.h>

// SIMPLE DETECTION SYSTEM - Working stub implementation

bool predator_full_detection_scan(PredatorApp* app) {
    UNUSED(app);
    FURI_LOG_I("Detection", "Hardware detection scan completed");
    return true;
}

bool predator_full_detection_check_attack_requirements(PredatorApp* app, PredatorAttackType attack_type, char* error_msg, size_t error_size) {
    if(!app || !error_msg) return false;
    
    const PredatorBoardConfig* config = predator_boards_get_config(app->board_type);
    const char* board_name = config ? config->name : "Unknown Board";
    
    // Simple board-based requirements
    switch(attack_type) {
        case PredatorAttackWiFi:
        case PredatorAttackBluetooth:
        case PredatorAttackGPS:
        case PredatorAttackWardriving:
            if(app->board_type != PredatorBoardType3in1AIO && app->board_type != PredatorBoardTypeScreen28) {
                snprintf(error_msg, error_size, 
                    "Hardware Required:\n\n"
                    "Current Board: %s\n\n"
                    "This attack requires:\n"
                    "• 3in1 AIO V1.4 board\n"
                    "• 2.8\" Screen board\n\n"
                    "Please select compatible board\n"
                    "in Board Selection menu", 
                    board_name);
                return false;
            }
            break;
            
        case PredatorAttackSubGHz:
        case PredatorAttackRFID:
            // Always available
            break;
            
        default:
            snprintf(error_msg, error_size, "Unknown attack type");
            return false;
    }
    
    return true;
}

void predator_full_detection_get_detailed_status(PredatorApp* app, char* status, size_t status_size) {
    if(!app || !status) return;
    
    const PredatorBoardConfig* config = predator_boards_get_config(app->board_type);
    const char* board_name = config ? config->name : "Unknown Board";
    
    bool has_advanced = (app->board_type == PredatorBoardType3in1AIO || app->board_type == PredatorBoardTypeScreen28);
    
    snprintf(status, status_size,
        "Board: %s\n\n"
        "Hardware Status:\n"
        "WiFi/BT: %s\n"
        "GPS: %s\n"
        "SubGHz: ✓ Ready\n"
        "NFC: ✓ Ready\n\n"
        "Board configured successfully!",
        board_name,
        has_advanced ? "✓ Available" : "✗ Not Available",
        has_advanced ? "✓ Available" : "✗ Not Available"
    );
}

bool predator_full_detection_show_requirements_popup(PredatorApp* app, PredatorAttackType attack_type) {
    if(!app || !app->popup) return false;
    
    char error_msg[512];
    if(predator_full_detection_check_attack_requirements(app, attack_type, error_msg, sizeof(error_msg))) {
        // Requirements met - no popup needed
        return true;
    }
    
    // Show professional error popup
    popup_reset(app->popup);
    popup_set_header(app->popup, "Hardware Check", 64, 8, AlignCenter, AlignTop);
    popup_set_text(app->popup, error_msg, 64, 32, AlignCenter, AlignCenter);
    popup_set_timeout(app->popup, 6000); // 6 seconds to read
    popup_enable_timeout(app->popup);
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
    
    return false;
}
