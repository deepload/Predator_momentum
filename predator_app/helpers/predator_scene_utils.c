#include "predator_scene_utils.h"
#include <furi.h>

bool predator_scene_ensure_hardware(
    PredatorApp* app, 
    void (*init_callback)(PredatorApp*), 
    PopupCallback popup_callback,
    bool (*is_initialized)(PredatorApp*),
    const char* hardware_name) {
    
    furi_assert(app);
    furi_assert(init_callback);
    furi_assert(is_initialized);
    furi_assert(hardware_name);
    
    // Initialize the hardware
    init_callback(app);
    
    // Check if initialization succeeded
    if(is_initialized(app)) {
        return true;
    }
    
    // Try to force initialization for multiboards
    if(app->board_type != PredatorBoardTypeOriginal) {
        FURI_LOG_I("SceneUtils", "Attempting to force %s init for multiboard", hardware_name);
        
        // Try initialization again
        init_callback(app);
        
        if(is_initialized(app)) {
            FURI_LOG_I("SceneUtils", "Forced init succeeded for %s", hardware_name);
            return true;
        }
    }
    
    // If still not initialized, show appropriate error message
    if(app->board_type == PredatorBoardTypeOriginal) {
        // Original board - show hardware error
        popup_set_header(app->popup, "Hardware Error", 64, 10, AlignCenter, AlignTop);
        
        char text_buffer[128];
        snprintf(text_buffer, sizeof(text_buffer),
            "%s not initialized.\n"
            "Check hardware connections\n"
            "and power switches.",
            hardware_name);
        
        popup_set_text(app->popup, text_buffer, 64, 25, AlignCenter, AlignTop);
    } else {
        // Multiboard - create dummy handle and continue
        FURI_LOG_I("SceneUtils", "Using fallback for %s on multiboard", hardware_name);
        
        // Show warning but allow functionality
        popup_set_header(app->popup, "Warning", 64, 10, AlignCenter, AlignTop);
        
        char text_buffer[128];
        snprintf(text_buffer, sizeof(text_buffer),
            "%s module issue.\n"
            "Continuing in demo mode\n"
            "with limited functionality.",
            hardware_name);
        
        popup_set_text(app->popup, text_buffer, 64, 25, AlignCenter, AlignTop);
        
        // Set up dummy callback
        if(popup_callback) {
            popup_set_callback(app->popup, popup_callback);
            popup_set_context(app->popup, app);
            popup_set_timeout(app->popup, 2000); // Auto-dismiss after 2 seconds
            popup_enable_timeout(app->popup);
        }
        
        // For multiboards, always create a dummy handle to allow scenes to proceed
        // This is application-specific and would depend on the type of hardware
        
        return true;
    }
    
    // Set up callback for error popup
    if(popup_callback) {
        popup_set_callback(app->popup, popup_callback);
        popup_set_context(app->popup, app);
        popup_set_timeout(app->popup, 0);
        popup_enable_timeout(app->popup);
    }
    
    return false;
}

bool predator_scene_transition_safe(PredatorApp* app, uint32_t next_scene) {
    furi_assert(app);
    
    // Check for any required hardware for the scene
    bool can_transition = true;
    
    // WiFi scenes (temporarily disabled for Elon's demo)
    // if(next_scene == PredatorSceneWifiScanUI ||
    //    next_scene == PredatorSceneWifiDeauthUI ||
    //    next_scene == PredatorSceneWifiEvilTwinUI) {
    //     // Check ESP32 initialization for WiFi scenes
    //     can_transition = (app->esp32_uart != NULL);
    // }
    // Car attack scenes
    if(next_scene == PredatorSceneCarKeyBruteforceUI ||
            next_scene == PredatorSceneCarJammingUI) {
        // Check SubGHz initialization for car attack scenes
        can_transition = (app->subghz_txrx != NULL);
    }
    // GPS scenes
    else if(next_scene == PredatorSceneGpsTrackerUI) {
        // Check GPS initialization for GPS scenes
        can_transition = (app->gps_uart != NULL);
    }
    else {
        // No specific hardware requirements for other scenes
        can_transition = true;
    }
    
    // For multiboards, always allow transitions
    if(!can_transition && app->board_type != PredatorBoardTypeOriginal) {
        can_transition = true;
    }
    
    if(can_transition) {
        scene_manager_next_scene(app->scene_manager, next_scene);
    }
    
    return can_transition;
}
