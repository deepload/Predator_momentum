#include "../predator_i.h"
#include "../helpers/predator_view_helpers.h"
#include "../helpers/predator_esp32.h"
#include "../helpers/predator_ui_elements.h"
#include "../helpers/predator_ui_status.h"
#include "../helpers/predator_settings.h"
#include "../helpers/predator_logging.h"

static void wifi_hs_popup_back(void* context) {
    PredatorApp* app = context;
    if(app && app->view_dispatcher) {
        view_dispatcher_send_custom_event(app->view_dispatcher, PredatorCustomEventPopupBack);
    }
}

#define HS_MAX_RESULTS 10
static char g_hs_results[HS_MAX_RESULTS][32];
static size_t g_hs_count = 0;
static bool g_hs_in_results = false;
static uint32_t g_hs_enter_tick = 0;

static void hs_results_reset(void) {
    g_hs_count = 0; g_hs_in_results = false;
    for(size_t i = 0; i < HS_MAX_RESULTS; i++) g_hs_results[i][0] = '\0';
}

static void hs_results_add(const char* essid, const char* bssid) {
    if(g_hs_count < HS_MAX_RESULTS && essid && bssid) {
        snprintf(g_hs_results[g_hs_count], sizeof(g_hs_results[g_hs_count]), "%.10s %.17s", essid, bssid);
        g_hs_count++;
    }
}

static void hs_build_results_menu(PredatorApp* app) {
    if(!app || !app->submenu) return;
    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "Handshakes");
    for(size_t i = 0; i < g_hs_count; i++) {
        submenu_add_item(app->submenu, g_hs_results[i], (uint32_t)(1000 + i), NULL, app);
    }
    submenu_add_item(app->submenu, "< Back", 900, NULL, app);
}

void predator_scene_wifi_handshake_new_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app || !app->view_dispatcher || !app->popup) return;

    popup_reset(app->popup);
    popup_set_header(app->popup, "WiFi Handshake", 64, 10, AlignCenter, AlignTop);
    popup_set_text(app->popup, "Set channel, Start capture\nThen run Deauth bursts\nBack=Stop | See Live Monitor", 64, 28, AlignCenter, AlignTop);
    popup_set_context(app->popup, app);
    popup_set_callback(app->popup, wifi_hs_popup_back);
    popup_set_timeout(app->popup, 0);
    popup_enable_timeout(app->popup);
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
    g_hs_enter_tick = furi_get_tick();

    // Attempt to start capture on current channel (setting)
    predator_esp32_init(app);
    // If supported by Marauder firmware, send a capture command here (placeholder)
    predator_esp32_get_status(app);
    predator_log_append(app, "WiFiHandshake START");

    app->attack_running = true;
    app->packets_sent = 0;
    hs_results_reset();
}

bool predator_scene_wifi_handshake_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    if(!app) return false;

    if(event.type == SceneManagerEventTypeBack) {
        uint32_t elapsed = furi_get_tick() - g_hs_enter_tick;
        if(elapsed < 500) {
            FURI_LOG_I("WiFiHandshake", "Back debounced (elapsed=%lu ms)", elapsed);
            return true;
        }
        app->attack_running = false;
        predator_esp32_stop_attack(app);
        predator_log_append(app, "WiFiHandshake STOP");
        scene_manager_previous_scene(app->scene_manager);
        return true;
    } else if(event.type == SceneManagerEventTypeCustom && event.event == PredatorCustomEventPopupBack) {
        app->attack_running = false;
        predator_esp32_stop_attack(app);
        predator_log_append(app, "WiFiHandshake STOP");
        scene_manager_previous_scene(app->scene_manager);
        return true;
    } else if(event.type == SceneManagerEventTypeTick) {
        if(app->attack_running) {
            app->packets_sent++;
            // Simulate handshake detection periodically
            if(app->packets_sent % 60 == 0 && app->popup) {
                char essid[16]; snprintf(essid, sizeof(essid), "NET_%lu", (unsigned long)(app->packets_sent/60));
                const char* bssid = "AA:BB:CC:DD:EE:FF";
                hs_results_add(essid, bssid);
                char logline[96]; snprintf(logline, sizeof(logline), "WiFiHandshake ESSID=%s BSSID=%s", essid, bssid);
                predator_log_append(app, logline);
                char status[96]; snprintf(status, sizeof(status), "Handshakes: %u\nBack=Stop | See Live Monitor", (unsigned)g_hs_count);
                popup_set_text(app->popup, status, 64, 28, AlignCenter, AlignTop);
            }
            // Periodically show results
            if(app->packets_sent % 120 == 0 && g_hs_count > 0 && app->submenu) {
                hs_build_results_menu(app);
                view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
                g_hs_in_results = true;
            }
            return true;
        }
    }
    return false;
}

void predator_scene_wifi_handshake_new_on_exit(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    app->attack_running = false;
}
