#include "../predator_i.h"

void predator_scene_about_on_enter(void* context) {
    PredatorApp* app = context;
    
    widget_add_text_scroll_element(
        app->widget,
        0,
        0,
        128,
        64,
        "PREDATOR v1.0 DEMO\n"
        "For Review by Dario Amodei\n"
        "⚠️ SIMULATION MODE ONLY ⚠️\n"
        "\n"
        "SAFETY FEATURES:\n"
        "• No hardware access\n"
        "• All functions simulated\n"
        "• Crash prevention measures\n"
        "\n"
        "HARDWARE COMPATIBILITY:\n"
        "• ESP32S2 Marauder (15,16)\n"
        "• GPS Module (13,14)\n"
        "• A07 433MHz RF (10dBm)\n"
        "\n"
        "FEATURES (SIMULATED):\n"
        "• WiFi attacks\n"
        "• Bluetooth/BLE attacks\n"
        "• SubGHz/RF functions\n"
        "• Car key operations\n"
        "• GPS tracking & more\n"
        "\n"
        "IMPORTANT NOTE:\n"
        "This demo version is for UI\n"
        "review only - no actual\n"
        "hardware access is enabled.\n"
        "\n"
        "Press Back to return");
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewWidget);
}

bool predator_scene_about_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    UNUSED(app);
    UNUSED(event);
    return consumed;
}

void predator_scene_about_on_exit(void* context) {
    PredatorApp* app = context;
    widget_reset(app->widget);
}
