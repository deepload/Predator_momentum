#include "../predator_i.h"
#include "../helpers/predator_view_helpers.h"
#include "../helpers/predator_esp32.h"
#include "../helpers/predator_ui_elements.h"
#include "../helpers/predator_compliance.h"
#include "../helpers/predator_settings.h"
#include "../helpers/predator_logging.h"

static void wifi_deauth_popup_back(void* context) {
    PredatorApp* app = context;
    if(app && app->view_dispatcher) {
        view_dispatcher_send_custom_event(app->view_dispatcher, PredatorCustomEventPopupBack);
    }
}

// File-scope burst state to persist across ticks within a session
static uint32_t s_burst_counter = 0;
static bool s_burst_on = true;
static bool s_live_started = false;

typedef struct {
    View* view;
    uint32_t packets_sent;
    uint8_t animation_frame;
    uint8_t target_channel;
    bool hardware_ready;
    uint8_t deauth_strength;
} WifiDeauthView;

/*
static void wifi_deauth_view_draw_callback(Canvas* canvas, void* context) {
    PredatorApp* app = context;
    
    if(!app) return;
    
    // Get view state
    WifiDeauthView* state = PREDATOR_GET_MODEL(app->view_dispatcher, WifiDeauthView);
    if(!state) return;
    
    // Update animation frame
    uint8_t animation_frame = (furi_get_tick() / 200) % 4;
    state->animation_frame = animation_frame;
    state->packets_sent = app->packets_sent;
    state->hardware_ready = (app->esp32_uart != NULL);
    
    // Every 500 packets, increase deauth strength for visual effect
    if(app->packets_sent > 0 && app->packets_sent % 500 == 0) {
        if(state->deauth_strength < 3) {
            state->deauth_strength++;
        }
    }
    
    canvas_clear(canvas);
    
    // Draw title
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, 64, 8, AlignCenter, AlignCenter, "WiFi Deauth Attack");
    
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
            // For multiboards, show different message
            canvas_draw_str_aligned(canvas, 64, 35, AlignCenter, AlignCenter, "ESP32 not connected");
            canvas_draw_str_aligned(canvas, 64, 45, AlignCenter, AlignCenter, "Check connections");
        }
    } else {
        // Draw router with deauth animation
        uint8_t router_x = 30;
        uint8_t router_y = 35;
        
        // Router base
        canvas_draw_box(canvas, router_x - 10, router_y - 5, 20, 10);
        
        // Router antennas
        canvas_draw_line(canvas, router_x - 5, router_y - 5, router_x - 5, router_y - 10);
        canvas_draw_line(canvas, router_x + 5, router_y - 5, router_x + 5, router_y - 10);
        
        // Router activity lights
        uint8_t light_size = 2;
        canvas_draw_box(canvas, router_x - 8, router_y, light_size, light_size);
        canvas_draw_box(canvas, router_x - 4, router_y, light_size, light_size);
        canvas_draw_box(canvas, router_x, router_y, light_size, light_size);
        canvas_draw_box(canvas, router_x + 4, router_y, light_size, light_size);
        
        // Attack visualization - X marks over signal waves
        for(uint8_t i = 0; i <= state->deauth_strength; i++) {
            uint8_t wave_x = router_x + 20 + (i * 8);
            uint8_t wave_y = router_y;
            
            // Deauth X pattern
            canvas_draw_line(canvas, wave_x - 3, wave_y - 3, wave_x + 3, wave_y + 3);
            canvas_draw_line(canvas, wave_x - 3, wave_y + 3, wave_x + 3, wave_y - 3);
        }
        
        // Client device getting disconnected
        uint8_t device_x = 85;
        uint8_t device_y = 35;
        canvas_draw_box(canvas, device_x - 5, device_y - 8, 10, 15);
        
        // Show disconnection based on animation frame
        if(animation_frame % 2 == 0) {
            canvas_draw_line(canvas, device_x, device_y - 15, device_x, device_y - 10);
        } else {
            // Draw disconnected WiFi icon
            canvas_draw_line(canvas, device_x, device_y - 15, device_x - 3, device_y - 12);
            canvas_draw_line(canvas, device_x, device_y - 15, device_x + 3, device_y - 12);
            canvas_draw_line(canvas, device_x, device_y - 15, device_x, device_y - 12);
        }
        
        // Status information
        predator_ui_draw_status_box(canvas, "Attack Status", 10, 50, 108, 28);
        
        canvas_set_font(canvas, FontSecondary);
        
        // Channel info
        char channel_text[24];
        snprintf(channel_text, sizeof(channel_text), "Channel: All");
        canvas_draw_str(canvas, 16, 60, channel_text);
        
        // Packets counter
        char packet_text[24];
        snprintf(packet_text, sizeof(packet_text), "Packets: %lu", state->packets_sent);
        canvas_draw_str(canvas, 16, 70, packet_text);
        
        // Success indicator - only show when we've sent enough packets
        if(state->packets_sent > 100) {
            canvas_draw_str(canvas, 82, 60, "Status:");
            
            if(animation_frame % 2 == 0) {
                canvas_draw_str(canvas, 82, 70, "ACTIVE");
            } else {
                canvas_draw_str(canvas, 82, 70, "JAMMING");
            }
        }
    }
    
    // Draw control hints
    canvas_set_font(canvas, FontSecondary);
    elements_button_left(canvas, "Back");
}
*/

/*
static bool wifi_deauth_view_input_callback(InputEvent* event, void* context) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(event->type == InputTypeShort || event->type == InputTypeRepeat) {
        switch(event->key) {
        case InputKeyBack:
        case InputKeyLeft:
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
static View* wifi_deauth_view_alloc(PredatorApp* app) {
    View* view = view_alloc();
    view_set_context(view, app);
    view_set_draw_callback(view, wifi_deauth_view_draw_callback);
    view_set_input_callback(view, wifi_deauth_view_input_callback);
    
    // Initialize model
    WifiDeauthView* state = malloc(sizeof(WifiDeauthView));
    state->packets_sent = 0;
    state->animation_frame = 0;
    state->target_channel = 1;
    state->hardware_ready = false;
    state->deauth_strength = 0;
    
    predator_view_set_model(view, state);
    predator_view_set_model_free_callback(view, free);
    
    return view;
}
*/

void predator_scene_wifi_deauth_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    if(!app) {
        FURI_LOG_E("WiFiDeauth", "App context is NULL on enter");
        return;
    }
    
    // Validate board type before any hardware initialization
    if(app->board_type == 0) {
        FURI_LOG_W("WiFiDeauth", "Board type is Unknown, defaulting to Original");
        app->board_type = 0; // Keep as Original
    }
    
    // Ensure scene_manager and view_dispatcher are valid to prevent crashes
    if(!app->scene_manager) {
        FURI_LOG_E("WiFiDeauth", "Scene manager is NULL, cannot proceed");
        return;
    }
    
    if(!app->view_dispatcher) {
        FURI_LOG_E("WiFiDeauth", "View dispatcher is NULL, cannot switch view");
        return;
    }
    
    // Ensure popup is valid
    if(!app->popup) {
        FURI_LOG_E("WiFiDeauth", "Popup is NULL, cannot initialize UI");
        return;
    }
    
    // Configure popup content to avoid blank screen; render and switch view first
    popup_reset(app->popup);
    popup_set_header(app->popup, "WiFi Deauth", 64, 10, AlignCenter, AlignTop);
    popup_set_text(app->popup, "Starting Deauth...\nBack=Stop | See Live Monitor", 64, 28, AlignCenter, AlignTop);
    popup_set_context(app->popup, app);
    popup_set_callback(app->popup, wifi_deauth_popup_back);
    popup_set_timeout(app->popup, 0);
    popup_enable_timeout(app->popup);
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);

    bool live_allowed = predator_compliance_is_feature_allowed(app, PredatorFeatureWifiDeauth, app->authorized);
    s_live_started = false;
    if(!live_allowed) {
        popup_set_text(app->popup, "Demo Mode — Authorization required\nPress Back to return", 64, 28, AlignCenter, AlignTop);
    } else {
        // Live path: initialize ESP32 and start deauth
        // Read preferred channel from settings (0 = All, else 1..13)
        int32_t channel = 6;
        predator_settings_get_int(app, "WIFI_CHANNEL", 6, &channel);
        if(channel < 0 || channel > 13) channel = 6;

        predator_esp32_init(app);
        bool started = predator_esp32_wifi_deauth(app, (uint8_t)channel);
        if(!started) {
            // Retry once after short delay with re-init
            FURI_LOG_W("WiFiDeauth", "First start failed, retrying once after reinit");
            predator_esp32_stop_attack(app);
            predator_esp32_init(app);
            started = predator_esp32_wifi_deauth(app, (uint8_t)channel);
        }
        if(started) {
            char msg[128];
            snprintf(
                msg,
                sizeof(msg),
                "Deauth running (ch %ld)\nSteps: Start EvilTwin, then short bursts here\nBack=Stop | See Live Monitor",
                (long)channel);
            popup_set_text(app->popup, msg, 64, 28, AlignCenter, AlignTop);
            char logline[80]; snprintf(logline, sizeof(logline), "WiFiDeauth START ch=%ld", (long)channel);
            predator_log_append(app, logline);
            FURI_LOG_I("WiFiDeauth", "Live deauth started (channel %ld)", (long)channel);
            s_live_started = true;
        } else {
            popup_set_text(
                app->popup,
                "ESP32 not ready — Demo Mode\nTip: Set WIFI_CHANNEL to target AP\nBack=Return | See Live Monitor",
                64,
                28,
                AlignCenter,
                AlignTop);
            FURI_LOG_W("WiFiDeauth", "ESP32 start failed; falling back to Demo text");
        }
    }

    // Start simulated WiFi deauth attack state/progress
    app->attack_running = true;
    app->packets_sent = 0;
    // Reset burst state for this session
    s_burst_counter = 0;
    s_burst_on = true;
    FURI_LOG_I("WiFiDeauth", "WiFi Deauth scene entered");
}

bool predator_scene_wifi_deauth_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(!app) {
        FURI_LOG_E("WiFiDeauth", "App context is NULL in event handler");
        return false;
    }
    
    if(event.type == SceneManagerEventTypeBack) {
        // Stop attack and return to previous scene
        app->attack_running = false;
        // If authorized/live, send stop to ESP32
        if(predator_compliance_is_feature_allowed(app, PredatorFeatureWifiDeauth, app->authorized)) {
            predator_esp32_stop_attack(app);
        }
        predator_log_append(app, "WiFiDeauth STOP");
        FURI_LOG_I("WiFiDeauth", "Back event received, stopping attack and navigating back");
        scene_manager_previous_scene(app->scene_manager);
        consumed = true;
    } else if(event.type == SceneManagerEventTypeCustom && event.event == PredatorCustomEventPopupBack) {
        // Popup back path
        app->attack_running = false;
        if(predator_compliance_is_feature_allowed(app, PredatorFeatureWifiDeauth, app->authorized)) {
            predator_esp32_stop_attack(app);
        }
        predator_log_append(app, "WiFiDeauth STOP");
        scene_manager_previous_scene(app->scene_manager);
        consumed = true;
    } else if(event.type == SceneManagerEventTypeTick) {
        // Deauth burst mode: 15s ON / 45s OFF cycle (tick duration is SDK-defined; use counters proportionally)
        static uint32_t burst_counter = 0;
        static bool burst_on = true;
        burst_counter++;

        bool live_allowed = predator_compliance_is_feature_allowed(app, PredatorFeatureWifiDeauth, app->authorized);
        if(live_allowed) {
            // Rough timing using counters; adjust thresholds as needed per tick rate
            uint32_t on_ticks = 150;   // approx 15s
            uint32_t off_ticks = 450;  // approx 45s
            if(burst_on && burst_counter >= on_ticks) {
                predator_esp32_stop_attack(app);
                predator_log_append(app, "WiFiDeauth BURST_OFF");
                burst_on = false; burst_counter = 0;
                if(app->popup) {
                    char msg[96]; snprintf(msg, sizeof(msg), "Burst OFF — cooling\nBack=Stop | See Live Monitor");
                    popup_set_text(app->popup, msg, 64, 28, AlignCenter, AlignTop);
                }
            } else if(!burst_on && burst_counter >= off_ticks) {
                // Resume attack on configured channel
                int32_t channel = 6; predator_settings_get_int(app, "WIFI_CHANNEL", 6, &channel);
                if(channel < 0 || channel > 13) channel = 6;
                predator_esp32_wifi_deauth(app, (uint8_t)channel);
                predator_log_append(app, "WiFiDeauth BURST_ON");
                burst_on = true; burst_counter = 0;
                if(app->popup) {
                    char msg[96]; snprintf(msg, sizeof(msg), "Burst ON (ch %ld)\nBack=Stop | See Live Monitor", (long)channel);
                    popup_set_text(app->popup, msg, 64, 28, AlignCenter, AlignTop);
                }
            }
        }

        if(app->attack_running) {
            app->packets_sent += 30; // Simulate sending deauth packets
            if(app->packets_sent % 150 == 0 && app->popup) {
                // Update popup text to show progress with footer
                char progress_text[96];
                snprintf(progress_text, sizeof(progress_text), "Deauth packets: %lu\nBack=Stop | See Live Monitor", app->packets_sent);
                popup_set_text(app->popup, progress_text, 64, 28, AlignCenter, AlignTop);
                FURI_LOG_I("WiFiDeauth", "Updated deauth packets sent: %lu", app->packets_sent);
            }
            consumed = true;
        }
    }
    
    return consumed;
}

void predator_scene_wifi_deauth_new_on_exit(void* context) {
    PredatorApp* app = context;
    
    if(!app) {
        FURI_LOG_E("WiFiDeauth", "App context is NULL on exit");
        return;
    }
    
    // Stop any running attack
    app->attack_running = false;
    if(predator_compliance_is_feature_allowed(app, PredatorFeatureWifiDeauth, app->authorized)) {
        predator_esp32_stop_attack(app);
    }
    FURI_LOG_I("WiFiDeauth", "Exited WiFi Deauth scene");
}
