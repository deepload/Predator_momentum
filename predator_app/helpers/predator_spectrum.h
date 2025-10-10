#pragma once

#include "../predator_i.h"

// Spectrum analyzer for RF signal intelligence
typedef struct {
    uint32_t frequency;
    int8_t rssi;
    uint32_t bandwidth;
    uint8_t modulation_type;
} SpectrumPoint;

typedef enum {
    ModulationUnknown,
    ModulationAM,
    ModulationFM,
    ModulationASK,
    ModulationFSK,
    ModulationPSK,
    ModulationQAM,
} ModulationType;

// Spectrum analyzer functions
bool predator_spectrum_init(PredatorApp* app);
void predator_spectrum_deinit(PredatorApp* app);
bool predator_spectrum_scan_range(PredatorApp* app, uint32_t freq_start, uint32_t freq_end, uint32_t step);
bool predator_spectrum_get_rssi(PredatorApp* app, uint32_t frequency, int8_t* rssi);
ModulationType predator_spectrum_detect_modulation(PredatorApp* app, uint32_t frequency);
bool predator_spectrum_measure_bandwidth(PredatorApp* app, uint32_t frequency, uint32_t* bandwidth);

// Waterfall display data
#define SPECTRUM_WATERFALL_HEIGHT 64
#define SPECTRUM_WATERFALL_WIDTH 128
typedef struct {
    int8_t data[SPECTRUM_WATERFALL_HEIGHT][SPECTRUM_WATERFALL_WIDTH];
    uint32_t freq_start;
    uint32_t freq_end;
    uint8_t current_row;
} SpectrumWaterfall;

void predator_spectrum_waterfall_init(SpectrumWaterfall* waterfall, uint32_t freq_start, uint32_t freq_end);
void predator_spectrum_waterfall_add_scan(SpectrumWaterfall* waterfall, int8_t* rssi_data, size_t count);
