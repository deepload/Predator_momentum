#pragma once
#include <stddef.h>

struct PredatorApp;

// Builds a compact status string with Region, Access, and an optional detail
// Example: "Region: EU  Access: Live\nCh:6"
void predator_ui_build_status(const struct PredatorApp* app, const char* detail, char* out, size_t out_len);
