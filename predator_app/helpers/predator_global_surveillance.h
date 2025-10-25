#pragma once

#include "../predator_i.h"
#include "predator_gps.h"
#include "predator_esp32.h"
#include "predator_crypto_engine.h"
#include "predator_logging.h"
#include "subghz/predator_subghz_core.h"

// 🌍 GLOBAL SURVEILLANCE NETWORK
// Combines ALL helpers for worldwide monitoring and intelligence gathering
// Government-grade surveillance capabilities for Tesla/Swiss/California contracts

typedef enum {
    SurveillanceStatusIdle,
    SurveillanceStatusInitializing,
    SurveillanceStatusActive,
    SurveillanceStatusAnalyzing,
    SurveillanceStatusSyncing
} SurveillanceStatus;

typedef struct {
    SurveillanceStatus status;
    GPSCoordinates current_location;
    uint32_t wifi_networks_mapped;
    uint32_t subghz_signals_analyzed;
    uint32_t crypto_protocols_identified;
    uint32_t global_sync_packets_sent;
    uint32_t surveillance_time_ms;
    bool gps_tracking_active;
    bool wifi_scanning_active;
    bool subghz_analysis_active;
    bool crypto_analysis_active;
    bool global_sync_active;
    char current_operation[64];
} GlobalSurveillanceState;

/**
 * @brief Start global surveillance network
 * @param app Application context
 * @return true if started successfully, false otherwise
 */
bool predator_global_surveillance_start(PredatorApp* app);

/**
 * @brief Stop global surveillance network
 * @param app Application context
 * @return true if stopped successfully, false otherwise
 */
bool predator_global_surveillance_stop(PredatorApp* app);

/**
 * @brief Get current surveillance state
 * @param app Application context
 * @return Pointer to surveillance state
 */
GlobalSurveillanceState* predator_global_surveillance_get_state(PredatorApp* app);

/**
 * @brief Process surveillance data and sync to global database
 * @param app Application context
 * @return true if processing successful, false otherwise
 */
bool predator_global_surveillance_process_data(PredatorApp* app);

/**
 * @brief Enable continuous spectrum analysis
 * @param app Application context
 * @return true if enabled successfully, false otherwise
 */
bool predator_global_surveillance_enable_spectrum_analysis(PredatorApp* app);

/**
 * @brief Sync surveillance data to global intelligence network
 * @param app Application context
 * @return true if sync successful, false otherwise
 */
bool predator_global_surveillance_sync_to_network(PredatorApp* app);
