#include "../predator_i.h"
#include "../helpers/predator_view_helpers.h"
#include "../helpers/predator_ui_elements.h"
#include "../helpers/predator_compliance.h"
#include "../helpers/predator_settings.h"
#include "../helpers/predator_logging.h"
#include <storage/storage.h>
#include <string.h>

// Submenu callback for Settings
static void settings_submenu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    if(!app || !app->view_dispatcher) return;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

typedef enum {
    SettingsRegionAuto = 100,
    SettingsRegionUS,
    SettingsRegionEU,
    SettingsRegionCH,
    SettingsRegionJP,
    SettingsRegionCN,
    SettingsRegionUnblock,
    SettingsDeauthBurstToggle = 200,
    SettingsDeauthBurstOn,
    SettingsDeauthBurstOff,
    SettingsEvilTwinChannelSource,
    SettingsClearLogs,
} SettingsItemId;

static void settings_persist_region(PredatorApp* app, PredatorRegion region) {
    if(!app) return;
    // Update in-memory region immediately
    predator_compliance_set_region(app, region);
    // Persist to /ext/predator_region.cfg
    Storage* storage = app->storage ? app->storage : furi_record_open(RECORD_STORAGE);
    bool close_storage = (storage != app->storage);
    File* file = storage_file_alloc(storage);
    if(storage_file_open(file, "/ext/predator_region.cfg", FSAM_WRITE, FSOM_CREATE_ALWAYS)) {
        const char* code = predator_compliance_region_str(region);
        size_t len = strlen(code);
        storage_file_write(file, code, len);
        storage_file_write(file, "\n", 1);
        storage_file_close(file);
        FURI_LOG_I("Settings", "Region persisted to %s", code);
    } else {
        FURI_LOG_W("Settings", "Failed to open predator_region.cfg for write");
    }
    storage_file_free(file);
    if(close_storage) furi_record_close(RECORD_STORAGE);
}

void predator_scene_settings_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    if(!app) {
        FURI_LOG_E("Settings", "App context is NULL on enter");
        return;
    }
    
    if(!app->scene_manager) {
        FURI_LOG_E("Settings", "Scene manager is NULL on enter");
        return;
    }
    
    if(!app->view_dispatcher) {
        FURI_LOG_E("Settings", "View dispatcher is NULL on enter");
        return;
    }
    
    // Validate board type before any hardware initialization
    if(app->board_type == PredatorBoardTypeUnknown) {
        FURI_LOG_W("Settings", "Board type is Unknown, defaulting to Original");
        app->board_type = PredatorBoardTypeOriginal;
    }
    
    // Build Settings submenu with Region selection (worldwide compliance)
    if(!app->submenu) {
        FURI_LOG_E("Settings", "Submenu is NULL on enter");
        return;
    }
    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "Settings");
    submenu_add_item(app->submenu, "Region: AUTO", SettingsRegionAuto, settings_submenu_callback, app);
    submenu_add_item(app->submenu, "Region: US", SettingsRegionUS, settings_submenu_callback, app);
    submenu_add_item(app->submenu, "Region: EU", SettingsRegionEU, settings_submenu_callback, app);
    submenu_add_item(app->submenu, "Region: CH", SettingsRegionCH, settings_submenu_callback, app);
    submenu_add_item(app->submenu, "Region: JP", SettingsRegionJP, settings_submenu_callback, app);
    submenu_add_item(app->submenu, "Region: CN", SettingsRegionCN, settings_submenu_callback, app);
    submenu_add_item(app->submenu, "Region: UNBLOCK (test)", SettingsRegionUnblock, settings_submenu_callback, app);

    // Operator toggles
    int32_t burst_enabled = 1; predator_settings_get_int(app, "DEAUTH_BURST_ENABLED", 1, &burst_enabled);
    int32_t burst_on_s = 15; predator_settings_get_int(app, "DEAUTH_BURST_ON_S", 15, &burst_on_s);
    int32_t burst_off_s = 45; predator_settings_get_int(app, "DEAUTH_BURST_OFF_S", 45, &burst_off_s);
    int32_t et_src = 0; predator_settings_get_int(app, "EVILTWIN_CHANNEL_SRC", 0, &et_src); // 0=Setting,1=Auto
    char line[40];
    snprintf(line, sizeof(line), "Deauth Burst: %s", burst_enabled?"On":"Off");
    submenu_add_item(app->submenu, line, SettingsDeauthBurstToggle, settings_submenu_callback, app);
    snprintf(line, sizeof(line), "Burst ON: %lds", (long)burst_on_s);
    submenu_add_item(app->submenu, line, SettingsDeauthBurstOn, settings_submenu_callback, app);
    snprintf(line, sizeof(line), "Burst OFF: %lds", (long)burst_off_s);
    submenu_add_item(app->submenu, line, SettingsDeauthBurstOff, settings_submenu_callback, app);
    snprintf(line, sizeof(line), "EvilTwin Channel: %s", et_src?"Auto":"Setting");
    submenu_add_item(app->submenu, line, SettingsEvilTwinChannelSource, settings_submenu_callback, app);
    submenu_add_item(app->submenu, "Clear Logs", SettingsClearLogs, settings_submenu_callback, app);

    // Highlight current region if set - Default to UNBLOCK for Tesla
    SettingsItemId selected = SettingsRegionUnblock;
    switch(app->region) {
        case PredatorRegionAuto: selected = SettingsRegionAuto; break;
        case PredatorRegionUS: selected = SettingsRegionUS; break;
        case PredatorRegionEU: selected = SettingsRegionEU; break;
        case PredatorRegionCH: selected = SettingsRegionCH; break;
        case PredatorRegionJP: selected = SettingsRegionJP; break;
        case PredatorRegionCN: selected = SettingsRegionCN; break;
        case PredatorRegionUnblock: selected = SettingsRegionUnblock; break;
        default: selected = SettingsRegionUnblock; break; // Tesla default
    }
    submenu_set_selected_item(app->submenu, selected);
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
    FURI_LOG_I("Settings", "Settings submenu initialized");
}

bool predator_scene_settings_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(!app) {
        FURI_LOG_E("Settings", "App context is NULL in event handler");
        return false;
    }
    
    if(event.type == SceneManagerEventTypeBack) {
        scene_manager_previous_scene(app->scene_manager);
        consumed = true;
    } else if(event.type == SceneManagerEventTypeCustom) {
        // Handle Region selection; persist and keep submenu visible
        switch(event.event) {
        case SettingsRegionAuto:
            settings_persist_region(app, PredatorRegionAuto);
            consumed = true;
            break;
        case SettingsRegionUS:
            settings_persist_region(app, PredatorRegionUS);
            consumed = true;
            break;
        case SettingsRegionEU:
            settings_persist_region(app, PredatorRegionEU);
            consumed = true;
            break;
        case SettingsRegionCH:
            settings_persist_region(app, PredatorRegionCH);
            consumed = true;
            break;
        case SettingsRegionJP:
            settings_persist_region(app, PredatorRegionJP);
            consumed = true;
            break;
        case SettingsRegionCN:
            settings_persist_region(app, PredatorRegionCN);
            consumed = true;
            break;
        case SettingsRegionUnblock:
            settings_persist_region(app, PredatorRegionUnblock);
            consumed = true;
            break;
        case SettingsDeauthBurstToggle: {
            int32_t v=1; predator_settings_get_int(app, "DEAUTH_BURST_ENABLED", 1, &v);
            predator_settings_set_int(app, "DEAUTH_BURST_ENABLED", v?0:1);
            predator_scene_settings_new_on_enter(app);
            consumed = true;
            break; }
        case SettingsDeauthBurstOn: {
            int32_t v=15; predator_settings_get_int(app, "DEAUTH_BURST_ON_S", 15, &v);
            // Cycle 10,15,20,30
            int32_t next = (v==10)?15:(v==15)?20:(v==20)?30:10;
            predator_settings_set_int(app, "DEAUTH_BURST_ON_S", next);
            predator_scene_settings_new_on_enter(app);
            consumed = true;
            break; }
        case SettingsDeauthBurstOff: {
            int32_t v=45; predator_settings_get_int(app, "DEAUTH_BURST_OFF_S", 45, &v);
            // Cycle 30,45,60,90
            int32_t next = (v==30)?45:(v==45)?60:(v==60)?90:30;
            predator_settings_set_int(app, "DEAUTH_BURST_OFF_S", next);
            predator_scene_settings_new_on_enter(app);
            consumed = true;
            break; }
        case SettingsEvilTwinChannelSource: {
            int32_t v=0; predator_settings_get_int(app, "EVILTWIN_CHANNEL_SRC", 0, &v);
            predator_settings_set_int(app, "EVILTWIN_CHANNEL_SRC", v?0:1);
            predator_scene_settings_new_on_enter(app);
            consumed = true;
            break; }
        case SettingsClearLogs: {
            // Truncate log file
            Storage* storage = app->storage ? app->storage : furi_record_open(RECORD_STORAGE);
            bool close_storage = (storage != app->storage);
            File* file = storage_file_alloc(storage);
            if(storage_file_open(file, "/ext/apps_data/predator/predator_logs.txt", FSAM_WRITE, FSOM_CREATE_ALWAYS)) {
                storage_file_close(file);
                FURI_LOG_I("Settings", "Logs cleared");
            }
            storage_file_free(file);
            if(close_storage) furi_record_close(RECORD_STORAGE);
            consumed = true;
            break; }
        default:
            consumed = false;
            break;
        }
    }
    
    return consumed;
}

void predator_scene_settings_new_on_exit(void* context) {
    PredatorApp* app = context;
    
    if(!app) {
        FURI_LOG_E("Settings", "App context is NULL on exit");
        return;
    }
    
    // Save settings to storage - Comment out if function is not defined
    // if(!predator_save_settings(app)) {
    //     FURI_LOG_E("Settings", "Failed to save settings");
    // }
    
    FURI_LOG_I("Settings", "Exiting Settings scene");
}
