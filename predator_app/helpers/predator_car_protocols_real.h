#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct PredatorApp PredatorApp;

// REAL 50+ CAR MANUFACTURER PROTOCOLS
// Professional Automotive Security Testing
// Government-Grade Vehicle Analysis

// Car Manufacturer Enum (50+ brands)
typedef enum {
    // Luxury European
    CAR_AUDI, CAR_BMW, CAR_MERCEDES, CAR_PORSCHE, CAR_VOLKSWAGEN,
    CAR_VOLVO, CAR_JAGUAR, CAR_LAND_ROVER, CAR_BENTLEY, CAR_ROLLS_ROYCE,
    CAR_FERRARI, CAR_LAMBORGHINI, CAR_MASERATI, CAR_ALFA_ROMEO,
    
    // American
    CAR_FORD, CAR_CHEVROLET, CAR_CADILLAC, CAR_BUICK, CAR_GMC,
    CAR_CHRYSLER, CAR_DODGE, CAR_JEEP, CAR_RAM, CAR_LINCOLN,
    CAR_TESLA, CAR_RIVIAN, CAR_LUCID,
    
    // Japanese
    CAR_TOYOTA, CAR_LEXUS, CAR_HONDA, CAR_ACURA, CAR_NISSAN,
    CAR_INFINITI, CAR_MAZDA, CAR_SUBARU, CAR_MITSUBISHI,
    
    // Korean
    CAR_HYUNDAI, CAR_KIA, CAR_GENESIS,
    
    // Chinese
    CAR_BYD, CAR_NIO, CAR_XPENG, CAR_LI_AUTO, CAR_GEELY,
    
    // Other European
    CAR_PEUGEOT, CAR_CITROEN, CAR_RENAULT, CAR_FIAT, CAR_SEAT,
    CAR_SKODA, CAR_OPEL, CAR_SAAB,
    
    CAR_MANUFACTURER_COUNT
} CarManufacturer;

// Key Fob Protocol Types
typedef enum {
    PROTOCOL_FIXED_CODE,        // Simple fixed code (old cars)
    PROTOCOL_ROLLING_CODE,      // KeeLoq, HCS series
    PROTOCOL_CHALLENGE_RESPONSE,// Immobilizer systems
    PROTOCOL_SMART_KEY,         // Proximity/push-button start
    PROTOCOL_BLE_KEY,          // Bluetooth Low Energy
    PROTOCOL_NFC_KEY,          // Near Field Communication
    PROTOCOL_UWB_KEY,          // Ultra-Wideband (Tesla, BMW)
} KeyProtocol;

// Car Security Information
typedef struct {
    CarManufacturer manufacturer;
    char model[32];
    uint16_t year_start;
    uint16_t year_end;
    KeyProtocol protocol;
    uint32_t frequency;         // Hz
    uint8_t key_length;         // bits
    uint64_t manufacturer_key;  // For rolling code
    bool immobilizer;
    bool smart_key;
    uint8_t vulnerability_score; // 1-10 (10 = most vulnerable)
    char notes[128];
} CarSecurityInfo;

// Protocol-specific functions
bool car_protocol_fixed_code_attack(PredatorApp* app, uint32_t code, uint32_t frequency);
bool car_protocol_rolling_code_attack(PredatorApp* app, CarManufacturer mfr, uint32_t captured_code);
bool car_protocol_smart_key_relay(PredatorApp* app, CarManufacturer mfr);
bool car_protocol_ble_key_attack(PredatorApp* app, CarManufacturer mfr);

// Manufacturer-specific implementations
bool car_attack_bmw(PredatorApp* app, const char* model, uint16_t year);
bool car_attack_mercedes(PredatorApp* app, const char* model, uint16_t year);
bool car_attack_audi(PredatorApp* app, const char* model, uint16_t year);
bool car_attack_tesla(PredatorApp* app, const char* model, uint16_t year);
bool car_attack_toyota(PredatorApp* app, const char* model, uint16_t year);
bool car_attack_ford(PredatorApp* app, const char* model, uint16_t year);
bool car_attack_honda(PredatorApp* app, const char* model, uint16_t year);
bool car_attack_hyundai(PredatorApp* app, const char* model, uint16_t year);

// Database functions
const CarSecurityInfo* car_get_security_info(CarManufacturer mfr, const char* model, uint16_t year);
bool car_load_manufacturer_database(PredatorApp* app);
size_t car_get_supported_manufacturers(CarManufacturer* manufacturers, size_t max_count);

// Analysis functions
bool car_analyze_all_manufacturers(PredatorApp* app);
bool car_vulnerability_assessment(PredatorApp* app, CarManufacturer mfr);
bool car_generate_manufacturer_report(PredatorApp* app);
