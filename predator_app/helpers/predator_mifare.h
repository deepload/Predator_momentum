#pragma once

#include "../predator_i.h"

// Advanced MIFARE attacks
typedef struct {
    uint8_t uid[7];
    uint8_t uid_len;
    uint8_t sak;
    uint8_t atqa[2];
    uint8_t keys_a[16][6];
    uint8_t keys_b[16][6];
    bool keys_found_a[16];
    bool keys_found_b[16];
    uint8_t data[16][4][16];
} MifareCard;

// MIFARE Classic attacks
bool predator_mifare_nested_attack(PredatorApp* app, MifareCard* card, uint8_t known_sector, uint8_t target_sector);
bool predator_mifare_hardnested_attack(PredatorApp* app, MifareCard* card, uint8_t known_sector, uint8_t target_sector);
bool predator_mifare_darkside_attack(PredatorApp* app, MifareCard* card);
bool predator_mifare_static_nested_attack(PredatorApp* app, MifareCard* card);

// Key recovery
bool predator_mifare_recover_key_a(PredatorApp* app, MifareCard* card, uint8_t sector, uint8_t* key);
bool predator_mifare_recover_key_b(PredatorApp* app, MifareCard* card, uint8_t sector, uint8_t* key);
bool predator_mifare_check_default_keys(PredatorApp* app, MifareCard* card);

// Dictionary attack
bool predator_mifare_dictionary_attack(PredatorApp* app, MifareCard* card, const char* dict_path);

// Card operations
bool predator_mifare_read_sector(PredatorApp* app, MifareCard* card, uint8_t sector, const uint8_t* key_a);
bool predator_mifare_write_sector(PredatorApp* app, MifareCard* card, uint8_t sector, const uint8_t* key_a, const uint8_t* data);
bool predator_mifare_format_card(PredatorApp* app, MifareCard* card);

// MIFARE Plus
bool predator_mifare_plus_attack(PredatorApp* app, MifareCard* card);

// MIFARE DESFire
typedef struct {
    uint8_t uid[7];
    uint8_t uid_len;
    uint8_t app_ids[28][3];
    uint8_t app_count;
    uint8_t master_key[16];
    bool master_key_found;
} MifareDESFireCard;

bool predator_desfire_enumerate_apps(PredatorApp* app, MifareDESFireCard* card);
bool predator_desfire_attack_master_key(PredatorApp* app, MifareDESFireCard* card);
bool predator_desfire_read_app_data(PredatorApp* app, MifareDESFireCard* card, const uint8_t* app_id);
