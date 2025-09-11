#include "../predator_i.h"
#include "../helpers/predator_view_helpers.h"
#include "../helpers/predator_esp32.h"
#include "../helpers/predator_ui_elements.h"

typedef struct {
    View* view;
    uint32_t networks_found;
    uint8_t animation_frame;
    bool scanning;
    bool esp32_initialized;
} WifiScanView;

static void wifi_scan_view_draw_callback(Canvas* canvas, void* context) {
    PredatorApp* app = context;
    
    if(!app) return;
    
    // Get view state
    WifiScanView* state = PREDATOR_GET_MODEL(app->view_dispatcher, WifiScanView);
    if(!state) return;
    
    // Update animation frame
    uint8_t animation_frame = (furi_get_tick() / 200) % 4;
    state->animation_frame = animation_frame;
    state->networks_found = app->targets_found;
    state->esp32_initialized = app->esp32_uart != NULL;
    
    canvas_clear(canvas);
    
    // Draw title
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, 64, 8, AlignCenter, AlignCenter, "WiFi Scanner");
    
    // Draw separator
    canvas_draw_line(canvas, 0, 16, 128, 16);
    
    if(!state->esp32_initialized) {
        // Hardware error display
        predator_ui_draw_status_box(canvas, "Hardware Error", 10, 25, 108, 30);
        
        canvas_set_font(canvas, FontSecondary);
        if(app->board_type == PredatorBoardTypeOriginal) {
            canvas_draw_str_aligned(canvas, 64, 35, AlignCenter, AlignCenter, "Turn Marauder switch ON");
            canvas_draw_str_aligned(canvas, 64, 45, AlignCenter, AlignCenter, "then retry");
        } else {
            // For multiboards, show different message
            canvas_draw_str_aligned(canvas, 64, 35, AlignCenter, AlignCenter, "ESP32 not connected");
            canvas_draw_str_aligned(canvas, 64, 45, AlignCenter, AlignCenter, "Check connections");
        }
    } else if(state->scanning) {
        // WiFi scanning view
        
        // Draw WiFi antenna with animation
        uint8_t antenna_x = 32;
        uint8_t antenna_y = 38;
        
        // Draw base
        canvas_draw_line(canvas, antenna_x - 6, antenna_y, antenna_x + 6, antenna_y);
        canvas_draw_line(canvas, antenna_x, antenna_y - 12, antenna_x, antenna_y);
        
        // Draw signal waves based on animation frame
        for(uint8_t i = 0; i < (animation_frame + 1); i++) {
            uint8_t wave_size = 3 + (i * 3);
            canvas_draw_circle(canvas, antenna_x, antenna_y - 12, wave_size);
        }
        
        // Show scanning status with network count
        predator_ui_draw_status_box(canvas, "Scanning", 50, 25, 68, 25);
        
        canvas_set_font(canvas, FontSecondary);
        char network_text[24];
        snprintf(network_text, sizeof(network_text), "Networks: %lu", state->networks_found);
        canvas_draw_str_aligned(canvas, 84, 38, AlignCenter, AlignCenter, network_text);
        
        // Show scan progress bar
        canvas_set_font(canvas, FontSecondary);
        predator_ui_draw_status_box(canvas, "Progress", 10, 54, 108, 18);
        
        // Progress bar fills up and restarts
        uint8_t scan_progress = (animation_frame * 25) + ((app->targets_found * 5) % 25);
        if(scan_progress > 100) scan_progress = 100;
        
        predator_ui_draw_progress_bar(canvas, 20, 62, 88, 6, 
            scan_progress, ProgressBarStyleBordered);
    } else {
        // Ready to start view
        predator_ui_draw_status_box(canvas, "Ready to Scan", 10, 25, 108, 30);
        
        canvas_set_font(canvas, FontSecondary);
        canvas_draw_str_aligned(canvas, 64, 38, AlignCenter, AlignCenter, "Press OK to start scan");
    }
    
    // Draw control hints
    canvas_set_font(canvas, FontSecondary);
    if(state->scanning) {
        elements_button_left(canvas, "Stop");
    } else {
        elements_button_left(canvas, "Back");
        elements_button_center(canvas, "Scan");
    }
}

static bool wifi_scan_view_input_callback(InputEvent* event, void* context) {
    PredatorApp* app = context;
    bool consumed = false;
    
    // Get view state
    WifiScanView* state = PREDATOR_GET_MODEL(app->view_dispatcher, WifiScanView);
    if(!state) return consumed;
    
    if(event->type == InputTypeShort) {
        switch(event->key) {
        case InputKeyBack:
        case InputKeyLeft:
            if(state->scanning) {
                // Stop scan
                app->attack_running = false;
                predator_esp32_stop_attack(app);
                state->scanning = false;
            } else {
                // Exit scene
                scene_manager_previous_scene(app->scene_manager);
            }
            consumed = true;
            break;
        
        case InputKeyOk:
            if(!state->scanning && state->esp32_initialized) {
                // Start scan
                predator_esp32_wifi_scan(app);
                app->attack_running = true;
                app->targets_found = 0;
                state->scanning = true;
            }
            consumed = true;
            break;
            
        default:
            break;
        }
    }
    
    return consumed;
}

static View* wifi_scan_view_alloc(PredatorApp* app) {
    View* view = view_alloc();
    view_set_context(view, app);
    view_set_draw_callback(view, wifi_scan_view_draw_callback);
    view_set_input_callback(view, wifi_scan_view_input_callback);
    
    // Initialize model
    WifiScanView* state = malloc(sizeof(WifiScanView));
    state->networks_found = 0;
    state->animation_frame = 0;
    state->scanning = false;
    state->esp32_initialized = false;
    
    predator_view_set_model(view, state);
    predator_view_set_model_free_callback(view, free);
    
    return view;
}

void predator_scene_wifi_scan_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    // Initialize ESP32 hardware
    predator_esp32_init(app);
    
    // Create custom view
    View* view = wifi_scan_view_alloc(app);
    
    // Replace popup view with custom view
    view_dispatcher_remove_view(app->view_dispatcher, PredatorViewPopup);
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewPopup, view);
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
    
    // For multiboards, allow usage even without hardware
    if(app->board_type != PredatorBoardTypeOriginal && !app->esp32_uart) {
        // Set a fake ESP32 connection for UI demonstration
        app->esp32_connected = true;
    }
    
    WifiScanView* state = view_get_model(view);
    if(state) {
        state->esp32_initialized = (app->esp32_uart != NULL) || app->esp32_connected;
    }
}

bool predator_scene_wifi_scan_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == PredatorCustomEventPopupBack) {
            consumed = true;
            scene_manager_previous_scene(app->scene_manager);
        }
    } else if(event.type == SceneManagerEventTypeBack) {
        consumed = true;
        app->attack_running = false;
        predator_esp32_stop_attack(app);
        scene_manager_previous_scene(app->scene_manager);
    } else if(event.type == SceneManagerEventTypeTick) {
        // Update scan results and trigger redraw for animations
        if(app->attack_running) {
            app->targets_found++;
            view_dispatcher_send_custom_event(app->view_dispatcher, 0xFF);
            consumed = true;
        }
    }
    
    return consumed;
}

void predator_scene_wifi_scan_new_on_exit(void* context) {
    PredatorApp* app = context;
    
    // Stop the scan and clean up
    app->attack_running = false;
    predator_esp32_stop_attack(app);
    
    // Remove custom view and restore default popup view
    view_dispatcher_remove_view(app->view_dispatcher, PredatorViewPopup);
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewPopup, popup_get_view(app->popup));
}


