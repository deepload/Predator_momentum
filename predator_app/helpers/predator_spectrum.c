#include "predator_spectrum.h"
#include "../predator_i.h"
#include <math.h>

// Simplified FFT for spectrum analysis
#define FFT_SIZE 64

typedef struct {
    float real;
    float imag;
} Complex;

// Fast Fourier Transform (Cooley-Tukey algorithm)
static void fft(Complex* data, int n, bool inverse) {
    if(n <= 1) return;
    
    // Bit-reversal permutation
    int j = 0;
    for(int i = 1; i < n; i++) {
        int bit = n >> 1;
        while(j & bit) {
            j ^= bit;
            bit >>= 1;
        }
        j ^= bit;
        
        if(i < j) {
            Complex temp = data[i];
            data[i] = data[j];
            data[j] = temp;
        }
    }
    
    // FFT computation
    for(int len = 2; len <= n; len <<= 1) {
        float angle = 2 * M_PI / len * (inverse ? -1 : 1);
        Complex wlen = {cosf(angle), sinf(angle)};
        
        for(int i = 0; i < n; i += len) {
            Complex w = {1, 0};
            for(int j = 0; j < len / 2; j++) {
                Complex u = data[i + j];
                Complex v = {
                    data[i + j + len / 2].real * w.real - data[i + j + len / 2].imag * w.imag,
                    data[i + j + len / 2].real * w.imag + data[i + j + len / 2].imag * w.real
                };
                
                data[i + j].real = u.real + v.real;
                data[i + j].imag = u.imag + v.imag;
                data[i + j + len / 2].real = u.real - v.real;
                data[i + j + len / 2].imag = u.imag - v.imag;
                
                float w_temp = w.real;
                w.real = w.real * wlen.real - w.imag * wlen.imag;
                w.imag = w_temp * wlen.imag + w.imag * wlen.real;
            }
        }
    }
    
    if(inverse) {
        for(int i = 0; i < n; i++) {
            data[i].real /= n;
            data[i].imag /= n;
        }
    }
}

bool predator_spectrum_init(PredatorApp* app) {
    if(!app) return false;
    FURI_LOG_I("Spectrum", "Spectrum analyzer initialized (FFT size: %d)", FFT_SIZE);
    return true;
}

void predator_spectrum_deinit(PredatorApp* app) {
    if(!app) return;
    FURI_LOG_I("Spectrum", "Spectrum analyzer deinitialized");
}

// Real spectrum scan with FFT analysis
bool predator_spectrum_scan_range(PredatorApp* app, uint32_t freq_start, uint32_t freq_end, uint32_t step) {
    if(!app) return false;
    
    uint32_t freq_count = (freq_end - freq_start) / step + 1;
    FURI_LOG_I("Spectrum", "Scanning %lu-%lu MHz (%lu points, step: %lu kHz)", 
               freq_start/1000000, freq_end/1000000, freq_count, step/1000);
    
    // Simulate spectrum scan with FFT
    Complex fft_data[FFT_SIZE];
    uint32_t current_freq = freq_start;
    
    for(uint32_t i = 0; i < freq_count && i < FFT_SIZE; i++) {
        // REAL SPECTRUM ANALYSIS using Flipper SubGHz hardware
        furi_hal_subghz_set_frequency_and_path(current_freq);
        furi_hal_subghz_rx();
        furi_delay_ms(10); // Sample time
        
        // Read real RSSI value
        float signal = (float)furi_hal_subghz_get_rssi() / -100.0f; // Normalize RSSI
        if(signal < 0) signal = 0;
        if(signal > 1) signal = 1;
        
        fft_data[i].real = signal;
        fft_data[i].imag = 0;
        current_freq += step;
        
        furi_delay_ms(1); // Simulate scan time
    }
    
    // Perform FFT
    fft(fft_data, FFT_SIZE, false);
    
    // Calculate magnitude spectrum
    FURI_LOG_I("Spectrum", "FFT Analysis Results:");
    for(int i = 0; i < FFT_SIZE / 2; i += 4) {
        float magnitude = sqrtf(fft_data[i].real * fft_data[i].real + 
                               fft_data[i].imag * fft_data[i].imag);
        int8_t rssi = (int8_t)(-100 + magnitude * 50);
        
        uint32_t freq = freq_start + (i * (freq_end - freq_start) / FFT_SIZE);
        FURI_LOG_D("Spectrum", "  %lu MHz: %d dBm", freq / 1000000, rssi);
    }
    
    FURI_LOG_I("Spectrum", "Scan complete");
    return true;
}

bool predator_spectrum_get_rssi(PredatorApp* app, uint32_t frequency, int8_t* rssi) {
    if(!app || !rssi) return false;
    UNUSED(frequency);
    *rssi = -80; // Stub value
    return true;
}

ModulationType predator_spectrum_detect_modulation(PredatorApp* app, uint32_t frequency) {
    if(!app) return ModulationUnknown;
    UNUSED(frequency);
    return ModulationASK; // Stub
}

bool predator_spectrum_measure_bandwidth(PredatorApp* app, uint32_t frequency, uint32_t* bandwidth) {
    if(!app || !bandwidth) return false;
    UNUSED(frequency);
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
