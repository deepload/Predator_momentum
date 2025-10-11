#pragma once
#include <stdint.h>
#include <stdbool.h>

typedef struct PredatorApp PredatorApp;

// Casino chip RFID types
typedef enum {
    CasinoChip_HF_13_56MHz,    // High Frequency (most common)
    CasinoChip_UHF_915MHz,     // Ultra High Frequency
    CasinoChip_LF_125kHz,      // Low Frequency (older)
} CasinoChipType;

// Slot machine protocols
typedef enum {
    SlotProtocol_SAS,          // Slot Accounting System
    SlotProtocol_TITO,         // Ticket In Ticket Out
    SlotProtocol_OASIS,        // Online Accounting System
    SlotProtocol_GSA,          // Gaming Standards Association
} SlotMachineProtocol;

// Casino card systems
typedef enum {
    CasinoCard_PlayerTracking,
    CasinoCard_CashlessGaming,
    CasinoCard_EmployeeAccess,
    CasinoCard_VIPAccess,
} CasinoCardType;

// Initialization
bool predator_casino_init(PredatorApp* app);
void predator_casino_deinit(PredatorApp* app);

// Casino chip analysis
bool predator_casino_chip_scan(PredatorApp* app, CasinoChipType type);
bool predator_casino_chip_read_id(PredatorApp* app, uint8_t* chip_id, size_t id_len);
bool predator_casino_chip_authenticate(PredatorApp* app, const uint8_t* key);
bool predator_casino_chip_value_read(PredatorApp* app, uint32_t* value_cents);

// Slot machine communication
bool predator_casino_slot_detect(PredatorApp* app, SlotMachineProtocol* protocol);
bool predator_casino_slot_sas_sniff(PredatorApp* app);
bool predator_casino_slot_tito_analyze(PredatorApp* app, const uint8_t* ticket_data);

// Casino access control
bool predator_casino_access_scan(PredatorApp* app);
bool predator_casino_access_clone(PredatorApp* app, CasinoCardType card_type);
bool predator_casino_vip_card_test(PredatorApp* app);

// Payment terminals
bool predator_casino_terminal_scan(PredatorApp* app);
bool predator_casino_terminal_protocol_detect(PredatorApp* app);
bool predator_casino_cashout_intercept(PredatorApp* app);

// Loyalty cards
bool predator_casino_loyalty_scan(PredatorApp* app);
bool predator_casino_loyalty_points_read(PredatorApp* app, uint32_t* points);
bool predator_casino_loyalty_tier_read(PredatorApp* app, char* tier_name, size_t len);

// Advanced RFID (Atlantic City specific)
bool predator_casino_desfire_full_dump(PredatorApp* app);
bool predator_casino_mifare_plus_attack(PredatorApp* app);
bool predator_casino_hid_iclass_elite(PredatorApp* app);
bool predator_casino_hotel_keycard_analyze(PredatorApp* app);
