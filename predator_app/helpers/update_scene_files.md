# Scene Files Update Guide

To fix the compilation errors, we need to update all scene files that directly access `view_dispatcher->current_view`. The pattern to update is:

## Find this pattern:

```c
SomeView* state = view_get_model(app->view_dispatcher->current_view);
```

## Replace with this pattern:

```c
SomeView* state = predator_get_view_model(app->view_dispatcher);
```

## Files to update:

- predator_scene_about_new.c
- predator_scene_ble_scan_new.c
- predator_scene_ble_spam_new.c
- predator_scene_bluetooth_attacks_new.c
- predator_scene_board_selection_new.c
- predator_scene_car_attacks_new.c
- predator_scene_car_jamming_new.c
- predator_scene_car_key_bruteforce_new.c
- predator_scene_car_models_new.c
- predator_scene_car_passive_opener_new.c
- predator_scene_car_tesla_new.c
- predator_scene_gps_tracker_new.c
- predator_scene_module_status_new.c
- predator_scene_rfid_attacks_new.c
- predator_scene_rfid_bruteforce_new.c
- predator_scene_settings_new.c
- predator_scene_subghz_attacks_new.c
- predator_scene_wardriving_new.c
- predator_scene_wifi_attacks_new.c
- predator_scene_wifi_deauth_new.c
- predator_scene_wifi_evil_twin_new.c
- predator_scene_wifi_scan_new.c

## Example update for one file (board_selection_new.c):

Original code:
```c
static void board_selection_view_draw_callback(Canvas* canvas, void* context) {
    PredatorApp* app = context;
    
    if(!app) return;
    
    // Get view state
    BoardSelectionView* state = view_get_model(app->view_dispatcher->current_view);
    if(!state) return;
    
    // ... rest of code ...
}
```

Updated code:
```c
static void board_selection_view_draw_callback(Canvas* canvas, void* context) {
    PredatorApp* app = context;
    
    if(!app) return;
    
    // Get view state using helper function
    BoardSelectionView* state = predator_get_view_model(app->view_dispatcher);
    if(!state) return;
    
    // ... rest of code ...
}
```

The update should be done in all draw_callback, input_callback, and scene event handler functions that use this pattern.
