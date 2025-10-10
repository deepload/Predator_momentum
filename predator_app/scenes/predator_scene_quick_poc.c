#include "../predator_i.h"
#include "predator_scene.h"
#include "../helpers/predator_view_helpers.h"
#include <furi.h>

// Local submenu indices for Quick Tests
typedef enum {
    QuickPocTesla = 1,
    QuickPocCarModels,
    QuickPocCarJamming,
    QuickPocCarKeyBruteforce,
    QuickPocCarPassiveOpener,
    QuickPocWifiScan,
    QuickPocWifiDeauth,
    QuickPocWifiEvilTwin,
    QuickPocBleScan,
    QuickPocBleSpam,
    QuickPocSubGhzAttacks,
    QuickPocRfidClone,
    QuickPocRfidBruteforce,
    QuickPocGpsTracker,
    QuickPocWardriving,
    QuickPocModuleStatus,
    QuickPocBoardSelection,
    QuickPocSettings,
    QuickPocAbout,
    QuickPocLiveMonitor,
} QuickPocIndex;

static void quick_poc_submenu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    if(!app || !app->view_dispatcher) return;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void predator_scene_quick_poc_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app) {
        FURI_LOG_E("QuickTests", "App context is NULL on enter");
        return;
    }
    if(!app->scene_manager || !app->view_dispatcher || !app->submenu) {
        FURI_LOG_E("QuickTests", "Invalid UI state on enter");
        return;
    }

    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "Quick Tests");

    // Car
    submenu_add_item(app->submenu, "Tesla Charge Port", QuickPocTesla, quick_poc_submenu_callback, app);
    submenu_add_item(app->submenu, "Car Models", QuickPocCarModels, quick_poc_submenu_callback, app);
    submenu_add_item(app->submenu, "Car Jamming", QuickPocCarJamming, quick_poc_submenu_callback, app);
    submenu_add_item(app->submenu, "Car Key Bruteforce", QuickPocCarKeyBruteforce, quick_poc_submenu_callback, app);
    submenu_add_item(app->submenu, "Car Passive Opener", QuickPocCarPassiveOpener, quick_poc_submenu_callback, app);

    // WiFi/BLE
    submenu_add_item(app->submenu, "WiFi Scan", QuickPocWifiScan, quick_poc_submenu_callback, app);
    submenu_add_item(app->submenu, "WiFi Deauth", QuickPocWifiDeauth, quick_poc_submenu_callback, app);
    submenu_add_item(app->submenu, "WiFi Evil Twin", QuickPocWifiEvilTwin, quick_poc_submenu_callback, app);
    submenu_add_item(app->submenu, "BLE Scan", QuickPocBleScan, quick_poc_submenu_callback, app);
    submenu_add_item(app->submenu, "BLE Spam", QuickPocBleSpam, quick_poc_submenu_callback, app);

    // SubGHz & RFID
    submenu_add_item(app->submenu, "SubGHz Attacks", QuickPocSubGhzAttacks, quick_poc_submenu_callback, app);
    submenu_add_item(app->submenu, "RFID Clone", QuickPocRfidClone, quick_poc_submenu_callback, app);
    submenu_add_item(app->submenu, "RFID Bruteforce", QuickPocRfidBruteforce, quick_poc_submenu_callback, app);

    // GPS & Tools
    submenu_add_item(app->submenu, "GPS Tracker", QuickPocGpsTracker, quick_poc_submenu_callback, app);
    submenu_add_item(app->submenu, "Wardriving", QuickPocWardriving, quick_poc_submenu_callback, app);
    submenu_add_item(app->submenu, "Module Status", QuickPocModuleStatus, quick_poc_submenu_callback, app);
    submenu_add_item(app->submenu, "Board Selection", QuickPocBoardSelection, quick_poc_submenu_callback, app);
    submenu_add_item(app->submenu, "Settings", QuickPocSettings, quick_poc_submenu_callback, app);
    submenu_add_item(app->submenu, "About", QuickPocAbout, quick_poc_submenu_callback, app);
    submenu_add_item(app->submenu, "Live Monitor (logs)", QuickPocLiveMonitor, quick_poc_submenu_callback, app);

    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

bool predator_scene_quick_poc_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    if(!app) return false;

    if(event.type == SceneManagerEventTypeBack) {
        scene_manager_previous_scene(app->scene_manager);
        return true;
    } else if(event.type == SceneManagerEventTypeCustom) {
        switch(event.event) {
        case QuickPocTesla:
            scene_manager_next_scene(app->scene_manager, PredatorSceneCarTeslaUI);
            return true;
        case QuickPocCarModels:
            scene_manager_next_scene(app->scene_manager, PredatorSceneCarModelSelector);
            return true;
        case QuickPocCarJamming:
            scene_manager_next_scene(app->scene_manager, PredatorSceneCarJammingUI);
            return true;
        case QuickPocCarKeyBruteforce:
            scene_manager_next_scene(app->scene_manager, PredatorSceneCarKeyBruteforceUI);
            return true;
        case QuickPocCarPassiveOpener:
            scene_manager_next_scene(app->scene_manager, PredatorSceneCarPassiveOpenerUI);
            return true;
        case QuickPocWifiScan:
            scene_manager_next_scene(app->scene_manager, PredatorSceneWifiScanUI);
            return true;
        case QuickPocWifiDeauth:
            scene_manager_next_scene(app->scene_manager, PredatorSceneWifiDeauthUI);
            return true;
        case QuickPocWifiEvilTwin:
            scene_manager_next_scene(app->scene_manager, PredatorSceneWifiEvilTwinUI);
            return true;
        case QuickPocBleScan:
            scene_manager_next_scene(app->scene_manager, PredatorSceneBleScanUI);
            return true;
        case QuickPocBleSpam:
            scene_manager_next_scene(app->scene_manager, PredatorSceneBleSpamUI);
            return true;
        case QuickPocSubGhzAttacks:
            scene_manager_next_scene(app->scene_manager, PredatorSceneSubGhzJammingUI);
            return true;
        case QuickPocRfidClone:
            scene_manager_next_scene(app->scene_manager, PredatorSceneRfidCloneUI);
            return true;
        case QuickPocRfidBruteforce:
            scene_manager_next_scene(app->scene_manager, PredatorSceneRfidBruteforceUI);
            return true;
        case QuickPocGpsTracker:
            scene_manager_next_scene(app->scene_manager, PredatorSceneGpsTrackerUI);
            return true;
        case QuickPocWardriving:
            scene_manager_next_scene(app->scene_manager, PredatorSceneWardrivingUI);
            return true;
        case QuickPocModuleStatus:
            scene_manager_next_scene(app->scene_manager, PredatorSceneModuleStatusUI);
            return true;
        case QuickPocBoardSelection:
            scene_manager_next_scene(app->scene_manager, PredatorSceneBoardSelectionUI);
            return true;
        case QuickPocSettings:
            scene_manager_next_scene(app->scene_manager, PredatorSceneSettingsUI);
            return true;
        case QuickPocAbout:
            scene_manager_next_scene(app->scene_manager, PredatorSceneAboutUI);
            return true;
        case QuickPocLiveMonitor:
            scene_manager_next_scene(app->scene_manager, PredatorSceneLiveMonitorUI);
            return true;
        default:
            return false;
        }
    }
    return false;
}

void predator_scene_quick_poc_on_exit(void* context) {
    (void)context;
}
