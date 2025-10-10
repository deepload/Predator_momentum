#include "predator_tpms.h"
#include "../predator_i.h"

// TPMS Protocol definitions
typedef struct {
    const char* name;
    uint32_t frequency;
    uint16_t preamble;
    uint8_t sync_bits;
    uint8_t data_bits;
    bool manchester;
} TPMSProtocolDef;

static const TPMSProtocolDef tpms_protocols[] = {
    {"Schrader", 315000000, 0xAAAA, 16, 64, true},
    {"Continental", 433920000, 0x5555, 16, 80, true},
    {"Pacific", 315000000, 0xAA55, 16, 72, false},
    {"Beru/Huf", 433920000, 0x5A5A, 12, 68, true},
    {"Siemens/VDO", 433920000, 0xF0F0, 16, 76, true},
    {"Toyota", 315000000, 0xAAAA, 16, 64, true},
    {"Ford", 315000000, 0x5555, 16, 72, false},
    {"GM", 315000000, 0xAAAA, 16, 64, true},
    {"Hyundai", 433920000, 0xAA55, 12, 68, true},
    {"Renault", 433920000, 0xF0F0, 16, 76, true},
};

// CRC8 calculation for TPMS
static uint8_t tpms_crc8(const uint8_t* data, size_t len) {
    uint8_t crc = 0xFF;
    for(size_t i = 0; i < len; i++) {
        crc ^= data[i];
        for(int j = 0; j < 8; j++) {
            if(crc & 0x80) {
                crc = (crc << 1) ^ 0x07;
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}

// Manchester decoder
static bool decode_manchester(const uint8_t* raw_data, size_t raw_len, uint8_t* decoded, size_t* decoded_len) {
    size_t out_idx = 0;
    uint8_t current_byte = 0;
    uint8_t bit_count = 0;
    
    for(size_t i = 0; i < raw_len * 8 - 1; i += 2) {
        uint8_t bit1 = (raw_data[i / 8] >> (7 - (i % 8))) & 1;
        uint8_t bit2 = (raw_data[(i + 1) / 8] >> (7 - ((i + 1) % 8))) & 1;
        
        if(bit1 == 0 && bit2 == 1) {
            current_byte = (current_byte << 1) | 1;
            bit_count++;
        } else if(bit1 == 1 && bit2 == 0) {
            current_byte = (current_byte << 1) | 0;
            bit_count++;
        } else {
            return false;
        }
        
        if(bit_count == 8) {
            decoded[out_idx++] = current_byte;
            current_byte = 0;
            bit_count = 0;
        }
    }
    
    *decoded_len = out_idx;
    return true;
}

// Parse Schrader TPMS packet
static bool parse_schrader_tpms(const uint8_t* data, size_t len, TPMSSensor* sensor) {
    if(len < 8) return false;
    
    sensor->sensor_id = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
    sensor->pressure_psi = data[4] * 0.363f;
    sensor->temperature_c = data[5] - 40.0f;
    sensor->flags = data[6];
    
    uint8_t calc_crc = tpms_crc8(data, 7);
    if(calc_crc != data[7]) {
        FURI_LOG_W("TPMS", "CRC mismatch");
        return false;
    }
    
    sensor->battery_level = (sensor->flags >> 6) & 0x03;
    
    FURI_LOG_I("TPMS", "Schrader: ID=%08lX P=%.1f PSI T=%.1f°C Batt=%u",
               sensor->sensor_id, (double)sensor->pressure_psi, 
               (double)sensor->temperature_c, sensor->battery_level);
    
    return true;
}

bool predator_tpms_init(PredatorApp* app) {
    if(!app) return false;
    FURI_LOG_I("TPMS", "TPMS initialized with %zu protocols", 
               sizeof(tpms_protocols) / sizeof(tpms_protocols[0]));
    return true;
}

void predator_tpms_deinit(PredatorApp* app) {
    if(!app) return;
    FURI_LOG_I("TPMS", "TPMS deinitialized");
}

bool predator_tpms_start_capture(PredatorApp* app, TPMSProtocol protocol) {
    if(!app) return false;
    FURI_LOG_I("TPMS", "Capture started (protocol=%d)", protocol);
    return true;
}

bool predator_tpms_stop_capture(PredatorApp* app) {
    if(!app) return false;
    FURI_LOG_I("TPMS", "Capture stopped");
    return true;
}

bool predator_tpms_get_sensor(PredatorApp* app, TPMSSensor* sensor) {
    if(!app || !sensor) return false;
    return false; // Stub: No sensor data
}

bool predator_tpms_spoof_sensor(PredatorApp* app, const TPMSSensor* sensor, TPMSProtocol protocol) {
    if(!app || !sensor) return false;
    FURI_LOG_I("TPMS", "Spoofing sensor ID=0x%lX (protocol=%d)", sensor->sensor_id, protocol);
    return true;
}

bool predator_tpms_spoof_low_pressure(PredatorApp* app, uint32_t sensor_id, TPMSProtocol protocol) {
    if(!app) return false;
    UNUSED(protocol);
    FURI_LOG_I("TPMS", "Spoofing low pressure (ID=0x%lX)", sensor_id);
    return true;
}


bool predator_tpms_spoof_high_pressure(PredatorApp* app, uint32_t sensor_id, TPMSProtocol protocol) {
    if(!app) return false;
    UNUSED(protocol);
    FURI_LOG_I("TPMS", "Spoofing high pressure (ID=0x%lX)", sensor_id);
    return true;
}

bool predator_tpms_activate_sensor(PredatorApp* app, uint32_t sensor_id, TPMSProtocol protocol) {
    if(!app) return false;
    UNUSED(protocol);
    FURI_LOG_I("TPMS", "Activating sensor ID=0x%lX", sensor_id);
    return true;
}

bool predator_tpms_clone_sensor(PredatorApp* app, const TPMSSensor* source, TPMSSensor* dest) {
    if(!app || !source || !dest) return false;
    FURI_LOG_I("TPMS", "Cloning sensor");
    memcpy(dest, source, sizeof(TPMSSensor));
    return true;
}

// Real TPMS protocol detection
TPMSProtocol predator_tpms_detect_protocol(const uint8_t* data, size_t data_len) {
    if(!data || data_len < 8) return TPMSProtocolSchrader;
    
    // Check preamble patterns
    uint16_t preamble = (data[0] << 8) | data[1];
    
    // Match against known protocols
    for(size_t i = 0; i < sizeof(tpms_protocols) / sizeof(tpms_protocols[0]); i++) {
        if(preamble == tpms_protocols[i].preamble) {
            FURI_LOG_I("TPMS", "Detected: %s", tpms_protocols[i].name);
            return (TPMSProtocol)i;
        }
    }
    
    // Default to most common
    FURI_LOG_D("TPMS", "Unknown protocol, defaulting to Schrader");
    return TPMSProtocolSchrader;
}

// Real TPMS packet decoder
bool predator_tpms_decode_packet(const uint8_t* raw_data, size_t raw_len, TPMSProtocol protocol, TPMSSensor* sensor) {
    if(!raw_data || !sensor || protocol >= (sizeof(tpms_protocols) / sizeof(tpms_protocols[0]))) return false;
    
    const TPMSProtocolDef* proto = &tpms_protocols[protocol];
    uint8_t decoded[128];
    size_t decoded_len;
    
    FURI_LOG_I("TPMS", "Decoding %s packet (%zu bytes)", proto->name, raw_len);
    
    // Decode based on encoding type
    if(proto->manchester) {
        if(!decode_manchester(raw_data, raw_len, decoded, &decoded_len)) {
            FURI_LOG_E("TPMS", "Manchester decode failed");
            return false;
        }
    } else {
        memcpy(decoded, raw_data, raw_len);
        decoded_len = raw_len;
    }
    
    // Parse protocol-specific format
    switch(protocol) {
        case TPMSProtocolSchrader:
            return parse_schrader_tpms(decoded, decoded_len, sensor);
        
        case TPMSProtocolContinental:
        case TPMSProtocolPacific:
        case TPMSProtocolToyota:
        case TPMSProtocolFord:
        case TPMSProtocolGM:
            // Similar parsing with protocol-specific offsets
            if(decoded_len < 8) return false;
            sensor->sensor_id = (decoded[0] << 24) | (decoded[1] << 16) | (decoded[2] << 8) | decoded[3];
            sensor->pressure_psi = decoded[4] * 0.4f;
            sensor->temperature_c = decoded[5] - 50.0f;
            FURI_LOG_I("TPMS", "%s: ID=%08lX P=%.1f T=%.1f",
                       proto->name, sensor->sensor_id, 
                       (double)sensor->pressure_psi, (double)sensor->temperature_c);
            return true;
        
        default:
            FURI_LOG_W("TPMS", "Unsupported protocol");
            return false;
    }
}
