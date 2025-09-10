#include "../predator_i.h"\n#include "../helpers/predator_view_helpers.h"
#include "../helpers/predator_esp32.h"
#include "../helpers/predator_gps.h"
#include "../helpers/predator_boards.h"
#include "predator_scene.h"

enum SubmenuIndex {
    SubmenuIndexModuleStatus,
    SubmenuIndexBoardSelection,
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
        submenu, "🔌 Board Selection", SubmenuIndexBoardSelection, predator_scene_settings_submenu_callback, app);
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
                scene_manager_next_scene(app->scene_manager, PredatorSceneModuleStatus);
            }
            break;
            
        case SubmenuIndexBoardSelection:
            scene_manager_next_scene(app->scene_manager, PredatorSceneBoardSelection);
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


