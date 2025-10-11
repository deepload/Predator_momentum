#pragma once

#include "../predator_i.h"

// CAN bus support for automotive testing
typedef struct {
    uint32_t id;
    uint8_t data[8];
    uint8_t dlc;
    bool extended;
    bool rtr;
} CANFrame;

typedef enum {
    CAN_125KBPS = 125000,
    CAN_250KBPS = 250000,
    CAN_500KBPS = 500000,
    CAN_1MBPS = 1000000,
} CANSpeed;

// CAN bus initialization
bool predator_can_init(PredatorApp* app, CANSpeed speed);
void predator_can_deinit(PredatorApp* app);

// CAN operations
bool predator_can_send_frame(PredatorApp* app, const CANFrame* frame);
bool predator_can_receive_frame(PredatorApp* app, CANFrame* frame, uint32_t timeout_ms);
bool predator_can_set_filter(PredatorApp* app, uint32_t id, uint32_t mask);

// CAN sniffing
bool predator_can_start_capture(PredatorApp* app);
bool predator_can_stop_capture(PredatorApp* app);
size_t predator_can_get_captured_count(PredatorApp* app);

// CAN replay
bool predator_can_replay_frame(PredatorApp* app, const CANFrame* frame, uint32_t repeat_count);
bool predator_can_replay_sequence(PredatorApp* app, const CANFrame* frames, size_t count);

// CAN fuzzing
bool predator_can_fuzz_frame(PredatorApp* app, uint32_t id, uint8_t mutation_rate);

// UDS (Unified Diagnostic Services)
typedef enum {
    UDSServiceDiagnosticSession = 0x10,
    UDSServiceECUReset = 0x11,
    UDSServiceReadDTC = 0x19,
    UDSServiceReadDataByID = 0x22,
    UDSServiceSecurityAccess = 0x27,
    UDSServiceWriteDataByID = 0x2E,
    UDSServiceRoutineControl = 0x31,
} UDSService;

bool predator_can_uds_request(PredatorApp* app, uint8_t service, const uint8_t* data, size_t data_len, uint8_t* response, size_t* response_len);
bool predator_can_uds_read_vin(PredatorApp* app, char* vin, size_t vin_len);
bool predator_can_uds_read_dtc(PredatorApp* app, uint16_t* dtc_codes, size_t* dtc_count);
