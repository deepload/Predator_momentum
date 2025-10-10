#pragma once

#include "../predator_i.h"

// Car model definitions
typedef enum {
    CarModelToyota,
    CarModelHonda,
    CarModelFord,
    CarModelChevrolet,
    CarModelBMW,
    CarModelMercedes,
    CarModelAudi,
    CarModelVolkswagen,
    CarModelNissan,
    CarModelHyundai,
    CarModelKia,
    CarModelTesla,
    CarModelSubaru,
    CarModelJeep,
    CarModelChrysler,
    CarModelDodge,
    CarModelCadillac,
    CarModelLexus,
    CarModelInfiniti,
    CarModelAcura,
    CarModelMazda,
    CarModelMitsubishi,
    CarModelPorsche,
    CarModelRangeRover,
    CarModelJaguar,
    CarModelVolvo,
    CarModelFiat,
    CarModelPeugeot,
    CarModelRenault,
    CarModelSkoda,
    CarModelLamborghini,
    CarModelFerrari,
    CarModelMaserati,
    CarModelBentley,
    CarModelRollsRoyce,
    CarModelCount
} CarModel;

// Car command definitions
typedef enum {
    CarCommandUnlock,
    CarCommandLock,
    CarCommandTrunk,
    CarCommandStart,
    CarCommandPanic,
    CarCommandCount
} CarCommand;

void predator_subghz_init(PredatorApp* app);
void predator_subghz_deinit(PredatorApp* app);
bool predator_subghz_start_car_bruteforce(PredatorApp* app, uint32_t frequency);
void predator_subghz_send_car_key(PredatorApp* app, uint32_t key_code);
bool predator_subghz_start_jamming(PredatorApp* app, uint32_t frequency);
bool predator_subghz_stop_attack(PredatorApp* app);
void predator_subghz_send_tesla_charge_port(PredatorApp* app);

// GOVERNMENT-GRADE: Real hardware transmission functions
bool predator_subghz_transmit_signal(
    PredatorApp* app,
    uint32_t frequency,
    const char* protocol,
    uint64_t key,
    uint8_t repeat_count
);

bool predator_subghz_transmit_raw(
    PredatorApp* app,
    uint32_t frequency,
    const uint32_t* timings,
    size_t timings_count,
    uint8_t repeat_count
);

bool predator_subghz_jam_frequency(
    PredatorApp* app,
    uint32_t frequency,
    uint32_t duration_ms,
    uint8_t power_level
);

// Rolling code attack functions
bool predator_subghz_start_rolling_code_attack(PredatorApp* app, uint32_t frequency);
void predator_subghz_stop_rolling_code_attack(PredatorApp* app);
void predator_subghz_rolling_code_attack_tick(PredatorApp* app);

// New car model specific functions
const char* predator_subghz_get_car_model_name(CarModel model);
const char* predator_subghz_get_car_command_name(CarCommand command);
bool predator_subghz_send_car_command(PredatorApp* app, CarModel model, CarCommand command);

// Passive car opener mode functions
void predator_subghz_start_passive_car_opener(PredatorApp* app);
void predator_subghz_stop_passive_car_opener(PredatorApp* app);
void predator_subghz_passive_car_opener_tick(PredatorApp* app);
