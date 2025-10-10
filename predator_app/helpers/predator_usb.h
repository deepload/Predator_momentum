#pragma once

#include "../predator_i.h"

// USB HID attacks (Rubber Ducky style)
typedef enum {
    USBDeviceKeyboard,
    USBDeviceMouse,
    USBDeviceStorage,
    USBDeviceSerial,
    USBDeviceComposite,
} USBDeviceType;

typedef struct {
    char* script;
    size_t script_len;
    uint32_t delay_ms;
} USBHIDScript;

// USB HID operations
bool predator_usb_hid_init(PredatorApp* app, USBDeviceType type);
void predator_usb_hid_deinit(PredatorApp* app);

// Keyboard injection
bool predator_usb_hid_type_string(PredatorApp* app, const char* text);
bool predator_usb_hid_press_key(PredatorApp* app, uint8_t key, uint8_t modifiers);
bool predator_usb_hid_execute_script(PredatorApp* app, const USBHIDScript* script);

// Mouse injection
bool predator_usb_hid_move_mouse(PredatorApp* app, int16_t x, int16_t y);
bool predator_usb_hid_click_mouse(PredatorApp* app, uint8_t button);

// Rubber Ducky script parser
bool predator_usb_parse_ducky_script(const char* script_path, USBHIDScript* parsed);

// Bad USB attacks
bool predator_usb_bad_usb_attack(PredatorApp* app, const char* payload);

// Mass storage emulation
bool predator_usb_emulate_storage(PredatorApp* app, const char* image_path);
bool predator_usb_autorun_payload(PredatorApp* app, const char* payload_path);
