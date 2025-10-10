#pragma once

#include "../predator_i.h"

// LoRa/LoRaWAN support
typedef struct {
    uint32_t frequency;
    uint8_t spreading_factor;
    uint8_t bandwidth;
    uint8_t coding_rate;
    int8_t rssi;
    float snr;
    uint8_t* payload;
    size_t payload_len;
} LoRaPacket;

typedef struct {
    uint8_t dev_eui[8];
    uint8_t app_eui[8];
    uint8_t app_key[16];
    uint8_t nwk_skey[16];
    uint8_t app_skey[16];
    uint32_t dev_addr;
    uint32_t frame_counter;
} LoRaWANDevice;

// LoRa initialization
bool predator_lora_init(PredatorApp* app, uint32_t frequency);
void predator_lora_deinit(PredatorApp* app);

// LoRa operations
bool predator_lora_send_packet(PredatorApp* app, const LoRaPacket* packet);
bool predator_lora_receive_packet(PredatorApp* app, LoRaPacket* packet, uint32_t timeout_ms);
bool predator_lora_set_parameters(PredatorApp* app, uint8_t sf, uint8_t bw, uint8_t cr);

// LoRaWAN operations
bool predator_lorawan_join(PredatorApp* app, LoRaWANDevice* device);
bool predator_lorawan_send_uplink(PredatorApp* app, LoRaWANDevice* device, const uint8_t* data, size_t data_len);
bool predator_lorawan_receive_downlink(PredatorApp* app, LoRaWANDevice* device, uint8_t* data, size_t* data_len);

// LoRaWAN attacks
bool predator_lorawan_key_extraction(PredatorApp* app, LoRaWANDevice* device);
bool predator_lorawan_replay_attack(PredatorApp* app, const LoRaPacket* packet);
bool predator_lorawan_jamming(PredatorApp* app, uint32_t frequency, uint32_t duration_ms);
