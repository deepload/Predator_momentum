#include "../predator_i.h"\n#include "../helpers/predator_view_helpers.h"

void predator_scene_about_on_enter(void* context) {
    PredatorApp* app = context;
    
    // Build complete about text with status
    char about_text[1024];
    
    const char* module_status = app->module_connected ? 
        "✅ Predator Module: Connected" : 
        "❌ Predator Module: Not Connected";
        
    snprintf(about_text, sizeof(about_text),
        "PREDATOR v1.0\n"
        "Advanced Penetration Testing\n"
        "Toolkit for Flipper Zero\n"
        "\n"
        "Hardware Status:\n"
        "%s\n"
        "\n"
        "Hardware Modules:\n"
        "• ESP32S2 Marauder (Pins 15,16)\n"
        "• GPS Module (Pins 13,14)\n"
        "• A07 433MHz RF (10dBm)\n"
        "• 2.8\" Display (800mAh)\n",
        module_status);
    
    // Append remaining capabilities to about text - using strlcat which is a safer API
    const char* attack_capabilities = 
        "\n"
        "Attack Capabilities:\n"
        "• WiFi (Deauth, Evil Twin, Scan)\n"
        "• Bluetooth/BLE (Spam, Scan)\n"
        "• SubGHz/RF (Jam, Bruteforce)\n"
        "• Car Keys (433/315MHz)\n"
        "• Tesla Charge Port (315MHz)\n"
        "• RFID/NFC (Clone, Bruteforce)\n"
        "• GPS Tracking & Wardriving\n"
        "• Social Engineering Tools\n";
    strlcat(about_text, attack_capabilities, sizeof(about_text));
    
    // Add hardware status section regardless of module connection
    const char* switch_controls = 
        "\n"
        "Switch Controls:\n"
        "• Left: GPS Power (Down=Flipper)\n"
        "• Right: Marauder Enable\n";
    strlcat(about_text, switch_controls, sizeof(about_text));
    
    // Add standard footer
    const char* footer = 
        "\n"
        "⚠\uFE0F FOR EDUCATIONAL USE ONLY\n"
        "Use responsibly and legally!\n"
        "\n"
        "Special thanks to: Dario Amodei\n"
        "\n"
        "Press Back to return";
    strlcat(about_text, footer, sizeof(about_text));
    
    widget_add_text_scroll_element(
        app->widget,
        0,
        0,
        128,
        64,
        about_text);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewWidget);
}

bool predator_scene_about_on_event(void* context, SceneManagerEvent event) {
    UNUSED(context);
    UNUSED(event);
    bool consumed = false;
    
    if(event.type == SceneManagerEventTypeCustom) {
        // Handle any custom events if needed in the future
        consumed = true;
    } else if(event.type == SceneManagerEventTypeBack) {
        // Let the scene manager handle the back button
        consumed = false;
    } else if(event.type == SceneManagerEventTypeTick) {
        // Update any dynamic content if needed
        // Currently no dynamic content in about screen
    }
    
    return consumed;
}

void predator_scene_about_on_exit(void* context) {
    PredatorApp* app = context;
    widget_reset(app->widget);
}



