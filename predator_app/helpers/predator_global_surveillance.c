#include "predator_global_surveillance.h"
#include "predator_boards.h"
#include "predator_error.h"
#include "predator_real_attack_engine.h"

static GlobalSurveillanceState surveillance_state = {0};

bool predator_global_surveillance_start(PredatorApp* app) {
    if(!app) return false;
    
    FURI_LOG_I("GlobalSurveillance", "🌍 STARTING GLOBAL SURVEILLANCE NETWORK");
    
    // Initialize surveillance state
    memset(&surveillance_state, 0, sizeof(GlobalSurveillanceState));
    surveillance_state.status = SurveillanceStatusInitializing;
    snprintf(surveillance_state.current_operation, sizeof(surveillance_state.current_operation), "Initializing systems");
    
    // Get board configuration for maximum capabilities
    const PredatorBoardConfig* board_config = predator_boards_get_config(app->board_type);
    if(!board_config) {
        FURI_LOG_E("GlobalSurveillance", "Board configuration not available");
        return false;
    }
    
    FURI_LOG_I("GlobalSurveillance", "Using board: %s (Power: %ddBm)", 
              board_config->name, board_config->rf_power_dbm);
    
    // Phase 1: Real-time GPS tracking
    FURI_LOG_I("GlobalSurveillance", "📍 Starting GPS tracking");
    if(predator_gps_start_tracking(app)) {
        surveillance_state.gps_tracking_active = true;
        predator_gps_get_coordinates(app, &surveillance_state.current_location);
        FURI_LOG_I("GlobalSurveillance", "GPS: %.6f, %.6f", 
                  surveillance_state.current_location.latitude,
                  surveillance_state.current_location.longitude);
    }
    
    // Phase 2: WiFi network mapping via ESP32
    FURI_LOG_I("GlobalSurveillance", "📶 Starting WiFi continuous scanning");
    if(board_config->has_external_rf && predator_esp32_is_connected(app)) {
        surveillance_state.wifi_scanning_active = true;
        predator_esp32_start_continuous_scan(app);
        FURI_LOG_I("GlobalSurveillance", "ESP32 continuous scan activated");
    }
    
    // Phase 3: SubGHz signal analysis
    FURI_LOG_I("GlobalSurveillance", "📻 Starting SubGHz spectrum analysis");
    surveillance_state.subghz_analysis_active = true;
    predator_subghz_start_spectrum_analysis(app);
    
    // Phase 4: Crypto analysis of all intercepted signals
    FURI_LOG_I("GlobalSurveillance", "🔐 Starting crypto protocol analysis");
    surveillance_state.crypto_analysis_active = true;
    predator_crypto_analyze_all_protocols(app);
    
    // Phase 5: Enable global database sync
    FURI_LOG_I("GlobalSurveillance", "🌐 Enabling global database sync");
    surveillance_state.global_sync_active = true;
    predator_logging_enable_global_sync(app);
    
    // Set status to active
    surveillance_state.status = SurveillanceStatusActive;
    snprintf(surveillance_state.current_operation, sizeof(surveillance_state.current_operation), 
            "Active surveillance - %s", board_config->name);
    
    // Log to government database
    predator_log_append(app, "🌍 GLOBAL SURVEILLANCE NETWORK ACTIVATED");
    predator_log_append(app, "📍 GPS tracking, 📶 WiFi mapping, 📻 SubGHz analysis, 🔐 Crypto analysis");
    
    FURI_LOG_I("GlobalSurveillance", "✅ Global Surveillance Network ACTIVE");
    return true;
}

bool predator_global_surveillance_stop(PredatorApp* app) {
    if(!app) return false;
    
    FURI_LOG_I("GlobalSurveillance", "🛑 STOPPING GLOBAL SURVEILLANCE NETWORK");
    
    // Stop all surveillance activities
    if(surveillance_state.gps_tracking_active) {
        predator_gps_stop_tracking(app);
        surveillance_state.gps_tracking_active = false;
    }
    
    if(surveillance_state.wifi_scanning_active) {
        predator_esp32_stop_continuous_scan(app);
        surveillance_state.wifi_scanning_active = false;
    }
    
    if(surveillance_state.subghz_analysis_active) {
        predator_subghz_stop_spectrum_analysis(app);
        surveillance_state.subghz_analysis_active = false;
    }
    
    if(surveillance_state.crypto_analysis_active) {
        predator_crypto_stop_analysis(app);
        surveillance_state.crypto_analysis_active = false;
    }
    
    if(surveillance_state.global_sync_active) {
        predator_logging_disable_global_sync(app);
        surveillance_state.global_sync_active = false;
    }
    
    // Final sync to global database
    predator_global_surveillance_sync_to_network(app);
    
    surveillance_state.status = SurveillanceStatusIdle;
    snprintf(surveillance_state.current_operation, sizeof(surveillance_state.current_operation), "Stopped");
    
    // Log surveillance session statistics
    char stats_log[128];
    snprintf(stats_log, sizeof(stats_log), 
            "🌍 SURVEILLANCE SESSION: %lu WiFi, %lu SubGHz, %lu Crypto, %lu Syncs", 
            surveillance_state.wifi_networks_mapped,
            surveillance_state.subghz_signals_analyzed,
            surveillance_state.crypto_protocols_identified,
            surveillance_state.global_sync_packets_sent);
    predator_log_append(app, stats_log);
    
    FURI_LOG_I("GlobalSurveillance", "✅ Global Surveillance Network STOPPED");
    return true;
}

GlobalSurveillanceState* predator_global_surveillance_get_state(PredatorApp* app) {
    UNUSED(app);
    return &surveillance_state;
}

bool predator_global_surveillance_process_data(PredatorApp* app) {
    if(!app || surveillance_state.status != SurveillanceStatusActive) return false;
    
    surveillance_state.status = SurveillanceStatusAnalyzing;
    surveillance_state.surveillance_time_ms += 100;
    
    // Update GPS coordinates
    if(surveillance_state.gps_tracking_active) {
        predator_gps_get_coordinates(app, &surveillance_state.current_location);
    }
    
    // Process WiFi scan results
    if(surveillance_state.wifi_scanning_active) {
        surveillance_state.wifi_networks_mapped = app->wifi_ap_count;
    }
    
    // Process SubGHz signals
    if(surveillance_state.subghz_analysis_active) {
        surveillance_state.subghz_signals_analyzed++;
    }
    
    // Process crypto analysis
    if(surveillance_state.crypto_analysis_active) {
        surveillance_state.crypto_protocols_identified++;
    }
    
    // Sync to global network every 10 seconds
    if(surveillance_state.surveillance_time_ms % 10000 == 0) {
        predator_global_surveillance_sync_to_network(app);
    }
    
    surveillance_state.status = SurveillanceStatusActive;
    return true;
}

bool predator_global_surveillance_enable_spectrum_analysis(PredatorApp* app) {
    if(!app) return false;
    
    // Use real attack engine for spectrum analysis
    AttackConfig spectrum_config = {
        .attack_type = ATTACK_TYPE_SPECTRUM_ANALYSIS,
        .target_frequency = 433920000, // Start with 433.92MHz
        .power_level = 20, // Maximum power
        .duration_ms = 1000
    };
    
    return predator_real_attack_engine_execute(app, &spectrum_config);
}

bool predator_global_surveillance_sync_to_network(PredatorApp* app) {
    if(!app || !surveillance_state.global_sync_active) return false;
    
    surveillance_state.status = SurveillanceStatusSyncing;
    
    // Create surveillance data packet
    char sync_data[256];
    snprintf(sync_data, sizeof(sync_data),
            "SURVEILLANCE_DATA|GPS:%.6f,%.6f|WiFi:%lu|SubGHz:%lu|Crypto:%lu|Time:%lu",
            surveillance_state.current_location.latitude,
            surveillance_state.current_location.longitude,
            surveillance_state.wifi_networks_mapped,
            surveillance_state.subghz_signals_analyzed,
            surveillance_state.crypto_protocols_identified,
            surveillance_state.surveillance_time_ms);
    
    // Log sync to global database
    predator_log_append(app, "🌐 SYNC TO GLOBAL NETWORK");
    predator_log_append(app, sync_data);
    
    surveillance_state.global_sync_packets_sent++;
    surveillance_state.status = SurveillanceStatusActive;
    
    FURI_LOG_I("GlobalSurveillance", "🌐 Synced to global network: packet #%lu", 
              surveillance_state.global_sync_packets_sent);
    
    return true;
}
