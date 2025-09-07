#include "predator_boards.h"
#include <furi.h>
#include <storage/storage.h>

#define PREDATOR_BOARD_SAVE_PATH "/ext/predator/board_config.bin"

// Configuration for the original Predator module
static const PredatorBoardConfig predator_original_config = {
    .type = PredatorBoardTypeOriginal,
    .name = "Original Predator Module",
    .esp32_tx_pin = &gpio_ext_pc0, // Pin 15
    .esp32_rx_pin = &gpio_ext_pc1, // Pin 16
    .esp32_baud_rate = 115200,
    .gps_tx_pin = &gpio_ext_pb2, // Pin 13
    .gps_rx_pin = &gpio_ext_pb3, // Pin 14
    .gps_baud_rate = 9600,
    .gps_power_switch = &gpio_ext_pa4,
    .marauder_switch = &gpio_ext_pa7,
    .has_external_rf = true,
    .rf_power_dbm = 10
};

// Configuration for the 3in1 AIO Board V1.4 (based on the image)
static const PredatorBoardConfig predator_3in1_aio_config = {
    .type = PredatorBoardType3in1AIO,
    .name = "3in1 AIO Board V1.4",
    .esp32_tx_pin = &gpio_ext_pc0, // Pin 15 
    .esp32_rx_pin = &gpio_ext_pc1, // Pin 16
    .esp32_baud_rate = 115200,
    .gps_tx_pin = &gpio_ext_pb2, // Pin 13
    .gps_rx_pin = &gpio_ext_pb3, // Pin 14
    .gps_baud_rate = 9600,
    .gps_power_switch = NULL, // No dedicated switch, always powered
    .marauder_switch = NULL, // No dedicated switch, always on
    .has_external_rf = true,
    .rf_power_dbm = 10
};

// Configuration for the DrB0rk Multi Board V2
static const PredatorBoardConfig predator_drb0rk_multi_v2_config = {
    .type = PredatorBoardTypeDrB0rkMultiV2,
    .name = "DrB0rk Multi Board V2",
    .esp32_tx_pin = &gpio_ext_pc0, // Pin 15
    .esp32_rx_pin = &gpio_ext_pc1, // Pin 16
    .esp32_baud_rate = 115200,
    .gps_tx_pin = &gpio_ext_pb2, // Pin 13
    .gps_rx_pin = &gpio_ext_pb3, // Pin 14
    .gps_baud_rate = 9600,
    .gps_power_switch = NULL, // No dedicated switch
    .marauder_switch = NULL, // No dedicated switch
    .has_external_rf = true, 
    .rf_power_dbm = 10
};

// Configuration for the 3-in-1 NRF24 + CC1101 + ESP32 Multiboard
static const PredatorBoardConfig predator_3in1_nrf_cc_esp_config = {
    .type = PredatorBoardType3in1NrfCcEsp,
    .name = "3-in-1 NRF24+CC1101+ESP32",
    .esp32_tx_pin = &gpio_ext_pc0, // Pin 15
    .esp32_rx_pin = &gpio_ext_pc1, // Pin 16
    .esp32_baud_rate = 115200,
    .gps_tx_pin = &gpio_ext_pb2, // Pin 13
    .gps_rx_pin = &gpio_ext_pb3, // Pin 14
    .gps_baud_rate = 9600,
    .gps_power_switch = NULL, // No dedicated switch
    .marauder_switch = NULL, // No dedicated switch
    .has_external_rf = true, 
    .rf_power_dbm = 10
};

// Array of all supported board configurations
static const PredatorBoardConfig predator_board_configs[] = {
    predator_original_config,
    predator_3in1_aio_config,
    predator_drb0rk_multi_v2_config,
    predator_3in1_nrf_cc_esp_config
};

const PredatorBoardConfig* predator_boards_get_configs() {
    return predator_board_configs;
}

size_t predator_boards_get_count() {
    return COUNT_OF(predator_board_configs);
}

// Simple board detection logic - override this with more sophisticated detection
// based on hardware-specific characteristics when more information is available
PredatorBoardType predator_boards_detect() {
    // Try to detect board based on hardware characteristics
    
    // The 3in1 AIO board has ESP32 but no switches for GPS/Marauder
    // The DrB0rk board has similar characteristics
    // The original Predator module has specific switches
    
    // GPIO state detection is challenging without reliable markers
    // For now return unknown and let user select in settings
    
    // Future: implement better detection based on unique GPIO patterns,
    // jumper settings, or board ID pins if available
    
    return PredatorBoardTypeUnknown;
}

const PredatorBoardConfig* predator_boards_get_config(PredatorBoardType type) {
    if(type == PredatorBoardTypeUnknown) {
        return &predator_board_configs[0]; // Default to original
    }
    
    for(size_t i = 0; i < predator_boards_get_count(); i++) {
        if(predator_board_configs[i].type == type) {
            return &predator_board_configs[i];
        }
    }
    
    // Fall back to original if specified type not found
    return &predator_board_configs[0];
}

const char* predator_boards_get_name(PredatorBoardType type) {
    const PredatorBoardConfig* config = predator_boards_get_config(type);
    return config ? config->name : "Unknown Board";
}

bool predator_boards_save_selection(Storage* storage, PredatorBoardType type) {
    if(!storage) return false;
    
    // Create directory if it doesn't exist
    if(!storage_dir_exists(storage, "/ext/predator")) {
        storage_simply_mkdir(storage, "/ext/predator");
    }
    
    // Save board type to file
    File* file = storage_file_alloc(storage);
    bool success = false;
    
    if(storage_file_open(file, PREDATOR_BOARD_SAVE_PATH, FSAM_WRITE, FSOM_CREATE_ALWAYS)) {
        uint8_t board_type = (uint8_t)type;
        success = storage_file_write(file, &board_type, sizeof(board_type)) == sizeof(board_type);
        storage_file_close(file);
    }
    
    storage_file_free(file);
    return success;
}

PredatorBoardType predator_boards_load_selection(Storage* storage) {
    if(!storage) return PredatorBoardTypeUnknown;
    
    // Check if config file exists
    if(!storage_file_exists(storage, PREDATOR_BOARD_SAVE_PATH)) {
        return PredatorBoardTypeUnknown;
    }
    
    // Load board type from file
    File* file = storage_file_alloc(storage);
    PredatorBoardType type = PredatorBoardTypeUnknown;
    
    if(storage_file_open(file, PREDATOR_BOARD_SAVE_PATH, FSAM_READ, FSOM_OPEN_EXISTING)) {
        uint8_t board_type = 0;
        if(storage_file_read(file, &board_type, sizeof(board_type)) == sizeof(board_type)) {
            if(board_type < PredatorBoardTypeCount) {
                type = (PredatorBoardType)board_type;
            }
        }
        storage_file_close(file);
    }
    
    storage_file_free(file);
    return type;
}
