#include "../predator_i.h"
#include "../helpers/predator_view_helpers.h"
#include "../helpers/predator_esp32.h"
#include "../helpers/predator_ui_elements.h"
#include "../helpers/predator_ui_status.h"
#include "../helpers/predator_logging.h"

// Simple in-memory list of discovered networks (mirrors app buffers for labels)
#define WIFI_SCAN_MAX_RESULTS 16
static char g_wifi_results[WIFI_SCAN_MAX_RESULTS][24];
static size_t g_wifi_count = 0;
static bool g_in_results = false;
static uint32_t g_enter_tick = 0;

static void wifi_scan_results_reset(void) {
    g_wifi_count = 0; g_in_results = false;
    for(size_t i = 0; i < WIFI_SCAN_MAX_RESULTS; i++) g_wifi_results[i][0] = '\0';
}

static void wifi_scan_results_add(const char* label) {
    if(g_wifi_count < WIFI_SCAN_MAX_RESULTS && label) {
        snprintf(g_wifi_results[g_wifi_count], sizeof(g_wifi_results[g_wifi_count]), "%.23s", label);
        g_wifi_count++;
    }
}



static void wifi_scan_results_cb(void* context, uint32_t index) {
    PredatorApp* app = context;
    if(app && app->view_dispatcher) {
        view_dispatcher_send_custom_event(app->view_dispatcher, index);
    }
}

static void wifi_scan_select_target(PredatorApp* app, size_t index) {
    if(!app || index >= g_wifi_count || index >= PREDATOR_WIFI_MAX_APS) return;
    
    // Store selected target
    strncpy(app->selected_wifi_ssid, g_wifi_results[index], sizeof(app->selected_wifi_ssid) - 1);
    app->selected_wifi_ssid[sizeof(app->selected_wifi_ssid) - 1] = '\0';
    app->selected_wifi_rssi = app->wifi_rssi[index];
    app->selected_wifi_ch = app->wifi_ch[index];
    app->wifi_target_selected = true;
    
    char logline[64];
    snprintf(logline, sizeof(logline), "Target: %s (ch%u, %ddBm)", 
        app->selected_wifi_ssid, (unsigned)app->selected_wifi_ch, (int)app->selected_wifi_rssi);
    predator_log_append(app, logline);
    FURI_LOG_I("WiFiScan", "Selected target: %s", app->selected_wifi_ssid);
}

static void wifi_scan_build_results_menu(PredatorApp* app) {
    if(!app || !app->submenu) return;
    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "Scan Results");
    // Optional refresh control
    submenu_add_item(app->submenu, "↻ Refresh Results", 910, wifi_scan_results_cb, app);
    for(size_t i = 0; i < g_wifi_count; i++) {
        // Inline RSSI/CH details if available from app arrays
        char label[40];
        int8_t rssi = 0; uint8_t ch = 0;
        if(i < PREDATOR_WIFI_MAX_APS) {
            rssi = app->wifi_rssi[i];
            ch = app->wifi_ch[i];
        }
        if(ch)
            snprintf(label, sizeof(label), "%.20s c%u %ddBm", g_wifi_results[i], (unsigned)ch, (int)rssi);
        else
            snprintf(label, sizeof(label), "%.22s %ddBm", g_wifi_results[i], (int)rssi);
        submenu_add_item(app->submenu, label, (uint32_t)(1000 + i), wifi_scan_results_cb, app);
    }
    submenu_add_item(app->submenu, "< Back to Scan", 900, wifi_scan_results_cb, app);
}

void predator_scene_wifi_scan_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    if(!app) {
        FURI_LOG_E("WiFiScan", "App context is NULL on enter");
        return;
    }
    
    // Validate board type before any hardware initialization
    if(app->board_type == 0) {
        FURI_LOG_W("WiFiScan", "Board type is Unknown, defaulting to Original");
        app->board_type = 0; // Keep as Original
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
    
    // Ensure popup is valid
    if(!app->popup) {
        FURI_LOG_E("WiFiScan", "Popup is NULL, cannot initialize UI");
        return;
    }
    
    // Pre-render popup content to avoid blank screen
    popup_reset(app->popup);
    popup_set_header(app->popup, "WiFi Scanner", 64, 10, AlignCenter, AlignTop);
    popup_set_text(app->popup, "Preparing WiFi scan...\nBack=Stop | See Live Monitor", 64, 28, AlignCenter, AlignTop);
    popup_set_context(app->popup, app);
    // Keep popup persistent; spinner/progress will be driven by scene ticks

    // Switch to popup view first
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
    // Record entry time for back-debounce
    g_enter_tick = furi_get_tick();
    // Show immediate scanning status so operators see feedback instantly
    popup_set_text(app->popup, "Scanning |\nTransport: Initializing...\nNo results yet", 64, 28, AlignCenter, AlignTop);

    // Initialize ESP32 and start scan (live path if available)
    predator_esp32_init(app);
    bool started = predator_esp32_wifi_scan(app);
    if(started) {
        char status[96]; predator_ui_build_status(app, "Mode: WiFi Scan", status, sizeof(status));
        // Append footer guidance
        size_t len = strlen(status);
        if(len < sizeof(status)) {
            snprintf(status + len, sizeof(status) - len, "\nBack=Stop | See Live Monitor");
        }
        popup_set_text(app->popup, status, 64, 28, AlignCenter, AlignTop);
        predator_log_append(app, "WiFiScan START");
        FURI_LOG_I("WiFiScan", "Live WiFi scan started");
    } else {
        popup_set_text(app->popup, "ESP32 not ready — Demo Mode\nBack=Stop | See Live Monitor", 64, 28, AlignCenter, AlignTop);
        FURI_LOG_W("WiFiScan", "ESP32 start failed; falling back to Demo text");
    }

    // Start scanning state/progress
    app->attack_running = true;
    app->targets_found = 0;
    app->packets_sent = 0;
    // Reset app-level SSID buffer so UI shows fresh results
    app->wifi_ap_count = 0;
    for(size_t i=0;i<PREDATOR_WIFI_MAX_APS;i++) app->wifi_ssids[i][0] = '\0';
    // Note: no demo fallback entries; UI will show guidance until data arrives
    // Fire an immediate UI update so operators see movement instantly
    if(app->view_dispatcher) {
        view_dispatcher_send_custom_event(app->view_dispatcher, 950);
    }
    FURI_LOG_I("WiFiScan", "WiFi Scan scene entered");
    wifi_scan_results_reset();
}

bool predator_scene_wifi_scan_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(!app) {
        FURI_LOG_E("WiFiScan", "App context is NULL in event handler");
        return false;
    }
    
    if(event.type == SceneManagerEventTypeBack) {
        // Back-debounce: ignore Back for first 500ms to prevent instant exit
        uint32_t elapsed = furi_get_tick() - g_enter_tick;
        if(elapsed < 500) {
            FURI_LOG_I("WiFiScan", "Back debounced (elapsed=%lu ms)", elapsed);
            return true;
        }
        if(g_in_results) {
            // Return to scanning popup instead of exiting scene
            g_in_results = false;
            if(app->popup) {
                popup_set_text(app->popup, "Resuming WiFi scan...\nBack=Stop | See Live Monitor", 64, 28, AlignCenter, AlignTop);
            }
            view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
            return true;
        }
        // Stop scanning and return to previous scene
        app->attack_running = false;
        FURI_LOG_I("WiFiScan", "Back event received, stopping scan and navigating back");
        predator_esp32_stop_attack(app);
        predator_log_append(app, "WiFiScan STOP");
        scene_manager_previous_scene(app->scene_manager);
        consumed = true;
    } else if(event.type == SceneManagerEventTypeCustom && event.event == PredatorCustomEventPopupBack) {
        // Popup back button path
        app->attack_running = false;
        FURI_LOG_I("WiFiScan", "Popup Back pressed, stopping scan and navigating back");
        predator_esp32_stop_attack(app);
        scene_manager_previous_scene(app->scene_manager);
        consumed = true;
    } else if(event.type == SceneManagerEventTypeCustom) {
        // Handle results menu custom events
        if(event.event == 900) {
            // Back to scan view
            g_in_results = false;
            if(app->popup) popup_set_text(app->popup, "Resuming WiFi scan...\nBack=Stop | See Live Monitor", 64, 28, AlignCenter, AlignTop);
            view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
            consumed = true;
        } else if(event.event == 910) {
            // Refresh results: rebuild menu from current buffers
            wifi_scan_build_results_menu(app);
            view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
            g_in_results = true;
            consumed = true;
        } else if(event.event >= 1000 && event.event < 1000 + WIFI_SCAN_MAX_RESULTS) {
            // User selected an AP from results list
            size_t index = event.event - 1000;
            wifi_scan_select_target(app, index);
            // Show attack options submenu
            submenu_reset(app->submenu);
            submenu_set_header(app->submenu, "Attack Target");
            char target_label[48];
            snprintf(target_label, sizeof(target_label), "Target: %.20s", app->selected_wifi_ssid);
            submenu_add_item(app->submenu, target_label, 2000, wifi_scan_results_cb, app);
            submenu_add_item(app->submenu, "Deauth Attack", 2001, wifi_scan_results_cb, app);
            submenu_add_item(app->submenu, "Evil Twin", 2002, wifi_scan_results_cb, app);
            submenu_add_item(app->submenu, "Capture Handshake", 2003, wifi_scan_results_cb, app);
            submenu_add_item(app->submenu, "< Back to Results", 900, wifi_scan_results_cb, app);
            view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
            consumed = true;
        } else if(event.event == 2001) {
            // Launch Deauth on selected target
            scene_manager_next_scene(app->scene_manager, PredatorSceneWifiDeauth);
            consumed = true;
        } else if(event.event == 2002) {
            // Launch Evil Twin on selected target
            scene_manager_next_scene(app->scene_manager, PredatorSceneWifiEvilTwin);
            consumed = true;
        } else if(event.event == 2003) {
            // Launch Handshake capture on selected target
            scene_manager_next_scene(app->scene_manager, PredatorSceneWifiHandshake);
            consumed = true;
        }
    } else if(event.type == SceneManagerEventTypeTick) {
        if(app->attack_running) {
            // Mirror any new SSIDs parsed by ESP32 RX callback into local results list
            if(app->wifi_ap_count > g_wifi_count) {
                for(size_t i = g_wifi_count; i < app->wifi_ap_count && i < WIFI_SCAN_MAX_RESULTS; i++) {
                    wifi_scan_results_add(app->wifi_ssids[i]);
                }
                app->targets_found = app->wifi_ap_count;
            }
            // Update popup text to show progress with spinner every tick
            if(app->popup) {
                static const char spin_chars[] = "|/-\\";
                char spinner = spin_chars[(app->packets_sent) % 4];
                char progress_text[96];
                const char* transport = app->esp32_uart ? "UART OK" : "Fallback";
                if(app->wifi_ap_count == 0) {
                    snprintf(progress_text, sizeof(progress_text), "Scanning %c | %s\nNo results yet\nBack=Stop", spinner, transport);
                } else {
                    snprintf(progress_text, sizeof(progress_text), "Scanning %c | %s\nNetworks: %lu\nBack=Stop", spinner, transport, (unsigned long)app->wifi_ap_count);
                }
                popup_set_text(app->popup, progress_text, 64, 28, AlignCenter, AlignTop);
            }
            // Increment packet counter
            app->packets_sent++;
            // Keep popup persistent (no auto-switch). Operator can use Refresh in Results when manually opened.
            // Log a lightweight heartbeat occasionally to Live Monitor to verify UI updates
            if((app->packets_sent % 20) == 0) {
                predator_log_append(app, "WiFiScan UI tick");
            }
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
    FURI_LOG_I("WiFiScan", "Exited WiFi Scan scene");
}
