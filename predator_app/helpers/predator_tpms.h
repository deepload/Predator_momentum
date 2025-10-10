#pragma once

#include "../predator_i.h"

// TPMS (Tire Pressure Monitoring System) support
typedef struct {
    uint32_t sensor_id;
    float pressure_psi;
    float temperature_c;
    uint8_t battery_level;
    uint8_t flags;
} TPMSSensor;

typedef enum {
    TPMSProtocolSchrader,
    TPMSProtocolContinental,
    TPMSProtocolPacific,
    TPMSProtocolBeru,
    TPMSProtocolSiemens,
    TPMSProtocolToyota,
    TPMSProtocolFord,
    TPMSProtocolGM,
    TPMSProtocolHyundai,
    TPMSProtocolRenault,
} TPMSProtocol;

// TPMS operations
bool predator_tpms_init(PredatorApp* app);
void predator_tpms_deinit(PredatorApp* app);

// TPMS capture
bool predator_tpms_start_capture(PredatorApp* app, TPMSProtocol protocol);
bool predator_tpms_stop_capture(PredatorApp* app);
bool predator_tpms_get_sensor(PredatorApp* app, TPMSSensor* sensor);

// TPMS spoofing
bool predator_tpms_spoof_sensor(PredatorApp* app, const TPMSSensor* sensor, TPMSProtocol protocol);
bool predator_tpms_spoof_low_pressure(PredatorApp* app, uint32_t sensor_id, TPMSProtocol protocol);
bool predator_tpms_spoof_high_pressure(PredatorApp* app, uint32_t sensor_id, TPMSProtocol protocol);

// TPMS activation
bool predator_tpms_activate_sensor(PredatorApp* app, uint32_t sensor_id, TPMSProtocol protocol);

// TPMS cloning
bool predator_tpms_clone_sensor(PredatorApp* app, const TPMSSensor* source, TPMSSensor* dest);

// Protocol detection
TPMSProtocol predator_tpms_detect_protocol(const uint8_t* data, size_t data_len);
