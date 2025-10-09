#include "../predator_i.h"
#include "../helpers/predator_view_helpers.h"
#include "../helpers/predator_ui_elements.h"
#include "../helpers/predator_ui_status.h"
#include "../helpers/predator_logging.h"

typedef struct {
    View* view;
    uint32_t keys_tried;
    uint32_t current_key;
    uint8_t animation_frame;
    uint8_t progress;
    bool key_found;
    char card_type[32];
} RfidBruteforceView;

/*
static void rfid_bruteforce_view_draw_callback(Canvas* canvas, void* context) {
    // ...
}
*/

/*
static bool rfid_bruteforce_view_input_callback(InputEvent* event, void* context) {
    // ...
}
*/

/*
static View* rfid_bruteforce_view_alloc(PredatorApp* app) {
    // ...
}
*/

void predator_scene_rfid_bruteforce_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    if(!app) {
        FURI_LOG_E("RfidBruteforce", "App context is NULL on enter");
        return;
    }
    
    // Validate board type before any hardware initialization
    if(app->board_type == 0) { // Assuming 0 represents Unknown or default
        FURI_LOG_W("RfidBruteforce", "Board type is Unknown, defaulting to Original");
        app->board_type = 0; // Keep as Original
    }
    
    // Ensure scene_manager and view_dispatcher are valid to prevent crashes
    if(!app->scene_manager) {
        FURI_LOG_E("RfidBruteforce", "Scene manager is NULL, cannot proceed");
        return;
    }
    
    if(!app->view_dispatcher) {
        FURI_LOG_E("RfidBruteforce", "View dispatcher is NULL, cannot switch view");
        return;
    }
    
    popup_reset(app->popup);
    popup_set_header(app->popup, "RFID Bruteforce", 64, 10, AlignCenter, AlignTop);
    {
        char status[64]; predator_ui_build_status(app, "Bruteforce: starting", status, sizeof(status));
        popup_set_text(app->popup, status, 64, 28, AlignCenter, AlignTop);
    }
    popup_set_context(app->popup, app);
    popup_set_timeout(app->popup, 0);
    popup_enable_timeout(app->popup);
    
    app->attack_running = true;
    app->packets_sent = 0;
    predator_log_append(app, "RfidBruteforce START");
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
    
    FURI_LOG_I("RfidBruteforce", "RFID Bruteforce scene entered with simulation mode");
}

bool predator_scene_rfid_bruteforce_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(!app) {
        // FURI_LOG_E("RfidBruteforce", "App context is NULL in event handler");
        return false;
    }
    
    if(event.type == SceneManagerEventTypeBack) {
        app->attack_running = false;
        scene_manager_previous_scene(app->scene_manager);
        consumed = true;
    } else if(event.type == SceneManagerEventTypeTick) {
        if(app->attack_running) {
            app->packets_sent += 1;
            if(app->packets_sent % 8 == 0 && app->popup) {
                char detail[48];
                snprintf(detail, sizeof(detail), "Keys tried: %lu", app->packets_sent);
                char status[64]; predator_ui_build_status(app, detail, status, sizeof(status));
                popup_set_text(app->popup, status, 64, 28, AlignCenter, AlignTop);
            }
            if(app->packets_sent >= 40 && app->popup) {
                popup_set_text(app->popup, "Key found!\nPress Back to return", 64, 28, AlignCenter, AlignTop);
                predator_log_append(app, "RfidBruteforce SUCCESS");
                app->attack_running = false;
            }
            consumed = true;
        }
    }
    
    return consumed;
}

void predator_scene_rfid_bruteforce_new_on_exit(void* context) {
    PredatorApp* app = context;
    
    if(!app) {
        // FURI_LOG_E("RfidBruteforce", "App context is NULL on exit");
        return;
    }
    
    app->attack_running = false;
    
    // FURI_LOG_I("RfidBruteforce", "Exiting RFID Bruteforce scene");
}
