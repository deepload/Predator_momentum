#include "../predator_i.h"
#include "../helpers/predator_subghz.h"
#include "predator_scene.h"

enum SubmenuIndex {
    SubmenuIndexCarModels,
    SubmenuIndexCarPassiveOpener,
    SubmenuIndexCarKeyBruteforce,
    SubmenuIndexCarJamming,
    SubmenuIndexCarTesla,
    SubmenuIndexCarRollingCode,
    SubmenuIndexCarTireMonitor,
};

void predator_scene_car_attacks_submenu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void predator_scene_car_attacks_on_enter(void* context) {
    PredatorApp* app = context;
    Submenu* submenu = app->submenu;

    submenu_add_item(
        submenu, "🚙 All Car Models", SubmenuIndexCarModels, predator_scene_car_attacks_submenu_callback, app);
    submenu_add_item(
        submenu, "👜 Passive Bag Mode", SubmenuIndexCarPassiveOpener, predator_scene_car_attacks_submenu_callback, app);
    submenu_add_item(
        submenu, "🔑 Key Bruteforce", SubmenuIndexCarKeyBruteforce, predator_scene_car_attacks_submenu_callback, app);
    submenu_add_item(
        submenu, "📡 Signal Jamming", SubmenuIndexCarJamming, predator_scene_car_attacks_submenu_callback, app);
    submenu_add_item(
        submenu, "🚗 Tesla Charge Port", SubmenuIndexCarTesla, predator_scene_car_attacks_submenu_callback, app);
    submenu_add_item(
        submenu, "🔄 Rolling Code Attack", SubmenuIndexCarRollingCode, predator_scene_car_attacks_submenu_callback, app);
    submenu_add_item(
        submenu, "🛞 Tire Monitor Spoof", SubmenuIndexCarTireMonitor, predator_scene_car_attacks_submenu_callback, app);

    submenu_set_selected_item(
        submenu, scene_manager_get_scene_state(app->scene_manager, PredatorSceneCarAttacks));

    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

// Popup callback for rolling code and tire monitor features
static void predator_scene_car_attacks_popup_callback(void* context) {
    PredatorApp* app = context;
    view_dispatcher_send_custom_event(app->view_dispatcher, PredatorCustomEventPopupBack);
}

bool predator_scene_car_attacks_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;

    if(event.type == SceneManagerEventTypeCustom) {
        // Handle custom event from popup callback
        if(event.event == PredatorCustomEventPopupBack) {
            // Return to submenu view
            view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
            
            // Clean up any running attacks
            if(app->attack_running) {
                // Check which attack was running to perform proper cleanup
                if(scene_manager_get_scene_state(app->scene_manager, PredatorSceneCarAttacks) == SubmenuIndexCarRollingCode) {
                    // Call specialized rolling code attack stop function
                    predator_subghz_stop_rolling_code_attack(app);
                }
                
                app->attack_running = false;
                predator_subghz_deinit(app);
            }
            consumed = true;
        } else {
            // Handle menu selection
            scene_manager_set_scene_state(app->scene_manager, PredatorSceneCarAttacks, event.event);
            consumed = true;
            switch(event.event) {
            case SubmenuIndexCarModels:
                scene_manager_next_scene(app->scene_manager, PredatorSceneCarModels);
                break;
            case SubmenuIndexCarPassiveOpener:
                scene_manager_next_scene(app->scene_manager, PredatorSceneCarPassiveOpener);
                break;
            case SubmenuIndexCarKeyBruteforce:
                scene_manager_next_scene(app->scene_manager, PredatorSceneCarKeyBruteforce);
                break;
            case SubmenuIndexCarJamming:
                scene_manager_next_scene(app->scene_manager, PredatorSceneCarJamming);
                break;
            case SubmenuIndexCarTesla:
                scene_manager_next_scene(app->scene_manager, PredatorSceneCarTesla);
                break;
            case SubmenuIndexCarRollingCode:
                // Setup popup for rolling code attack
                popup_set_header(app->popup, "Rolling Code Attack", 64, 10, AlignCenter, AlignTop);
                popup_set_text(app->popup, 
                    "Capturing rolling codes...\n"
                    "Waiting for signal\n"
                    "Press Back to stop", 
                    64, 25, AlignCenter, AlignTop);
                popup_set_callback(app->popup, predator_scene_car_attacks_popup_callback);
                popup_set_context(app->popup, app);
                popup_set_timeout(app->popup, 0);
                popup_enable_timeout(app->popup);
                
                // Initialize SubGHz and start rolling code attack
                predator_subghz_init(app);
                predator_subghz_start_rolling_code_attack(app, 433920000); // Use most common frequency
                app->attack_running = true;
                app->packets_sent = 0;
                
                view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
                break;
            case SubmenuIndexCarTireMonitor:
                // Setup popup for tire monitor spoofing
                popup_set_header(app->popup, "Tire Monitor Spoof", 64, 10, AlignCenter, AlignTop);
                popup_set_text(app->popup, 
                    "Spoofing TPMS signals...\n"
                    "Pressure: 15 PSI (Low)\n"
                    "Press Back to stop", 
                    64, 25, AlignCenter, AlignTop);
                popup_set_callback(app->popup, predator_scene_car_attacks_popup_callback);
                popup_set_context(app->popup, app);
                popup_set_timeout(app->popup, 0);
                popup_enable_timeout(app->popup);
                
                // Initialize SubGHz
                predator_subghz_init(app);
                app->attack_running = true;
                app->packets_sent = 0;
                
                view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
                break;
            default:
                break;
            }
        }
    } else if(event.type == SceneManagerEventTypeTick) {
        // Update rolling code and tire monitor UI if active
        if(app->attack_running) {
            // Update UI based on which feature is active
            if(scene_manager_get_scene_state(app->scene_manager, PredatorSceneCarAttacks) == SubmenuIndexCarRollingCode) {
                // Call the specialized rolling code attack tick function
                predator_subghz_rolling_code_attack_tick(app);
                
                // Update rolling code UI
                char status_text[128];
                
                // Different display based on number of codes captured
                if(app->packets_sent > 0) {
                    if(app->packets_sent >= 3) {
                        snprintf(status_text, sizeof(status_text), 
                            "Rolling codes captured!\n"
                            "Codes: %lu - REPLAYING\n"
                            "Press Back to stop", 
                            app->packets_sent);
                    } else {
                        snprintf(status_text, sizeof(status_text), 
                            "Capturing rolling codes...\n"
                            "Codes captured: %lu\n"
                            "Press Back to stop", 
                            app->packets_sent);
                    }
                } else {
                    snprintf(status_text, sizeof(status_text), 
                        "Capturing rolling codes...\n"
                        "Waiting for signal\n"
                        "Press Back to stop");
                }
                popup_set_text(app->popup, status_text, 64, 25, AlignCenter, AlignTop);
            } 
            else if(scene_manager_get_scene_state(app->scene_manager, PredatorSceneCarAttacks) == SubmenuIndexCarTireMonitor) {
                // Update tire monitor UI with decreasing pressure
                uint8_t pressure = 30 - ((app->packets_sent / 50) % 20); // Range from 30 to 10 PSI
                char status_text[128];
                snprintf(status_text, sizeof(status_text), 
                    "Spoofing TPMS signals...\n"
                    "Pressure: %d PSI (%s)\n"
                    "Press Back to stop", 
                    pressure,
                    pressure < 18 ? "Low" : "Normal");
                popup_set_text(app->popup, status_text, 64, 25, AlignCenter, AlignTop);
                
                // Blink LED for activity
                if(app->packets_sent % 30 == 0) {
                    notification_message(app->notifications, &sequence_blink_red_10);
                }
            }
        }
    }

    return consumed;
}

void predator_scene_car_attacks_on_exit(void* context) {
    PredatorApp* app = context;
    submenu_reset(app->submenu);
}
