#pragma once

#include "../predator_i.h"

// Advanced Bluetooth attacks
typedef struct {
    uint8_t address[6];
    char name[32];
    int8_t rssi;
    bool classic;
    bool ble;
    uint16_t services[16];
    uint8_t service_count;
} BTDevice;

// BLE GATT operations
typedef struct {
    uint16_t handle;
    uint8_t uuid[16];
    uint8_t properties;
    uint16_t value_handle;
} BLECharacteristic;

bool predator_ble_enumerate_services(PredatorApp* app, const uint8_t* address, uint16_t* services, size_t* service_count);
bool predator_ble_enumerate_characteristics(PredatorApp* app, const uint8_t* address, uint16_t service, BLECharacteristic* chars, size_t* char_count);
bool predator_ble_read_characteristic(PredatorApp* app, const uint8_t* address, uint16_t handle, uint8_t* data, size_t* data_len);
bool predator_ble_write_characteristic(PredatorApp* app, const uint8_t* address, uint16_t handle, const uint8_t* data, size_t data_len);

// BLE fuzzing
bool predator_ble_fuzz_gatt(PredatorApp* app, const uint8_t* address);
bool predator_ble_fuzz_l2cap(PredatorApp* app, const uint8_t* address);
bool predator_ble_fuzz_att(PredatorApp* app, const uint8_t* address);
bool predator_ble_fuzz_smp(PredatorApp* app, const uint8_t* address);

// BLE attacks
bool predator_ble_pairing_bypass(PredatorApp* app, const uint8_t* address);
bool predator_ble_mitm_attack(PredatorApp* app, const uint8_t* target_address, const uint8_t* victim_address);
bool predator_ble_relay_attack(PredatorApp* app, const uint8_t* target_address);

// Connection parameter manipulation
bool predator_ble_set_connection_interval(PredatorApp* app, const uint8_t* address, uint16_t min_interval, uint16_t max_interval);
bool predator_ble_set_mtu_size(PredatorApp* app, const uint8_t* address, uint16_t mtu);
bool predator_ble_set_slave_latency(PredatorApp* app, const uint8_t* address, uint16_t latency);

// Bluetooth Classic attacks
bool predator_bt_bluebug(PredatorApp* app, const uint8_t* address);
bool predator_bt_bluesnarf(PredatorApp* app, const uint8_t* address);
bool predator_bt_bluejack(PredatorApp* app, const uint8_t* address, const char* message);

// HID injection
bool predator_ble_hid_inject_keystroke(PredatorApp* app, const uint8_t* address, const char* text);
bool predator_ble_hid_inject_mouse(PredatorApp* app, const uint8_t* address, int16_t x, int16_t y);

// BLE spam (extended)
typedef enum {
    BLESpamApple,
    BLESpamSamsung,
    BLESpamWindows,
    BLESpamAndroid,
    BLESpamAirTag,
    BLESpamSmartTag,
    BLESpamTile,
    BLESpamChipolo,
} BLESpamType;

bool predator_ble_spam_extended(PredatorApp* app, BLESpamType type, uint16_t count);

// Device fingerprinting
bool predator_ble_fingerprint_device(PredatorApp* app, const uint8_t* address, char* device_type, size_t type_len);
