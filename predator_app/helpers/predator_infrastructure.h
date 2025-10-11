#pragma once
#include <stdint.h>
#include <stdbool.h>

typedef struct PredatorApp PredatorApp;

// Infrastructure protocols
typedef enum {
    InfraProtocol_IEC61850,     // Power grid substation automation
    InfraProtocol_DNP3,         // Distributed Network Protocol (utilities)
    InfraProtocol_Modbus,       // Industrial control (most common)
    InfraProtocol_Profibus,     // Manufacturing automation
    InfraProtocol_EAS,          // Emergency Alert System
    InfraProtocol_SCADA,        // Supervisory Control and Data Acquisition
} InfrastructureProtocol;

// Critical systems
typedef enum {
    CriticalSystem_PowerGrid,
    CriticalSystem_WaterTreatment,
    CriticalSystem_GasPipeline,
    CriticalSystem_NuclearFacility,
    CriticalSystem_Dam,
    CriticalSystem_Airport,
    CriticalSystem_Hospital,
    CriticalSystem_Military,
} CriticalSystemType;

// Initialization
bool predator_infra_init(PredatorApp* app);
void predator_infra_deinit(PredatorApp* app);

// Protocol detection
bool predator_infra_scan_protocols(PredatorApp* app);
bool predator_infra_protocol_detect(PredatorApp* app, const uint8_t* data, size_t len, InfrastructureProtocol* protocol);

// Power grid (IEC 61850, DNP3)
bool predator_infra_power_grid_scan(PredatorApp* app);
bool predator_infra_power_grid_status(PredatorApp* app, bool* operational);
bool predator_infra_substation_enumerate(PredatorApp* app);
bool predator_infra_circuit_breaker_status(PredatorApp* app, uint32_t breaker_id, bool* closed);

// Water system SCADA
bool predator_infra_water_system_scan(PredatorApp* app);
bool predator_infra_water_flow_monitor(PredatorApp* app, float* flow_rate);
bool predator_infra_water_valve_status(PredatorApp* app, uint32_t valve_id, bool* open);
bool predator_infra_water_quality_test(PredatorApp* app);

// Industrial control (Modbus/Profibus)
bool predator_infra_industrial_scan(PredatorApp* app);
bool predator_infra_plc_enumerate(PredatorApp* app);
bool predator_infra_modbus_read_coils(PredatorApp* app, uint16_t address, uint16_t count);
bool predator_infra_modbus_read_registers(PredatorApp* app, uint16_t address, uint16_t* values, size_t count);

// Emergency Alert System
bool predator_infra_eas_scan(PredatorApp* app);
bool predator_infra_eas_header_decode(PredatorApp* app, const uint8_t* header, size_t len);
bool predator_infra_eas_test_signal(PredatorApp* app);

// Gas pipeline monitoring
bool predator_infra_gas_pipeline_scan(PredatorApp* app);
bool predator_infra_gas_pressure_monitor(PredatorApp* app, float* pressure_psi);
bool predator_infra_gas_leak_detection(PredatorApp* app);

// Dam/hydroelectric
bool predator_infra_dam_control_scan(PredatorApp* app);
bool predator_infra_dam_gate_status(PredatorApp* app, uint8_t gate_id, float* position_percent);
bool predator_infra_dam_water_level(PredatorApp* app, float* level_meters);

// Airport ground control
bool predator_infra_airport_scan(PredatorApp* app);
bool predator_infra_airport_lighting_control(PredatorApp* app);
bool predator_infra_airport_radar_test(PredatorApp* app);

// Hospital systems
bool predator_infra_hospital_scan(PredatorApp* app);
bool predator_infra_medical_device_enumerate(PredatorApp* app);
bool predator_infra_hvac_control_test(PredatorApp* app);
