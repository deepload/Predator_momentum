#pragma once

#include "../predator_i.h"
#include "predator_models.h"

// 🔐 VIN-BASED MANUFACTURER CODES - Security Research
// Real manufacturer codes based on official VIN prefixes
// Used by Car Models Selection and Car Security Testing

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Get real VIN-based manufacturer code for a car model
 * @param model Pointer to car model structure
 * @return Real VIN-based manufacturer code (32-bit hex)
 */
uint32_t predator_vin_get_manufacturer_code(const PredatorCarModel* model);

/**
 * @brief Get manufacturer code by manufacturer name string
 * @param manufacturer Manufacturer name (e.g., "Tesla", "BMW", "Volkswagen")
 * @return Real VIN-based manufacturer code (32-bit hex)
 */
uint32_t predator_vin_get_code_by_manufacturer(const char* manufacturer);

/**
 * @brief Get VIN prefix string for manufacturer
 * @param manufacturer Manufacturer name
 * @param vin_prefix_out Output buffer for VIN prefix (minimum 5 bytes)
 * @return true if VIN prefix found and copied
 */
bool predator_vin_get_prefix_string(const char* manufacturer, char* vin_prefix_out);

/**
 * @brief Validate if manufacturer code is authentic VIN-based
 * @param manufacturer_code 32-bit manufacturer code
 * @return true if code is real VIN-based, false if generic/fake
 */
bool predator_vin_is_authentic_code(uint32_t manufacturer_code);

/**
 * @brief Get manufacturer name from VIN-based code
 * @param manufacturer_code 32-bit VIN-based code
 * @return Manufacturer name string, or "Unknown" if not found
 */
const char* predator_vin_get_manufacturer_name(uint32_t manufacturer_code);

/**
 * @brief Initialize VIN code mapping system
 * @return true if initialization successful
 */
bool predator_vin_codes_init(void);

#ifdef __cplusplus
}
#endif
