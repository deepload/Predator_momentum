#pragma once

#include "../predator_i.h"

// OBD-II support for vehicle diagnostics
typedef enum {
    OBDModeCurrentData = 0x01,
    OBDModeFreezeFrame = 0x02,
    OBDModeReadDTC = 0x03,
    OBDModeClearDTC = 0x04,
    OBDModeOxygenSensor = 0x05,
    OBDModeTestResults = 0x06,
    OBDModePendingDTC = 0x07,
    OBDModeControlOperation = 0x08,
    OBDModeVehicleInfo = 0x09,
    OBDModePermanentDTC = 0x0A,
} OBDMode;

typedef struct {
    uint8_t pid;
    char name[32];
    char unit[8];
    float value;
    bool available;
} OBDParameter;

// OBD initialization
bool predator_obd_init(PredatorApp* app);
void predator_obd_deinit(PredatorApp* app);

// OBD operations
bool predator_obd_request(PredatorApp* app, uint8_t mode, uint8_t pid, uint8_t* response, size_t* response_len);
bool predator_obd_read_parameter(PredatorApp* app, uint8_t pid, OBDParameter* param);

// Common PIDs
#define OBD_PID_ENGINE_RPM 0x0C
#define OBD_PID_VEHICLE_SPEED 0x0D
#define OBD_PID_COOLANT_TEMP 0x05
#define OBD_PID_FUEL_LEVEL 0x2F
#define OBD_PID_THROTTLE_POS 0x11
#define OBD_PID_ENGINE_LOAD 0x04

// DTC operations
bool predator_obd_read_dtc(PredatorApp* app, char dtc_codes[][6], size_t* dtc_count);
bool predator_obd_clear_dtc(PredatorApp* app);

// Vehicle info
bool predator_obd_read_vin(PredatorApp* app, char* vin, size_t vin_len);
bool predator_obd_read_calibration_id(PredatorApp* app, char* cal_id, size_t cal_id_len);

// Live data monitoring
bool predator_obd_start_live_data(PredatorApp* app, const uint8_t* pids, size_t pid_count);
bool predator_obd_stop_live_data(PredatorApp* app);
bool predator_obd_get_live_data(PredatorApp* app, OBDParameter* params, size_t* param_count);
