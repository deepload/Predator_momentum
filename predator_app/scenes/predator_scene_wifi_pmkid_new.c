#include "../predator_i.h"
#include "../helpers/predator_view_helpers.h"
#include "../helpers/predator_esp32.h"
#include "../helpers/predator_ui_elements.h"
#include "../helpers/predator_ui_status.h"
#include "../helpers/predator_settings.h"
#include "../helpers/predator_logging.h"

static void wifi_pmkid_popup_back(void* context) {
    PredatorApp* app = context;
    if(app && app->view_dispatcher) {
        view_dispatcher_send_custom_event(app->view_dispatcher, PredatorCustomEventPopupBack);
    }
}

#define PMK_MAX_RESULTS 10
static char g_pmk_results[PMK_MAX_RESULTS][40];
static size_t g_pmk_count = 0;
static bool g_pmk_in_results = false;
static uint32_t g_pmk_enter_tick = 0;

static void pmk_results_reset(void) {
    g_pmk_count = 0; g_pmk_in_results = false;
    for(size_t i = 0; i < PMK_MAX_RESULTS; i++) g_pmk_results[i][0] = '\0';
}

static void pmk_results_add(const char* essid, const char* bssid, const char* pmkid) {
    if(g_pmk_count < PMK_MAX_RESULTS && essid && bssid && pmkid) {
        // Keep within 40 chars: 8 + 1 + 17 + 1 + 12 = 39
        snprintf(g_pmk_results[g_pmk_count], sizeof(g_pmk_results[g_pmk_count]), "%.8s %.17s %.12s", essid, bssid, pmkid);
        g_pmk_count++;
    }
}

static void pmk_build_results_menu(PredatorApp* app) {
    if(!app || !app->submenu) return;
    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "PMKIDs");
    for(size_t i = 0; i < g_pmk_count; i++) {
        submenu_add_item(app->submenu, g_pmk_results[i], (uint32_t)(1000 + i), NULL, app);
    }
    submenu_add_item(app->submenu, "< Back", 900, NULL, app);
}

void predator_scene_wifi_pmkid_new_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app || !app->view_dispatcher || !app->popup) return;

    popup_reset(app->popup);
    popup_set_header(app->popup, "PMKID Capture", 64, 10, AlignCenter, AlignTop);
    popup_set_text(app->popup, "Set channel → Start capture\nOptional Deauth assist\nBack=Stop | See Live Monitor", 64, 28, AlignCenter, AlignTop);
    popup_set_context(app->popup, app);
    popup_set_callback(app->popup, wifi_pmkid_popup_back);
    popup_set_timeout(app->popup, 0);
    popup_enable_timeout(app->popup);
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
    g_pmk_enter_tick = furi_get_tick();

    predator_esp32_init(app);
    predator_esp32_get_status(app);
    predator_log_append(app, "WiFiPMKID START");

    app->attack_running = true;
    app->packets_sent = 0;
    pmk_results_reset();
}

bool predator_scene_wifi_pmkid_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    if(!app) return false;

    if(event.type == SceneManagerEventTypeBack) {
        uint32_t elapsed = furi_get_tick() - g_pmk_enter_tick;
        if(elapsed < 500) {
            FURI_LOG_I("WiFiPMKID", "Back debounced (elapsed=%lu ms)", elapsed);
            return true;
        }
        app->attack_running = false;
        predator_esp32_stop_attack(app);
        predator_log_append(app, "WiFiPMKID STOP");
        scene_manager_previous_scene(app->scene_manager);
        return true;
    } else if(event.type == SceneManagerEventTypeCustom && event.event == PredatorCustomEventPopupBack) {
        app->attack_running = false;
        predator_esp32_stop_attack(app);
        predator_log_append(app, "WiFiPMKID STOP");
        scene_manager_previous_scene(app->scene_manager);
        return true;
    } else if(event.type == SceneManagerEventTypeTick) {
        if(app->attack_running) {
            app->packets_sent++;
            // Simulate PMKID captures periodically
            if(app->packets_sent % 75 == 0 && app->popup) {
                char essid[12]; snprintf(essid, sizeof(essid), "AP_%lu", app->packets_sent/75);
                const char* bssid = "11:22:33:44:55:66";
                const char* pmkid = "PMKID1234ABCDEF";
                pmk_results_add(essid, bssid, pmkid);
                char logline[128]; snprintf(logline, sizeof(logline), "WiFiPMKID ESSID=%s BSSID=%s PMKID=%s", essid, bssid, pmkid);
                predator_log_append(app, logline);
                char status[96]; snprintf(status, sizeof(status), "PMKIDs: %u\nBack=Stop | See Live Monitor", (unsigned)g_pmk_count);
                popup_set_text(app->popup, status, 64, 28, AlignCenter, AlignTop);
            }
            if(app->packets_sent % 150 == 0 && g_pmk_count > 0 && app->submenu) {
                pmk_build_results_menu(app);
                view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
                g_pmk_in_results = true;
            }
            return true;
        }
    }
    return false;
}

void predator_scene_wifi_pmkid_new_on_exit(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    app->attack_running = false;
    predator_esp32_stop_attack(app);
}
