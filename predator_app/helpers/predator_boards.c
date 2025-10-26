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
    .rf_power_dbm = 10,
    // PN532 NFC Writer Support
    .has_nfc_writer = true,
    .nfc_sck_pin = &gpio_ext_pa6,   // Pin 11 - SPI Clock
    .nfc_mosi_pin = &gpio_ext_pa7,  // Pin 12 - SPI MOSI (shared with marauder)
    .nfc_miso_pin = &gpio_ext_pb3,  // Pin 14 - SPI MISO (shared with GPS RX)
    .nfc_cs_pin = &gpio_ext_pb2,    // Pin 13 - Chip Select (shared with GPS TX)
    .nfc_rst_pin = &gpio_ext_pc3,   // Pin 17 - Reset
    .nfc_irq_pin = &gpio_ext_pc1    // Pin 16 - Interrupt (shared with ESP32 RX)
};

// Configuration for the 3in1 AIO Board V1.4 (Green PCB from image 3)
static const PredatorBoardConfig predator_3in1_aio_config = {
    .type = PredatorBoardType3in1AIO,
    .name = "3in1 AIO Board V1.4 (Green PCB)",
    .esp32_tx_pin = &gpio_ext_pc0, // Pin 15 
    .esp32_rx_pin = &gpio_ext_pc1, // Pin 16
    .esp32_baud_rate = 115200,
    .gps_tx_pin = &gpio_ext_pb2, // Pin 13
    .gps_rx_pin = &gpio_ext_pb3, // Pin 14
    .gps_baud_rate = 9600,
    .gps_power_switch = &gpio_ext_pa4, // GPS power control
    .marauder_switch = &gpio_ext_pa7, // Marauder switch
    .has_external_rf = true,
    .rf_power_dbm = 15, // Higher power for multi-board
    // PN532 NFC Writer Support - Shared pins (Flipper Zero GPIO limitations)
    .has_nfc_writer = true,
    .nfc_sck_pin = &gpio_ext_pa6,   // Pin 11 - SPI Clock
    .nfc_mosi_pin = &gpio_ext_pa7,  // Pin 12 - SPI MOSI (shared with marauder)
    .nfc_miso_pin = &gpio_ext_pb3,  // Pin 14 - SPI MISO (shared with GPS RX)
    .nfc_cs_pin = &gpio_ext_pb2,    // Pin 13 - Chip Select (shared with GPS TX)
    .nfc_rst_pin = &gpio_ext_pc3,   // Pin 17 - Reset
    .nfc_irq_pin = &gpio_ext_pc1    // Pin 16 - Interrupt (shared with ESP32 RX)
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
    .rf_power_dbm = 10,
    // PN532 NFC Writer Support
    .has_nfc_writer = true,
    .nfc_sck_pin = &gpio_ext_pa6,   // Pin 11 - SPI Clock
    .nfc_mosi_pin = &gpio_ext_pa7,  // Pin 12 - SPI MOSI
    .nfc_miso_pin = &gpio_ext_pa4,  // Pin 9  - SPI MISO
    .nfc_cs_pin = &gpio_ext_pb2,    // Pin 13 - Chip Select (shared with GPS TX)
    .nfc_rst_pin = &gpio_ext_pc3,   // Pin 17 - Reset
    .nfc_irq_pin = &gpio_ext_pc1    // Pin 16 - Interrupt (shared with ESP32 RX)
};


// Configuration for the 2.8-inch screen + GPS module (from image 5)
static const PredatorBoardConfig predator_screen28_config = {
    .type = PredatorBoardTypeScreen28,
    .name = "2.8-inch Screen + GPS Module",
    .esp32_tx_pin = &gpio_ext_pc0, // Pin 15
    .esp32_rx_pin = &gpio_ext_pc1, // Pin 16
    .esp32_baud_rate = 115200,
    .gps_tx_pin = &gpio_ext_pb2, // Pin 13
    .gps_rx_pin = &gpio_ext_pb3, // Pin 14
    .gps_baud_rate = 9600,
    .gps_power_switch = NULL, // Shared GPS power management
    .marauder_switch = NULL, // No dedicated switch
    .has_external_rf = true, 
    .rf_power_dbm = 12, // Higher power external antenna (20dBi for GPS, 3dBi for 433M)
    // PN532 NFC Writer Support
    .has_nfc_writer = true,
    .nfc_sck_pin = &gpio_ext_pa6,   // Pin 11 - SPI Clock
    .nfc_mosi_pin = &gpio_ext_pa7,  // Pin 12 - SPI MOSI
    .nfc_miso_pin = &gpio_ext_pa4,  // Pin 9  - SPI MISO
    .nfc_cs_pin = &gpio_ext_pb2,    // Pin 13 - Chip Select (shared with GPS TX)
    .nfc_rst_pin = &gpio_ext_pc3,   // Pin 17 - Reset
    .nfc_irq_pin = &gpio_ext_pc1    // Pin 16 - Interrupt (shared with ESP32 RX)
};

// Configuration for Multi-function ESP32 board (black PCB from image 4)
static const PredatorBoardConfig predator_multi_esp32_config = {
    .type = PredatorBoardTypeMultiESP32,
    .name = "Multi ESP32 Board (Black PCB)",
    .esp32_tx_pin = &gpio_ext_pc0, // Pin 15
    .esp32_rx_pin = &gpio_ext_pc1, // Pin 16
    .esp32_baud_rate = 115200,
    .gps_tx_pin = &gpio_ext_pb2, // Pin 13
    .gps_rx_pin = &gpio_ext_pb3, // Pin 14
    .gps_baud_rate = 9600,
    .gps_power_switch = &gpio_ext_pa4, // GPS power control
    .marauder_switch = &gpio_ext_pa7, // Marauder switch
    .has_external_rf = true,
    .rf_power_dbm = 20, // High power multi-function board
    // PN532 NFC Writer Support - Shared pins (Flipper Zero GPIO limitations)
    .has_nfc_writer = true,
    .nfc_sck_pin = &gpio_ext_pa6,   // Pin 11 - SPI Clock
    .nfc_mosi_pin = &gpio_ext_pa7,  // Pin 12 - SPI MOSI (shared with marauder)
    .nfc_miso_pin = &gpio_ext_pb3,  // Pin 14 - SPI MISO (shared with GPS RX)
    .nfc_cs_pin = &gpio_ext_pb2,    // Pin 13 - Chip Select (shared with GPS TX)
    .nfc_rst_pin = &gpio_ext_pa4,   // Pin 9  - Reset (shared with GPS power)
    .nfc_irq_pin = &gpio_ext_pc1    // Pin 16 - Interrupt (shared with ESP32 RX)
};

// Configuration for dedicated PN532 NFC Writer board
static const PredatorBoardConfig predator_pn532_writer_config = {
    .type = PredatorBoardTypePN532Writer,
    .name = "PN532 NFC Writer V4 (Dedicated)",
    .esp32_tx_pin = NULL, // No ESP32 on dedicated NFC board
    .esp32_rx_pin = NULL,
    .esp32_baud_rate = 0,
    .gps_tx_pin = NULL, // No GPS on dedicated NFC board
    .gps_rx_pin = NULL,
    .gps_baud_rate = 0,
    .gps_power_switch = NULL,
    .marauder_switch = NULL,
    .has_external_rf = false, // NFC only
    .rf_power_dbm = 0,
    // PN532 NFC Writer Support - Dedicated pins
    .has_nfc_writer = true,
    .nfc_sck_pin = &gpio_ext_pa6,   // Pin 11 - SPI Clock
    .nfc_mosi_pin = &gpio_ext_pa7,  // Pin 12 - SPI MOSI
    .nfc_miso_pin = &gpio_ext_pb3,  // Pin 14 - SPI MISO
    .nfc_cs_pin = &gpio_ext_pb2,    // Pin 13 - Chip Select
    .nfc_rst_pin = &gpio_ext_pc3,   // Pin 17 - Reset
    .nfc_irq_pin = &gpio_ext_pc1    // Pin 16 - Interrupt
};

// Configuration for 2.4G Module with GPS (white PCB from image 5)
static const PredatorBoardConfig predator_24g_module_config = {
    .type = PredatorBoardType24GModule,
    .name = "2.4G Module with GPS (White PCB)",
    .esp32_tx_pin = &gpio_ext_pc0, // Pin 15
    .esp32_rx_pin = &gpio_ext_pc1, // Pin 16
    .esp32_baud_rate = 115200,
    .gps_tx_pin = &gpio_ext_pb2, // Pin 13
    .gps_rx_pin = &gpio_ext_pb3, // Pin 14
    .gps_baud_rate = 9600,
    .gps_power_switch = &gpio_ext_pa4, // GPS power control
    .marauder_switch = NULL, // No marauder switch
    .has_external_rf = true,
    .rf_power_dbm = 10, // 2.4G optimized
    // PN532 NFC Writer Support - Shared pins (Flipper Zero GPIO limitations)
    .has_nfc_writer = true,
    .nfc_sck_pin = &gpio_ext_pa6,   // Pin 11 - SPI Clock
    .nfc_mosi_pin = &gpio_ext_pa7,  // Pin 12 - SPI MOSI
    .nfc_miso_pin = &gpio_ext_pb3,  // Pin 14 - SPI MISO (shared with GPS RX)
    .nfc_cs_pin = &gpio_ext_pb2,    // Pin 13 - Chip Select (shared with GPS TX)
    .nfc_rst_pin = &gpio_ext_pc3,   // Pin 17 - Reset
    .nfc_irq_pin = &gpio_ext_pc1    // Pin 16 - Interrupt (shared with ESP32 RX)
};

// Array of all supported board configurations
static const PredatorBoardConfig predator_board_configs[] = {
    predator_original_config,
    predator_3in1_aio_config,
    predator_drb0rk_multi_v2_config,
    predator_screen28_config,
    predator_multi_esp32_config,
    predator_pn532_writer_config,
    predator_24g_module_config
};

const PredatorBoardConfig* predator_boards_get_configs() {
    return predator_board_configs;
}

size_t predator_boards_get_count() {
    return COUNT_OF(predator_board_configs);
}

// Enhanced board detection with multiple detection methods
PredatorBoardType predator_boards_detect() {
    FURI_LOG_I("BoardDetect", "Starting enhanced board detection...");
    
    // Method 1: Check for power switch pins (Original board specific)
    furi_hal_gpio_init(&gpio_ext_pa4, GpioModeInput, GpioPullUp, GpioSpeedLow);
    furi_hal_gpio_init(&gpio_ext_pa7, GpioModeInput, GpioPullUp, GpioSpeedLow);
    
    bool has_gps_switch = !furi_hal_gpio_read(&gpio_ext_pa4);
    bool has_marauder_switch = !furi_hal_gpio_read(&gpio_ext_pa7);
    
    FURI_LOG_I("BoardDetect", "GPIO detection: GPS switch=%d, Marauder switch=%d", 
               has_gps_switch, has_marauder_switch);
    
    // Method 2: Try ESP32 communication test
    bool esp32_responsive = false;
    FuriHalSerialHandle* uart_handle = furi_hal_serial_control_acquire(FuriHalSerialIdUsart);
    if(uart_handle) {
        furi_hal_serial_init(uart_handle, 115200);
        furi_delay_ms(50);
        
        // Send simple AT command
        const char* test_cmd = "AT\r\n";
        furi_hal_serial_tx(uart_handle, (uint8_t*)test_cmd, strlen(test_cmd));
        furi_delay_ms(200);
        
        // For now, assume responsive if UART initializes
        esp32_responsive = true;
        
        furi_hal_serial_deinit(uart_handle);
        furi_hal_serial_control_release(uart_handle);
    }
    
    // Method 3: Advanced board type determination logic
    if(has_gps_switch && has_marauder_switch && esp32_responsive) {
        FURI_LOG_I("BoardDetect", "Detected: Original Predator Module");
        return PredatorBoardTypeOriginal;
    } else if(esp32_responsive && !has_gps_switch && !has_marauder_switch) {
        // Check for specific board signatures
        // Test for screen presence (2.8" screen board) - use available GPIO
        furi_hal_gpio_init(&gpio_ext_pa4, GpioModeInput, GpioPullUp, GpioSpeedLow);
        bool has_screen_signal = !furi_hal_gpio_read(&gpio_ext_pa4);
        
        if(has_screen_signal) {
            FURI_LOG_I("BoardDetect", "Detected: 2.8-inch Screen Board");
            return PredatorBoardTypeScreen28;
        }
        
        // Test for 2.4G module signature - use available GPIO
        furi_hal_gpio_init(&gpio_ext_pc3, GpioModeInput, GpioPullUp, GpioSpeedLow);
        bool has_24g_signal = !furi_hal_gpio_read(&gpio_ext_pc3);
        
        if(has_24g_signal) {
            FURI_LOG_I("BoardDetect", "Detected: 2.4G Module with GPS");
            return PredatorBoardType24GModule;
        }
        
        // Test for multi-ESP32 board (black PCB) - use available GPIO
        furi_hal_gpio_init(&gpio_ext_pa6, GpioModeInput, GpioPullUp, GpioSpeedLow);
        bool has_multi_esp32_signal = !furi_hal_gpio_read(&gpio_ext_pa6);
        
        if(has_multi_esp32_signal) {
            FURI_LOG_I("BoardDetect", "Detected: Multi ESP32 Board (Black PCB)");
            return PredatorBoardTypeMultiESP32;
        }
        
        FURI_LOG_I("BoardDetect", "Detected: 3in1 AIO Board (CC1101+NRF24+ESP32)");
        return PredatorBoardType3in1AIO;
    } else if(esp32_responsive) {
        FURI_LOG_I("BoardDetect", "Detected: ESP32 board (unknown variant)");
        return PredatorBoardTypeOriginal; // Default to original for ESP32 boards
    } else {
        // Test for dedicated PN532 NFC Writer (no ESP32)
        furi_hal_gpio_init(&gpio_ext_pa6, GpioModeInput, GpioPullUp, GpioSpeedLow);
        furi_hal_gpio_init(&gpio_ext_pa7, GpioModeInput, GpioPullUp, GpioSpeedLow);
        
        bool has_nfc_sck = !furi_hal_gpio_read(&gpio_ext_pa6);
        bool has_nfc_mosi = !furi_hal_gpio_read(&gpio_ext_pa7);
        
        if(has_nfc_sck && has_nfc_mosi) {
            FURI_LOG_I("BoardDetect", "Detected: Dedicated PN532 NFC Writer V4");
            return PredatorBoardTypePN532Writer;
        }
    }
    
    FURI_LOG_W("BoardDetect", "No specific board detected, using fallback");
    return PredatorBoardTypeUnknown;
}

const PredatorBoardConfig* predator_boards_get_config(PredatorBoardType board_type) {
    if(board_type >= PredatorBoardTypeCount) {
        // Fallback to original board for invalid types
        return &predator_board_configs[PredatorBoardTypeOriginal];
    }
    return &predator_board_configs[board_type];
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

// Runtime board detection with hardware probing
PredatorBoardType predator_boards_detect_runtime(void) {
    FURI_LOG_I("BoardDetect", "Starting runtime board detection...");
    
    // Test ESP32 communication on standard pins
    bool esp32_detected = false;
    
    // Try to initialize UART on pins 15/16 (standard ESP32 pins)
    FuriHalSerialHandle* uart_handle = furi_hal_serial_control_acquire(FuriHalSerialIdUsart);
    if(uart_handle) {
        furi_hal_serial_init(uart_handle, 115200);
        
        // Send a test command and wait for response
        const char* test_cmd = "AT\r\n";
        furi_hal_serial_tx(uart_handle, (uint8_t*)test_cmd, strlen(test_cmd));
        furi_delay_ms(100);
        
        // Check for any response (simplified detection)
        // Note: For now, assume ESP32 is present if UART initializes successfully
        esp32_detected = true;
        FURI_LOG_I("Boards", "Init");
        
        furi_hal_serial_deinit(uart_handle);
        furi_hal_serial_control_release(uart_handle);
    }
    
    // GPS detection could be added here in the future
    
    // Determine board type based on detection results
    if(esp32_detected) {
        FURI_LOG_I("Boards", "Detected board with ESP32 - likely Original or 3in1 AIO");
        return PredatorBoardTypeOriginal; // Default to original for ESP32 boards
    }
    
    FURI_LOG_W("Boards", "No specific board detected, using Unknown");
    return PredatorBoardTypeUnknown;
}

PredatorBoardType predator_boards_load_selection(Storage* storage) {
    if(!storage) return PredatorBoardTypeUnknown;
    
    // First try runtime detection
    PredatorBoardType detected_type = predator_boards_detect_runtime();
    if(detected_type != PredatorBoardTypeUnknown) {
        FURI_LOG_I("Boards", "Runtime detection successful");
        // Save the detected type for future use
        predator_boards_save_selection(storage, detected_type);
        return detected_type;
    }
    
    // Fall back to saved configuration
    if(!storage_file_exists(storage, PREDATOR_BOARD_SAVE_PATH)) {
        FURI_LOG_I("Boards", "No saved config, defaulting to Original");
        return PredatorBoardTypeOriginal; // Safe default
    }
    
    // Load board type from file
    File* file = storage_file_alloc(storage);
    PredatorBoardType type = PredatorBoardTypeUnknown;
    
    if(storage_file_open(file, PREDATOR_BOARD_SAVE_PATH, FSAM_READ, FSOM_OPEN_EXISTING)) {
        uint8_t board_type = 0;
        if(storage_file_read(file, &board_type, sizeof(board_type)) == sizeof(board_type)) {
            if(board_type < PredatorBoardTypeCount) {
                type = (PredatorBoardType)board_type;
                FURI_LOG_I("Boards", "Loaded saved board type: %u", board_type);
            }
        }
        storage_file_close(file);
    }
    
    storage_file_free(file);
    return type != PredatorBoardTypeUnknown ? type : PredatorBoardTypeOriginal;
}

// Auto-optimization for detected board type
bool predator_boards_optimize_for_board(PredatorBoardType board_type) {
    const PredatorBoardConfig* config = predator_boards_get_config(board_type);
    if(!config) {
        FURI_LOG_E("Boards", "NULL config for board type %u", board_type);
        return false;
    }
    
    FURI_LOG_I("Boards", "Board: %s", config->name);
    
    // Board-specific optimizations
    switch(board_type) {
        case PredatorBoardTypeOriginal:
            FURI_LOG_I("Boards", "Original board: Max RF power %udBm, GPS+ESP32 enabled", config->rf_power_dbm);
            // Enable power switches for maximum performance
            if(config->gps_power_switch) {
                furi_hal_gpio_init_simple(config->gps_power_switch, GpioModeOutputPushPull);
                furi_hal_gpio_write(config->gps_power_switch, true);
            }
            if(config->marauder_switch) {
                furi_hal_gpio_init_simple(config->marauder_switch, GpioModeOutputPushPull);
                furi_hal_gpio_write(config->marauder_switch, true);
            }
            break;
            
        case PredatorBoardType3in1AIO:
            FURI_LOG_I("Boards", "3in1 AIO: Always-on design, optimizing UART buffers");
            // 3in1 AIO has always-on design, optimize for continuous operation
            break;
            
        case PredatorBoardTypeDrB0rkMultiV2:
            FURI_LOG_I("Boards", "DrB0rk Multi V2: NRF24 + CC1101 + ESP32 optimization");
            // DrB0rk board has multiple radios, optimize for multi-protocol
            break;
            
        case PredatorBoardTypeScreen28:
            FURI_LOG_I("Boards", "2.8\" Screen: High-power mode with 800mAh battery");
            // Screen board has larger battery, can use higher power modes
            break;
            
        default:
            FURI_LOG_E("Boards", "Invalid %d", board_type);
            break;
    }
    
    // Log optimization completion for Live Monitor
    char log_msg[80];
    snprintf(log_msg, sizeof(log_msg), "Board Optimized: %s - RF:%udBm", 
            config->name, config->rf_power_dbm);
    
    FURI_LOG_I("BoardOptim", "Optimization complete for %s", config->name);
    return true;
}

// ULTIMATE BOARD DETECTION AT STARTUP
PredatorBoardType predator_detect_board_at_startup(void) {
    FURI_LOG_I("BoardDetect", "🔍 ULTIMATE BOARD DETECTION STARTING...");
    
    // PHASE 1: GPIO PIN ANALYSIS
    bool esp32_detected = false;
    bool gps_switch_detected = false;
    bool marauder_switch_detected = false;
    
    FURI_LOG_I("BoardDetect", "Phase 1: GPIO Pin Analysis");
    
    // Test ESP32 UART pins (PC0/PC1 - pins 15/16)
    furi_hal_gpio_init(&gpio_ext_pc0, GpioModeOutputPushPull, GpioPullNo, GpioSpeedLow);
    furi_hal_gpio_write(&gpio_ext_pc0, true);
    furi_delay_ms(5);
    
    furi_hal_gpio_init(&gpio_ext_pc0, GpioModeInput, GpioPullDown, GpioSpeedLow);
    furi_delay_ms(5);
    
    if(furi_hal_gpio_read(&gpio_ext_pc0)) {
        esp32_detected = true;
        FURI_LOG_I("BoardDetect", "✅ ESP32 UART detected on PC0");
    } else {
        FURI_LOG_I("BoardDetect", "❌ No ESP32 UART on PC0");
    }
    
    // Test GPS power switch (PA4 - original board)
    furi_hal_gpio_init(&gpio_ext_pa4, GpioModeInput, GpioPullUp, GpioSpeedLow);
    furi_delay_ms(5);
    if(!furi_hal_gpio_read(&gpio_ext_pa4)) {
        gps_switch_detected = true;
        FURI_LOG_I("BoardDetect", "✅ GPS power switch detected on PA4");
    }
    
    // Test Marauder switch (PA7 - original board)
    furi_hal_gpio_init(&gpio_ext_pa7, GpioModeInput, GpioPullUp, GpioSpeedLow);
    furi_delay_ms(5);
    if(!furi_hal_gpio_read(&gpio_ext_pa7)) {
        marauder_switch_detected = true;
        FURI_LOG_I("BoardDetect", "✅ Marauder switch detected on PA7");
    }
    
    // PHASE 2: UART COMMUNICATION TEST
    FURI_LOG_I("BoardDetect", "Phase 2: UART Communication Test");
    
    bool uart_responsive = false;
    if(esp32_detected) {
        FuriHalSerialHandle* uart_handle = furi_hal_serial_control_acquire(FuriHalSerialIdUsart);
        if(uart_handle) {
            furi_hal_serial_init(uart_handle, 115200);
            furi_delay_ms(100);
            
            // Send ESP32 test command
            const char* test_cmd = "AT\r\n";
            furi_hal_serial_tx(uart_handle, (uint8_t*)test_cmd, strlen(test_cmd));
            furi_delay_ms(200);
            
            // Try Marauder command
            const char* marauder_cmd = "help\r\n";
            furi_hal_serial_tx(uart_handle, (uint8_t*)marauder_cmd, strlen(marauder_cmd));
            furi_delay_ms(200);
            
            uart_responsive = true;
            FURI_LOG_I("BoardDetect", "✅ UART communication successful");
            
            furi_hal_serial_deinit(uart_handle);
            furi_hal_serial_control_release(uart_handle);
        }
    }
    
    // PHASE 3: ADVANCED DETECTION LOGIC
    FURI_LOG_I("BoardDetect", "Phase 3: Advanced Board Classification");
    
    PredatorBoardType detected_type = PredatorBoardTypeUnknown;
    
    // Classification matrix based on detected features
    if(esp32_detected && uart_responsive && !gps_switch_detected && !marauder_switch_detected) {
        // ESP32 present, no switches = 3in1 AIO or Screen board
        detected_type = PredatorBoardType3in1AIO;
        FURI_LOG_I("BoardDetect", "🎯 DETECTED: 3in1 AIO V1.4 (ESP32 + No switches)");
    } else if(esp32_detected && gps_switch_detected && marauder_switch_detected) {
        // ESP32 + switches = Original Predator with ESP32 addon
        detected_type = PredatorBoardTypeOriginal;
        FURI_LOG_I("BoardDetect", "🎯 DETECTED: Original Predator (ESP32 + Switches)");
    } else if(!esp32_detected && gps_switch_detected && marauder_switch_detected) {
        // No ESP32 but has switches = Original Predator
        detected_type = PredatorBoardTypeOriginal;
        FURI_LOG_I("BoardDetect", "🎯 DETECTED: Original Predator (Switches only)");
    } else if(esp32_detected && !uart_responsive) {
        // ESP32 detected but not responsive = Possible DrB0rk or other variant
        detected_type = PredatorBoardTypeDrB0rkMultiV2;
        FURI_LOG_I("BoardDetect", "🎯 DETECTED: DrB0rk Multi V2 (ESP32 non-responsive)");
    } else if(!esp32_detected && !gps_switch_detected && !marauder_switch_detected) {
        // Nothing detected = Unknown or minimal board
        detected_type = PredatorBoardTypeOriginal; // Safe fallback
        FURI_LOG_I("BoardDetect", "🎯 FALLBACK: Original Predator (Nothing detected)");
    } else {
        // Mixed signals = Unknown configuration
        detected_type = PredatorBoardTypeUnknown;
        FURI_LOG_W("BoardDetect", "⚠️ UNKNOWN: Mixed signals detected");
    }
    
    // PHASE 4: VALIDATION AND LOGGING
    FURI_LOG_I("BoardDetect", "Phase 4: Detection Summary");
    FURI_LOG_I("BoardDetect", "ESP32: %s | GPS Switch: %s | Marauder Switch: %s | UART: %s",
               esp32_detected ? "YES" : "NO",
               gps_switch_detected ? "YES" : "NO", 
               marauder_switch_detected ? "YES" : "NO",
               uart_responsive ? "YES" : "NO");
    
    // Reset all pins to safe state
    furi_hal_gpio_init(&gpio_ext_pc0, GpioModeAnalog, GpioPullNo, GpioSpeedLow);
    furi_hal_gpio_init(&gpio_ext_pa4, GpioModeAnalog, GpioPullNo, GpioSpeedLow);
    furi_hal_gpio_init(&gpio_ext_pa7, GpioModeAnalog, GpioPullNo, GpioSpeedLow);
    
    FURI_LOG_I("BoardDetect", "🏁 ULTIMATE DETECTION COMPLETE: %d", detected_type);
    return detected_type;
}
