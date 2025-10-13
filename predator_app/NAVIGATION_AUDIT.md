# Navigation Safety Audit - Predator Momentum

## Overview
This document audits all scenes for navigation safety to prevent the app from quitting unexpectedly during Tesla/Government demonstrations.

## Navigation Patterns Analysis

### ✅ SAFE PATTERNS (Scenes following correct navigation)

#### Submenu-based Scenes (Using framework navigation)
- `predator_scene_main_menu_ui.c` - ✅ Properly exits app only from main menu
- `predator_scene_wifi_attacks_ui.c` - ✅ Uses scene_manager_previous_scene()
- `predator_scene_bluetooth_attacks_ui.c` - ✅ Uses scene_manager_previous_scene()
- `predator_scene_car_attacks_ui.c` - ✅ Uses scene_manager_previous_scene()
- `predator_scene_rfid_attacks_ui.c` - ✅ Uses scene_manager_previous_scene()
- `predator_scene_subghz_attacks_ui.c` - ✅ Uses scene_manager_previous_scene()

#### Custom View Scenes (Proper input callback handling)
- `predator_scene_wifi_scan_ui.c` - ✅ Returns false on back, lets scene manager handle
- `predator_scene_wifi_deauth_ui.c` - ✅ Returns false on back
- `predator_scene_wifi_evil_twin_ui.c` - ✅ Returns false on back
- `predator_scene_car_tesla_ui.c` - ✅ Returns false on back
- `predator_scene_settings_ui.c` - ✅ Returns false on back
- `predator_scene_gps_tracker_ui.c` - ✅ Proper scene manager handling
- `predator_scene_module_status_ui.c` - ✅ Proper scene manager handling

#### Professional Stabilized Scenes (With back-debounce)
- `predator_scene_universal_car_hacker.c` - ✅ 500ms back-debounce + proper navigation
- `predator_scene_tesla_security_suite.c` - ✅ 500ms back-debounce + proper navigation
- `predator_scene_car_model_tests.c` - ✅ 500ms back-debounce + proper navigation
- `predator_scene_maximum_power.c` - ✅ 500ms back-debounce + proper navigation

### 🔧 FIXED PATTERNS

#### Multi-Screen Custom Views (Fixed with custom events)
- `predator_scene_board_selection_ui.c` - ✅ FIXED: Uses custom event (999) for complex navigation

## Navigation Safety Rules

### 1. Input Callback Pattern (Custom Views)
```c
static bool scene_input_callback(InputEvent* event, void* context) {
    PredatorApp* app = context;
    if(!app) return false;
    
    if(event->type == InputTypeShort && event->key == InputKeyBack) {
        // Perform cleanup
        cleanup_scene_resources();
        
        // CRITICAL: Return false to let scene manager handle navigation
        return false;
    }
    
    // Handle other inputs
    return true;
}
```

### 2. Scene Event Handler Pattern (All Scenes)
```c
bool scene_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    if(!app) return false;
    
    if(event.type == SceneManagerEventTypeBack) {
        scene_manager_previous_scene(app->scene_manager);
        return true;
    }
    
    return false;
}
```

### 3. Professional Back-Debounce (Critical Scenes)
```c
static uint32_t last_back_press = 0;

if(event.type == SceneManagerEventTypeBack) {
    uint32_t current_tick = furi_get_tick();
    if(current_tick - last_back_press < 500) {
        return true; // Debounced
    }
    last_back_press = current_tick;
    
    scene_manager_previous_scene(app->scene_manager);
    return true;
}
```

### 4. Multi-Screen Navigation (Complex Scenes)
```c
// In input callback:
case InputKeyBack:
    if(complex_scene_state.current_screen == MainScreen) {
        // Send custom event to scene manager
        view_dispatcher_send_custom_event(app->view_dispatcher, 999);
    } else {
        // Navigate within scene
        complex_scene_state.current_screen = PreviousScreen;
    }
    return true;

// In scene event handler:
if(event.type == SceneManagerEventTypeCustom && event.event == 999) {
    scene_manager_previous_scene(app->scene_manager);
    return true;
}
```

## Critical Navigation Points

### ⚠️ NEVER DO THESE:
1. `scene_manager_stop()` - Only allowed in main menu
2. `view_dispatcher_stop()` - Only allowed in main menu
3. Direct scene manager calls in input callbacks (except custom events)
4. Returning true on back button without handling navigation

### ✅ ALWAYS DO THESE:
1. Return false on back button in input callbacks
2. Use scene_manager_previous_scene() in event handlers
3. Perform cleanup before navigation
4. Use back-debounce for professional stability
5. Test navigation flow thoroughly

## Tesla/Government Demo Requirements

All scenes must maintain:
- **Stability**: No unexpected app exits
- **Professional Feel**: Smooth navigation with debouncing
- **Reliability**: Consistent behavior across all scenarios
- **Government Grade**: Zero tolerance for navigation failures

## Audit Status: ✅ COMPLETE

All scenes have been audited and follow safe navigation patterns. The board selection scene was the only problematic scene and has been fixed with proper multi-screen navigation using custom events.

**Result**: Navigation system is now Tesla/Government demonstration ready.
