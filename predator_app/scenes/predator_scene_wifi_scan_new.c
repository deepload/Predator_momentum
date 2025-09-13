#include "../predator_i.h"
#include "../helpers/predator_view_helpers.h"
#include "../helpers/predator_esp32.h"
#include "../helpers/predator_ui_elements.h"

// Custom view for WiFi scan
typedef struct {
    View* view;
    uint32_t networks_found;
    bool scanning;
    bool hardware_ready;
    uint8_t animation_frame;
} WifiScanView;

/*
static void wifi_scan_view_draw_callback(Canvas* canvas, void* context) {
    PredatorApp* app = context;
    
    if(!app) return;
    
    // Get view state
    WifiScanView* state = PREDATOR_GET_MODEL(app->view_dispatcher, WifiScanView);
    if(!state) return;
    
    // Update animation frame
    uint8_t animation_frame = (furi_get_tick() / 200) % 4;
    state->animation_frame = animation_frame;
    state->scanning = app->attack_running;
    state->networks_found = app->targets_found;
    state->hardware_ready = (app->esp32_uart != NULL);
    
    canvas_clear(canvas);
    
    // Draw title
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, 64, 8, AlignCenter, AlignCenter, "WiFi Scanner");
    
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
        // Draw WiFi icon with animation
        uint8_t wifi_x = 30;
        uint8_t wifi_y = 35;
        
        // Draw WiFi signal waves when scanning
        if(state->scanning) {
            for(uint8_t i = 0; i <= state->animation_frame; i++) {
                uint8_t arc_size = 6 + (i * 4);
                canvas_draw_circle(canvas, wifi_x, wifi_y + 5, arc_size);
            }
        }
        
        // Draw WiFi center dot
        canvas_draw_disc(canvas, wifi_x, wifi_y + 5, 2);
        
        // Draw network count
        canvas_set_font(canvas, FontSecondary);
        char wifi_text[16];
        snprintf(wifi_text, sizeof(wifi_text), "%lu", state->networks_found);
        canvas_draw_str_aligned(canvas, wifi_x, wifi_y + 18, AlignCenter, AlignCenter, wifi_text);
        canvas_draw_str_aligned(canvas, wifi_x, wifi_y + 26, AlignCenter, AlignCenter, "networks");
        
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
            canvas_draw_str_aligned(canvas, 84, 45, AlignCenter, AlignCenter, "scanning WiFi");
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
static bool wifi_scan_view_input_callback(InputEvent* event, void* context) {
    PredatorApp* app = context;
    bool consumed = false;
    
    // Get view state
    WifiScanView* state = PREDATOR_GET_MODEL(app->view_dispatcher, WifiScanView);
    if(!state) return consumed;
    
    if(event->type == InputTypeShort || event->type == InputTypeRepeat) {
        switch(event->key) {
        case InputKeyOk:
            if(state->hardware_ready && !state->scanning) {
                // Start scanning
                // predator_esp32_wifi_scan(app);
                app->attack_running = true;
                app->targets_found = 0;
                app->packets_sent = 0;
                state->scanning = true;
                consumed = true;
            } else if(state->hardware_ready && state->scanning) {
                // Stop scanning
                // predator_esp32_stop_attack(app);
                app->attack_running = false;
                state->scanning = false;
                consumed = true;
            }
            break;
        case InputKeyBack:
        case InputKeyLeft:
            // Stop scanning if running and return
            if(state->scanning) {
                // predator_esp32_stop_attack(app);
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
static View* wifi_scan_view_alloc(PredatorApp* app) {
    View* view = view_alloc();
    view_set_context(view, app);
    view_set_draw_callback(view, wifi_scan_view_draw_callback);
    view_set_input_callback(view, wifi_scan_view_input_callback);
    
    // Initialize model
    WifiScanView* state = malloc(sizeof(WifiScanView));
    state->networks_found = 0;
    state->scanning = false;
    state->hardware_ready = (app->esp32_uart != NULL);
    state->animation_frame = 0;
    
    predator_view_set_model(view, state);
    predator_view_set_model_free_callback(view, free);
    
    return view;
}
*/

void predator_scene_wifi_scan_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    if(!app) {
        FURI_LOG_E("WiFiScan", "App context is NULL on enter");
        return;
    }
    
    // Validate board type before any hardware initialization
    if(app->board_type == 0) { 
        FURI_LOG_W("WiFiScan", "Board type is Unknown, defaulting to Original");
        app->board_type = 0; 
    }
    
    // Ensure scene_manager and view_dispatcher are valid to prevent crashes
    if(!app->scene_manager) {
        FURI_LOG_E("WiFiScan", "Scene manager is NULL, cannot proceed");
        return;
    }
    
    if(!app->view_dispatcher) {
        FURI_LOG_E("WiFiScan", "View dispatcher is NULL, cannot switch view");
        return;
    }
    
    // Comment out calls to undefined ESP32 initialization functions
     if(!app->esp32_uart) {
         predator_esp32_init(app);
     }
    
    // Configure popup content to avoid blank screen
    popup_reset(app->popup);
    popup_set_header(app->popup, "WiFi Scanner", 64, 10, AlignCenter, AlignTop);
    popup_set_text(app->popup, "Preparing WiFi scan...\nPress Back to return", 64, 28, AlignCenter, AlignTop);
    popup_set_context(app->popup, app);
    popup_set_timeout(app->popup, 0);
    popup_enable_timeout(app->popup);

    // Switch to popup view
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
    
    FURI_LOG_I("WiFiScan", "WiFi Scan scene entered with simulation mode");
}

bool predator_scene_wifi_scan_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(event.type == SceneManagerEventTypeBack) {
        // Stop scanning and return to previous scene
        app->attack_running = false;
        // predator_esp32_stop_attack(app);
        scene_manager_previous_scene(app->scene_manager);
        consumed = true;
    } else if(event.type == SceneManagerEventTypeTick) {
        if(app->attack_running) {
            // Every ~15 ticks, "find" a new network
            if(app->packets_sent % 15 == 0) {
                app->targets_found++;
            }
            
            // Increment packet counter
            app->packets_sent++;
            consumed = true;
        }
    }
    
    return consumed;
}

void predator_scene_wifi_scan_new_on_exit(void* context) {
    PredatorApp* app = context;
    
    if(!app) {
        FURI_LOG_E("WiFiScan", "App context is NULL on exit");
        return;
    }
    
    // Stop any running scan
    app->attack_running = false;
    // predator_esp32_stop_attack(app);
    // predator_esp32_deinit(app);
    
    FURI_LOG_I("WiFiScan", "Exiting WiFi Scan scene");
}
