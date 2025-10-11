#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct PredatorApp PredatorApp;

// REAL CAN BUS IMPLEMENTATION
// Requires MCP2515 CAN controller connected to Flipper Zero GPIO
// Professional automotive testing

// CAN Frame Structure
typedef struct {
    uint32_t id;           // CAN ID (11-bit or 29-bit)
    uint8_t data[8];       // Data payload (0-8 bytes)
    uint8_t dlc;           // Data Length Code
    bool extended;         // Extended frame format
    bool remote;           // Remote transmission request
} CAN_Frame;

// Automotive CAN IDs (Real OBD-II and manufacturer-specific)
#define CAN_ID_OBD2_REQUEST         0x7DF
#define CAN_ID_OBD2_RESPONSE        0x7E8
#define CAN_ID_ENGINE_RPM           0x0C0
#define CAN_ID_VEHICLE_SPEED        0x0C1
#define CAN_ID_BRAKE_PRESSURE       0x1A0
#define CAN_ID_STEERING_ANGLE       0x260
#define CAN_ID_AIRBAG_STATUS        0x050
#define CAN_ID_DOOR_LOCKS           0x3C0
#define CAN_ID_IMMOBILIZER          0x3D0

// Tesla-specific CAN IDs (from reverse engineering)
#define CAN_ID_TESLA_CHARGE_PORT    0x2F2
#define CAN_ID_TESLA_DOOR_HANDLES   0x3D2
#define CAN_ID_TESLA_AUTOPILOT      0x108
#define CAN_ID_TESLA_BATTERY_TEMP   0x2A2

// BMW-specific CAN IDs
#define CAN_ID_BMW_KOMFORT          0x2FC
#define CAN_ID_BMW_CENTRAL_LOCKING  0x2F7
#define CAN_ID_BMW_IMMOBILIZER      0x153

// Mercedes-specific CAN IDs
#define CAN_ID_MERCEDES_SAM         0x1E5
#define CAN_ID_MERCEDES_KESSY       0x5C7

// Hardware initialization
bool can_init_mcp2515(PredatorApp* app);
bool can_set_bitrate(PredatorApp* app, uint32_t bitrate);
bool can_set_filter(PredatorApp* app, uint32_t filter, uint32_t mask);

// Frame transmission/reception
bool can_send_frame(PredatorApp* app, const CAN_Frame* frame);
bool can_receive_frame(PredatorApp* app, CAN_Frame* frame, uint32_t timeout_ms);

// OBD-II Functions
bool can_obd2_request(PredatorApp* app, uint8_t mode, uint8_t pid, uint8_t* response, size_t* len);
bool can_obd2_clear_codes(PredatorApp* app);
bool can_obd2_get_vin(PredatorApp* app, char* vin, size_t vin_len);

// Automotive Attacks
bool can_door_unlock_attack(PredatorApp* app, const char* manufacturer);
bool can_engine_start_attack(PredatorApp* app);
bool can_immobilizer_bypass(PredatorApp* app);
bool can_airbag_disable_attack(PredatorApp* app);

// Tesla-specific attacks
bool can_tesla_charge_port_unlock(PredatorApp* app);
bool can_tesla_door_handle_present(PredatorApp* app);
bool can_tesla_autopilot_spoof(PredatorApp* app);

// Security analysis
bool can_bus_security_scan(PredatorApp* app);
bool can_detect_gateway_filtering(PredatorApp* app);
bool can_replay_attack(PredatorApp* app, uint32_t target_id);
