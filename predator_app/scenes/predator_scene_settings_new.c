#include "../predator_i.h"
#include "../helpers/predator_ui_elements.h"

typedef struct {
    View* view;
    uint8_t selected_setting;
    uint8_t scroll_position;
    uint8_t num_settings;
    bool edit_mode;
    uint8_t edit_value;
} SettingsView;

// Settings definitions
typedef enum {
    SettingWifiChannelTime,
    SettingGpsBaud,
    SettingSubghzPower,
    SettingLedBrightness,
    SettingBacklight,
    SettingCount
} SettingType;

typedef struct {
    const char* name;
    const char* description;
    uint8_t min_value;
    uint8_t max_value;
    uint8_t current_value;
    const char* value_format;
    const char* units;
} SettingItem;

// Settings data with defaults
static SettingItem settings_data[] = {
    {"WiFi Channel Time", "Seconds per channel", 1, 60, 5, "%d", "sec"},
    {"GPS Baud Rate", "Communication speed", 0, 3, 1, "%s", ""},
    {"SubGHz TX Power", "Transmit power level", 0, 7, 7, "%d", "dBm"},
    {"LED Brightness", "Notification LEDs", 0, 255, 128, "%d", ""},
    {"Backlight", "Screen brightness", 0, 100, 70, "%d", "%"}
};

// Custom values for GPS baud rates
static const char* gps_baud_rates[] = {"4800", "9600", "38400", "115200"};

static void settings_view_draw_callback(Canvas* canvas, void* context) {
    PredatorApp* app = context;
    
    if(!app) return;
    
    // Get view state
    SettingsView* state = view_get_model(app->view_dispatcher->current_view);
    if(!state) return;
    
    canvas_clear(canvas);
    
    // Draw title
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str_aligned(canvas, 64, 8, AlignCenter, AlignCenter, "Settings");
    
    // Draw separator
    canvas_draw_line(canvas, 0, 16, 128, 16);
    
    // Calculate visible range based on selection and scroll position
    uint8_t start_idx = state->scroll_position;
    uint8_t visible_items = MIN(ITEMS_ON_SCREEN, state->num_settings - start_idx);
    
    // Settings list area
    canvas_set_font(canvas, FontSecondary);
    
    for(uint8_t i = 0; i < visible_items; i++) {
        uint8_t item_idx = i + start_idx;
        uint8_t y_pos = 25 + (i * 12);
        
        // Highlight selected setting
        if(item_idx == state->selected_setting) {
            canvas_draw_box(canvas, 0, y_pos - 1, 128, 11);
            canvas_set_color(canvas, ColorWhite);
        }
        
        // Draw setting name and value
        canvas_draw_str(canvas, 4, y_pos + 7, settings_data[item_idx].name);
        
        // Format value based on setting type
        char value_str[32];
        if(item_idx == SettingGpsBaud) {
            // Special handling for GPS baud rate enum
            snprintf(value_str, sizeof(value_str), "%s", 
                   gps_baud_rates[settings_data[item_idx].current_value]);
        } else {
            // Normal numeric value
            snprintf(value_str, sizeof(value_str), 
                   settings_data[item_idx].value_format, 
                   settings_data[item_idx].current_value);
        }
        
        // Add units if applicable
        if(settings_data[item_idx].units[0] != '\0') {
            strncat(value_str, settings_data[item_idx].units, sizeof(value_str) - strlen(value_str) - 1);
        }
        
        // Draw value with right alignment
        int16_t value_width = canvas_string_width(canvas, value_str);
        canvas_draw_str(canvas, 124 - value_width, y_pos + 7, value_str);
        
        // Reset color
        canvas_set_color(canvas, ColorBlack);
    }
    
    // Draw scroll indicators if needed
    if(state->scroll_position > 0) {
        canvas_draw_icon(canvas, 120, 17, &I_ButtonUp_4x7);
    }
    
    if(state->scroll_position + visible_items < state->num_settings) {
        canvas_draw_icon(canvas, 120, 60, &I_ButtonDown_4x7);
    }
    
    // Draw setting description
    predator_ui_draw_status_box(canvas, "Description", 4, 86, 120, 24);
    
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str_aligned(
        canvas, 
        64, 
        98, 
        AlignCenter, 
        AlignCenter, 
        settings_data[state->selected_setting].description);
    
    // Draw edit mode indicator
    if(state->edit_mode) {
        predator_ui_draw_status_box(canvas, "Editing", 34, 63, 60, 16);
        
        // Draw edit value with animation
        canvas_draw_str_aligned(canvas, 64, 72, AlignCenter, AlignCenter, "▲ / ▼ to change");
        
        // Animated edit indicator
        uint8_t animation_frame = (furi_get_tick() / 200) % 4;
        for(uint8_t i = 0; i < animation_frame; i++) {
            canvas_draw_box(canvas, 61 + (i * 3), 78, 2, 2);
        }
    }
    
    // Draw controls
    if(state->edit_mode) {
        elements_button_left(canvas, "Cancel");
        elements_button_right(canvas, "Save");
    } else {
        elements_button_left(canvas, "Back");
        elements_button_center(canvas, "Edit");
        elements_button_right(canvas, "Board");
    }
}

static bool settings_view_input_callback(InputEvent* event, void* context) {
    PredatorApp* app = context;
    bool consumed = false;
    
    // Get view state
    SettingsView* state = view_get_model(app->view_dispatcher->current_view);
    if(!state) return consumed;
    
    if(event->type == InputTypeShort || event->type == InputTypeRepeat) {
        if(state->edit_mode) {
            // Edit mode controls
            switch(event->key) {
            case InputKeyUp:
                // Increment value
                if(settings_data[state->selected_setting].current_value < 
                   settings_data[state->selected_setting].max_value) {
                    settings_data[state->selected_setting].current_value++;
                    consumed = true;
                }
                break;
                
            case InputKeyDown:
                // Decrement value
                if(settings_data[state->selected_setting].current_value > 
                   settings_data[state->selected_setting].min_value) {
                    settings_data[state->selected_setting].current_value--;
                    consumed = true;
                }
                break;
                
            case InputKeyOk:
            case InputKeyRight:
                // Save value and exit edit mode
                state->edit_mode = false;
                // TODO: Save settings to storage
                consumed = true;
                break;
                
            case InputKeyBack:
            case InputKeyLeft:
                // Cancel edit and exit edit mode
                state->edit_mode = false;
                // TODO: Restore original value
                consumed = true;
                break;
                
            default:
                break;
            }
        } else {
            // Navigation mode controls
            switch(event->key) {
            case InputKeyUp:
                if(state->selected_setting > 0) {
                    state->selected_setting--;
                    
                    // Adjust scroll position if needed
                    if(state->selected_setting < state->scroll_position) {
                        state->scroll_position = state->selected_setting;
                    }
                    consumed = true;
                }
                break;
                
            case InputKeyDown:
                if(state->selected_setting < state->num_settings - 1) {
                    state->selected_setting++;
                    
                    // Adjust scroll position if needed
                    if(state->selected_setting >= state->scroll_position + ITEMS_ON_SCREEN) {
                        state->scroll_position = state->selected_setting - ITEMS_ON_SCREEN + 1;
                    }
                    consumed = true;
                }
                break;
                
            case InputKeyOk:
            case InputKeyCenter:
                // Enter edit mode
                state->edit_mode = true;
                state->edit_value = settings_data[state->selected_setting].current_value;
                consumed = true;
                break;
                
            case InputKeyRight:
                // Board selection
                scene_manager_next_scene(app->scene_manager, PredatorSceneBoardSelection);
                consumed = true;
                break;
                
            case InputKeyBack:
            case InputKeyLeft:
                scene_manager_previous_scene(app->scene_manager);
                consumed = true;
                break;
                
            default:
                break;
            }
        }
    }
    
    return consumed;
}

static View* settings_view_alloc(PredatorApp* app) {
    View* view = view_alloc();
    view_set_context(view, app);
    view_set_draw_callback(view, settings_view_draw_callback);
    view_set_input_callback(view, settings_view_input_callback);
    
    // Initialize model
    SettingsView* state = malloc(sizeof(SettingsView));
    state->selected_setting = 0;
    state->scroll_position = 0;
    state->num_settings = SettingCount;
    state->edit_mode = false;
    state->edit_value = 0;
    
    view_set_model(view, state);
    view_set_model_free_callback(view, free);
    
    return view;
}

static void settings_view_free(View* view) {
    view_free(view);
}

void predator_scene_settings_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    // Create custom view
    View* view = settings_view_alloc(app);
    
    // Replace widget view with custom view
    view_dispatcher_remove_view(app->view_dispatcher, PredatorViewWidget);
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewWidget, view);
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewWidget);
}

bool predator_scene_settings_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(event.type == SceneManagerEventTypeCustom) {
        // Handle custom events
        consumed = true;
    } else if(event.type == SceneManagerEventTypeBack) {
        // Handle back event
        // Get view state
        SettingsView* state = view_get_model(app->view_dispatcher->current_view);
        
        if(state && state->edit_mode) {
            // If in edit mode, cancel edit instead of exiting scene
            state->edit_mode = false;
            consumed = true;
        }
    }
    
    return consumed;
}

void predator_scene_settings_new_on_exit(void* context) {
    PredatorApp* app = context;
    
    // Remove and free custom view
    view_dispatcher_remove_view(app->view_dispatcher, PredatorViewWidget);
    View* view = view_dispatcher_get_current_view(app->view_dispatcher);
    if(view) {
        settings_view_free(view);
    }
    
    // Restore standard widget view
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewWidget, widget_get_view(app->widget));
}
