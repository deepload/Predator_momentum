#pragma once

#include <furi.h>
#include <furi_hal.h>
#include <gui/gui.h>
#include <gui/view_dispatcher.h>
#include <gui/scene_manager.h>
#include <gui/modules/submenu.h>
#include <gui/modules/text_input.h>
#include <gui/modules/popup.h>
#include <gui/modules/loading.h>
#include <gui/modules/widget.h>
#include <notification/notification_messages.h>
#include <dialogs/dialogs.h>
#include <storage/storage.h>

#define PREDATOR_TEXT_STORE_SIZE 256

typedef enum {
    PredatorViewSubmenu,
    PredatorViewTextInput,
    PredatorViewPopup,
    PredatorViewLoading,
    PredatorViewWidget,
} PredatorView;

typedef enum {
    PredatorEventTypeKey,
    PredatorEventTypeCustom,
} PredatorEventType;

typedef enum {
    PredatorCustomEventPopupBack = 100,
    PredatorCustomEventEsp32Connected,
    PredatorCustomEventEsp32Disconnected,
    PredatorCustomEventWifiScanComplete,
    PredatorCustomEventDeauthComplete,
} PredatorCustomEvent;

typedef struct PredatorApp {
    Gui* gui;
    NotificationApp* notifications;
    DialogsApp* dialogs;
    Storage* storage;
    
    ViewDispatcher* view_dispatcher;
    SceneManager* scene_manager;
    
    Submenu* submenu;
    TextInput* text_input;
    Popup* popup;
    Loading* loading;
    Widget* widget;
    
    char text_store[PREDATOR_TEXT_STORE_SIZE + 1];
    
    // Attack state
    bool attack_running;
    uint32_t packets_sent;
    uint32_t targets_found;
    
    // ESP32 communication
    bool esp32_connected;
    FuriStreamBuffer* esp32_stream;
    struct PredatorUart* esp32_uart;
    
    // GPS data
    bool gps_connected;
    float latitude;
    float longitude;
    uint32_t satellites;
    struct PredatorUart* gps_uart;
    
} PredatorApp;



// Predator Module Pin Definitions (Actual Hardware)
// ESP32S2 Marauder Module - Pins 15,16 (as per documentation)
#define PREDATOR_ESP32_UART_TX_PIN &gpio_ext_pc0  // Pin 15
#define PREDATOR_ESP32_UART_RX_PIN &gpio_ext_pc1  // Pin 16
#define PREDATOR_ESP32_UART_BAUD   115200

// GPS Module - Pins 13,14 (as per documentation)
#define PREDATOR_GPS_UART_TX_PIN   &gpio_ext_pb2  // Pin 13
#define PREDATOR_GPS_UART_RX_PIN   &gpio_ext_pb3  // Pin 14
#define PREDATOR_GPS_UART_BAUD     9600

// A07 433MHz RF Module (External SubGHz, 10dBm)
#define PREDATOR_A07_POWER_DBM     10
#define PREDATOR_USE_EXTERNAL_RF   1

// Hardware Control Pins
#define PREDATOR_GPS_POWER_SWITCH  &gpio_ext_pa4  // GPS power switch (front left)
#define PREDATOR_MARAUDER_SWITCH   &gpio_ext_pa7  // Marauder switch (front right)
#define PREDATOR_CHARGING_LED      &gpio_ext_pa6  // Charging indicator
#define PREDATOR_ESP32_BOOT_BTN    &gpio_ext_pa5  // ESP32 boot button

// ESP32S2 Marauder Commands (Compatible with Marauder firmware)
#define MARAUDER_CMD_WIFI_SCAN       "scanap"
#define MARAUDER_CMD_WIFI_DEAUTH     "attack -t deauth -c"
#define MARAUDER_CMD_WIFI_EVIL_TWIN  "attack -t evil_portal"
#define MARAUDER_CMD_BLE_SCAN        "scandevices -t ble"
#define MARAUDER_CMD_BLE_SPAM        "attack -t ble_spam"
#define MARAUDER_CMD_WARDRIVE        "wardrive"
#define MARAUDER_CMD_STATUS          "status"
#define MARAUDER_CMD_STOP            "stop"

// Hardware status
#define PREDATOR_BATTERY_CAPACITY    800  // 800mAh battery
#define PREDATOR_ANTENNA_WIFI_DBI    3    // 3dBi WiFi antenna
#define PREDATOR_ANTENNA_GPS_DBI     20   // 20dBi GPS antenna
#define PREDATOR_ANTENNA_433_DBI     3    // 3dBi 433MHz antenna
