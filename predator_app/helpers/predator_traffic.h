#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct PredatorApp PredatorApp;

// Traffic light signal types
typedef enum {
    TrafficLightRed,
    TrafficLightYellow,
    TrafficLightGreen,
    TrafficLightEmergency,
} TrafficLightSignal;

// Traffic protocols
typedef enum {
    TrafficProtocol_3M_Opticom,      // Emergency vehicle preemption
    TrafficProtocol_GTT_Canoga,      // Traffic management
    TrafficProtocol_NTCIP,           // National Transportation Communications
    TrafficProtocol_Railroad,        // Railroad crossing
    TrafficProtocol_Toll_DSRC,       // Dedicated Short Range Communications
} TrafficProtocol;

// Traffic signal data
typedef struct {
    uint32_t intersection_id;
    TrafficLightSignal current_state;
    uint32_t timing_ms;
    uint8_t phase;
    bool emergency_mode;
} TrafficSignalData;

// Initialization
bool predator_traffic_init(PredatorApp* app);
void predator_traffic_deinit(PredatorApp* app);

// Signal analysis
bool predator_traffic_scan_signals(PredatorApp* app, uint32_t frequency);
bool predator_traffic_decode_signal(PredatorApp* app, const uint8_t* data, size_t len, TrafficSignalData* signal);
bool predator_traffic_detect_protocol(PredatorApp* app, const uint8_t* data, size_t len, TrafficProtocol* protocol);

// Emergency vehicle preemption (Opticom)
bool predator_traffic_test_opticom(PredatorApp* app, uint32_t intersection_id);
bool predator_traffic_emergency_preemption(PredatorApp* app, bool enable);

// Railroad crossing
bool predator_traffic_railroad_detect(PredatorApp* app);
bool predator_traffic_railroad_timing(PredatorApp* app, uint32_t* gate_delay_ms);

// Toll booth DSRC
bool predator_traffic_toll_scan(PredatorApp* app);
bool predator_traffic_toll_analyze(PredatorApp* app, uint8_t* transponder_id, size_t id_len);

// Traffic light timing
bool predator_traffic_analyze_timing(PredatorApp* app, uint32_t intersection_id, uint32_t* red_ms, uint32_t* green_ms);
bool predator_traffic_detect_pattern(PredatorApp* app, const char* pattern_name, size_t name_len);
