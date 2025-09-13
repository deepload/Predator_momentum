#include "../predator_i.h"
#include "../helpers/predator_view_helpers.h"
#include "../helpers/predator_esp32.h"
#include "../helpers/predator_gps.h"
#include "../helpers/predator_ui_elements.h"

typedef struct {
    View* view;
    uint32_t networks_found;
    bool wardriving_active;
    bool gps_available;
    bool esp32_available;
    uint8_t animation_frame;
    char latitude[16];
    char longitude[16];
    char satellites[8];
} WardrivingView;

void predator_scene_wardriving_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    if(!app) {
        FURI_LOG_E("Wardriving", "App context is NULL on enter");
        return;
    }
    
    if(!app->scene_manager) {
        FURI_LOG_E("Wardriving", "Scene manager is NULL, cannot proceed");
        return;
    }
    
    if(!app->view_dispatcher) {
        FURI_LOG_E("Wardriving", "View dispatcher is NULL, cannot switch view");
        return;
    }
    
    popup_reset(app->popup);
    popup_set_header(app->popup, "Wardriving", 64, 10, AlignCenter, AlignTop);
    popup_set_text(app->popup, "Starting GPS+WiFi wardrive...\nPress Back to stop", 64, 28, AlignCenter, AlignTop);
    popup_set_context(app->popup, app);
    popup_set_timeout(app->popup, 0);
    popup_enable_timeout(app->popup);
    
    app->attack_running = true;
    app->targets_found = 0;
    app->packets_sent = 0;
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
    
    FURI_LOG_I("Wardriving", "Wardriving scene entered with simulation mode");
}

bool predator_scene_wardriving_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(!app) {
        FURI_LOG_E("Wardriving", "App context is NULL in event handler");
        return false;
    }
    
    if(event.type == SceneManagerEventTypeBack) {
        app->attack_running = false;
        scene_manager_previous_scene(app->scene_manager);
        consumed = true;
    } else if(event.type == SceneManagerEventTypeTick) {
        if(app->attack_running) {
            app->packets_sent += 1;
            if(app->packets_sent >= 15) {
                app->targets_found += 1;
                app->packets_sent = 0;
                popup_set_text(app->popup, "Networks found\nPress Back to stop", 64, 28, AlignCenter, AlignTop);
            }
            consumed = true;
        }
    }
    
    return consumed;
}

void predator_scene_wardriving_new_on_exit(void* context) {
    PredatorApp* app = context;
    
    if(!app) {
        FURI_LOG_E("Wardriving", "App context is NULL on exit");
        return;
    }
    
    app->attack_running = false;
    
    FURI_LOG_I("Wardriving", "Exiting Wardriving scene");
}
