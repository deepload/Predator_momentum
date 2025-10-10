#pragma once

#include "../predator_i.h"

// Infrared universal remote support
typedef enum {
    IRProtocolNEC,
    IRProtocolSamsung,
    IRProtocolSony,
    IRProtocolRC5,
    IRProtocolRC6,
    IRProtocolSIRC,
    IRProtocolRaw,
} IRProtocol;

typedef struct {
    IRProtocol protocol;
    uint32_t address;
    uint32_t command;
    uint32_t* timings;
    size_t timing_count;
    uint32_t frequency;
} IRSignal;

// IR initialization
bool predator_ir_init(PredatorApp* app);
void predator_ir_deinit(PredatorApp* app);

// IR capture
bool predator_ir_start_capture(PredatorApp* app);
bool predator_ir_stop_capture(PredatorApp* app);
bool predator_ir_get_captured_signal(PredatorApp* app, IRSignal* signal);

// IR transmission
bool predator_ir_transmit_signal(PredatorApp* app, const IRSignal* signal);
bool predator_ir_transmit_nec(PredatorApp* app, uint32_t address, uint32_t command);
bool predator_ir_transmit_samsung(PredatorApp* app, uint32_t address, uint32_t command);
bool predator_ir_transmit_sony(PredatorApp* app, uint32_t address, uint32_t command);

// IR database
typedef enum {
    IRDeviceTV,
    IRDeviceAC,
    IRDeviceProjector,
    IRDeviceDVD,
    IRDeviceAudio,
    IRDeviceFan,
} IRDeviceType;

typedef struct {
    char brand[32];
    IRDeviceType type;
    IRProtocol protocol;
    uint32_t power_on_code;
    uint32_t power_off_code;
    uint32_t vol_up_code;
    uint32_t vol_down_code;
} IRDeviceProfile;

bool predator_ir_load_device_profile(PredatorApp* app, const char* brand, IRDeviceType type, IRDeviceProfile* profile);
bool predator_ir_brute_force_power(PredatorApp* app, IRDeviceType type);

// IR protocol analysis
IRProtocol predator_ir_detect_protocol(const uint32_t* timings, size_t count);
bool predator_ir_decode_signal(const IRSignal* signal, uint32_t* address, uint32_t* command);
