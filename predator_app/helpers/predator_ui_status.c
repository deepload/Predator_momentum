#include "predator_ui_status.h"
#include "predator_compliance.h"
#include "../predator_i.h"
#include <stdio.h>
#include <string.h>

void predator_ui_build_status(const struct PredatorApp* app, const char* detail, char* out, size_t out_len) {
    if(!out || out_len == 0) return;
    out[0] = '\0';

    const char* region = "EU";
    const char* access = "Demo";
    if(app) {
        region = predator_compliance_region_str(app->region);
        access = app->authorized ? "Live" : "Demo";
    }

    if(detail && detail[0] != '\0') {
        snprintf(out, out_len, "Region: %s  Access: %s\n%s", region, access, detail);
    } else {
        snprintf(out, out_len, "Region: %s  Access: %s", region, access);
    }
}
