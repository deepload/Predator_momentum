#include "predator_ui_elements.h"
#include <furi.h>

void predator_ui_draw_progress_bar(
    Canvas* canvas,
    uint8_t x, uint8_t y, 
    uint8_t width, uint8_t height,
    uint8_t progress, // 0-100
    ProgressBarStyle style) {
    
    // Clamp progress to valid range
    if(progress > 100) progress = 100;
    
    // Calculate filled width based on progress
    uint8_t fill_width = (width * progress) / 100;
    
    switch(style) {
    case ProgressBarStyleBordered:
        // Draw border
        canvas_draw_frame(canvas, x, y, width, height);
        // Draw fill (slightly smaller to not overlap border)
        if(fill_width > 2) {
            canvas_draw_box(canvas, x + 1, y + 1, fill_width - 2, height - 2);
        }
        break;
        
    case ProgressBarStyleDashed:
        {
            // Draw border
            canvas_draw_frame(canvas, x, y, width, height);
            
            // Draw dashed segments inside
            uint8_t segment_count = 10; // Number of segments
            uint8_t segment_width = (width - 2) / segment_count;
            uint8_t filled_segments = (segment_count * progress) / 100;
            
            for(uint8_t i = 0; i < filled_segments; i++) {
                canvas_draw_box(
                    canvas,
                    x + 1 + (i * segment_width),
                    y + 1,
                    segment_width - 1,
                    height - 2);
            }
        }
        break;
        
    case ProgressBarStyleAnimated:
        {
            // Draw border
            canvas_draw_frame(canvas, x, y, width, height);
            
            // Get current time for animation
            uint32_t ticks = furi_get_tick();
            
            if(progress < 100) {
                // For less than 100%, show animated pulsing bar
                uint8_t animation_width = width / 3;
                uint8_t animation_pos = (ticks / 100) % (width - animation_width);
                
                // Draw animated portion
                canvas_draw_box(
                    canvas,
                    x + 1 + animation_pos,
                    y + 1,
                    animation_width,
                    height - 2);
            } else {
                // At 100%, fill completely
                canvas_draw_box(canvas, x + 1, y + 1, width - 2, height - 2);
            }
        }
        break;
        
    case ProgressBarStyleNormal:
    default:
        // Simple progress bar with no border
        canvas_draw_box(canvas, x, y, fill_width, height);
        break;
    }
}

void predator_ui_draw_status_box(
    Canvas* canvas, 
    const char* title, 
    uint8_t x, uint8_t y, 
    uint8_t width, uint8_t height) {
    
    // Draw box with rounded corners
    elements_slightly_rounded_frame(canvas, x, y, width, height);
    
    // Get current font
    Font previous_font = FontSecondary;
    canvas_set_font(canvas, FontSecondary);
    
    // Calculate title width
    int16_t title_width = canvas_string_width(canvas, title);
    
    // Draw white background for title to create a floating title effect
    canvas_set_color(canvas, ColorWhite);
    canvas_draw_box(canvas, x + 4, y - 4, title_width + 2, 8);
    
    // Draw the title
    canvas_set_color(canvas, ColorBlack);
    canvas_draw_str(canvas, x + 5, y, title);
    
    // Restore font
    canvas_set_font(canvas, previous_font);
}

void predator_ui_draw_header_text(
    Canvas* canvas,
    const char* header,
    const char* text,
    uint8_t x, uint8_t y) {
    
    // Save current font and set to bold font for header
    Font previous_font = FontSecondary;
    
    // Draw header
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, x, y, header);
    
    // Draw separator line under the header
    uint8_t header_width = canvas_string_width(canvas, header);
    canvas_draw_line(canvas, x, y + 2, x + header_width, y + 2);
    
    // Draw text content below header
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str(canvas, x, y + 12, text);
    
    // Restore previous font
    canvas_set_font(canvas, previous_font);
}
