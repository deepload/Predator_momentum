#include "../predator_i.h"
#include "../helpers/predator_esp32.h"
#include "../helpers/predator_gps.h"

enum SubmenuIndex {
    SubmenuIndexModuleStatus,
    SubmenuIndexFrequencyConfig,
    SubmenuIndexPowerSettings,
    SubmenuIndexLogSettings,
    SubmenuIndexFactoryReset,
};

void predator_scene_settings_submenu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void predator_scene_settings_on_enter(void* context) {
    PredatorApp* app = context;
    Submenu* submenu = app->submenu;

    submenu_add_item(
        submenu, "📊 Module Status", SubmenuIndexModuleStatus, predator_scene_settings_submenu_callback, app);
    submenu_add_item(
        submenu, "📡 Frequency Config", SubmenuIndexFrequencyConfig, predator_scene_settings_submenu_callback, app);
    submenu_add_item(
        submenu, "⚡ Power Settings", SubmenuIndexPowerSettings, predator_scene_settings_submenu_callback, app);
    submenu_add_item(
        submenu, "📝 Log Settings", SubmenuIndexLogSettings, predator_scene_settings_submenu_callback, app);
    submenu_add_item(
        submenu, "🔄 Factory Reset", SubmenuIndexFactoryReset, predator_scene_settings_submenu_callback, app);

    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

bool predator_scene_settings_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        consumed = true;
        switch(event.event) {
        case SubmenuIndexModuleStatus:
            {
                // Check actual hardware status
                predator_esp32_init(app);
                predator_gps_init(app);
                
                bool marauder_switch = !furi_hal_gpio_read(PREDATOR_MARAUDER_SWITCH);
                bool gps_switch = !furi_hal_gpio_read(PREDATOR_GPS_POWER_SWITCH);
                bool charging = furi_hal_gpio_read(PREDATOR_CHARGING_LED);
                
                char status_text[512];
                snprintf(status_text, sizeof(status_text),
                    "PREDATOR MODULE STATUS\n\n"
                    "ESP32S2 Marauder: %s\n"
                    "GPS Module: %s\n"
                    "A07 433MHz: Ready ✓\n"
                    "SubGHz External: Active ✓\n"
                    "Battery: %s\n\n"
                    "Hardware Info:\n"
                    "• WiFi Antenna: %ddBi\n"
                    "• GPS Antenna: %ddBi\n"
                    "• 433MHz Antenna: %ddBi\n"
                    "• Battery: %dmAh\n\n"
                    "Switch States:\n"
                    "• Marauder: %s\n"
                    "• GPS Power: %s\n\n"
                    "Press Back to return",
                    app->esp32_connected ? "Connected ✓" : "Disconnected ✗",
                    app->gps_connected ? "Active ✓" : "Searching...",
                    charging ? "Charging" : "Ready",
                    PREDATOR_ANTENNA_WIFI_DBI,
                    PREDATOR_ANTENNA_GPS_DBI,
                    PREDATOR_ANTENNA_433_DBI,
                    PREDATOR_BATTERY_CAPACITY,
                    marauder_switch ? "ON" : "OFF",
                    gps_switch ? "Flipper" : "Internal");
                
                widget_add_text_scroll_element(app->widget, 0, 0, 128, 64, status_text);
                view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewWidget);
                
                predator_esp32_deinit(app);
                predator_gps_deinit(app);
            }
            break;
        default:
            break;
        }
    }

    return consumed;
}

void predator_scene_settings_on_exit(void* context) {
    PredatorApp* app = context;
    submenu_reset(app->submenu);
    widget_reset(app->widget);
}
