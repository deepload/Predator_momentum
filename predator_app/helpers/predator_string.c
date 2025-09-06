#include "predator_string.h"

// Custom implementation to replace strtok
char* predator_get_next_field(const char* str, int field_index, char delimiter) {
    if (!str || field_index < 0) return NULL;
    
    static char buffer[128];
    buffer[0] = '\0';
    
    int current_field = 0;
    int start_pos = 0;
    int i = 0;
    
    // Find the start of our target field
    while (str[i] != '\0') {
        if (str[i] == delimiter) {
            current_field++;
            if (current_field > field_index) break;
            start_pos = i + 1;
        }
        i++;
    }
    
    // If we didn't find enough fields
    if (current_field < field_index) return NULL;
    
    // Copy the field content to our buffer
    i = 0;
    while (str[start_pos] != '\0' && str[start_pos] != delimiter && i < 127) {
        buffer[i++] = str[start_pos++];
    }
    buffer[i] = '\0';
    
    return buffer;
}
