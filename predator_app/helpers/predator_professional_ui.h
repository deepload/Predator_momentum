#pragma once

#include <gui/gui.h>
#include <gui/view.h>
#include <gui/view_dispatcher.h>
#include <gui/modules/submenu.h>
#include <gui/modules/popup.h>
#include <gui/modules/dialog_ex.h>
#include <gui/modules/text_input.h>
#include <gui/modules/variable_item_list.h>

// Professional UI System for Enterprise-Grade Interface
// Provides consistent, professional UI components across all apps

#define PREDATOR_UI_THEME_PROFESSIONAL "Professional"
#define PREDATOR_UI_THEME_GOVERNMENT "Government"
#define PREDATOR_UI_THEME_VIP "VIP"
#define PREDATOR_UI_THEME_EMERGENCY "Emergency"

// Professional Color Schemes
typedef enum {
    PredatorUIThemeProfessional,
    PredatorUIThemeGovernment,
    PredatorUIThemeVip,
    PredatorUIThemeEmergency,
    PredatorUIThemeCustom
} PredatorUITheme;

// Professional Status Indicators
typedef enum {
    PredatorUIStatusIdle,
    PredatorUIStatusWorking,
    PredatorUIStatusSuccess,
    PredatorUIStatusError,
    PredatorUIStatusWarning,
    PredatorUIStatusCritical,
    PredatorUIStatusGovernment,
    PredatorUIStatusVip
} PredatorUIStatus;

// Professional Animation Types
typedef enum {
    PredatorUIAnimationNone,
    PredatorUIAnimationPulse,
    PredatorUIAnimationSpin,
    PredatorUIAnimationProgress,
    PredatorUIAnimationBlink,
    PredatorUIAnimationSlide
} PredatorUIAnimation;

// Professional UI Configuration
typedef struct {
    PredatorUITheme theme;
    bool animations_enabled;
    bool sounds_enabled;
    bool haptic_enabled;
    uint8_t brightness;
    uint8_t contrast;
    bool show_status_bar;
    bool show_performance_metrics;
    bool show_app_info;
    char organization[64];
    char operator_name[32];
    char license_info[64];
} PredatorUIConfig;

// Professional Status Bar Information
typedef struct {
    char app_name[32];
    char version[16];
    PredatorUIStatus status;
    uint32_t memory_usage;
    uint32_t cpu_usage;
    uint32_t battery_level;
    bool wifi_connected;
    bool gps_active;
    bool subghz_active;
    bool government_mode;
    bool vip_mode;
    bool emergency_mode;
    uint32_t operations_count;
    uint32_t success_rate;
} PredatorUIStatusInfo;

// Professional Menu Item Structure
typedef struct {
    char title[64];
    char subtitle[128];
    char icon[8];
    PredatorUIStatus status;
    bool enabled;
    bool government_only;
    bool vip_only;
    uint32_t access_level;
    void (*callback)(void* context, uint32_t index);
} PredatorUIMenuItem;

// Professional Dialog Configuration
typedef struct {
    char title[64];
    char message[256];
    char button_left[32];
    char button_right[32];
    char button_center[32];
    PredatorUITheme theme;
    bool show_status;
    bool show_progress;
    uint8_t progress_value;
} PredatorUIDialog;

// Professional Progress Indicator
typedef struct {
    char title[64];
    char current_step[128];
    uint8_t progress_percent;
    uint32_t steps_total;
    uint32_t steps_completed;
    uint32_t estimated_time_remaining;
    bool cancellable;
    bool show_details;
} PredatorUIProgress;

// Professional Notification System
typedef struct {
    char title[64];
    char message[256];
    PredatorUIStatus type;
    uint32_t duration_ms;
    bool persistent;
    bool requires_acknowledgment;
    bool government_priority;
    bool vip_priority;
} PredatorUINotification;

// Professional UI Initialization
bool predator_ui_init(PredatorUIConfig* config);
void predator_ui_deinit(void);
bool predator_ui_set_theme(PredatorUITheme theme);
bool predator_ui_apply_config(const PredatorUIConfig* config);

// Professional Status Bar
bool predator_ui_status_bar_init(void);
void predator_ui_status_bar_deinit(void);
bool predator_ui_status_bar_update(const PredatorUIStatusInfo* info);
bool predator_ui_status_bar_show_notification(const char* message, PredatorUIStatus type);
bool predator_ui_status_bar_set_mode(bool government, bool vip, bool emergency);

// Professional Menu System
Submenu* predator_ui_create_professional_menu(const char* title, const char* subtitle);
bool predator_ui_add_menu_item(Submenu* menu, const PredatorUIMenuItem* item, uint32_t index);
bool predator_ui_add_menu_separator(Submenu* menu, const char* label);
bool predator_ui_add_menu_header(Submenu* menu, const char* header, const char* description);
bool predator_ui_set_menu_theme(Submenu* menu, PredatorUITheme theme);

// Professional Dialog System
DialogEx* predator_ui_create_professional_dialog(const PredatorUIDialog* config);
bool predator_ui_show_confirmation(const char* title, const char* message, bool* result);
bool predator_ui_show_information(const char* title, const char* message);
bool predator_ui_show_error(const char* title, const char* error_message);
bool predator_ui_show_warning(const char* title, const char* warning_message);
bool predator_ui_show_government_dialog(const char* title, const char* message);
bool predator_ui_show_vip_dialog(const char* title, const char* message);

// Professional Progress System
Popup* predator_ui_create_progress_popup(const PredatorUIProgress* config);
bool predator_ui_update_progress(Popup* popup, uint8_t percent, const char* step);
bool predator_ui_show_operation_progress(const char* operation, uint8_t percent);
bool predator_ui_show_loading(const char* message);
bool predator_ui_hide_loading(void);

// Professional Input System
TextInput* predator_ui_create_professional_input(const char* title, const char* placeholder);
bool predator_ui_get_text_input(const char* title, const char* placeholder, char* result, size_t max_length);
bool predator_ui_get_password_input(const char* title, char* result, size_t max_length);
bool predator_ui_get_numeric_input(const char* title, uint32_t* result, uint32_t min, uint32_t max);

// Professional Variable List System
VariableItemList* predator_ui_create_professional_settings(const char* title);
bool predator_ui_add_setting_toggle(VariableItemList* list, const char* label, bool* value);
bool predator_ui_add_setting_choice(VariableItemList* list, const char* label, const char** choices, uint32_t choice_count, uint32_t* selected);
bool predator_ui_add_setting_range(VariableItemList* list, const char* label, uint32_t* value, uint32_t min, uint32_t max);
bool predator_ui_add_setting_info(VariableItemList* list, const char* label, const char* value);

// Professional Notification System
bool predator_ui_show_notification(const PredatorUINotification* notification);
bool predator_ui_show_success_notification(const char* message);
bool predator_ui_show_error_notification(const char* message);
bool predator_ui_show_warning_notification(const char* message);
bool predator_ui_show_government_notification(const char* message);
bool predator_ui_show_vip_notification(const char* message);
bool predator_ui_clear_notifications(void);

// Professional Animation System
bool predator_ui_start_animation(PredatorUIAnimation animation, uint32_t duration_ms);
bool predator_ui_stop_animation(void);
bool predator_ui_set_animation_speed(uint32_t speed_percent);

// Professional Theming
bool predator_ui_set_professional_colors(void);
bool predator_ui_set_government_colors(void);
bool predator_ui_set_vip_colors(void);
bool predator_ui_set_emergency_colors(void);
bool predator_ui_set_custom_colors(uint32_t primary, uint32_t secondary, uint32_t accent);

// Professional Layout System
bool predator_ui_create_dashboard_layout(void);
bool predator_ui_create_monitoring_layout(void);
bool predator_ui_create_control_layout(void);
bool predator_ui_create_government_layout(void);
bool predator_ui_create_vip_layout(void);

// Professional Utilities
bool predator_ui_validate_access_level(uint32_t required_level);
bool predator_ui_check_government_access(void);
bool predator_ui_check_vip_access(void);
const char* predator_ui_get_status_icon(PredatorUIStatus status);
const char* predator_ui_get_theme_name(PredatorUITheme theme);
bool predator_ui_save_preferences(void);
bool predator_ui_load_preferences(void);
