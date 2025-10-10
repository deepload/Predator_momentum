#pragma once

#include "../predator_i.h"

// Advanced WiFi attacks beyond basic scan/deauth
typedef struct {
    char ssid[33];
    uint8_t bssid[6];
    uint8_t channel;
    int8_t rssi;
    bool wpa;
    bool wpa2;
    bool wpa3;
    bool wps;
    bool has_handshake;
    bool has_pmkid;
    uint8_t client_count;
} WiFiTarget;

// WPA3 attacks
bool predator_wifi_wpa3_downgrade(PredatorApp* app, const char* ssid, uint8_t channel);
bool predator_wifi_wpa3_dragonblood(PredatorApp* app, const char* ssid, uint8_t channel);

// PMKID attacks
bool predator_wifi_pmkid_capture(PredatorApp* app, const char* ssid, uint8_t channel);
bool predator_wifi_pmkid_optimized(PredatorApp* app, WiFiTarget* target);

// WPS attacks
bool predator_wifi_wps_pixie_dust(PredatorApp* app, const char* ssid, uint8_t channel);
bool predator_wifi_wps_reaver(PredatorApp* app, const char* ssid, uint8_t channel);
bool predator_wifi_wps_null_pin(PredatorApp* app, const char* ssid, uint8_t channel);

// Evil Twin with templates
typedef enum {
    CaptivePortalHotel,
    CaptivePortalAirport,
    CaptivePortalCoffeeShop,
    CaptivePortalCorporate,
    CaptivePortalPublicWiFi,
} CaptivePortalTemplate;

bool predator_wifi_evil_twin_with_template(PredatorApp* app, const char* ssid, CaptivePortalTemplate template);
bool predator_wifi_evil_twin_credential_harvest(PredatorApp* app, const char* ssid);

// KARMA/MANA attacks
bool predator_wifi_karma_attack(PredatorApp* app);
bool predator_wifi_mana_attack(PredatorApp* app);

// Advanced deauth
bool predator_wifi_targeted_deauth(PredatorApp* app, const uint8_t* client_mac, const uint8_t* ap_mac, uint8_t channel);
bool predator_wifi_deauth_all_clients(PredatorApp* app, const char* ssid, uint8_t channel);

// Client tracking
bool predator_wifi_track_clients(PredatorApp* app, uint8_t* client_macs, size_t* client_count);
bool predator_wifi_probe_request_monitor(PredatorApp* app);

// Hidden SSID enumeration
bool predator_wifi_enumerate_hidden_ssid(PredatorApp* app, const uint8_t* bssid, char* ssid);

// Beacon flooding
bool predator_wifi_beacon_flood(PredatorApp* app, uint16_t beacon_count);

// Handshake capture
bool predator_wifi_capture_handshake(PredatorApp* app, WiFiTarget* target);
bool predator_wifi_force_handshake(PredatorApp* app, WiFiTarget* target);

// Rogue AP detection
bool predator_wifi_detect_rogue_ap(PredatorApp* app, WiFiTarget* targets, size_t target_count);
