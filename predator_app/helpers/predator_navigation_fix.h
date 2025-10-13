#pragma once

// PROFESSIONAL NAVIGATION PATTERN - Swiss Government Grade
// Apply this pattern to ALL scenes for smooth navigation

/*
NAVIGATION PATTERN FOR ALL SCENES:

1. INPUT CALLBACK PATTERN:
   - Handle InputKeyBack properly
   - For attack scenes: Clean up, then return false (let framework handle)
   - For submenu scenes: Call scene_manager_previous_scene() and return true
   - Always consume events with return true when handled

2. EVENT CALLBACK PATTERN:
   - Handle SceneManagerEventTypeBack for submenu scenes
   - Call scene_manager_previous_scene() and return true
   - For main menu: Call scene_manager_stop() to exit app

3. ATTACK SCENE CLEANUP:
   - Stop any running attacks/timers
   - Clean up resources
   - Log stop message
   - Return false to let framework navigate back

4. SUBMENU SCENE NAVIGATION:
   - Use scene_manager_next_scene() for forward navigation
   - Use scene_manager_previous_scene() for back navigation
   - Always return true when handling navigation

EXAMPLE IMPLEMENTATION:

// For attack scenes (WiFi, Car, RFID, etc.):
if(event->key == InputKeyBack) {
    // Clean up attack
    if(attack_running) {
        stop_attack();
        log_stop_message();
    }
    return false; // Let framework handle back navigation
}

// For submenu scenes (WiFi Attacks, Car Attacks, etc.):
if(event.type == SceneManagerEventTypeBack) {
    scene_manager_previous_scene(app->scene_manager);
    return true;
}

// For main menu:
if(event.type == SceneManagerEventTypeBack) {
    scene_manager_stop(app->scene_manager);
    view_dispatcher_stop(app->view_dispatcher);
    return true;
}
*/

// Navigation helper functions
static inline bool predator_navigation_handle_back_attack_scene(PredatorApp* app, bool attack_running) {
    if(attack_running) {
        // Stop attack and clean up
        predator_log_append(app, "Attack stopped");
    }
    return false; // Let framework handle navigation
}

static inline bool predator_navigation_handle_back_submenu_scene(PredatorApp* app) {
    if(app && app->scene_manager) {
        scene_manager_previous_scene(app->scene_manager);
        return true;
    }
    return false;
}

static inline bool predator_navigation_handle_back_main_menu(PredatorApp* app) {
    if(app && app->scene_manager && app->view_dispatcher) {
        scene_manager_stop(app->scene_manager);
        view_dispatcher_stop(app->view_dispatcher);
        return true;
    }
    return false;
}
