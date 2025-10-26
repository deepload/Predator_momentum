#pragma once

#include <furi.h>
#include <furi_hal.h>
#include <furi_hal_spi.h>
#include "predator_boards.h"
#include "predator_calypso_production_keys.h"
#include "predator_crypto_engine.h"

// PN532 Commands
#define PN532_COMMAND_GETFIRMWAREVERSION    0x02
#define PN532_COMMAND_SAMCONFIGURATION      0x14
#define PN532_COMMAND_INLISTPASSIVETARGET   0x4A
#define PN532_COMMAND_INDATAEXCHANGE        0x40
#define PN532_COMMAND_INDESELECT            0x44
#define PN532_COMMAND_INRELEASE             0x52

// PN532 Frame Structure
#define PN532_PREAMBLE                      0x00
#define PN532_STARTCODE1                    0x00
#define PN532_STARTCODE2                    0xFF
#define PN532_POSTAMBLE                     0x00

#define PN532_HOSTTOPN532                   0xD4
#define PN532_PN532TOHOST                   0xD5

// ISO14443 Type B (Calypso)
#define PN532_MIFARE_ISO14443A              0x00
#define PN532_ISO14443B                     0x03

// PN532 Context
typedef struct {
    const PredatorBoardConfig* board_config;
    FuriHalSpiBusHandle* spi_handle;
    bool initialized;
    uint8_t firmware_version[4];
    uint8_t last_error;
} PN532Context;

// Card Information
typedef struct {
    uint8_t uid[10];
    uint8_t uid_length;
    uint8_t sak;
    uint8_t atqa[2];
    bool is_calypso;
    CalypsoNetworkId network_id;
} PN532CardInfo;

/**
 * @brief Initialize PN532 NFC writer
 * @param ctx PN532 context
 * @param board_config Board configuration with GPIO pins
 * @return true if initialization successful
 */
bool predator_pn532_init(PN532Context* ctx, const PredatorBoardConfig* board_config);

/**
 * @brief Deinitialize PN532 NFC writer
 * @param ctx PN532 context
 */
void predator_pn532_deinit(PN532Context* ctx);

/**
 * @brief Get PN532 firmware version
 * @param ctx PN532 context
 * @return true if successful
 */
bool predator_pn532_get_firmware_version(PN532Context* ctx);

/**
 * @brief Configure PN532 SAM (Security Access Module)
 * @param ctx PN532 context
 * @return true if successful
 */
bool predator_pn532_sam_config(PN532Context* ctx);

/**
 * @brief Scan for ISO14443 cards (including Calypso)
 * @param ctx PN532 context
 * @param card_info Card information output
 * @return true if card found
 */
bool predator_pn532_scan_card(PN532Context* ctx, PN532CardInfo* card_info);

/**
 * @brief Read Calypso card data
 * @param ctx PN532 context
 * @param card_info Card information
 * @param calypso_ctx Output Calypso context
 * @return true if successful
 */
bool predator_pn532_read_calypso(PN532Context* ctx, PN532CardInfo* card_info, CalypsoContext* calypso_ctx);

/**
 * @brief Write Calypso card data to blank card
 * @param ctx PN532 context
 * @param calypso_ctx Source Calypso context
 * @return true if successful
 */
bool predator_pn532_write_calypso(PN532Context* ctx, CalypsoContext* calypso_ctx);

/**
 * @brief Authenticate with Calypso card using production keys
 * @param ctx PN532 context
 * @param network_id Network identifier
 * @param card_id Card ID
 * @return true if authentication successful
 */
bool predator_pn532_authenticate_calypso(PN532Context* ctx, CalypsoNetworkId network_id, uint8_t* card_id);

/**
 * @brief Clone Calypso card (read source, write to blank)
 * @param ctx PN532 context
 * @param progress_callback Progress callback function
 * @return true if cloning successful
 */
bool predator_pn532_clone_calypso_card(PN532Context* ctx, void (*progress_callback)(uint8_t percent));

/**
 * @brief Check if board supports NFC writer
 * @param board_config Board configuration
 * @return true if NFC writer supported
 */
bool predator_pn532_is_supported(const PredatorBoardConfig* board_config);
