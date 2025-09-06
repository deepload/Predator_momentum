#include "../predator_i.h"

enum SubmenuIndex {
    SubmenuIndexSubghzScan,
    SubmenuIndexSubghzJamming,
    SubmenuIndexSubghzReplay,
    SubmenuIndexSubghzBruteforce,
    SubmenuIndexSubghzClone,
};

void predator_scene_subghz_attacks_submenu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void predator_scene_subghz_attacks_on_enter(void* context) {
    PredatorApp* app = context;
    Submenu* submenu = app->submenu;

    submenu_add_item(
        submenu, "📻 Frequency Scanner", SubmenuIndexSubghzScan, predator_scene_subghz_attacks_submenu_callback, app);
    submenu_add_item(
        submenu, "📡 Signal Jamming", SubmenuIndexSubghzJamming, predator_scene_subghz_attacks_submenu_callback, app);
    submenu_add_item(
        submenu, "🔄 Signal Replay", SubmenuIndexSubghzReplay, predator_scene_subghz_attacks_submenu_callback, app);
    submenu_add_item(
        submenu, "💥 Bruteforce Attack", SubmenuIndexSubghzBruteforce, predator_scene_subghz_attacks_submenu_callback, app);
    submenu_add_item(
        submenu, "📋 Signal Clone", SubmenuIndexSubghzClone, predator_scene_subghz_attacks_submenu_callback, app);

    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

bool predator_scene_subghz_attacks_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        consumed = true;
        switch(event.event) {
        case SubmenuIndexSubghzJamming:
            popup_set_header(app->popup, "RF Jamming", 64, 10, AlignCenter, AlignTop);
            popup_set_text(app->popup, "Jamming 433.92 MHz...\nPress Back to stop", 64, 25, AlignCenter, AlignTop);
            view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
            break;
        default:
            break;
        }
    }

    return consumed;
}

void predator_scene_subghz_attacks_on_exit(void* context) {
    PredatorApp* app = context;
    submenu_reset(app->submenu);
}
