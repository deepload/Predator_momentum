#include "predator_spectrum.h"
#include "../predator_i.h"

bool predator_spectrum_init(PredatorApp* app) {
    if(!app) return false;
    FURI_LOG_I("Spectrum", "Spectrum analyzer initialized");
    return true;
}

void predator_spectrum_deinit(PredatorApp* app) {
    if(!app) return;
    FURI_LOG_I("Spectrum", "Spectrum analyzer deinitialized");
}

bool predator_spectrum_scan_range(PredatorApp* app, uint32_t freq_start, uint32_t freq_end, uint32_t step) {
    if(!app) return false;
    FURI_LOG_I("Spectrum", "Scanning %lu-%lu MHz (step: %lu)", freq_start/1000000, freq_end/1000000, step);
    return true;
}

bool predator_spectrum_get_rssi(PredatorApp* app, uint32_t frequency, int8_t* rssi) {
    if(!app || !rssi) return false;
    *rssi = -80; // Stub value
    return true;
}

ModulationType predator_spectrum_detect_modulation(PredatorApp* app, uint32_t frequency) {
    if(!app) return ModulationUnknown;
    return ModulationASK; // Stub
}

bool predator_spectrum_measure_bandwidth(PredatorApp* app, uint32_t frequency, uint32_t* bandwidth) {
    if(!app || !bandwidth) return false;
    *bandwidth = 25000; // Stub: 25kHz
    return true;
}

void predator_spectrum_waterfall_init(SpectrumWaterfall* waterfall, uint32_t freq_start, uint32_t freq_end) {
    if(!waterfall) return;
    waterfall->freq_start = freq_start;
    waterfall->freq_end = freq_end;
    waterfall->current_row = 0;
    memset(waterfall->data, -100, sizeof(waterfall->data));
}

void predator_spectrum_waterfall_add_scan(SpectrumWaterfall* waterfall, int8_t* rssi_data, size_t count) {
    if(!waterfall || !rssi_data) return;
    size_t copy_count = count < SPECTRUM_WATERFALL_WIDTH ? count : SPECTRUM_WATERFALL_WIDTH;
    memcpy(waterfall->data[waterfall->current_row], rssi_data, copy_count);
    waterfall->current_row = (waterfall->current_row + 1) % SPECTRUM_WATERFALL_HEIGHT;
}
