#include "../predator_i.h"
#include "../helpers/predator_view_helpers.h"
#include "../helpers/predator_esp32.h"
#include "../helpers/predator_ui_elements.h"

// Custom view for BLE scan
typedef struct {
    View* view;
    uint32_t devices_found;
    bool scanning;
    bool hardware_ready;
    uint8_t animation_frame;
} BleScanView;

/*
static void ble_scan_view_draw_callback(Canvas* canvas, void* context) {
    PredatorApp* app = context;
    
    if(!app) return;
    
    // Get view state
    BleScanView* state = PREDATOR_GET_MODEL(app->view_dispatcher, BleScanView);
    if(!state) return;
    
    // Update animation frame
    uint8_t animation_frame = (furi_get_tick() / 200) % 4;
    state->animation_frame = animation_frame;
    state->scanning = app->attack_running;
    state->devices_found = app->targets_found;
    state->hardware_ready = (app->esp32_uart != NULL);
    
    canvas_clear(canvas);
    
    // Draw title
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, 64, 8, AlignCenter, AlignCenter, "BLE Scanner");
    
    // Draw separator
    canvas_draw_line(canvas, 0, 16, 128, 16);
    
    if(!state->hardware_ready) {
        // Hardware error display
        predator_ui_draw_status_box(canvas, "Hardware Error", 10, 25, 108, 30);
        
        canvas_set_font(canvas, FontSecondary);
        if(app->board_type == PredatorBoardTypeOriginal) {
            canvas_draw_str_aligned(canvas, 64, 35, AlignCenter, AlignCenter, "Turn Marauder switch ON");
            canvas_draw_str_aligned(canvas, 64, 45, AlignCenter, AlignCenter, "then retry");
        } else {
            canvas_draw_str_aligned(canvas, 64, 35, AlignCenter, AlignCenter, "ESP32 not connected");
            canvas_draw_str_aligned(canvas, 64, 45, AlignCenter, AlignCenter, "Check connections");
        }
    } else {
        // Draw Bluetooth icon with animation
        uint8_t bt_x = 30;
        uint8_t bt_y = 35;
        uint8_t bt_size = 16;
        
        // Draw Bluetooth logo
        canvas_draw_box(canvas, bt_x - 3, bt_y - bt_size/2, 6, bt_size);
        
        // Draw the pointed part at the top
        canvas_draw_line(canvas, bt_x, bt_y - bt_size/2, bt_x + bt_size/3, bt_y - bt_size/4);
        canvas_draw_line(canvas, bt_x + bt_size/3, bt_y - bt_size/4, bt_x, bt_y);
        
        // Draw the pointed part at the bottom
        canvas_draw_line(canvas, bt_x, bt_y, bt_x + bt_size/3, bt_y + bt_size/4);
        canvas_draw_line(canvas, bt_x + bt_size/3, bt_y + bt_size/4, bt_x, bt_y + bt_size/2);
        
        // Draw signal waves when scanning
        if(state->scanning) {
            for(uint8_t i = 0; i <= state->animation_frame; i++) {
                uint8_t wave_size = 5 + (i * 3);
                canvas_draw_circle(canvas, bt_x, bt_y, wave_size);
            }
        }
        
        // Draw device count
        canvas_set_font(canvas, FontSecondary);
        char bt_text[16];
        snprintf(bt_text, sizeof(bt_text), "%lu", state->devices_found);
        canvas_draw_str_aligned(canvas, bt_x, bt_y + 18, AlignCenter, AlignCenter, bt_text);
        canvas_draw_str_aligned(canvas, bt_x, bt_y + 26, AlignCenter, AlignCenter, "devices");
        
        // Status box
        predator_ui_draw_status_box(canvas, "Scan Status", 50, 25, 68, 30);
        
        canvas_set_font(canvas, FontSecondary);
        if(state->scanning) {
            char scan_text[32];
            snprintf(scan_text, sizeof(scan_text), "Scanning...");
            canvas_draw_str_aligned(canvas, 84, 35, AlignCenter, AlignCenter, scan_text);
            canvas_draw_str_aligned(canvas, 84, 45, AlignCenter, AlignCenter, "Press Back to stop");
        } else {
            canvas_draw_str_aligned(canvas, 84, 35, AlignCenter, AlignCenter, "Press OK to start");
            canvas_draw_str_aligned(canvas, 84, 45, AlignCenter, AlignCenter, "scanning BLE");
        }
    }
    
    // Draw controls
    elements_button_left(canvas, "Back");
    if(state->hardware_ready && !state->scanning) {
        elements_button_center(canvas, "Start");
    } else if(state->hardware_ready && state->scanning) {
        elements_button_center(canvas, "Stop");
    }
}
*/

/*
static bool ble_scan_view_input_callback(InputEvent* event, void* context) {
    PredatorApp* app = context;
    bool consumed = false;
    
    // Get view state
    BleScanView* state = PREDATOR_GET_MODEL(app->view_dispatcher, BleScanView);
    if(!state) return consumed;
    
    if(event->type == InputTypeShort || event->type == InputTypeRepeat) {
        switch(event->key) {
        case InputKeyOk:
            if(state->hardware_ready && !state->scanning) {
                // Start scanning
                predator_esp32_ble_scan(app);
                app->attack_running = true;
                app->targets_found = 0;
                app->packets_sent = 0;
                state->scanning = true;
                consumed = true;
            } else if(state->hardware_ready && state->scanning) {
                // Stop scanning
                predator_esp32_stop_attack(app);
                app->attack_running = false;
                state->scanning = false;
                consumed = true;
            }
            break;
        case InputKeyBack:
        case InputKeyLeft:
            // Stop scanning if running and return
            if(state->scanning) {
                predator_esp32_stop_attack(app);
                app->attack_running = false;
                state->scanning = false;
            }
            scene_manager_previous_scene(app->scene_manager);
            consumed = true;
            break;
        default:
            break;
        }
    }
    
    return consumed;
}
*/

/*
static View* ble_scan_view_alloc(PredatorApp* app) {
    View* view = view_alloc();
    view_set_context(view, app);
    view_set_draw_callback(view, ble_scan_view_draw_callback);
    view_set_input_callback(view, ble_scan_view_input_callback);
    
    // Initialize model
    BleScanView* state = malloc(sizeof(BleScanView));
    state->devices_found = 0;
    state->scanning = false;
    state->hardware_ready = (app->esp32_uart != NULL);
    state->animation_frame = 0;
    
    predator_view_set_model(view, state);
    predator_view_set_model_free_callback(view, free);
    
    return view;
}
*/

void predator_scene_ble_scan_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    if(!app) {
        FURI_LOG_E("BLEScan", "App context is NULL on enter");
        return;
    }
    
    // Validate board type before any hardware initialization
    if(app->board_type == 0) { // Assuming 0 represents Unknown or default
        FURI_LOG_W("BLEScan", "Board type is Unknown, defaulting to Original");
        app->board_type = 0; // Keep as Original
    }
    
    // Ensure scene_manager and view_dispatcher are valid to prevent crashes
    if(!app->scene_manager) {
        FURI_LOG_E("BLEScan", "Scene manager is NULL, cannot proceed");
        return;
    }
    
    if(!app->view_dispatcher) {
        FURI_LOG_E("BLEScan", "View dispatcher is NULL, cannot switch view");
        return;
    }
    
    // Comment out calls to undefined ESP32 initialization functions
    // if(!app->esp32_uart) {
    //     predator_esp32_init(app);
    // }
    
    // Configure popup content to avoid blank screen
    popup_reset(app->popup);
    popup_set_header(app->popup, "BLE Scanner", 64, 10, AlignCenter, AlignTop);
    popup_set_text(app->popup, "Preparing BLE scan...\nPress Back to return", 64, 28, AlignCenter, AlignTop);
    popup_set_context(app->popup, app);
    popup_set_timeout(app->popup, 0);
    popup_enable_timeout(app->popup);

    // Switch to popup view
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
    
    FURI_LOG_I("BLEScan", "BLE Scan scene entered with simulation mode");
}

bool predator_scene_ble_scan_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(event.type == SceneManagerEventTypeBack) {
        // Stop scanning and return to previous scene
        app->attack_running = false;
        predator_esp32_stop_attack(app);
        scene_manager_previous_scene(app->scene_manager);
        consumed = true;
    } else if(event.type == SceneManagerEventTypeTick) {
        if(app->attack_running) {
            // Every ~20 ticks, "find" a new device
            if(app->packets_sent % 20 == 0) {
                app->targets_found++;
            }
            
            // Increment packet counter
            app->packets_sent++;
            consumed = true;
        }
    }
    
    return consumed;
}

void predator_scene_ble_scan_new_on_exit(void* context) {
    PredatorApp* app = context;
    
    // Stop any running scan
    app->attack_running = false;
    predator_esp32_stop_attack(app);
    
    // Do not remove/add core Popup view here; it is managed in app init
}
