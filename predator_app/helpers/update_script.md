# Scene Files Update Instructions

To fix the compilation issues in all scene files, follow these pattern replacements:

## 1. Replace Include Headers

In each scene file, replace:
```c
#include "../helpers/predator_view_dispatcher_patch.h"
```
With:
```c
#include "../helpers/predator_view_helpers.h"
```

## 2. Replace View Model Access

Find all occurrences of the pattern:
```c
TypeName* state = view_get_model(app->view_dispatcher->current_view);
```

Replace with:
```c
TypeName* state = PREDATOR_GET_MODEL(app->view_dispatcher, TypeName);
```

## 3. Fix Exit Function

Replace code that uses `view_dispatcher_get_current_view` with:
```c
// Free custom view
View* view = view_dispatcher_get_view(app->view_dispatcher, ViewID);
if(view) {
    view_free_function(view);
}

// Remove the view from dispatcher
view_dispatcher_remove_view(app->view_dispatcher, ViewID);
```

## Files to Update

1. predator_scene_about_new.c
2. predator_scene_ble_scan_new.c
3. predator_scene_ble_spam_new.c
4. predator_scene_bluetooth_attacks_new.c
5. predator_scene_car_attacks_new.c
6. predator_scene_car_jamming_new.c
7. predator_scene_car_key_bruteforce_new.c
8. predator_scene_car_models_new.c
9. predator_scene_car_passive_opener_new.c
10. predator_scene_car_tesla_new.c
11. predator_scene_gps_tracker_new.c
12. predator_scene_module_status_new.c
13. predator_scene_rfid_attacks_new.c
14. predator_scene_rfid_bruteforce_new.c
15. predator_scene_settings_new.c
16. predator_scene_subghz_attacks_new.c
17. predator_scene_wardriving_new.c
18. predator_scene_wifi_attacks_new.c
19. predator_scene_wifi_deauth_new.c
20. predator_scene_wifi_evil_twin_new.c
21. predator_scene_wifi_scan_new.c

## Example of Full Update (board_selection_new.c)

Original:
```c
#include "../predator_i.h"
#include "../helpers/predator_boards.h"
#include "../helpers/predator_ui_elements.h"
#include "../helpers/predator_view_dispatcher_patch.h"
#include "predator_scene.h"

// ...

static void board_selection_view_draw_callback(Canvas* canvas, void* context) {
    PredatorApp* app = context;
    
    if(!app) return;
    
    // Get view state
    BoardSelectionView* state = view_get_model(app->view_dispatcher->current_view);
    if(!state) return;
    
    // ... rest of function ...
}

// ...

void predator_scene_board_selection_new_on_exit(void* context) {
    PredatorApp* app = context;
    
    // Remove and free custom view
    view_dispatcher_remove_view(app->view_dispatcher, PredatorViewWidget);
    View* view = view_dispatcher_get_current_view(app->view_dispatcher);
    if(view) {
        board_selection_view_free(view);
    }
    
    // Restore standard widget view
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewWidget, widget_get_view(app->widget));
}
```

Updated:
```c
#include "../predator_i.h"
#include "../helpers/predator_boards.h"
#include "../helpers/predator_ui_elements.h"
#include "../helpers/predator_view_helpers.h"
#include "predator_scene.h"

// ...

static void board_selection_view_draw_callback(Canvas* canvas, void* context) {
    PredatorApp* app = context;
    
    if(!app) return;
    
    // Get view state using helper macro
    BoardSelectionView* state = PREDATOR_GET_MODEL(app->view_dispatcher, BoardSelectionView);
    if(!state) return;
    
    // ... rest of function ...
}

// ...

void predator_scene_board_selection_new_on_exit(void* context) {
    PredatorApp* app = context;
    
    // Free custom view
    View* view = view_dispatcher_get_view(app->view_dispatcher, PredatorViewWidget);
    if(view) {
        board_selection_view_free(view);
    }
    
    // Remove the view from dispatcher
    view_dispatcher_remove_view(app->view_dispatcher, PredatorViewWidget);
    
    // Restore standard widget view
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewWidget, widget_get_view(app->widget));
}
```

## Special Case: Fix wardriving_new.c
In the `wardriving_new.c` file, also update the call to `predator_esp32_start_wardriving` in the input_callback function.
