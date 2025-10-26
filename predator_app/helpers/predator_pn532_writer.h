#pragma once

#include <furi.h>
#include <furi_hal.h>
#include "predator_boards.h"

/**
 * @brief PN532 NFC Writer V4 Support
 * Dedicated support for PN532 NFC modules for TL Lausanne card writing
 */

typedef enum {
    PN532CommandGetFirmwareVersion = 0x02,
    PN532CommandSAMConfiguration = 0x14,
    PN532CommandInListPassiveTarget = 0x4A,
    PN532CommandInDataExchange = 0x40,
    PN532CommandInAutoPoll = 0x60
} PN532Command;

typedef enum {
    PN532CardTypeISO14443A = 0x00,
    PN532CardTypeISO14443B = 0x01,
    PN532CardTypeFelica212 = 0x02,
    PN532CardTypeFelica424 = 0x03,
    PN532CardTypeJewel = 0x04,
    PN532CardTypeMifare = 0x10,
    PN532CardTypeDesfire = 0x20
} PN532CardType;

typedef struct {
    uint8_t uid[10];
    uint8_t uid_length;
    PN532CardType card_type;
    uint16_t atqa;
    uint8_t sak;
    bool is_present;
} PN532CardInfo;

typedef struct {
    FuriHalSerialHandle* uart_handle;
    const PredatorBoardConfig* board_config;
    bool initialized;
    bool card_present;
    PN532CardInfo current_card;
} PN532Context;

/**
 * @brief Initialize PN532 NFC Writer
 * @param app PredatorApp context
 * @return PN532Context* or NULL on failure
 */
PN532Context* predator_pn532_init(struct PredatorApp* app);

/**
 * @brief Deinitialize PN532 NFC Writer
 * @param ctx PN532 context
 */
void predator_pn532_deinit(PN532Context* ctx);

/**
 * @brief Get PN532 firmware version
 * @param ctx PN532 context
 * @param version Buffer to store version (4 bytes)
 * @return true on success
 */
bool predator_pn532_get_firmware_version(PN532Context* ctx, uint8_t* version);

/**
 * @brief Scan for NFC cards
 * @param ctx PN532 context
 * @return true if card detected
 */
bool predator_pn532_scan_card(PN532Context* ctx);

/**
 * @brief Read card data
 * @param ctx PN532 context
 * @param block Block number to read
 * @param data Buffer to store data (16 bytes)
 * @return true on success
 */
bool predator_pn532_read_card(PN532Context* ctx, uint8_t block, uint8_t* data);

/**
 * @brief Write card data (for TL Lausanne cards)
 * @param ctx PN532 context
 * @param block Block number to write
 * @param data Data to write (16 bytes)
 * @return true on success
 */
bool predator_pn532_write_card(PN532Context* ctx, uint8_t block, const uint8_t* data);

/**
 * @brief Clone TL Lausanne card
 * @param ctx PN532 context
 * @param source_uid Source card UID
 * @param target_uid Target card UID
 * @return true on success
 */
bool predator_pn532_clone_tl_card(PN532Context* ctx, const uint8_t* source_uid, const uint8_t* target_uid);

/**
 * @brief Get current card info
 * @param ctx PN532 context
 * @return PN532CardInfo* or NULL if no card
 */
const PN532CardInfo* predator_pn532_get_card_info(PN532Context* ctx);
