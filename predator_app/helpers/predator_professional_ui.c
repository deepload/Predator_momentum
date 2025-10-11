#include "predator_professional_ui.h"
#include <furi_hal.h>

// Professional UI System Implementation
// Enterprise-grade user interface components

static PredatorUITheme current_theme = PredatorUIThemeDefault;
static bool ui_initialized = false;

// Professional UI Initialization
bool predator_professional_ui_init(void) {
    if(ui_initialized) return true;
    
    // Set default theme
    current_theme = PredatorUIThemeDefault;
    ui_initialized = true;
    
    return true;
}

void predator_professional_ui_deinit(void) {
    ui_initialized = false;
}

// Professional Theme Management
bool predator_professional_ui_set_theme(PredatorUITheme theme) {
    current_theme = theme;
    return true;
}

PredatorUITheme predator_professional_ui_get_theme(void) {
    return current_theme;
}

// Professional Status Indicators
void predator_professional_ui_draw_status_bar(Canvas* canvas, const PredatorUIStatusBar* status_bar) {
    if(!canvas || !status_bar) return;
    
    canvas_set_font(canvas, FontSecondary);
    
    // Draw status indicators
    uint8_t x_pos = 2;
    
    // Connection status
    if(status_bar->wifi_connected) {
        canvas_draw_str(canvas, x_pos, 8, "📶");
        x_pos += 12;
    }
    
    if(status_bar->gps_connected) {
        canvas_draw_str(canvas, x_pos, 8, "🛰️");
        x_pos += 12;
    }
    
    if(status_bar->esp32_connected) {
        canvas_draw_str(canvas, x_pos, 8, "📡");
        x_pos += 12;
    }
    
    // Mode indicators
    if(status_bar->vip_mode) {
        canvas_draw_str(canvas, x_pos, 8, "💎");
        x_pos += 12;
    }
    
    if(status_bar->government_mode) {
        canvas_draw_str(canvas, x_pos, 8, "🏛️");
        x_pos += 12;
    }
    
    if(status_bar->emergency_mode) {
        canvas_draw_str(canvas, x_pos, 8, "🚨");
        x_pos += 12;
    }
    
    // Battery level (right side)
    char battery_str[8];
    snprintf(battery_str, sizeof(battery_str), "%d%%", status_bar->battery_level);
    canvas_draw_str_aligned(canvas, 126, 8, AlignRight, AlignTop, battery_str);
}

// Professional Menu Items
void predator_professional_ui_draw_menu_item(Canvas* canvas, const PredatorUIMenuItem* item, uint8_t y_pos, bool selected) {
    if(!canvas || !item) return;
    
    canvas_set_font(canvas, FontSecondary);
    
    // Draw selection indicator
    if(selected) {
        canvas_draw_box(canvas, 0, y_pos - 1, 128, 10);
        canvas_set_color(canvas, ColorWhite);
    }
    
    // Draw icon
    canvas_draw_str(canvas, 2, y_pos + 7, item->icon);
    
    // Draw text
    canvas_draw_str(canvas, 16, y_pos + 7, item->text);
    
    // Draw status indicator if present
    if(item->status_icon[0] != '\0') {
        canvas_draw_str_aligned(canvas, 126, y_pos + 7, AlignRight, AlignTop, item->status_icon);
    }
    
    // Reset color
    canvas_set_color(canvas, ColorBlack);
}

// Professional Progress Bars
void predator_professional_ui_draw_progress_bar(Canvas* canvas, const PredatorUIProgressBar* progress_bar) {
    if(!canvas || !progress_bar) return;
    
    // Draw background
    canvas_draw_frame(canvas, progress_bar->x, progress_bar->y, progress_bar->width, progress_bar->height);
    
    // Calculate progress width
    uint8_t progress_width = (progress_bar->progress * (progress_bar->width - 2)) / 100;
    
    // Draw progress fill
    if(progress_width > 0) {
        canvas_draw_box(canvas, progress_bar->x + 1, progress_bar->y + 1, progress_width, progress_bar->height - 2);
    }
    
    // Draw percentage text
    if(progress_bar->show_percentage) {
        char percent_str[8];
        snprintf(percent_str, sizeof(percent_str), "%d%%", progress_bar->progress);
        
        canvas_set_font(canvas, FontSecondary);
        canvas_set_color(canvas, ColorWhite);
        canvas_draw_str_aligned(canvas, 
                               progress_bar->x + progress_bar->width / 2, 
                               progress_bar->y + progress_bar->height / 2 + 2, 
                               AlignCenter, AlignCenter, 
                               percent_str);
        canvas_set_color(canvas, ColorBlack);
    }
}

// Professional Dialogs
bool predator_professional_ui_show_dialog(const PredatorUIDialog* dialog) {
    if(!dialog) return false;
    
    // In a real implementation, this would create and show a dialog
    // For now, we simulate the dialog response
    return true;
}

// Professional Notifications
void predator_professional_ui_show_notification(const PredatorUINotification* notification) {
    if(!notification) return;
    
    // In a real implementation, this would show a notification popup
    // For now, we log the notification
    FURI_LOG_I("ProfessionalUI", "Notification: %s - %s", notification->title, notification->message);
}

// Professional Utility Functions
const char* predator_professional_ui_theme_to_string(PredatorUITheme theme) {
    switch(theme) {
        case PredatorUIThemeDefault: return "Default";
        case PredatorUIThemeDark: return "Dark";
        case PredatorUIThemeGovernment: return "Government";
        case PredatorUIThemeVIP: return "VIP";
        case PredatorUIThemeEmergency: return "Emergency";
        default: return "Unknown";
    }
}

bool predator_professional_ui_validate_coordinates(uint8_t x, uint8_t y, uint8_t width, uint8_t height) {
    return (x + width <= 128) && (y + height <= 64);
}

// Professional Color Schemes
void predator_professional_ui_apply_theme_colors(Canvas* canvas, PredatorUITheme theme) {
    if(!canvas) return;
    
    switch(theme) {
        case PredatorUIThemeDefault:
            canvas_set_color(canvas, ColorBlack);
            break;
        case PredatorUIThemeDark:
            canvas_set_color(canvas, ColorWhite);
            break;
        case PredatorUIThemeGovernment:
            canvas_set_color(canvas, ColorBlack);
            break;
        case PredatorUIThemeVIP:
            canvas_set_color(canvas, ColorBlack);
            break;
        case PredatorUIThemeEmergency:
            canvas_set_color(canvas, ColorBlack);
            break;
    }
}

// Professional Layout Helpers
void predator_professional_ui_center_text(Canvas* canvas, const char* text, uint8_t y) {
    if(!canvas || !text) return;
    
    canvas_draw_str_aligned(canvas, 64, y, AlignCenter, AlignTop, text);
}

void predator_professional_ui_draw_header(Canvas* canvas, const char* title) {
    if(!canvas || !title) return;
    
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, 64, 2, AlignCenter, AlignTop, title);
    
    // Draw separator line
    canvas_draw_line(canvas, 0, 12, 127, 12);
}

void predator_professional_ui_draw_footer(Canvas* canvas, const char* instructions) {
    if(!canvas || !instructions) return;
    
    // Draw separator line
    canvas_draw_line(canvas, 0, 52, 127, 52);
    
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str_aligned(canvas, 64, 56, AlignCenter, AlignTop, instructions);
}

// Professional Animation Support
void predator_professional_ui_animate_progress(PredatorUIProgressBar* progress_bar, uint8_t target_progress, uint32_t duration_ms) {
    if(!progress_bar) return;
    
    // Simple animation - in production would use proper timing
    uint8_t steps = 10;
    uint8_t step_size = (target_progress - progress_bar->progress) / steps;
    
    for(uint8_t i = 0; i < steps; i++) {
        progress_bar->progress += step_size;
        furi_delay_ms(duration_ms / steps);
    }
    
    progress_bar->progress = target_progress;
}

// Professional Input Validation
bool predator_professional_ui_validate_input(const char* input, PredatorUIInputType type) {
    if(!input) return false;
    
    switch(type) {
        case PredatorUIInputTypeNumeric:
            // Check if all characters are digits
            for(size_t i = 0; i < strlen(input); i++) {
                if(input[i] < '0' || input[i] > '9') {
                    return false;
                }
            }
            return true;
            
        case PredatorUIInputTypeAlphanumeric:
            // Check if all characters are alphanumeric
            for(size_t i = 0; i < strlen(input); i++) {
                char c = input[i];
                if(!((c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))) {
                    return false;
                }
            }
            return true;
            
        case PredatorUIInputTypeHex:
            // Check if all characters are hex digits
            for(size_t i = 0; i < strlen(input); i++) {
                char c = input[i];
                if(!((c >= '0' && c <= '9') || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f'))) {
                    return false;
                }
            }
            return true;
            
        case PredatorUIInputTypeFrequency:
            // Validate frequency format (e.g., "433.92")
            bool has_dot = false;
            for(size_t i = 0; i < strlen(input); i++) {
                char c = input[i];
                if(c == '.') {
                    if(has_dot) return false; // Multiple dots
                    has_dot = true;
                } else if(c < '0' || c > '9') {
                    return false;
                }
            }
            return true;
            
        default:
            return false;
    }
}
