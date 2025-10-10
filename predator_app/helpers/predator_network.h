#pragma once

#include "../predator_i.h"

// Network protocol support (via ESP32)
typedef struct {
    uint8_t src_mac[6];
    uint8_t dst_mac[6];
    uint16_t ethertype;
    uint8_t* payload;
    size_t payload_len;
} EthernetFrame;

typedef struct {
    uint32_t src_ip;
    uint32_t dst_ip;
    uint16_t src_port;
    uint16_t dst_port;
    uint8_t protocol;
    uint8_t* payload;
    size_t payload_len;
} IPPacket;

// Network operations (via ESP32)
bool predator_network_init(PredatorApp* app);
void predator_network_deinit(PredatorApp* app);

// Packet capture
bool predator_network_start_capture(PredatorApp* app);
bool predator_network_stop_capture(PredatorApp* app);
bool predator_network_get_packet(PredatorApp* app, EthernetFrame* frame);

// ARP operations
bool predator_network_arp_scan(PredatorApp* app, uint32_t network, uint8_t prefix_len, uint32_t* hosts, size_t* host_count);
bool predator_network_arp_spoof(PredatorApp* app, uint32_t target_ip, uint32_t gateway_ip, const uint8_t* attacker_mac);

// Port scanning
bool predator_network_port_scan(PredatorApp* app, uint32_t target_ip, uint16_t* open_ports, size_t* port_count);
bool predator_network_service_detect(PredatorApp* app, uint32_t target_ip, uint16_t port, char* service, size_t service_len);

// DNS operations
bool predator_network_dns_spoof(PredatorApp* app, const char* domain, uint32_t spoofed_ip);
bool predator_network_dns_tunnel(PredatorApp* app, const uint8_t* data, size_t data_len);

// MITM attacks
bool predator_network_mitm_start(PredatorApp* app, uint32_t target_ip, uint32_t gateway_ip);
bool predator_network_mitm_stop(PredatorApp* app);
bool predator_network_mitm_inject(PredatorApp* app, const uint8_t* data, size_t data_len);

// SSL/TLS
bool predator_network_ssl_strip(PredatorApp* app, bool enable);
bool predator_network_ssl_version_detect(PredatorApp* app, uint32_t target_ip, uint16_t port, char* version, size_t version_len);
