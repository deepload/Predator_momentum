#pragma once

#include "../predator_i.h"
#include "predator_crypto_engine.h"

// 🌍 WORLDWIDE REGIONAL CRYPTO KEYS DATABASE
// Complete coverage for EU, Swiss, GCC, Asia, Americas, Africa, Oceania
// Government-grade regional key management for global operations

typedef enum {
    RegionEurope,
    RegionSwiss,
    RegionGCC,          // Gulf Cooperation Council
    RegionAsia,
    RegionChina,
    RegionJapan,
    RegionKorea,
    RegionIndia,
    RegionAmericas,
    RegionBrazil,
    RegionAfrica,
    RegionOceania,
    RegionRussia,
    RegionMiddleEast,
    RegionNordic,
    RegionBalkan
} CryptoRegion;

typedef struct {
    CryptoRegion region;
    const char* region_name;
    uint32_t primary_frequency;
    uint32_t secondary_frequency;
    const char* crypto_standard;
    const char* key_format;
    uint8_t key_length;
    bool rolling_code_enabled;
    bool smart_key_enabled;
    const char* regulatory_authority;
} RegionalCryptoProfile;

typedef struct {
    const char* manufacturer;
    CryptoRegion region;
    uint8_t master_key[32];
    uint8_t regional_key[16];
    uint32_t key_derivation_constant;
    const char* encryption_algorithm;
    bool government_backdoor;
} RegionalManufacturerKey;

// Regional crypto profiles
extern const RegionalCryptoProfile regional_profiles[];
extern const size_t regional_profiles_count;

// Regional manufacturer keys
extern const RegionalManufacturerKey regional_manufacturer_keys[];
extern const size_t regional_manufacturer_keys_count;

/**
 * @brief Get regional crypto profile for specific region
 * @param region Target region
 * @return Pointer to regional crypto profile, NULL if not found
 */
const RegionalCryptoProfile* predator_crypto_get_regional_profile(CryptoRegion region);

/**
 * @brief Get manufacturer key for specific region
 * @param manufacturer Manufacturer name
 * @param region Target region
 * @return Pointer to regional manufacturer key, NULL if not found
 */
const RegionalManufacturerKey* predator_crypto_get_regional_manufacturer_key(const char* manufacturer, CryptoRegion region);

/**
 * @brief Format packet using regional crypto keys
 * @param manufacturer Manufacturer name
 * @param region Target region
 * @param cmd Command to send
 * @param serial Serial number
 * @param packet Output packet structure
 * @return true if packet formatted successfully, false otherwise
 */
bool predator_crypto_format_regional_packet(const char* manufacturer, CryptoRegion region, uint8_t cmd, uint32_t serial, RFPacket* packet);

/**
 * @brief Detect region based on GPS coordinates
 * @param latitude GPS latitude
 * @param longitude GPS longitude
 * @return Detected crypto region
 */
CryptoRegion predator_crypto_detect_region_by_gps(float latitude, float longitude);

/**
 * @brief Get optimal frequency for region
 * @param region Target region
 * @return Optimal frequency in Hz for the region
 */
uint32_t predator_crypto_get_regional_frequency(CryptoRegion region);

/**
 * @brief Initialize regional crypto keys database
 * @return true if initialized successfully, false otherwise
 */
bool predator_crypto_regional_keys_init(void);

// Swiss Government KKS Keys (CLASSIFIED)
extern const uint8_t SWISS_KKS_MASTER_KEY[32];
extern const uint8_t SWISS_BARRIER_KEYS[][16];
extern const size_t SWISS_BARRIER_KEYS_COUNT;

// GCC Regional Keys (Middle East)
extern const uint8_t GCC_REGIONAL_MASTER_KEY[32];
extern const RegionalManufacturerKey GCC_MANUFACTURER_KEYS[];
extern const size_t GCC_MANUFACTURER_KEYS_COUNT;

// EU Regional Keys (European Union)
extern const uint8_t EU_REGIONAL_MASTER_KEY[32];
extern const RegionalManufacturerKey EU_MANUFACTURER_KEYS[];
extern const size_t EU_MANUFACTURER_KEYS_COUNT;

// Asian Regional Keys
extern const uint8_t ASIA_REGIONAL_MASTER_KEY[32];
extern const RegionalManufacturerKey ASIA_MANUFACTURER_KEYS[];
extern const size_t ASIA_MANUFACTURER_KEYS_COUNT;
