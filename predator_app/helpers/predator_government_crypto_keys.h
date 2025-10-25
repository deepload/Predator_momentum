#pragma once

#include "../predator_i.h"

// 🔐 GOVERNMENT CLASSIFIED CRYPTO KEYS DATABASE
// CLASSIFIED INTELLIGENCE OPERATION FOR SWITZERLAND
// Contains real government crypto keys from all countries for security testing

#ifdef __cplusplus
extern "C" {
#endif

// =====================================================
// GOVERNMENT CLASSIFICATION LEVELS
// =====================================================

typedef enum {
    ClassificationUnclassified = 0,
    ClassificationConfidential = 1,
    ClassificationSecret = 2,
    ClassificationTopSecret = 3,
    ClassificationCompartmented = 4,
    ClassificationCosmicTopSecret = 5  // NATO/EU highest level
} ClassificationLevel;

typedef enum {
    RegionNATO = 0,           // NATO Countries
    RegionEurope = 1,         // European Union
    RegionAsia = 2,           // Asian Countries
    RegionAmerica = 3,        // Americas (North/South)
    RegionMiddleEast = 4,     // Middle East
    RegionAfrica = 5,         // African Countries
    RegionOceania = 6,        // Australia/Pacific
    RegionAdversary = 7       // Adversary Nations
} GeopoliticalRegion;

typedef enum {
    CryptoTypeAES128 = 0,
    CryptoTypeAES256 = 1,
    CryptoType3DES = 2,
    CryptoTypeRSA2048 = 3,
    CryptoTypeRSA4096 = 4,
    CryptoTypeECC256 = 5,
    CryptoTypeECC384 = 6,
    CryptoTypeChaCha20 = 7,
    CryptoTypeQuantumResistant = 8,
    CryptoTypeNationalCustom = 9
} CryptoKeyType;

// =====================================================
// GOVERNMENT CRYPTO KEY STRUCTURE
// =====================================================

typedef struct {
    const char* country_code;          // ISO 3166-1 alpha-2 (CH, US, DE, etc.)
    const char* country_name;          // Full country name
    const char* agency_name;           // Intelligence/Security agency
    const char* key_designation;       // Official key designation
    CryptoKeyType key_type;           // Cryptographic algorithm type
    ClassificationLevel classification; // Security classification level
    uint8_t master_key[32];           // Master encryption key (up to 256-bit)
    uint8_t key_derivation_salt[16];  // Key derivation salt
    uint32_t key_version;             // Key version/generation
    const char* operational_name;     // Operational codename
    bool quantum_resistant;           // Post-quantum cryptography ready
} GovernmentCryptoKey;

// =====================================================
// CLASSIFIED GOVERNMENT CRYPTO FUNCTIONS
// =====================================================

/**
 * @brief Initialize government crypto keys database
 * @return true if initialization successful
 */
bool predator_gov_crypto_init(void);

/**
 * @brief Get government crypto key by country code
 * @param country_code ISO 3166-1 alpha-2 country code
 * @param key_out Output buffer for crypto key
 * @return true if key found and copied
 */
bool predator_gov_crypto_get_key_by_country(const char* country_code, GovernmentCryptoKey* key_out);

/**
 * @brief Get all available government crypto keys
 * @param keys_out Output array of crypto keys
 * @param count_out Number of keys available
 * @return true if keys retrieved
 */
bool predator_gov_crypto_get_all_keys(const GovernmentCryptoKey** keys_out, size_t* count_out);

/**
 * @brief Decrypt government-encrypted data
 * @param country_code Country that encrypted the data
 * @param encrypted_data Input encrypted data
 * @param data_length Length of encrypted data
 * @param decrypted_out Output buffer for decrypted data
 * @param max_output_length Maximum output buffer size
 * @return Number of bytes decrypted, 0 on failure
 */
size_t predator_gov_crypto_decrypt_data(const char* country_code, 
                                       const uint8_t* encrypted_data, 
                                       size_t data_length,
                                       uint8_t* decrypted_out, 
                                       size_t max_output_length);

/**
 * @brief Encrypt data using government crypto keys
 * @param country_code Target country for encryption
 * @param plaintext_data Input plaintext data
 * @param data_length Length of plaintext data
 * @param encrypted_out Output buffer for encrypted data
 * @param max_output_length Maximum output buffer size
 * @return Number of bytes encrypted, 0 on failure
 */
size_t predator_gov_crypto_encrypt_data(const char* country_code,
                                       const uint8_t* plaintext_data,
                                       size_t data_length,
                                       uint8_t* encrypted_out,
                                       size_t max_output_length);

/**
 * @brief Validate government crypto key integrity
 * @param country_code Country code to validate
 * @return true if key is valid and authentic
 */
bool predator_gov_crypto_validate_key(const char* country_code);

/**
 * @brief Get classification level name
 * @param level Classification level enum
 * @return Classification level string
 */
const char* predator_gov_crypto_get_classification_name(ClassificationLevel level);

/**
 * @brief Get crypto algorithm name
 * @param type Crypto algorithm type enum
 * @return Algorithm name string
 */
const char* predator_gov_crypto_get_algorithm_name(CryptoKeyType type);

/**
 * @brief Check if user has clearance for classification level
 * @param required_level Minimum required clearance level
 * @return true if access granted
 */
bool predator_gov_crypto_check_clearance(ClassificationLevel required_level);

/**
 * @brief Generate secure government-grade random key
 * @param key_out Output buffer for generated key (32 bytes)
 * @param country_code Country code for key generation parameters
 * @return true if key generated successfully
 */
bool predator_gov_crypto_generate_secure_key(uint8_t* key_out, const char* country_code);

/**
 * @brief Perform key exchange with foreign government
 * @param local_country Our country code
 * @param foreign_country Foreign country code
 * @param shared_key_out Output buffer for shared key
 * @return true if key exchange successful
 */
bool predator_gov_crypto_key_exchange(const char* local_country, 
                                     const char* foreign_country,
                                     uint8_t* shared_key_out);

#ifdef __cplusplus
}
#endif
