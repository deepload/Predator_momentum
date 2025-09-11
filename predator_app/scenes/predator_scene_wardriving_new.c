#include "../predator_i.h"
#include "../helpers/predator_view_helpers.h"
#include "../helpers/predator_esp32.h"
#include "../helpers/predator_gps.h"
#include "../helpers/predator_ui_elements.h"
#include "../helpers/predator_view_helpers.h"

typedef struct {
    View* view;
    uint32_t networks_found;
    uint32_t gps_points;
    bool scanning;
    bool gps_active;
    uint8_t animation_frame;
    float last_latitude;
    float last_longitude;
    char strongest_network[32];
    int8_t strongest_rssi;
} WardrivingView;

static void wardriving_view_draw_callback(Canvas* canvas, void* context) {
    PredatorApp* app = context;
    
    if(!app) return;
    
    // Get view state using helper macro
    WardrivingView* state = PREDATOR_GET_MODEL(app->view_dispatcher, WardrivingView);
    if(!state) return;
    
    // Update animation frame
    uint8_t animation_frame = (furi_get_tick() / 200) % 4;
    state->animation_frame = animation_frame;
    
    // Update state from app
    state->scanning = app->attack_running;
    state->gps_active = app->gps_connected;
    state->networks_found = app->targets_found;
    
    // Update GPS coordinates if connected
    if(state->gps_active) {
        state->last_latitude = app->latitude;
        state->last_longitude = app->longitude;
        
        // Count GPS points if valid coordinates
        if(app->latitude != 0.0f || app->longitude != 0.0f) {
            if(app->packets_sent % 10 == 0) { // Only count every 10th update to simulate points
                state->gps_points++;
            }
        }
    }
    
    canvas_clear(canvas);
    
    // Draw title
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, 64, 8, AlignCenter, AlignCenter, "Wardriving");
    
    // Draw separator
    canvas_draw_line(canvas, 0, 16, 128, 16);
    
    // Split screen into left and right sections
    
    // Left section - WiFi status
    uint8_t wifi_x = 30;
    uint8_t wifi_y = 30;
    
    // Draw WiFi icon with animation
    for(uint8_t i = 0; i < MIN(3, animation_frame + 1); i++) {
        uint8_t arc_size = 6 + (i * 4);
        // Draw wifi arc segments
        canvas_draw_circle(canvas, wifi_x, wifi_y + 5, arc_size);
    }
    
    // Draw WiFi center dot
    canvas_draw_disc(canvas, wifi_x, wifi_y + 5, 2);
    
    // Draw network count
    canvas_set_font(canvas, FontSecondary);
    char wifi_text[16];
    snprintf(wifi_text, sizeof(wifi_text), "%lu", state->networks_found);
    canvas_draw_str_aligned(canvas, wifi_x, wifi_y + 18, AlignCenter, AlignCenter, wifi_text);
    canvas_draw_str_aligned(canvas, wifi_x, wifi_y + 26, AlignCenter, AlignCenter, "networks");
    
    // Right section - GPS status
    uint8_t gps_x = 90;
    uint8_t gps_y = 30;
    
    // Draw GPS icon with animation
    canvas_draw_box(canvas, gps_x - 6, gps_y + 6, 12, 3);
    canvas_draw_line(canvas, gps_x, gps_y - 5, gps_x, gps_y + 6);
    
    // Draw GPS signal waves when active
    if(state->gps_active) {
        for(uint8_t i = 0; i < MIN(3, animation_frame + 1); i++) {
            uint8_t arc_size = 4 + (i * 3);
            canvas_draw_circle(canvas, gps_x, gps_y - 5, arc_size);
        }
    }
    
    // Draw GPS point count
    char gps_text[16];
    snprintf(gps_text, sizeof(gps_text), "%lu", state->gps_points);
    canvas_draw_str_aligned(canvas, gps_x, gps_y + 18, AlignCenter, AlignCenter, gps_text);
    canvas_draw_str_aligned(canvas, gps_x, gps_y + 26, AlignCenter, AlignCenter, "points");
    
    // Draw status box with details
    predator_ui_draw_status_box(canvas, "Status", 8, 48, 112, 28);
    
    canvas_set_font(canvas, FontSecondary);
    
    // Draw status or coordinates
    if(state->scanning) {
        // Draw coordinates if GPS is active
        if(state->gps_active && (state->last_latitude != 0.0f || state->last_longitude != 0.0f)) {
            char lat_text[16];
            char lon_text[16];
            snprintf(lat_text, sizeof(lat_text), "Lat: %.4f", (double)state->last_latitude);
            snprintf(lon_text, sizeof(lon_text), "Lon: %.4f", (double)state->last_longitude);
            
            canvas_draw_str(canvas, 12, 58, lat_text);
            canvas_draw_str(canvas, 12, 68, lon_text);
        } else if(state->gps_active) {
            canvas_draw_str(canvas, 12, 58, "GPS: Acquiring...");
            canvas_draw_str(canvas, 12, 68, "WiFi: Scanning...");
        } else {
            canvas_draw_str(canvas, 12, 58, "GPS: Not available");
            canvas_draw_str(canvas, 12, 68, "WiFi: Scanning...");
        }
        
        // Draw scanning animation (avoid strcat)
        char scan_text[16] = "SCANNING";
        size_t base_len = 8; // strlen("SCANNING")
        uint8_t dots = animation_frame;
        if(dots > 3) dots = 3;
        for(uint8_t i = 0; i < dots && (base_len + i + 1) < sizeof(scan_text); i++) {
            scan_text[base_len + i] = '.';
            scan_text[base_len + i + 1] = '\0';
        }
        canvas_draw_str(canvas, 12, 78, scan_text);
    } else {
        // Show instruction when not scanning
        canvas_draw_str_aligned(canvas, 64, 58, AlignCenter, AlignCenter, "Press OK to start");
        canvas_draw_str_aligned(canvas, 64, 68, AlignCenter, AlignCenter, "wardriving");
    }
    
    // Draw controls
    elements_button_left(canvas, "Back");
    elements_button_center(canvas, state->scanning ? "Stop" : "Start");
}

static bool wardriving_view_input_callback(InputEvent* event, void* context) {
    PredatorApp* app = context;
    bool consumed = false;
    
    // Get view state using helper macro
    WardrivingView* state = PREDATOR_GET_MODEL(app->view_dispatcher, WardrivingView);
    if(!state) return consumed;
    
    if(event->type == InputTypeShort || event->type == InputTypeRepeat) {
        switch(event->key) {
        case InputKeyOk:
            // Toggle scanning
            if(state->scanning) {
                // Stop scanning
                predator_esp32_stop_attack(app);
                app->attack_running = false;
                state->scanning = false;
            } else {
                // Start scanning
                predator_esp32_start_wardriving(app);
                app->attack_running = true;
                state->scanning = true;
                
                // Reset counters if starting from zero
                if(app->targets_found == 0) {
                    state->networks_found = 0;
                    state->gps_points = 0;
                }
            }
            consumed = true;
            break;
            
        case InputKeyBack:
        case InputKeyLeft:
            // Stop scanning if running and exit
            if(state->scanning) {
                predator_esp32_stop_attack(app);
                app->attack_running = false;
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

static View* wardriving_view_alloc(PredatorApp* app) {
    View* view = view_alloc();
    view_set_context(view, app);
    view_set_draw_callback(view, wardriving_view_draw_callback);
    view_set_input_callback(view, wardriving_view_input_callback);
    
    // Initialize model
    WardrivingView* state = malloc(sizeof(WardrivingView));
    state->networks_found = app->targets_found;
    state->gps_points = 0;
    state->scanning = false;
    state->gps_active = false;
    state->animation_frame = 0;
    state->last_latitude = 0.0f;
    state->last_longitude = 0.0f;
    state->strongest_rssi = -100;
    memset(state->strongest_network, 0, sizeof(state->strongest_network));
    
    predator_view_set_model(view, state);
    predator_view_set_model_free_callback(view, free);
    
    return view;
}


void predator_scene_wardriving_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    // Initialize ESP32 for WiFi
    predator_esp32_init(app);
    
    // Initialize GPS
    predator_gps_init(app);
    
    // Create custom view
    View* view = wardriving_view_alloc(app);
    
    // Switch to a safe view before replacing to avoid dispatcher crash
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewLoading);
    // Replace popup view with custom view
    view_dispatcher_remove_view(app->view_dispatcher, PredatorViewPopup);
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewPopup, view);
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
}

bool predator_scene_wardriving_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(event.type == SceneManagerEventTypeBack) {
        // Stop scanning if running
        if(app->attack_running) {
            predator_esp32_stop_attack(app);
            app->attack_running = false;
        }
        consumed = false; // Allow default back behavior
    } else if(event.type == SceneManagerEventTypeTick) {
        // Update GPS data
        predator_gps_update(app);
        
        if(app->attack_running) {
            // Simulate finding WiFi networks (every ~10 ticks)
            if(app->packets_sent % 10 == 0) {
                app->targets_found++;
            }
            
            // Increment packet counter
            app->packets_sent++;
            
            // Force view refresh for animations
            view_dispatcher_send_custom_event(app->view_dispatcher, 0xFF);
            consumed = true;
        }
    }
    
    return consumed;
}

void predator_scene_wardriving_new_on_exit(void* context) {
    PredatorApp* app = context;
    
    // Stop scanning if running
    if(app->attack_running) {
        predator_esp32_stop_attack(app);
        app->attack_running = false;
    }
    
    // Clean up hardware
    predator_esp32_deinit(app);
    predator_gps_deinit(app);
    
    // Switch to a safe view before removing to avoid dispatcher crash
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewLoading);
    // Remove custom view and restore default popup view
    view_dispatcher_remove_view(app->view_dispatcher, PredatorViewPopup);
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewPopup, popup_get_view(app->popup));
}


