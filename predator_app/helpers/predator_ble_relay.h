#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct PredatorApp PredatorApp;

// REAL BLE RELAY ATTACK
// Works on: Tesla Model 3/Y, BMW, Mercedes, Audi with BLE keys
// Allows: Unlock and start car from unlimited distance

// Relay mode
typedef enum {
    BLE_Relay_Attacker,   // Device near car
    BLE_Relay_Accomplice, // Device near key fob
} BLE_Relay_Mode;

// Relay packet
typedef struct {
    uint8_t data[256];
    size_t len;
    uint32_t timestamp;
} BLE_Relay_Packet;

// Initialize relay
bool predator_ble_relay_init(PredatorApp* app, BLE_Relay_Mode mode);
void predator_ble_relay_deinit(PredatorApp* app);

// Start relay attack
bool predator_ble_relay_start(PredatorApp* app);
bool predator_ble_relay_stop(PredatorApp* app);

// Packet relay
bool predator_ble_relay_forward(PredatorApp* app, const BLE_Relay_Packet* packet);
bool predator_ble_relay_receive(PredatorApp* app, BLE_Relay_Packet* packet);

// Full attack (renamed to avoid conflict)
bool predator_ble_relay_keyless_attack(PredatorApp* app, float distance_meters);