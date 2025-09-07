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

// Popup callback for when an attack is running
static void predator_scene_subghz_attacks_popup_callback(void* context) {
    PredatorApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, PredatorCustomEventPopupBack);
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
        // Handle popup back button press
        if(event.event == PredatorCustomEventPopupBack) {
            // Return to submenu view
            view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
            
            // Clean up any running attacks
            if(app->attack_running) {
                app->attack_running = false;
            }
            
            consumed = true;
        } else {
            // Handle menu selection
            consumed = true;
            switch(event.event) {
            case SubmenuIndexSubghzJamming:
                popup_set_header(app->popup, "RF Jamming", 64, 10, AlignCenter, AlignTop);
                popup_set_text(app->popup, "Jamming 433.92 MHz...\nPress Back to stop", 64, 25, AlignCenter, AlignTop);
                popup_set_callback(app->popup, predator_scene_subghz_attacks_popup_callback);
                popup_set_context(app->popup, app);
                popup_set_timeout(app->popup, 0);
                popup_enable_timeout(app->popup);
                app->attack_running = true;
                view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
                break;
            case SubmenuIndexSubghzReplay:
                popup_set_header(app->popup, "Signal Replay", 64, 10, AlignCenter, AlignTop);
                popup_set_text(app->popup, "Replaying captured signal...\nFrequency: 433.92MHz\nPress Back to stop", 64, 25, AlignCenter, AlignTop);
                popup_set_callback(app->popup, predator_scene_subghz_attacks_popup_callback);
                popup_set_context(app->popup, app);
                popup_set_timeout(app->popup, 0);
                popup_enable_timeout(app->popup);
                app->attack_running = true;
                view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
                break;
            case SubmenuIndexSubghzBruteforce:
                popup_set_header(app->popup, "RF Bruteforce", 64, 10, AlignCenter, AlignTop);
                popup_set_text(app->popup, "Bruteforcing codes...\nFrequency: 433.92MHz\nProgress: 0%\nPress Back to stop", 64, 25, AlignCenter, AlignTop);
                popup_set_callback(app->popup, predator_scene_subghz_attacks_popup_callback);
                popup_set_context(app->popup, app);
                popup_set_timeout(app->popup, 0);
                popup_enable_timeout(app->popup);
                app->attack_running = true;
                app->packets_sent = 0;
                view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
                break;
            default:
                break;
            }
        }
    }

    return consumed;
}

void predator_scene_subghz_attacks_on_exit(void* context) {
    PredatorApp* app = context;
    submenu_reset(app->submenu);
}
