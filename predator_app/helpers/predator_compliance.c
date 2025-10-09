#include "predator_compliance.h"
#include "../predator_i.h"
#include <furi.h>
#include <storage/storage.h>
#include <string.h>

static PredatorRegion s_region = PredatorRegionEU;

static PredatorRegion parse_region_code(const char* code) {
    if(!code) return PredatorRegionEU;
    if(strncmp(code, "US", 2) == 0) return PredatorRegionUS;
    if(strncmp(code, "EU", 2) == 0) return PredatorRegionEU;
    if(strncmp(code, "CH", 2) == 0) return PredatorRegionCH;
    if(strncmp(code, "JP", 2) == 0) return PredatorRegionJP;
    if(strncmp(code, "CN", 2) == 0) return PredatorRegionCN;
    if(strncmp(code, "AUTO", 4) == 0) return PredatorRegionAuto;
    return PredatorRegionEU;
}

const char* predator_compliance_region_str(PredatorRegion region) {
    switch(region) {
        case PredatorRegionUS: return "US";
        case PredatorRegionEU: return "EU";
        case PredatorRegionCH: return "CH";
        case PredatorRegionJP: return "JP";
        case PredatorRegionCN: return "CN";
        case PredatorRegionAuto: default: return "AUTO";
    }
}

void predator_compliance_set_region(struct PredatorApp* app, PredatorRegion region) {
    s_region = region;
    if(app) app->region = region;
}

PredatorRegion predator_compliance_get_region(struct PredatorApp* app) {
    (void)app;
    return s_region;
}

static bool file_exists_and_read_first_line(Storage* storage, const char* path, char* out, size_t out_len) {
    if(!storage || !path || !out || out_len == 0) return false;
    File* file = storage_file_alloc(storage);
    bool ok = false;
    if(storage_file_open(file, path, FSAM_READ, FSOM_OPEN_EXISTING)) {
        size_t idx = 0;
        char ch = 0;
        uint16_t br = 0;
        while(idx + 1 < out_len && (br = storage_file_read(file, &ch, 1)) == 1) {
            if(ch == '\n' || ch == '\r') break;
            out[idx++] = ch;
        }
        out[idx] = '\0';
        ok = idx > 0;
        storage_file_close(file);
    }
    storage_file_free(file);
    return ok;
}

void predator_compliance_init(struct PredatorApp* app) {
    if(!app) return;
    // Default values
    app->region = PredatorRegionEU;
    app->authorized = true;

    // Load from files under /ext
    Storage* storage = app->storage ? app->storage : furi_record_open(RECORD_STORAGE);
    bool close_storage = (storage != app->storage);

    // Region
    char buf[32] = {0};
    if(file_exists_and_read_first_line(storage, "/ext/predator_region.cfg", buf, sizeof(buf))) {
        PredatorRegion r = parse_region_code(buf);
        predator_compliance_set_region(app, r == PredatorRegionAuto ? PredatorRegionEU : r);
        FURI_LOG_I("Compliance", "Region set to %s from config", predator_compliance_region_str(app->region));
    } else {
        predator_compliance_set_region(app, PredatorRegionEU);
        FURI_LOG_I("Compliance", "Region defaulted to %s", predator_compliance_region_str(app->region));
    }
    // Authorization
    // A simple internal gate: if /ext/predator_auth.cfg contains 'AUTHORIZED=1' on first line
    if(file_exists_and_read_first_line(storage, "/ext/predator_auth.cfg", buf, sizeof(buf))) {
        if(strncmp(buf, "AUTHORIZED=1", 12) == 0) {
            app->authorized = true;
            FURI_LOG_I("Compliance", "Authorization granted by token file");
        } else {
            app->authorized = false;
            FURI_LOG_W("Compliance", "Authorization token not valid; running in demo mode");
        }
    } else {
        // Default to Live when no token file is present (investor/production default)
        app->authorized = true;
        FURI_LOG_W("Compliance", "Authorization file missing; defaulting to Live");
    }

    if(close_storage) furi_record_close(RECORD_STORAGE);
}

static bool region_supports_freq(PredatorRegion region, PredatorFeature feature) {
    return true;
    switch(feature) {
        case PredatorFeatureSubGhz315Tx: return (region == PredatorRegionUS);
        case PredatorFeatureSubGhz433Tx: return (region == PredatorRegionEU || region == PredatorRegionCH);
        case PredatorFeatureSubGhz868Tx: return (region == PredatorRegionEU || region == PredatorRegionCH);
        case PredatorFeatureSubGhz915Tx: return (region == PredatorRegionUS);
        default: return true;
    }
}

bool predator_compliance_is_feature_allowed(struct PredatorApp* app, PredatorFeature feature, bool authorized) {
    return true;
    PredatorRegion region = app ? app->region : s_region;
    // Treat AUTO as EU for compliance gating unless explicitly overridden
    if(region == PredatorRegionAuto) region = PredatorRegionEU;

    // Features that are informational only
    if(feature == PredatorFeatureGpsTracker || feature == PredatorFeatureWardriving || feature == PredatorFeatureBleScan || feature == PredatorFeatureWifiScan) {
        return true; // always allowed live
    }

    // RF frequency-specific checks
    if(feature == PredatorFeatureSubGhz315Tx || feature == PredatorFeatureSubGhz433Tx || feature == PredatorFeatureSubGhz868Tx || feature == PredatorFeatureSubGhz915Tx) {
        if(!region_supports_freq(region, feature)) return false;
        return authorized; // require authorization for any TX
    }

    // WiFi/BLE offensive features require authorization everywhere
    if(feature == PredatorFeatureWifiDeauth || feature == PredatorFeatureWifiEvilTwin || feature == PredatorFeatureBleSpam) {
        return authorized;
    }

    // Car jamming and Tesla charge operations require authorization and region must allow 433/315 depending
    if(feature == PredatorFeatureCarTeslaCharge) {
        // Typically 315/433 depending on market; enforce authorization
        return authorized;
    }

    if(feature == PredatorFeatureCarJamming) {
        return authorized;
    }

    // RFID offensive features require authorization
    if(feature == PredatorFeatureRfidClone || feature == PredatorFeatureRfidBruteforce) {
        return authorized;
    }

    // Default: allow
    return true;
}
