#include "predator_ui_clean.h"
#include "../predator_i.h"
#include <gui/canvas.h>
#include <furi.h>

// CLEAN UI HELPER - Professional scrollable menus with proper spacing

void predator_ui_draw_header(Canvas* canvas, const char* title) {
    if(!canvas || !title) return;
    
    canvas_set_font(canvas, FontPrimary);
    canvas_set_color(canvas, ColorBlack);
    
    // Clear header area
    canvas_draw_box(canvas, 0, 0, 128, 14);
    canvas_set_color(canvas, ColorWhite);
    canvas_draw_box(canvas, 1, 1, 126, 12);
    
    // Draw title centered
    canvas_set_color(canvas, ColorBlack);
    uint8_t title_width = canvas_string_width(canvas, title);
    uint8_t title_x = (128 - title_width) / 2;
    canvas_draw_str(canvas, title_x, 10, title);
    
    // Draw separator line
    canvas_draw_line(canvas, 0, 13, 128, 13);
}

void predator_ui_draw_scrollable_menu(Canvas* canvas, PredatorScrollableMenu* menu) {
    if(!canvas || !menu || !menu->items || menu->item_count == 0) return;
    
    canvas_set_font(canvas, FontSecondary);
    canvas_set_color(canvas, ColorBlack);
    
    // Calculate visible range
    uint8_t visible_items = 4; // Show 4 items at once
    uint8_t start_index = 0;
    
    if(menu->selected_index >= visible_items) {
        start_index = menu->selected_index - visible_items + 1;
    }
    
    uint8_t end_index = start_index + visible_items;
    if(end_index > menu->item_count) {
        end_index = menu->item_count;
        start_index = (end_index > visible_items) ? end_index - visible_items : 0;
    }
    
    // Draw menu items
    uint8_t y_pos = 25; // Start below header
    for(uint8_t i = start_index; i < end_index; i++) {
        bool is_selected = (i == menu->selected_index);
        
        // Draw selection background
        if(is_selected) {
            canvas_set_color(canvas, ColorBlack);
            canvas_draw_box(canvas, 2, y_pos - 8, 124, 10);
            canvas_set_color(canvas, ColorWhite);
        } else {
            canvas_set_color(canvas, ColorBlack);
        }
        
        // Draw selection indicator
        if(is_selected) {
            canvas_draw_str(canvas, 4, y_pos, ">");
        }
        
        // Draw menu item text
        const char* item_text = menu->items[i];
        if(item_text) {
            // Truncate long text to fit screen
            char display_text[25];
            strncpy(display_text, item_text, sizeof(display_text) - 1);
            display_text[sizeof(display_text) - 1] = '\0';
            
            canvas_draw_str(canvas, is_selected ? 12 : 8, y_pos, display_text);
        }
        
        y_pos += 12; // Proper spacing between items
    }
    
    // Draw scroll indicators
    predator_ui_draw_scroll_indicators(canvas, menu, start_index, end_index);
}

void predator_ui_draw_scroll_indicators(Canvas* canvas, PredatorScrollableMenu* menu, uint8_t start_index, uint8_t end_index) {
    if(!canvas || !menu) return;
    
    canvas_set_color(canvas, ColorBlack);
    canvas_set_font(canvas, FontSecondary);
    
    // Show scroll position info
    char scroll_info[16];
    snprintf(scroll_info, sizeof(scroll_info), "%d/%d", menu->selected_index + 1, menu->item_count);
    uint8_t info_width = canvas_string_width(canvas, scroll_info);
    canvas_draw_str(canvas, 128 - info_width - 2, 62, scroll_info);
    
    // Draw scroll arrows
    if(start_index > 0) {
        canvas_draw_str(canvas, 118, 20, "↑");
    }
    if(end_index < menu->item_count) {
        canvas_draw_str(canvas, 118, 55, "↓");
    }
    
    // Draw scrollbar
    if(menu->item_count > 4) {
        uint8_t scrollbar_height = 35; // Available height for scrollbar
        uint8_t scrollbar_y = 20;
        
        // Background
        canvas_draw_line(canvas, 122, scrollbar_y, 122, scrollbar_y + scrollbar_height);
        
        // Thumb
        uint8_t thumb_height = (scrollbar_height * 4) / menu->item_count;
        if(thumb_height < 3) thumb_height = 3;
        
        uint8_t thumb_pos = (scrollbar_height - thumb_height) * menu->selected_index / (menu->item_count - 1);
        canvas_draw_box(canvas, 121, scrollbar_y + thumb_pos, 3, thumb_height);
    }
}

bool predator_ui_handle_menu_input(PredatorScrollableMenu* menu, InputEvent* event) {
    if(!menu || !event || event->type != InputTypeShort) return false;
    
    switch(event->key) {
        case InputKeyUp:
            if(menu->selected_index > 0) {
                menu->selected_index--;
                return true;
            }
            break;
            
        case InputKeyDown:
            if(menu->selected_index < menu->item_count - 1) {
                menu->selected_index++;
                return true;
            }
            break;
            
        case InputKeyOk:
            // Let the calling scene handle OK
            return false;
            
        case InputKeyBack:
            // Let the calling scene handle Back
            return false;
            
        default:
            break;
    }
    
    return false;
}

void predator_ui_draw_status_bar(Canvas* canvas, const char* status) {
    if(!canvas) return;
    
    canvas_set_font(canvas, FontSecondary);
    canvas_set_color(canvas, ColorBlack);
    
    // Clear status area
    canvas_draw_box(canvas, 0, 54, 128, 10);
    canvas_set_color(canvas, ColorWhite);
    canvas_draw_box(canvas, 1, 55, 126, 8);
    
    // Draw status text
    canvas_set_color(canvas, ColorBlack);
    if(status) {
        uint8_t status_width = canvas_string_width(canvas, status);
        uint8_t status_x = (128 - status_width) / 2;
        canvas_draw_str(canvas, status_x, 61, status);
    }
}

void predator_ui_draw_info_box(Canvas* canvas, const char* title, const char* info, uint8_t y_pos) {
    if(!canvas || !title) return;
    
    canvas_set_font(canvas, FontSecondary);
    canvas_set_color(canvas, ColorBlack);
    
    // Draw box outline
    canvas_draw_frame(canvas, 2, y_pos, 124, 20);
    
    // Draw title
    canvas_draw_str(canvas, 4, y_pos + 8, title);
    
    // Draw info if provided
    if(info) {
        canvas_draw_str(canvas, 4, y_pos + 16, info);
    }
}
