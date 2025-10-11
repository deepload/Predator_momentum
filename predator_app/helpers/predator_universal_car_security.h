#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct PredatorApp PredatorApp;

// UNIVERSAL CAR SECURITY TESTING SYSTEM
// Government Requirement: Test ALL brands the same way
// Swiss Government + Chinese Brand Analysis

// Comprehensive manufacturer list (80+ brands)
typedef enum {
    // European Luxury
    CAR_AUDI, CAR_BMW, CAR_MERCEDES, CAR_PORSCHE, CAR_VOLKSWAGEN,
    CAR_VOLVO, CAR_JAGUAR, CAR_LAND_ROVER, CAR_BENTLEY, CAR_ROLLS_ROYCE,
    CAR_FERRARI, CAR_LAMBORGHINI, CAR_MASERATI, CAR_ALFA_ROMEO,
    CAR_ASTON_MARTIN, CAR_MCLAREN, CAR_LOTUS,
    
    // European Mass Market
    CAR_PEUGEOT, CAR_CITROEN, CAR_RENAULT, CAR_FIAT, CAR_SEAT,
    CAR_SKODA, CAR_OPEL, CAR_SAAB, CAR_LANCIA, CAR_DACIA,
    
    // American
    CAR_FORD, CAR_CHEVROLET, CAR_CADILLAC, CAR_BUICK, CAR_GMC,
    CAR_CHRYSLER, CAR_DODGE, CAR_JEEP, CAR_RAM, CAR_LINCOLN,
    CAR_TESLA, CAR_RIVIAN, CAR_LUCID, CAR_FISKER,
    
    // Japanese
    CAR_TOYOTA, CAR_LEXUS, CAR_HONDA, CAR_ACURA, CAR_NISSAN,
    CAR_INFINITI, CAR_MAZDA, CAR_SUBARU, CAR_MITSUBISHI, CAR_SUZUKI,
    CAR_ISUZU,
    
    // Korean
    CAR_HYUNDAI, CAR_KIA, CAR_GENESIS, CAR_SSANGYONG,
    
    // Chinese (Government Priority)
    CAR_BYD, CAR_NIO, CAR_XPENG, CAR_LI_AUTO, CAR_GEELY,
    CAR_GREAT_WALL, CAR_CHERY, CAR_JAC, CAR_SAIC, CAR_DONGFENG,
    CAR_FAW, CAR_BAIC, CAR_CHANGAN, CAR_HAVAL, CAR_WEY,
    CAR_LYNK_CO, CAR_POLESTAR, CAR_ZEEKR, CAR_ORA, CAR_TANK,
    CAR_HONGQI, CAR_ROEWE, CAR_MG_MOTOR, CAR_MAXUS, CAR_WELTMEISTER,
    
    // Indian
    CAR_TATA, CAR_MAHINDRA, CAR_MARUTI_SUZUKI,
    
    // Russian
    CAR_LADA, CAR_UAZ, CAR_GAZ,
    
    // Other
    CAR_DAIHATSU, CAR_PROTON, CAR_PERODUA,
    
    CAR_TOTAL_MANUFACTURERS
} UniversalCarManufacturer;

// Standardized Security Test Results
typedef struct {
    UniversalCarManufacturer manufacturer;
    char model[32];
    uint16_t year;
    
    // Standardized Test Results (0-100 scale)
    uint8_t key_fob_security;        // Key fob encryption strength
    uint8_t immobilizer_security;    // Immobilizer bypass resistance
    uint8_t can_bus_security;        // CAN bus protection level
    uint8_t smart_key_security;      // Smart key/proximity security
    uint8_t ota_security;           // Over-the-air update security
    uint8_t physical_security;       // Physical access protection
    
    // Overall Security Score (0-100)
    uint8_t overall_security_score;
    
    // Vulnerability Flags
    bool vulnerable_to_replay;
    bool vulnerable_to_relay;
    bool vulnerable_to_jamming;
    bool vulnerable_to_cloning;
    bool vulnerable_to_can_injection;
    
    // Attack Success Rates (0-100%)
    uint8_t unlock_success_rate;
    uint8_t start_success_rate;
    uint8_t immobilizer_bypass_rate;
    
    // Protocol Information
    uint32_t key_frequency;          // Hz
    char protocol_type[32];          // "KeeLoq", "Smart Key", "BLE", etc.
    char encryption_type[32];        // "None", "AES-128", "Proprietary"
    
    // Government Classification
    char security_rating[16];        // "CRITICAL", "HIGH", "MEDIUM", "LOW"
    bool government_approved;        // Meets security standards
    
} UniversalSecurityReport;

// Standardized Testing Functions
bool universal_test_key_fob_security(PredatorApp* app, UniversalCarManufacturer mfr, const char* model, uint16_t year, UniversalSecurityReport* report);
bool universal_test_immobilizer_security(PredatorApp* app, UniversalCarManufacturer mfr, const char* model, uint16_t year, UniversalSecurityReport* report);
bool universal_test_can_bus_security(PredatorApp* app, UniversalCarManufacturer mfr, const char* model, uint16_t year, UniversalSecurityReport* report);
bool universal_test_smart_key_security(PredatorApp* app, UniversalCarManufacturer mfr, const char* model, uint16_t year, UniversalSecurityReport* report);

// Comprehensive Testing
bool universal_test_all_security_aspects(PredatorApp* app, UniversalCarManufacturer mfr, const char* model, uint16_t year, UniversalSecurityReport* report);

// Comparison Functions
bool universal_compare_manufacturers(PredatorApp* app, UniversalCarManufacturer* manufacturers, size_t count);
bool universal_compare_chinese_vs_western(PredatorApp* app);
bool universal_generate_government_comparison_report(PredatorApp* app);

// Database Functions
const char* universal_get_manufacturer_name(UniversalCarManufacturer mfr);
bool universal_is_chinese_manufacturer(UniversalCarManufacturer mfr);
bool universal_is_european_manufacturer(UniversalCarManufacturer mfr);
bool universal_is_american_manufacturer(UniversalCarManufacturer mfr);
bool universal_is_japanese_manufacturer(UniversalCarManufacturer mfr);

// Government Reporting
bool universal_generate_swiss_government_report(PredatorApp* app);
bool universal_generate_security_ranking_report(PredatorApp* app);
bool universal_generate_chinese_brand_analysis(PredatorApp* app);
