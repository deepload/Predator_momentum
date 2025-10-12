#pragma once

#include <gui/canvas.h>
#include <input/input.h>
#include <stdint.h>
#include <stdbool.h>

// CLEAN UI HELPER - Professional scrollable menus

typedef struct {
    const char** items;
    uint8_t item_count;
    uint8_t selected_index;
} PredatorScrollableMenu;

// Header drawing
void predator_ui_draw_header(Canvas* canvas, const char* title);

// Scrollable menu system
void predator_ui_draw_scrollable_menu(Canvas* canvas, PredatorScrollableMenu* menu);
void predator_ui_draw_scroll_indicators(Canvas* canvas, PredatorScrollableMenu* menu, uint8_t start_index, uint8_t end_index);
bool predator_ui_handle_menu_input(PredatorScrollableMenu* menu, InputEvent* event);

// Status and info display
void predator_ui_draw_status_bar(Canvas* canvas, const char* status);
void predator_ui_draw_info_box(Canvas* canvas, const char* title, const char* info, uint8_t y_pos);
