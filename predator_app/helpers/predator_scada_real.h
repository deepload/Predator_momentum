#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct PredatorApp PredatorApp;

// REAL SCADA/MODBUS IMPLEMENTATION
// Critical Infrastructure Security Testing
// Government-Grade Industrial Protocol Analysis

// Modbus Function Codes
#define MODBUS_READ_COILS           0x01
#define MODBUS_READ_DISCRETE        0x02
#define MODBUS_READ_HOLDING_REG     0x03
#define MODBUS_READ_INPUT_REG       0x04
#define MODBUS_WRITE_SINGLE_COIL    0x05
#define MODBUS_WRITE_SINGLE_REG     0x06
#define MODBUS_WRITE_MULTIPLE_COILS 0x0F
#define MODBUS_WRITE_MULTIPLE_REG   0x10

// DNP3 Function Codes
#define DNP3_READ                   0x01
#define DNP3_WRITE                  0x02
#define DNP3_SELECT                 0x03
#define DNP3_OPERATE                0x04
#define DNP3_DIRECT_OPERATE         0x05

// IEC 61850 GOOSE Message Types
#define IEC61850_GOOSE_TYPE_BOOLEAN 0x83
#define IEC61850_GOOSE_TYPE_INTEGER 0x85
#define IEC61850_GOOSE_TYPE_FLOAT   0x87

// SCADA Device Types
typedef enum {
    SCADA_PLC,              // Programmable Logic Controller
    SCADA_RTU,              // Remote Terminal Unit
    SCADA_HMI,              // Human Machine Interface
    SCADA_HISTORIAN,        // Data Historian
    SCADA_GATEWAY,          // Protocol Gateway
    SCADA_SUBSTATION,       // Electrical Substation
    SCADA_PUMP_STATION,     // Water/Oil Pump Station
    SCADA_GENERATOR,        // Power Generator
} SCADA_DeviceType;

// SCADA Device Information
typedef struct {
    uint32_t device_id;
    SCADA_DeviceType type;
    char ip_address[16];
    uint16_t port;
    char vendor[32];
    char model[32];
    char firmware[16];
    bool authenticated;
    uint8_t security_level;
    char location[64];
} SCADA_Device;

// Modbus Communication
bool modbus_tcp_connect(PredatorApp* app, const char* ip, uint16_t port);
bool modbus_read_registers(PredatorApp* app, uint8_t unit_id, uint16_t address, uint16_t count, uint16_t* data);
bool modbus_write_register(PredatorApp* app, uint8_t unit_id, uint16_t address, uint16_t value);
bool modbus_read_coils(PredatorApp* app, uint8_t unit_id, uint16_t address, uint16_t count, uint8_t* data);

// DNP3 Communication
bool dnp3_connect(PredatorApp* app, const char* ip, uint16_t port);
bool dnp3_read_data(PredatorApp* app, uint16_t src_addr, uint16_t dst_addr, uint8_t function);
bool dnp3_write_data(PredatorApp* app, uint16_t src_addr, uint16_t dst_addr, uint16_t address, uint32_t value);

// Device Discovery
bool scada_discover_devices(PredatorApp* app, const char* network_range, SCADA_Device* devices, size_t* count);
bool scada_fingerprint_device(PredatorApp* app, const char* ip, SCADA_Device* device);

// Security Testing
bool scada_security_scan(PredatorApp* app, const char* ip);
bool scada_test_authentication(PredatorApp* app, const char* ip, uint16_t port);
bool scada_check_encryption(PredatorApp* app, const char* ip);

// Attack Simulations (Authorized Testing Only)
bool scada_unauthorized_read(PredatorApp* app, const char* ip);
bool scada_unauthorized_write(PredatorApp* app, const char* ip);
bool scada_replay_attack(PredatorApp* app, const char* ip);

// Critical Infrastructure Analysis
bool scada_power_grid_analysis(PredatorApp* app);
bool scada_water_system_analysis(PredatorApp* app);
bool scada_manufacturing_analysis(PredatorApp* app);
