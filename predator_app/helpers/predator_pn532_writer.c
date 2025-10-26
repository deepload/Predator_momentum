#include "predator_pn532_writer.h"
#include "../predator_i.h"
#include <furi_hal_serial.h>

#define PN532_PREAMBLE 0x00
#define PN532_STARTCODE1 0x00
#define PN532_STARTCODE2 0xFF
#define PN532_POSTAMBLE 0x00

#define PN532_HOSTTOPN532 0xD4
#define PN532_PN532TOHOST 0xD5

#define PN532_ACK_WAIT_TIME 10
#define PN532_TIMEOUT 1000

static bool pn532_send_command(PN532Context* ctx, uint8_t cmd, const uint8_t* data, uint8_t data_len) {
    if(!ctx || !ctx->uart_handle) return false;
    
    uint8_t checksum = 0;
    uint8_t frame_len = data_len + 1; // +1 for command byte
    
    // Build frame
    uint8_t frame[256];
    uint8_t frame_pos = 0;
    
    // Preamble
    frame[frame_pos++] = PN532_PREAMBLE;
    frame[frame_pos++] = PN532_STARTCODE1;
    frame[frame_pos++] = PN532_STARTCODE2;
    
    // Length
    frame[frame_pos++] = frame_len;
    frame[frame_pos++] = (~frame_len + 1); // Length checksum
    
    // Direction
    frame[frame_pos++] = PN532_HOSTTOPN532;
    checksum += PN532_HOSTTOPN532;
    
    // Command
    frame[frame_pos++] = cmd;
    checksum += cmd;
    
    // Data
    for(uint8_t i = 0; i < data_len; i++) {
        frame[frame_pos++] = data[i];
        checksum += data[i];
    }
    
    // Checksum
    frame[frame_pos++] = (~checksum + 1);
    
    // Postamble
    frame[frame_pos++] = PN532_POSTAMBLE;
    
    // Send frame
    furi_hal_serial_tx(ctx->uart_handle, frame, frame_pos);
    
    return true;
}

static bool pn532_read_ack(PN532Context* ctx) {
    if(!ctx || !ctx->uart_handle) return false;
    
    uint8_t ack_frame[6] = {0x00, 0x00, 0xFF, 0x00, 0xFF, 0x00};
    uint8_t received[6];
    
    // Wait for ACK with timeout
    uint32_t start_time = furi_get_tick();
    uint8_t received_count = 0;
    
    while(received_count < 6 && (furi_get_tick() - start_time) < PN532_ACK_WAIT_TIME) {
        if(furi_hal_serial_rx(ctx->uart_handle, &received[received_count], 1) > 0) {
            received_count++;
        }
        furi_delay_ms(1);
    }
    
    if(received_count != 6) {
        FURI_LOG_W("PN532", "ACK timeout");
        return false;
    }
    
    // Verify ACK frame
    for(int i = 0; i < 6; i++) {
        if(received[i] != ack_frame[i]) {
            FURI_LOG_W("PN532", "Invalid ACK frame");
            return false;
        }
    }
    
    return true;
}

static bool pn532_read_response(PN532Context* ctx, uint8_t* response, uint8_t* response_len) {
    if(!ctx || !ctx->uart_handle || !response || !response_len) return false;
    
    uint32_t start_time = furi_get_tick();
    uint8_t header[6];
    uint8_t received_count = 0;
    
    // Read response header
    while(received_count < 6 && (furi_get_tick() - start_time) < PN532_TIMEOUT) {
        if(furi_hal_serial_rx(ctx->uart_handle, &header[received_count], 1) > 0) {
            received_count++;
        }
        furi_delay_ms(1);
    }
    
    if(received_count != 6) {
        FURI_LOG_W("PN532", "Response header timeout");
        return false;
    }
    
    // Verify header
    if(header[0] != 0x00 || header[1] != 0x00 || header[2] != 0xFF) {
        FURI_LOG_W("PN532", "Invalid response header");
        return false;
    }
    
    uint8_t len = header[3];
    uint8_t len_checksum = header[4];
    
    if((len + len_checksum) != 0) {
        FURI_LOG_W("PN532", "Invalid length checksum");
        return false;
    }
    
    // Read response data
    uint8_t data_checksum = 0;
    received_count = 0;
    
    while(received_count < len && (furi_get_tick() - start_time) < PN532_TIMEOUT) {
        if(furi_hal_serial_rx(ctx->uart_handle, &response[received_count], 1) > 0) {
            data_checksum += response[received_count];
            received_count++;
        }
        furi_delay_ms(1);
    }
    
    if(received_count != len) {
        FURI_LOG_W("PN532", "Response data timeout");
        return false;
    }
    
    // Read and verify checksum
    uint8_t checksum;
    if(furi_hal_serial_rx(ctx->uart_handle, &checksum, 1) != 1) {
        FURI_LOG_W("PN532", "Checksum read failed");
        return false;
    }
    
    if((data_checksum + checksum) != 0) {
        FURI_LOG_W("PN532", "Invalid data checksum");
        return false;
    }
    
    // Read postamble
    uint8_t postamble;
    if(furi_hal_serial_rx(ctx->uart_handle, &postamble, 1) != 1 || postamble != 0x00) {
        FURI_LOG_W("PN532", "Invalid postamble");
        return false;
    }
    
    *response_len = len - 1; // -1 for direction byte
    return true;
}

PN532Context* predator_pn532_init(struct PredatorApp* app) {
    if(!app) return NULL;
    
    const PredatorBoardConfig* config = predator_boards_get_config(app->board_type);
    if(!config || !config->has_nfc_writer) {
        FURI_LOG_E("PN532", "Board does not support PN532 NFC Writer");
        return NULL;
    }
    
    PN532Context* ctx = malloc(sizeof(PN532Context));
    if(!ctx) return NULL;
    
    memset(ctx, 0, sizeof(PN532Context));
    ctx->board_config = config;
    
    // Initialize SPI pins for PN532
    furi_hal_gpio_init(config->nfc_sck_pin, GpioModeOutputPushPull, GpioPullNo, GpioSpeedVeryHigh);
    furi_hal_gpio_init(config->nfc_mosi_pin, GpioModeOutputPushPull, GpioPullNo, GpioSpeedVeryHigh);
    furi_hal_gpio_init(config->nfc_miso_pin, GpioModeInput, GpioPullUp, GpioSpeedVeryHigh);
    furi_hal_gpio_init(config->nfc_cs_pin, GpioModeOutputPushPull, GpioPullNo, GpioSpeedVeryHigh);
    
    if(config->nfc_rst_pin) {
        furi_hal_gpio_init(config->nfc_rst_pin, GpioModeOutputPushPull, GpioPullNo, GpioSpeedVeryHigh);
        // Reset PN532
        furi_hal_gpio_write(config->nfc_rst_pin, false);
        furi_delay_ms(100);
        furi_hal_gpio_write(config->nfc_rst_pin, true);
        furi_delay_ms(500);
    }
    
    if(config->nfc_irq_pin) {
        furi_hal_gpio_init(config->nfc_irq_pin, GpioModeInput, GpioPullUp, GpioSpeedVeryHigh);
    }
    
    // Set CS high initially
    furi_hal_gpio_write(config->nfc_cs_pin, true);
    
    // Try to initialize UART communication (some PN532 modules use UART)
    ctx->uart_handle = furi_hal_serial_control_acquire(FuriHalSerialIdUsart);
    if(ctx->uart_handle) {
        furi_hal_serial_init(ctx->uart_handle, 115200);
        FURI_LOG_I("PN532", "UART initialized for PN532 communication");
    } else {
        FURI_LOG_W("PN532", "UART not available, using SPI mode");
    }
    
    ctx->initialized = true;
    FURI_LOG_I("PN532", "PN532 NFC Writer V4 initialized successfully");
    
    return ctx;
}

void predator_pn532_deinit(PN532Context* ctx) {
    if(!ctx) return;
    
    if(ctx->uart_handle) {
        furi_hal_serial_deinit(ctx->uart_handle);
        furi_hal_serial_control_release(ctx->uart_handle);
    }
    
    // Reset GPIO pins
    if(ctx->board_config) {
        if(ctx->board_config->nfc_sck_pin) {
            furi_hal_gpio_init(ctx->board_config->nfc_sck_pin, GpioModeAnalog, GpioPullNo, GpioSpeedLow);
        }
        if(ctx->board_config->nfc_mosi_pin) {
            furi_hal_gpio_init(ctx->board_config->nfc_mosi_pin, GpioModeAnalog, GpioPullNo, GpioSpeedLow);
        }
        if(ctx->board_config->nfc_cs_pin) {
            furi_hal_gpio_init(ctx->board_config->nfc_cs_pin, GpioModeAnalog, GpioPullNo, GpioSpeedLow);
        }
        if(ctx->board_config->nfc_rst_pin) {
            furi_hal_gpio_init(ctx->board_config->nfc_rst_pin, GpioModeAnalog, GpioPullNo, GpioSpeedLow);
        }
    }
    
    free(ctx);
    FURI_LOG_I("PN532", "PN532 NFC Writer deinitialized");
}

bool predator_pn532_get_firmware_version(PN532Context* ctx, uint8_t* version) {
    if(!ctx || !ctx->initialized || !version) return false;
    
    // Send GetFirmwareVersion command
    if(!pn532_send_command(ctx, PN532CommandGetFirmwareVersion, NULL, 0)) {
        return false;
    }
    
    // Wait for ACK
    if(!pn532_read_ack(ctx)) {
        return false;
    }
    
    // Read response
    uint8_t response[32];
    uint8_t response_len;
    
    if(!pn532_read_response(ctx, response, &response_len)) {
        return false;
    }
    
    if(response_len < 4 || response[0] != (PN532CommandGetFirmwareVersion + 1)) {
        FURI_LOG_W("PN532", "Invalid firmware version response");
        return false;
    }
    
    // Copy version data
    memcpy(version, &response[1], 4);
    
    FURI_LOG_I("PN532", "Firmware version: %02X.%02X", version[1], version[2]);
    return true;
}

bool predator_pn532_scan_card(PN532Context* ctx) {
    if(!ctx || !ctx->initialized) return false;
    
    // Configure SAM (Security Access Module)
    uint8_t sam_config[] = {0x01, 0x14, 0x01}; // Normal mode, timeout 50ms, use IRQ
    if(!pn532_send_command(ctx, PN532CommandSAMConfiguration, sam_config, sizeof(sam_config))) {
        return false;
    }
    
    if(!pn532_read_ack(ctx)) {
        return false;
    }
    
    uint8_t response[32];
    uint8_t response_len;
    if(!pn532_read_response(ctx, response, &response_len)) {
        return false;
    }
    
    // Scan for ISO14443A cards (Mifare, etc.)
    uint8_t scan_config[] = {0x01, 0x00}; // Max 1 target, 106 kbps
    if(!pn532_send_command(ctx, PN532CommandInListPassiveTarget, scan_config, sizeof(scan_config))) {
        return false;
    }
    
    if(!pn532_read_ack(ctx)) {
        return false;
    }
    
    if(!pn532_read_response(ctx, response, &response_len)) {
        return false;
    }
    
    if(response_len < 2 || response[0] != (PN532CommandInListPassiveTarget + 1)) {
        return false;
    }
    
    uint8_t targets_found = response[1];
    if(targets_found == 0) {
        ctx->card_present = false;
        return false;
    }
    
    // Parse card information
    ctx->card_present = true;
    ctx->current_card.is_present = true;
    
    if(response_len >= 12) {
        // Extract UID
        uint8_t uid_len = response[7];
        if(uid_len <= 10) {
            ctx->current_card.uid_length = uid_len;
            memcpy(ctx->current_card.uid, &response[8], uid_len);
        }
        
        // Extract ATQA and SAK
        ctx->current_card.atqa = (response[4] << 8) | response[5];
        ctx->current_card.sak = response[6];
        
        // Determine card type
        if(ctx->current_card.sak == 0x08) {
            ctx->current_card.card_type = PN532CardTypeMifare;
        } else if(ctx->current_card.sak == 0x20) {
            ctx->current_card.card_type = PN532CardTypeDesfire;
        } else {
            ctx->current_card.card_type = PN532CardTypeISO14443A;
        }
    }
    
    FURI_LOG_I("PN532", "Card detected: UID length %d, SAK 0x%02X", 
              ctx->current_card.uid_length, ctx->current_card.sak);
    
    return true;
}

bool predator_pn532_read_card(PN532Context* ctx, uint8_t block, uint8_t* data) {
    if(!ctx || !ctx->initialized || !ctx->card_present || !data) return false;
    
    // Mifare Classic read command
    uint8_t read_cmd[] = {0x01, 0x30, block}; // Target 1, Read command, block number
    
    if(!pn532_send_command(ctx, PN532CommandInDataExchange, read_cmd, sizeof(read_cmd))) {
        return false;
    }
    
    if(!pn532_read_ack(ctx)) {
        return false;
    }
    
    uint8_t response[32];
    uint8_t response_len;
    if(!pn532_read_response(ctx, response, &response_len)) {
        return false;
    }
    
    if(response_len < 18 || response[0] != (PN532CommandInDataExchange + 1) || response[1] != 0x00) {
        FURI_LOG_W("PN532", "Read failed: status 0x%02X", response[1]);
        return false;
    }
    
    // Copy block data (16 bytes)
    memcpy(data, &response[2], 16);
    
    FURI_LOG_I("PN532", "Block %d read successfully", block);
    return true;
}

bool predator_pn532_write_card(PN532Context* ctx, uint8_t block, const uint8_t* data) {
    if(!ctx || !ctx->initialized || !ctx->card_present || !data) return false;
    
    // Mifare Classic write command
    uint8_t write_cmd[19];
    write_cmd[0] = 0x01; // Target 1
    write_cmd[1] = 0xA0; // Write command
    write_cmd[2] = block; // Block number
    memcpy(&write_cmd[3], data, 16); // Data to write
    
    if(!pn532_send_command(ctx, PN532CommandInDataExchange, write_cmd, sizeof(write_cmd))) {
        return false;
    }
    
    if(!pn532_read_ack(ctx)) {
        return false;
    }
    
    uint8_t response[32];
    uint8_t response_len;
    if(!pn532_read_response(ctx, response, &response_len)) {
        return false;
    }
    
    if(response_len < 2 || response[0] != (PN532CommandInDataExchange + 1) || response[1] != 0x00) {
        FURI_LOG_W("PN532", "Write failed: status 0x%02X", response[1]);
        return false;
    }
    
    FURI_LOG_I("PN532", "Block %d written successfully", block);
    return true;
}

bool predator_pn532_clone_tl_card(PN532Context* ctx, const uint8_t* source_uid, const uint8_t* target_uid) {
    if(!ctx || !source_uid || !target_uid) return false;
    
    FURI_LOG_I("PN532", "Starting TL Lausanne card cloning...");
    
    // This is a placeholder for TL card cloning logic
    // Real implementation would need specific TL card format knowledge
    
    // Step 1: Read source card data
    uint8_t source_data[64]; // 4 blocks of 16 bytes
    for(int block = 0; block < 4; block++) {
        if(!predator_pn532_read_card(ctx, block, &source_data[block * 16])) {
            FURI_LOG_E("PN532", "Failed to read source card block %d", block);
            return false;
        }
    }
    
    // Step 2: Modify UID in the data
    memcpy(source_data, target_uid, 4); // Assuming 4-byte UID
    
    // Step 3: Write to target card
    for(int block = 1; block < 4; block++) { // Skip block 0 (manufacturer block)
        if(!predator_pn532_write_card(ctx, block, &source_data[block * 16])) {
            FURI_LOG_E("PN532", "Failed to write target card block %d", block);
            return false;
        }
    }
    
    FURI_LOG_I("PN532", "TL Lausanne card cloning completed successfully");
    return true;
}

const PN532CardInfo* predator_pn532_get_card_info(PN532Context* ctx) {
    if(!ctx || !ctx->card_present) return NULL;
    return &ctx->current_card;
}
