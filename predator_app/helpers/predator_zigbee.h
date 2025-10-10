#pragma once

#include "../predator_i.h"

// Zigbee support
typedef struct {
    uint16_t pan_id;
    uint16_t short_addr;
    uint64_t extended_addr;
    uint8_t channel;
    uint8_t* payload;
    size_t payload_len;
    int8_t rssi;
    uint8_t lqi;
} ZigbeePacket;

typedef struct {
    uint16_t pan_id;
    uint64_t extended_addr;
    uint8_t network_key[16];
    uint8_t link_key[16];
    bool key_found;
} ZigbeeNetwork;

// Zigbee initialization
bool predator_zigbee_init(PredatorApp* app, uint8_t channel);
void predator_zigbee_deinit(PredatorApp* app);

// Zigbee operations
bool predator_zigbee_send_packet(PredatorApp* app, const ZigbeePacket* packet);
bool predator_zigbee_receive_packet(PredatorApp* app, ZigbeePacket* packet, uint32_t timeout_ms);
bool predator_zigbee_set_channel(PredatorApp* app, uint8_t channel);

// Zigbee scanning
bool predator_zigbee_scan_networks(PredatorApp* app, ZigbeeNetwork* networks, size_t* network_count);
bool predator_zigbee_scan_devices(PredatorApp* app, uint16_t pan_id, uint64_t* devices, size_t* device_count);

// Zigbee attacks
bool predator_zigbee_key_sniffing(PredatorApp* app, ZigbeeNetwork* network);
bool predator_zigbee_replay_attack(PredatorApp* app, const ZigbeePacket* packet);
bool predator_zigbee_jamming(PredatorApp* app, uint8_t channel, uint32_t duration_ms);
bool predator_zigbee_join_network(PredatorApp* app, ZigbeeNetwork* network);
