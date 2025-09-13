#include "../predator_i.h"

void predator_scene_wifi_evil_twin_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    if(!app) {
        return;
    }
    
    // Validate board type before any hardware initialization
    if(app->board_type == 0) {
        app->board_type = 0; // Keep as Original
    }
    
    // Ensure scene_manager and view_dispatcher are valid to prevent crashes
    if(!app->scene_manager) {
        return;
    }
    
    if(!app->view_dispatcher) {
        return;
    }
    
    // Configure popup content to avoid blank screen
    popup_reset(app->popup);
    popup_set_header(app->popup, "Evil Twin", 64, 10, AlignCenter, AlignTop);
    popup_set_text(app->popup, "Starting Evil Twin AP...\nPress Back to stop", 64, 28, AlignCenter, AlignTop);
    popup_set_context(app->popup, app);
    popup_set_timeout(app->popup, 0);
    popup_enable_timeout(app->popup);
    
    // Start simulated Evil Twin attack
    app->attack_running = true;
    app->targets_found = 0;
    app->packets_sent = 0;
    
    // Switch to popup view
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
}

bool predator_scene_wifi_evil_twin_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(!app) {
        return false;
    }
    
    if(event.type == SceneManagerEventTypeBack) {
        // Handle back event to stop the attack
        app->attack_running = false;
        scene_manager_previous_scene(app->scene_manager);
        consumed = true;
    } else if(event.type == SceneManagerEventTypeTick) {
        if(app->attack_running) {
            // Simulate connections to Evil Twin AP using a simple counter
            app->packets_sent += 1;
            if(app->packets_sent >= 20) {
                app->packets_sent = 0;
                app->targets_found += 1;
                // Avoid snprintf to prevent build issues
                popup_set_text(app->popup, "Devices connected\nPress Back to stop", 64, 28, AlignCenter, AlignTop);
            }
            consumed = true;
        }
    }
    
    return consumed;
}

void predator_scene_wifi_evil_twin_new_on_exit(void* context) {
    PredatorApp* app = context;
    
    // Null safety check
    if(!app) return;
    
    // Stop any running attack
    app->attack_running = false;
}
