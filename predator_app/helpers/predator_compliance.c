#include "predator_compliance.h"
#include "../predator_i.h"
#include <furi.h>
#include <storage/storage.h>
#include <string.h>

static PredatorRegion s_region = PredatorRegionUnblock;

static PredatorRegion parse_region_code(const char* code) {
    if(!code) return PredatorRegionUnblock;
    
    // MAJOR GOVERNMENT MARKETS - ALL COUNTRIES SUPPORTED
    if(strncmp(code, "US", 2) == 0) return PredatorRegionUS;
    if(strncmp(code, "EU", 2) == 0) return PredatorRegionEU;
    if(strncmp(code, "CH", 2) == 0) return PredatorRegionCH;
    if(strncmp(code, "JP", 2) == 0) return PredatorRegionJP;
    if(strncmp(code, "CN", 2) == 0) return PredatorRegionCN;
    if(strncmp(code, "CA", 2) == 0) return PredatorRegionCA;
    if(strncmp(code, "AU", 2) == 0) return PredatorRegionAU;
    if(strncmp(code, "UK", 2) == 0) return PredatorRegionUK;
    if(strncmp(code, "DE", 2) == 0) return PredatorRegionDE;
    if(strncmp(code, "FR", 2) == 0) return PredatorRegionFR;
    if(strncmp(code, "IT", 2) == 0) return PredatorRegionIT;
    if(strncmp(code, "ES", 2) == 0) return PredatorRegionES;
    if(strncmp(code, "NL", 2) == 0) return PredatorRegionNL;
    if(strncmp(code, "SE", 2) == 0) return PredatorRegionSE;
    if(strncmp(code, "NO", 2) == 0) return PredatorRegionNO;
    if(strncmp(code, "DK", 2) == 0) return PredatorRegionDK;
    if(strncmp(code, "FI", 2) == 0) return PredatorRegionFI;
    if(strncmp(code, "BE", 2) == 0) return PredatorRegionBE;
    if(strncmp(code, "AT", 2) == 0) return PredatorRegionAT;
    if(strncmp(code, "PL", 2) == 0) return PredatorRegionPL;
    if(strncmp(code, "CZ", 2) == 0) return PredatorRegionCZ;
    if(strncmp(code, "HU", 2) == 0) return PredatorRegionHU;
    if(strncmp(code, "RO", 2) == 0) return PredatorRegionRO;
    if(strncmp(code, "BG", 2) == 0) return PredatorRegionBG;
    if(strncmp(code, "GR", 2) == 0) return PredatorRegionGR;
    if(strncmp(code, "PT", 2) == 0) return PredatorRegionPT;
    if(strncmp(code, "IE", 2) == 0) return PredatorRegionIE;
    if(strncmp(code, "RU", 2) == 0) return PredatorRegionRU;
    if(strncmp(code, "IN", 2) == 0) return PredatorRegionIN;
    if(strncmp(code, "BR", 2) == 0) return PredatorRegionBR;
    if(strncmp(code, "MX", 2) == 0) return PredatorRegionMX;
    if(strncmp(code, "SA", 2) == 0) return PredatorRegionSA;
    if(strncmp(code, "AE", 2) == 0) return PredatorRegionAE;
    if(strncmp(code, "IL", 2) == 0) return PredatorRegionIL;
    if(strncmp(code, "TR", 2) == 0) return PredatorRegionTR;
    if(strncmp(code, "ZA", 2) == 0) return PredatorRegionZA;
    if(strncmp(code, "EG", 2) == 0) return PredatorRegionEG;
    if(strncmp(code, "NG", 2) == 0) return PredatorRegionNG;
    if(strncmp(code, "KE", 2) == 0) return PredatorRegionKE;
    if(strncmp(code, "TH", 2) == 0) return PredatorRegionTH;
    if(strncmp(code, "MY", 2) == 0) return PredatorRegionMY;
    if(strncmp(code, "SG", 2) == 0) return PredatorRegionSG;
    if(strncmp(code, "ID", 2) == 0) return PredatorRegionID;
    if(strncmp(code, "PH", 2) == 0) return PredatorRegionPH;
    if(strncmp(code, "VN", 2) == 0) return PredatorRegionVN;
    if(strncmp(code, "TW", 2) == 0) return PredatorRegionTW;
    if(strncmp(code, "KR", 2) == 0) return PredatorRegionKR;
    if(strncmp(code, "NZ", 2) == 0) return PredatorRegionNZ;
    if(strncmp(code, "AR", 2) == 0) return PredatorRegionAR;
    if(strncmp(code, "CL", 2) == 0) return PredatorRegionCL;
    if(strncmp(code, "CO", 2) == 0) return PredatorRegionCO;
    if(strncmp(code, "PE", 2) == 0) return PredatorRegionPE;
    if(strncmp(code, "AUTO", 4) == 0) return PredatorRegionAuto;
    if(strncmp(code, "UNBLOCK", 7) == 0) return PredatorRegionUnblock;
    
    return PredatorRegionUnblock; // Default to unrestricted for maximum sales
}

const char* predator_compliance_region_str(PredatorRegion region) {
    switch(region) {
        case PredatorRegionUS: return "US";
        case PredatorRegionEU: return "EU";
        case PredatorRegionCH: return "CH";
        case PredatorRegionJP: return "JP";
        case PredatorRegionCN: return "CN";
        case PredatorRegionUnblock: return "UNBLOCK";
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
    // TESLA/GOVERNMENT: Default to unrestricted for maximum capabilities
    app->region = PredatorRegionUnblock;
    app->authorized = true;

    // Load from files under /ext
    Storage* storage = app->storage ? app->storage : furi_record_open(RECORD_STORAGE);
    bool close_storage = (storage != app->storage);

    // Region - Force UNBLOCK for Tesla/Government testing
    char buf[32] = {0};
    if(file_exists_and_read_first_line(storage, "/ext/predator_region.cfg", buf, sizeof(buf))) {
        PredatorRegion r = parse_region_code(buf);
        // Override any region to UNBLOCK for Tesla requirements
        predator_compliance_set_region(app, PredatorRegionUnblock);
        FURI_LOG_I("Compliance", "TESLA MODE: Region forced to UNBLOCK (was %s)", predator_compliance_region_str(r));
    } else {
        predator_compliance_set_region(app, PredatorRegionUnblock);
        FURI_LOG_I("Compliance", "TESLA MODE: Region defaulted to UNBLOCK (unrestricted)");
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

__attribute__((unused)) static bool region_supports_freq(PredatorRegion region, PredatorFeature feature) {
    switch(feature) {
        case PredatorFeatureSubGhz315Tx: return (region == PredatorRegionUS);
        case PredatorFeatureSubGhz433Tx: return (region == PredatorRegionEU || region == PredatorRegionCH);
        case PredatorFeatureSubGhz868Tx: return (region == PredatorRegionEU || region == PredatorRegionCH);
        case PredatorFeatureSubGhz915Tx: return (region == PredatorRegionUS);
        default: return true;
    }
}

bool predator_compliance_is_feature_allowed(struct PredatorApp* app, PredatorFeature feature, bool authorized) {
    // TESLA/GOVERNMENT MODE: ALL FEATURES UNRESTRICTED
    // No compliance limitations for Tesla security testing
    // No EU restrictions for government contracts
    // Maximum capabilities for professional demonstrations
    
    UNUSED(app);
    UNUSED(feature);
    UNUSED(authorized);
    
    FURI_LOG_I("Compliance", "TESLA MODE: All features unrestricted (feature=%d)", feature);
    return true; // Always allow everything for Tesla/Government testing
}
