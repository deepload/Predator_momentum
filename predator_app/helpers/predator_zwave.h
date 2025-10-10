#pragma once

#include "../predator_i.h"

// Z-Wave support
typedef struct {
    uint8_t home_id[4];
    uint8_t node_id;
    uint8_t* payload;
    size_t payload_len;
    int8_t rssi;
} ZWavePacket;

typedef struct {
    uint8_t home_id[4];
    uint8_t node_id;
    uint8_t device_class;
    uint8_t command_classes[32];
    uint8_t command_class_count;
} ZWaveDevice;

// Z-Wave initialization
bool predator_zwave_init(PredatorApp* app);
void predator_zwave_deinit(PredatorApp* app);

// Z-Wave operations
bool predator_zwave_send_packet(PredatorApp* app, const ZWavePacket* packet);
bool predator_zwave_receive_packet(PredatorApp* app, ZWavePacket* packet, uint32_t timeout_ms);

// Z-Wave scanning
bool predator_zwave_scan_networks(PredatorApp* app, uint8_t home_ids[][4], size_t* network_count);
bool predator_zwave_scan_devices(PredatorApp* app, const uint8_t* home_id, ZWaveDevice* devices, size_t* device_count);

// Z-Wave attacks
bool predator_zwave_replay_attack(PredatorApp* app, const ZWavePacket* packet);
bool predator_zwave_jamming(PredatorApp* app, uint32_t duration_ms);
bool predator_zwave_join_network(PredatorApp* app, const uint8_t* home_id);
bool predator_zwave_control_device(PredatorApp* app, const ZWaveDevice* device, uint8_t command);
