#pragma once

#include <furi.h>
#include <furi_hal.h>
#include <storage/storage.h>

/**
 * @brief Supported expansion board types for Predator app
 */
typedef enum {
    PredatorBoardTypeUnknown = 0, // Unknown or custom board
    PredatorBoardTypeOriginal, // Original Predator module
    PredatorBoardType3in1AIO, // 3in1-AIO-Expansion-Board V1.4 (Green PCB)
    PredatorBoardTypeDrB0rkMultiV2, // DrB0rk Multi Board v2
    PredatorBoardTypeScreen28, // 2.8-inch screen + GPS module
    PredatorBoardTypeMultiESP32, // Multi-function ESP32 board (black PCB)
    PredatorBoardTypePN532Writer, // Dedicated PN532 NFC Writer V4
    PredatorBoardType24GModule, // 2.4G Module with GPS (white PCB)
    PredatorBoardTypeAutoDetect, // Auto-detection mode
    PredatorBoardTypeCount // Keep this last
} PredatorBoardType;

/**
 * @brief Board configuration structure
 */
typedef struct {
    PredatorBoardType type;
    const char* name;
    const GpioPin* esp32_tx_pin;
    const GpioPin* esp32_rx_pin;
    uint32_t esp32_baud_rate;
    const GpioPin* gps_tx_pin;
    const GpioPin* gps_rx_pin;
    uint32_t gps_baud_rate;
    const GpioPin* gps_power_switch;
    const GpioPin* marauder_switch;
    bool has_external_rf;
    uint8_t rf_power_dbm;
    // PN532 NFC Writer Support
    bool has_nfc_writer;
    const GpioPin* nfc_sck_pin;    // SPI Clock
    const GpioPin* nfc_mosi_pin;   // SPI MOSI
    const GpioPin* nfc_miso_pin;   // SPI MISO
    const GpioPin* nfc_cs_pin;     // Chip Select
    const GpioPin* nfc_rst_pin;    // Reset (optional)
    const GpioPin* nfc_irq_pin;    // Interrupt (optional)
} PredatorBoardConfig;

/**
 * @brief Initialize board configurations
 * @return Array of board configurations
 */
const PredatorBoardConfig* predator_boards_get_configs();

/**
 * @brief Get number of supported board configurations
 * @return Count of board configurations
 */
size_t predator_boards_get_count();

/**
 * @brief Attempt to detect connected board type
 * @return Detected board type or PredatorBoardTypeUnknown if not detected
 */
PredatorBoardType predator_boards_detect();

/**
 * @brief Get configuration for specified board type
 * @param type Board type to get configuration for
 * @return Board configuration or NULL if type not found
 */
const PredatorBoardConfig* predator_boards_get_config(PredatorBoardType type);

/**
 * @brief Get board type name
 * @param type Board type to get name for
 * @return Human-readable board name
 */
const char* predator_boards_get_name(PredatorBoardType type);

/**
 * @brief Save selected board type to storage
 * @param storage Storage instance
 * @param type Board type to save
 * @return true if saved successfully
 */
bool predator_boards_save_selection(Storage* storage, PredatorBoardType type);

/**
 * @brief Load selected board type from storage
 * @param storage Storage instance
 * @return Loaded board type or PredatorBoardTypeUnknown if not found
 */
PredatorBoardType predator_boards_load_selection(Storage* storage);

/**
 * @brief Detect connected board type at runtime using hardware probing
 * @return Detected board type or PredatorBoardTypeUnknown if detection fails
 */
PredatorBoardType predator_boards_detect_runtime(void);

/**
 * @brief Auto-optimize system configuration for detected board type
 * @param board_type Board type to optimize for
 * @return true if optimization successful
 */
bool predator_boards_optimize_for_board(PredatorBoardType board_type);

/**
 * @brief Detect board at startup with comprehensive hardware probing
 * @return Detected board type with automatic configuration
 */
PredatorBoardType predator_detect_board_at_startup(void);
