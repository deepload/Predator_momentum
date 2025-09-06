#include "../predator_i.h"
#include "../helpers/predator_gps.h"
#include "predator_scene.h"
#include <furi.h>
#include <gui/view.h>
#include <gui/elements.h>

static void predator_scene_gps_tracker_popup_callback(void* context) {
    PredatorApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, PredatorCustomEventPopupBack);
}

void predator_scene_gps_tracker_on_enter(void* context) {
    PredatorApp* app = context;
    
    predator_gps_init(app);
    
    popup_set_header(app->popup, "GPS Tracker", 64, 10, AlignCenter, AlignTop);
    popup_set_text(app->popup, 
        "Acquiring GPS signal...\n"
        "Satellites: 0\n"
        "Lat: ---.------\n"
        "Lon: ---.------\n\n"
        "Press right for Debug", 
        64, 25, AlignCenter, AlignTop);
    popup_set_callback(app->popup, predator_scene_gps_tracker_popup_callback);
    popup_set_context(app->popup, app);
    popup_set_timeout(app->popup, 0);
    popup_enable_timeout(app->popup);
    // Use text instead of icon
    popup_set_text(app->popup, "[->] Debug", 98, 48, AlignCenter, AlignCenter);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
    
    app->attack_running = true;
}

bool predator_scene_gps_tracker_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        if(event.event == PredatorCustomEventPopupBack) {
            consumed = true;
            scene_manager_previous_scene(app->scene_manager);
        }
    } else if(event.type == SceneManagerEventTypeBack) {
        consumed = true;
        scene_manager_previous_scene(app->scene_manager);
    } else if(event.type == SceneManagerEventTypeCustom && event.event == InputKeyRight) {
        // Switch to GPS debug scene - use GpsDebug scene which is at index 20 per predator_scene_config.h
        scene_manager_next_scene(app->scene_manager, 20); // PredatorSceneGpsDebug
        consumed = true;
    } else if(event.type == SceneManagerEventTypeTick) {
        if(app->attack_running) {
            predator_gps_update(app);
            
            char status_text[128];
            if(app->gps_connected && app->satellites > 0) {
                snprintf(status_text, sizeof(status_text), 
                    "GPS Signal Acquired!\n"
                    "Satellites: %lu\n"
                    "Lat: %.6f\n"
                    "Lon: %.6f\n\n"
                    "Press right for Debug", 
                    app->satellites, (double)app->latitude, (double)app->longitude);
            } else {
                snprintf(status_text, sizeof(status_text), 
                    "Acquiring GPS signal...\n"
                    "Satellites: %lu\n"
                    "Lat: ---.------\n"
                    "Lon: ---.------\n\n"
                    "Press right for Debug", 
                    app->satellites);
            }
            popup_set_text(app->popup, status_text, 64, 25, AlignCenter, AlignTop);
        }
    }

    return consumed;
}

void predator_scene_gps_tracker_on_exit(void* context) {
    PredatorApp* app = context;
    app->attack_running = false;
    predator_gps_deinit(app);
    popup_reset(app->popup);
}
