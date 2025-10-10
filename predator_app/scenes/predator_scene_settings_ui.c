#include "../predator_i.h"
#include "../helpers/predator_compliance.h"
#include "../helpers/predator_logging.h"
#include <gui/view.h>
#include <string.h>

// Settings - Professional UI
// Shows configuration options with selection interface

typedef enum {
    SettingRegion,
    SettingDeauthBurst,
    SettingLogging,
    SettingBrightness,
    SettingCount
} SettingItem;

typedef struct {
    SettingItem current_setting;
    uint8_t region_index;
    bool deauth_burst_enabled;
    bool logging_enabled;
    uint8_t brightness;
    bool settings_changed;
} SettingsState;

static SettingsState settings_state;

static const char* region_names[] = {
    "Auto",
    "US/CA",
    "EU",
    "China",
    "Japan"
};
static const uint8_t region_count = 5;

static void draw_settings_header(Canvas* canvas) {
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 2, 10, "SETTINGS");
    canvas_draw_line(canvas, 0, 12, 128, 12);
}

static void draw_settings_list(Canvas* canvas, SettingsState* state) {
    canvas_set_font(canvas, FontSecondary);
    
    // Region setting
    uint8_t y = 24;
    if(state->current_setting == SettingRegion) {
        canvas_draw_str(canvas, 2, y, ">");
    }
    canvas_draw_str(canvas, 12, y, "Region:");
    canvas_draw_str(canvas, 70, y, region_names[state->region_index]);
    
    // Deauth burst setting
    y += 10;
    if(state->current_setting == SettingDeauthBurst) {
        canvas_draw_str(canvas, 2, y, ">");
    }
    canvas_draw_str(canvas, 12, y, "Deauth Burst:");
    canvas_draw_str(canvas, 90, y, state->deauth_burst_enabled ? "ON" : "OFF");
    
    // Logging setting
    y += 10;
    if(state->current_setting == SettingLogging) {
        canvas_draw_str(canvas, 2, y, ">");
    }
    canvas_draw_str(canvas, 12, y, "Logging:");
    canvas_draw_str(canvas, 90, y, state->logging_enabled ? "ON" : "OFF");
    
    // Brightness setting
    y += 10;
    if(state->current_setting == SettingBrightness) {
        canvas_draw_str(canvas, 2, y, ">");
    }
    canvas_draw_str(canvas, 12, y, "Brightness:");
    char bright_str[8];
    snprintf(bright_str, sizeof(bright_str), "%u%%", (unsigned)state->brightness);
    canvas_draw_str(canvas, 90, y, bright_str);
    
    // Status
    if(state->settings_changed) {
        canvas_draw_str(canvas, 2, 58, "* Settings changed");
    }
}

static void settings_ui_draw_callback(Canvas* canvas, void* context) {
    furi_assert(canvas);
    furi_assert(context);
    
    UNUSED(context);
    
    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);
    
    draw_settings_header(canvas);
    draw_settings_list(canvas, &settings_state);
    
    canvas_set_font(canvas, FontSecondary);
    if(settings_state.settings_changed) {
        canvas_draw_str(canvas, 25, 64, "OK=Save  Back=Cancel");
    } else {
        canvas_draw_str(canvas, 20, 64, "↑↓=Select  ←→=Change");
    }
}

static bool settings_ui_input_callback(InputEvent* event, void* context) {
    PredatorApp* app = context;
    if(!app) return false;
    
    if(event->type == InputTypeShort) {
        if(event->key == InputKeyBack) {
            if(settings_state.settings_changed) {
                // Cancel changes
                settings_state.settings_changed = false;
                // Reload settings from app
                return true;
            }
            return false; // Let scene manager handle back
        } else if(event->key == InputKeyOk) {
            if(settings_state.settings_changed) {
                // Save settings
                predator_compliance_set_region(app, (PredatorRegion)settings_state.region_index);
                settings_state.settings_changed = false;
                
                predator_log_append(app, "Settings saved");
                FURI_LOG_I("SettingsUI", "Settings saved");
                return true;
            }
        } else if(event->key == InputKeyUp) {
            if(settings_state.current_setting > 0) {
                settings_state.current_setting--;
                return true;
            }
        } else if(event->key == InputKeyDown) {
            if(settings_state.current_setting < (SettingCount - 1)) {
                settings_state.current_setting++;
                return true;
            }
        } else if(event->key == InputKeyLeft) {
            settings_state.settings_changed = true;
            switch(settings_state.current_setting) {
                case SettingRegion:
                    if(settings_state.region_index > 0) {
                        settings_state.region_index--;
                    }
                    break;
                case SettingDeauthBurst:
                    settings_state.deauth_burst_enabled = !settings_state.deauth_burst_enabled;
                    break;
                case SettingLogging:
                    settings_state.logging_enabled = !settings_state.logging_enabled;
                    break;
                case SettingBrightness:
                    if(settings_state.brightness > 10) {
                        settings_state.brightness -= 10;
                    }
                    break;
                default:
                    break;
            }
            return true;
        } else if(event->key == InputKeyRight) {
            settings_state.settings_changed = true;
            switch(settings_state.current_setting) {
                case SettingRegion:
                    if(settings_state.region_index < (region_count - 1)) {
                        settings_state.region_index++;
                    }
                    break;
                case SettingDeauthBurst:
                    settings_state.deauth_burst_enabled = !settings_state.deauth_burst_enabled;
                    break;
                case SettingLogging:
                    settings_state.logging_enabled = !settings_state.logging_enabled;
                    break;
                case SettingBrightness:
                    if(settings_state.brightness < 100) {
                        settings_state.brightness += 10;
                    }
                    break;
                default:
                    break;
            }
            return true;
        }
    }
    
    return true;
}

void predator_scene_settings_ui_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    memset(&settings_state, 0, sizeof(SettingsState));
    settings_state.current_setting = SettingRegion;
    settings_state.region_index = (uint8_t)app->region;
    settings_state.deauth_burst_enabled = true;
    settings_state.logging_enabled = true;
    settings_state.brightness = 80;
    settings_state.settings_changed = false;
    
    if(!app->view_dispatcher) {
        FURI_LOG_E("SettingsUI", "View dispatcher is NULL");
        return;
    }
    
    View* view = view_alloc();
    if(!view) {
        FURI_LOG_E("SettingsUI", "Failed to allocate view");
        return;
    }
    
    view_set_context(view, app);
    view_set_draw_callback(view, settings_ui_draw_callback);
    view_set_input_callback(view, settings_ui_input_callback);
    
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewSettingsUI, view);
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSettingsUI);
    
    FURI_LOG_I("SettingsUI", "Settings UI initialized");
}

bool predator_scene_settings_ui_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    if(!app) return false;
    
    if(event.type == SceneManagerEventTypeCustom) {
        return true;
    }
    
    return false;
}

void predator_scene_settings_ui_on_exit(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    if(app->view_dispatcher) {
        view_dispatcher_remove_view(app->view_dispatcher, PredatorViewSettingsUI);
    }
    
    FURI_LOG_I("SettingsUI", "Settings UI exited");
}
