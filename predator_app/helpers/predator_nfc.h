#pragma once

#include <furi.h>
#include <nfc/nfc.h>
#include <nfc/nfc_device.h>
#include <nfc/protocols/mf_classic/mf_classic.h>
#include <nfc/protocols/mf_ultralight/mf_ultralight.h>
#include <nfc/protocols/iso14443_3a/iso14443_3a.h>

typedef struct PredatorApp PredatorApp;

// NFC card types
typedef enum {
    PredatorNfcTypeUnknown,
    PredatorNfcTypeMifareClassic1K,
    PredatorNfcTypeMifareClassic4K,
    PredatorNfcTypeMifareUltralight,
    PredatorNfcTypeNTAG213,
    PredatorNfcTypeNTAG215,
    PredatorNfcTypeNTAG216,
    PredatorNfcTypeISO14443A,
} PredatorNfcType;

// NFC operation status
typedef enum {
    PredatorNfcStatusIdle,
    PredatorNfcStatusDetecting,
    PredatorNfcStatusReading,
    PredatorNfcStatusWriting,
    PredatorNfcStatusComplete,
    PredatorNfcStatusError,
} PredatorNfcStatus;

// NFC card data structure
typedef struct {
    PredatorNfcType type;
    uint8_t uid[10];
    uint8_t uid_len;
    char uid_str[32];
    char type_str[32];
    uint16_t total_blocks;
    uint16_t blocks_read;
    uint16_t blocks_written;
    bool authenticated;
    void* protocol_data; // Protocol-specific data
} PredatorNfcCard;

// NFC management functions
void predator_nfc_init(PredatorApp* app);
void predator_nfc_deinit(PredatorApp* app);

// Card detection and reading
bool predator_nfc_detect_card(PredatorApp* app, PredatorNfcCard* card);
bool predator_nfc_read_card(PredatorApp* app, PredatorNfcCard* card);
bool predator_nfc_write_card(PredatorApp* app, PredatorNfcCard* card);

// Card information
const char* predator_nfc_get_type_string(PredatorNfcType type);
void predator_nfc_format_uid(uint8_t* uid, uint8_t uid_len, char* out, size_t out_len);

// Card operations
bool predator_nfc_save_card(PredatorApp* app, PredatorNfcCard* card, const char* name);
bool predator_nfc_load_card(PredatorApp* app, PredatorNfcCard* card, const char* name);

// Status
bool predator_nfc_is_ready(PredatorApp* app);
