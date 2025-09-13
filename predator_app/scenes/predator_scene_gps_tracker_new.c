#include "../predator_i.h"
#include "../helpers/predator_view_helpers.h"
#include "../helpers/predator_gps.h"
#include "../helpers/predator_ui_elements.h"
#include "predator_scene.h"
#include <furi.h>
#include <gui/view.h>
#include <gui/elements.h>

// Custom view for GPS tracker
typedef struct {
    View* view;
    bool satellites_acquired;
    uint32_t satellites;
    float latitude;
    float longitude;
    uint8_t animation_frame;
} GpsTrackerView;

void predator_scene_gps_tracker_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    if(!app) {
        FURI_LOG_E("GPSTracker", "App context is NULL on enter");
        return;
    }
    
    // Validate board type before any hardware initialization
    if(app->board_type == PredatorBoardTypeUnknown) {
        FURI_LOG_W("GPSTracker", "Board type is Unknown, defaulting to Original");
        app->board_type = PredatorBoardTypeOriginal;
    }
    
    // Configure popup content to avoid blank screen
    if(app->view_dispatcher && app->popup) {
        popup_reset(app->popup);
        popup_set_header(app->popup, "GPS Tracker", 64, 10, AlignCenter, AlignTop);
        popup_set_text(app->popup, "Initializing GPS...\nPress Back to return", 64, 28, AlignCenter, AlignTop);
        popup_set_context(app->popup, app);
        popup_set_timeout(app->popup, 0);
        popup_enable_timeout(app->popup);
        view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
    } else {
        FURI_LOG_E("GPSTracker", "View dispatcher or popup is NULL, cannot switch view");
    }
    
    FURI_LOG_I("GPSTracker", "GPS Tracker scene entered with simulation mode");
}

bool predator_scene_gps_tracker_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(!app) {
        FURI_LOG_E("GPSTracker", "App context is NULL in event handler");
        return false;
    }
    
    if(event.type == SceneManagerEventTypeBack) {
        scene_manager_previous_scene(app->scene_manager);
        consumed = true;
    }
    
    return consumed;
}

void predator_scene_gps_tracker_new_on_exit(void* context) {
    PredatorApp* app = context;
    
    if(!app) {
        FURI_LOG_E("GPSTracker", "App context is NULL on exit");
        return;
    }
    
    FURI_LOG_I("GPSTracker", "Exiting GPS Tracker scene");
}
