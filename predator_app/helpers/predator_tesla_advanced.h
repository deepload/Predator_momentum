#pragma once
#include <stdint.h>
#include <stdbool.h>

typedef struct PredatorApp PredatorApp;

// Tesla vehicle systems
typedef enum {
    TeslaSystem_VCSEC,          // Vehicle Controller Secure Element
    TeslaSystem_FSD,            // Full Self-Driving
    TeslaSystem_Charging,       // Charging network
    TeslaSystem_OTA,            // Over-the-Air updates
    TeslaSystem_Autopilot,      // Autopilot sensors
    TeslaSystem_BMS,            // Battery Management System
} TeslaSystem;

// Tesla vulnerabilities (2024/2025 research)
typedef enum {
    TeslaVuln_TPMS_RCE,        // TPMS zero-click RCE (Pwn2Own)
    TeslaVuln_VCSEC_Bypass,    // VCSEC authentication bypass
    TeslaVuln_BLE_Relay,       // BLE relay attack
    TeslaVuln_UWB_Spoof,       // Ultra-Wideband spoofing
    TeslaVuln_FSD_Jamming,     // FSD sensor interference
    TeslaVuln_Charging_Bypass, // Supercharger payment bypass
} TeslaVulnerability;

// Initialization
bool predator_tesla_advanced_init(PredatorApp* app);
void predator_tesla_advanced_deinit(PredatorApp* app);

// VCSEC exploits
bool predator_tesla_vcsec_scan(PredatorApp* app);
bool predator_tesla_vcsec_bypass(PredatorApp* app, uint32_t vehicle_id);
bool predator_tesla_vcsec_challenge_response(PredatorApp* app, const uint8_t* challenge, uint8_t* response);

// TPMS zero-click RCE (Pwn2Own 2024/2025)
bool predator_tesla_tpms_rce_test(PredatorApp* app);
bool predator_tesla_tpms_inject_payload(PredatorApp* app, const uint8_t* payload, size_t len);

// BLE/UWB attacks
bool predator_tesla_ble_relay_attack(PredatorApp* app, bool enable);
bool predator_tesla_uwb_spoof(PredatorApp* app, float distance_meters);
bool predator_tesla_proximity_unlock_test(PredatorApp* app);

// FSD interference
bool predator_tesla_fsd_camera_jam(PredatorApp* app, bool enable);
bool predator_tesla_fsd_radar_spoof(PredatorApp* app, uint32_t fake_object_distance);
bool predator_tesla_fsd_lidar_test(PredatorApp* app);

// Charging network
bool predator_tesla_supercharger_scan(PredatorApp* app);
bool predator_tesla_supercharger_bypass(PredatorApp* app, const char* vehicle_vin);
bool predator_tesla_charging_session_hijack(PredatorApp* app);

// OTA update security
bool predator_tesla_ota_intercept(PredatorApp* app);
bool predator_tesla_ota_signature_test(PredatorApp* app);
bool predator_tesla_firmware_downgrade(PredatorApp* app, uint32_t target_version);

// Battery Management System
bool predator_tesla_bms_diagnostic(PredatorApp* app);
bool predator_tesla_bms_cell_analysis(PredatorApp* app, uint16_t* cell_voltages, size_t cell_count);
bool predator_tesla_battery_thermal_test(PredatorApp* app);
