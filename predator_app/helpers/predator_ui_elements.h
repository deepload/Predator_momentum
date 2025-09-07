#pragma once

#include <furi.h>
#include <gui/canvas.h>
#include <gui/view.h>
#include <gui/view_dispatcher.h>
#include <gui/elements.h>

// Include standard icons
extern const Icon I_ButtonLeft_4x7;
extern const Icon I_ButtonRight_4x7;
extern const Icon I_ButtonUp_4x7;
extern const Icon I_ButtonDown_4x7;

// Progress bar styles
typedef enum {
    ProgressBarStyleNormal,    // Default style
    ProgressBarStyleBordered,  // With border
    ProgressBarStyleDashed,    // Dashed/segmented progress
    ProgressBarStyleAnimated   // Animated/pulsing effect for indeterminate progress
} ProgressBarStyle;

// SubMenu indices for RFID attacks
typedef enum {
    SubmenuIndexRfidClone,
    SubmenuIndexRfidBruteforce,
    SubmenuIndexRfidFuzzing,
    SubmenuIndexNfcClone,
    SubmenuIndexMifareHack,
} RfidSubMenuIndices;

// SubMenu indices for car attacks
typedef enum {
    SubmenuIndexCarModels,
    SubmenuIndexCarPassiveOpener,
    SubmenuIndexCarKeyBruteforce,
    SubmenuIndexCarJamming,
    SubmenuIndexCarTesla,
    SubmenuIndexCarRollingCode,
    SubmenuIndexCarTireMonitor,
} CarSubMenuIndices;

// SubMenu indices for WiFi attacks
typedef enum {
    SubmenuIndexWifiScan,
    SubmenuIndexWifiDeauth,
    SubmenuIndexWifiEvilTwin,
    SubmenuIndexWifiHandshakeCapture,
    SubmenuIndexWifiPwnagotchi,
} WifiSubMenuIndices;

// SubMenu indices for BLE attacks
typedef enum {
    SubmenuIndexBleScan,
    SubmenuIndexBleSpam,
} BleSubMenuIndices;

// Shared UI element drawing functions
void predator_ui_draw_progress_bar(
    Canvas* canvas,
    uint8_t x, uint8_t y, 
    uint8_t width, uint8_t height,
    uint8_t progress, // 0-100
    ProgressBarStyle style);

// Draw bordered box with title
void predator_ui_draw_status_box(
    Canvas* canvas, 
    const char* title, 
    uint8_t x, uint8_t y, 
    uint8_t width, uint8_t height);

// Draw styled text with header
void predator_ui_draw_header_text(
    Canvas* canvas,
    const char* header,
    const char* text,
    uint8_t x, uint8_t y);

// Car attacks and RFID attacks popup callbacks
void predator_scene_car_attacks_popup_callback(void* context);
void predator_scene_rfid_attacks_popup_callback(void* context);

// Helper for sin/cos calculations in animations
extern const uint8_t sin_array[256];
extern const uint8_t cos_array[256];
