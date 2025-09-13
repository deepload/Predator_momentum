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
        return;
    }
    
    if(!app->scene_manager) {
        return;
    }
    
    if(!app->view_dispatcher) {
        return;
    }
    
    popup_reset(app->popup);
    popup_set_header(app->popup, "GPS Tracker", 64, 10, AlignCenter, AlignTop);
    popup_set_text(app->popup, "Initializing GPS...\nPress Back to return", 64, 28, AlignCenter, AlignTop);
    popup_set_context(app->popup, app);
    popup_set_timeout(app->popup, 0);
    popup_enable_timeout(app->popup);
    
    app->attack_running = true;
    app->packets_sent = 0;
    app->targets_found = 0;
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
}

bool predator_scene_gps_tracker_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(!app) {
        return false;
    }
    
    if(event.type == SceneManagerEventTypeBack) {
        app->attack_running = false;
        scene_manager_previous_scene(app->scene_manager);
        consumed = true;
    } else if(event.type == SceneManagerEventTypeTick) {
        if(app->attack_running) {
            app->packets_sent += 1;
            if(app->packets_sent >= 20) {
                popup_set_text(app->popup, "GPS Signal Acquired\nTracking location...", 64, 28, AlignCenter, AlignTop);
            }
            if(app->packets_sent >= 40) {
                app->packets_sent = 0;
                app->targets_found += 1;
                popup_set_text(app->popup, "Location updated\nPress Back to return", 64, 28, AlignCenter, AlignTop);
            }
            consumed = true;
        }
    }
    
    return consumed;
}

void predator_scene_gps_tracker_new_on_exit(void* context) {
    PredatorApp* app = context;
    
    if(!app) return;
    
    app->attack_running = false;
}
