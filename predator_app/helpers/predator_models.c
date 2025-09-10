#include "predator_models.h"
#include <furi.h>
#include <furi_hal.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_CAR_MODELS 1000

static PredatorCarModel g_models[MAX_CAR_MODELS];
static size_t g_model_count = 0;

static void trim(char* s) {
    // Trim leading
    char* p = s;
    while(*p && isspace((unsigned char)*p)) p++;
    if(p != s) memmove(s, p, strlen(p) + 1);
    // Trim trailing
    size_t n = strlen(s);
    while(n > 0 && isspace((unsigned char)s[n-1])) {
        s[n-1] = '\0';
        n--;
    }
}

// Parse decimal string with optional fractional part and scale to integer without floats
// Example: s="433.92" scale=1000000 -> 433920000
static bool parse_decimal_scaled(const char* s, uint32_t scale, uint32_t* out) {
    if(!s || !out) return false;
    // Extract signless integer and fractional parts
    const char* dot = strchr(s, '.');
    uint64_t result = 0;
    if(!dot) {
        // No fractional part
        for(const char* p = s; *p; ++p) {
            if(!isdigit((unsigned char)*p)) return false;
            result = result * 10 + (uint64_t)(*p - '0');
            if(result > 0xFFFFFFFFULL) return false;
        }
        // Multiply by scale
        uint64_t hz = result * (uint64_t)scale;
        if(hz > 0xFFFFFFFFULL) return false;
        *out = (uint32_t)hz;
        return true;
    } else {
        // Integer part
        for(const char* p = s; p < dot; ++p) {
            if(!isdigit((unsigned char)*p)) return false;
            result = result * 10 + (uint64_t)(*p - '0');
            if(result > 0xFFFFFFFFULL) return false;
        }
        // Start with integer * scale
        uint64_t hz = result * (uint64_t)scale;
        // Fractional part: take up to 6 digits to avoid overflow
        const char* f = dot + 1;
        uint32_t frac = 0;
        uint32_t frac_scale = scale;
        uint8_t digits = 0;
        while(*f && isdigit((unsigned char)*f) && digits < 6 && frac_scale > 1) {
            frac_scale /= 10;
            frac = frac * 10 + (uint32_t)(*f - '0');
            f++; digits++;
        }
        // Round to nearest by peeking next digit if any
        if(*f && isdigit((unsigned char)*f) && frac_scale > 1) {
            uint8_t next = (uint8_t)(*f - '0');
            uint32_t add = (next >= 5) ? 1 : 0;
            frac += add;
        }
        // Apply fractional as frac * (scale / 10^digits)
        uint64_t hz_add = (uint64_t)frac * (uint64_t)frac_scale;
        hz += hz_add;
        if(hz > 0xFFFFFFFFULL) return false;
        *out = (uint32_t)hz;
        return true;
    }
}

static bool parse_frequency(const char* token, uint32_t* out_hz) {
    if(!token || !out_hz) return false;
    char buf[32];
    strncpy(buf, token, sizeof(buf)-1);
    buf[sizeof(buf)-1] = '\0';
    trim(buf);

    // Detect suffix
    char* suffix = NULL;
    for(size_t i = 0; buf[i]; i++) {
        if(!isdigit((unsigned char)buf[i]) && buf[i] != '.') {
            suffix = &buf[i];
            break;
        }
    }

    if(!suffix) {
        // Plain integer Hz
        char* endptr = NULL;
        unsigned long v = strtoul(buf, &endptr, 10);
        if(endptr == buf) return false;
        *out_hz = (uint32_t)v;
        return true;
    }

    // Split value and unit
    char valbuf[32];
    size_t vlen = (size_t)(suffix - buf);
    if(vlen >= sizeof(valbuf)) vlen = sizeof(valbuf) - 1;
    memcpy(valbuf, buf, vlen);
    valbuf[vlen] = '\0';
    trim(valbuf);

    // Normalize unit
    for(char* p = suffix; *p; ++p) *p = (char)tolower((unsigned char)*p);

    if(strstr(suffix, "mhz")) {
        return parse_decimal_scaled(valbuf, 1000000U, out_hz);
    } else if(strstr(suffix, "khz")) {
        return parse_decimal_scaled(valbuf, 1000U, out_hz);
    } else if(strstr(suffix, "hz")) {
        return parse_decimal_scaled(valbuf, 1U, out_hz);
    }

    return false;
}

size_t predator_models_count(void) {
    return g_model_count;
}

const PredatorCarModel* predator_models_get(size_t index) {
    if(index >= g_model_count) return NULL;
    return &g_models[index];
}

void predator_models_clear(void) {
    g_model_count = 0;
}

static bool read_line(File* file, char* line, size_t maxlen) {
    size_t pos = 0;
    uint8_t ch;
    uint16_t rd;
    while(pos + 1 < maxlen) {
        rd = storage_file_read(file, &ch, 1);
        if(rd != 1) {
            // EOF
            if(pos == 0) return false;
            break;
        }
        if(ch == '\r') continue;
        if(ch == '\n') break;
        line[pos++] = (char)ch;
    }
    line[pos] = '\0';
    return true;
}

size_t predator_models_load_csv(Storage* storage, const char* path) {
    if(!storage || !path) return 0;

    File* file = storage_file_alloc(storage);
    if(!storage_file_open(file, path, FSAM_READ, FSOM_OPEN_EXISTING)) {
        storage_file_free(file);
        return 0;
    }

    g_model_count = 0;

    char line[256];
    while(read_line(file, line, sizeof(line))) {
        // Skip empty/comment
        char* p = line;
        while(*p && isspace((unsigned char)*p)) p++;
        if(*p == '\0' || *p == '#') continue;

        // Parse CSV tokens
        char* make = strtok(line, ",");
        char* model = strtok(NULL, ",");
        char* freq = strtok(NULL, ",");
        char* type = strtok(NULL, ",");
        if(!make || !model || !freq || !type) continue;

        trim(make); trim(model); trim(freq); trim(type);

        uint32_t hz = 0;
        if(!parse_frequency(freq, &hz)) continue;

        if(g_model_count < MAX_CAR_MODELS) {
            PredatorCarModel* m = &g_models[g_model_count++];
            strncpy(m->make, make, sizeof(m->make)-1); m->make[sizeof(m->make)-1] = '\0';
            strncpy(m->model, model, sizeof(m->model)-1); m->model[sizeof(m->model)-1] = '\0';
            strncpy(m->remote_type, type, sizeof(m->remote_type)-1); m->remote_type[sizeof(m->remote_type)-1] = '\0';
            m->frequency = hz;
        }
    }

    storage_file_close(file);
    storage_file_free(file);

    return g_model_count;
}

size_t predator_models_load_default(Storage* storage) {
    const char* default_path = "/ext/apps_data/predator/car_models.csv";
    return predator_models_load_csv(storage, default_path);
}
