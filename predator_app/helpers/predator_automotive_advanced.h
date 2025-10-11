#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct PredatorApp PredatorApp;

// Extended car manufacturers (beyond base 90+ models)
typedef enum {
    AutoMfg_Lamborghini,
    AutoMfg_Ferrari,
    AutoMfg_Maserati,
    AutoMfg_AstonMartin,
    AutoMfg_Bentley,
    AutoMfg_RollsRoyce,
    AutoMfg_McLaren,
    AutoMfg_Bugatti,
    AutoMfg_Rivian,
    AutoMfg_Lucid,
    AutoMfg_Polestar,
    AutoMfg_Genesis,
} ExtendedAutoManufacturer;

// Advanced key systems
typedef enum {
    KeySystem_Megamos,       // VW/Audi crypto
    KeySystem_Hitag2,        // Older European
    KeySystem_DST40,         // Texas Instruments
    KeySystem_DST80,         // Texas Instruments advanced
    KeySystem_PCF7935,       // NXP transponder
    KeySystem_Tiris,         // TI RFID
} KeySystemType;

// Immobilizer types
typedef enum {
    Immobilizer_Crypto1,     // Older systems
    Immobilizer_Crypto2,     // Modern systems
    Immobilizer_AES,         // Advanced encryption
    Immobilizer_None,        // Older vehicles
} ImmobilizerType;

// Initialization
bool predator_auto_advanced_init(PredatorApp* app);
void predator_auto_advanced_deinit(PredatorApp* app);

// Rolling code advanced
bool predator_auto_rolling_code_analyze(PredatorApp* app, const uint8_t* signal, size_t len);
bool predator_auto_rolling_code_predict(PredatorApp* app, uint32_t current_code, uint32_t* next_code);
bool predator_auto_rolling_code_jam_replay(PredatorApp* app, uint32_t frequency);

// Keyless entry exploits
bool predator_auto_keyless_relay_attack(PredatorApp* app, float max_distance);
bool predator_auto_keyless_amplification(PredatorApp* app);
bool predator_auto_passive_entry_test(PredatorApp* app);

// Immobilizer bypass
bool predator_auto_immobilizer_detect(PredatorApp* app, ImmobilizerType* type);
bool predator_auto_immobilizer_bypass(PredatorApp* app, ImmobilizerType type);
bool predator_auto_transponder_clone(PredatorApp* app, KeySystemType key_type);

// Push-button start
bool predator_auto_pushbutton_test(PredatorApp* app);
bool predator_auto_starter_interrupt(PredatorApp* app, bool enable);

// Remote start
bool predator_auto_remote_start_capture(PredatorApp* app, uint8_t* command, size_t* len);
bool predator_auto_remote_start_replay(PredatorApp* app, const uint8_t* command, size_t len);

// Car alarm systems
bool predator_auto_alarm_detect(PredatorApp* app, const char* manufacturer, size_t mfg_len);
bool predator_auto_alarm_disable(PredatorApp* app);
bool predator_auto_alarm_trigger_test(PredatorApp* app);

// Extended manufacturer database
bool predator_auto_luxury_brand_test(PredatorApp* app, ExtendedAutoManufacturer mfg);
bool predator_auto_ev_brand_test(PredatorApp* app, ExtendedAutoManufacturer mfg);
