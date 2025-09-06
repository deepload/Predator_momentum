#include "../predator_i.h"

void predator_scene_about_on_enter(void* context) {
    PredatorApp* app = context;
    
    widget_add_text_scroll_element(
        app->widget,
        0,
        0,
        128,
        64,
        "PREDATOR v1.0\n"
        "Advanced Penetration Testing\n"
        "Toolkit for Flipper Zero\n"
        "\n"
        "Hardware Modules:\n"
        "• ESP32S2 Marauder (Pins 15,16)\n"
        "• GPS Module (Pins 13,14)\n"
        "• A07 433MHz RF (10dBm)\n"
        "• 2.8\" Display (800mAh)\n"
        "\n"
        "Attack Capabilities:\n"
        "• WiFi (Deauth, Evil Twin, Scan)\n"
        "• Bluetooth/BLE (Spam, Scan)\n"
        "• SubGHz/RF (Jam, Bruteforce)\n"
        "• Car Keys (433/315MHz)\n"
        "• Tesla Charge Port (315MHz)\n"
        "• RFID/NFC (Clone, Bruteforce)\n"
        "• GPS Tracking & Wardriving\n"
        "• Social Engineering Tools\n"
        "\n"
        "Switch Controls:\n"
        "• Left: GPS Power (Down=Flipper)\n"
        "• Right: Marauder Enable\n"
        "\n"
        "⚠️ FOR EDUCATIONAL USE ONLY\n"
        "Use responsibly and legally!\n"
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
