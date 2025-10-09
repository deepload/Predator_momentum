#include "../predator_i.h"
#include "../helpers/predator_view_helpers.h"
#include "../helpers/predator_ui_elements.h"
#include "../helpers/predator_compliance.h"
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

    // Highlight current region if set
    SettingsItemId selected = SettingsRegionEU;
    switch(app->region) {
        case PredatorRegionAuto: selected = SettingsRegionAuto; break;
        case PredatorRegionUS: selected = SettingsRegionUS; break;
        case PredatorRegionEU: selected = SettingsRegionEU; break;
        case PredatorRegionCH: selected = SettingsRegionCH; break;
        case PredatorRegionJP: selected = SettingsRegionJP; break;
        case PredatorRegionCN: selected = SettingsRegionCN; break;
        default: selected = SettingsRegionEU; break;
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
