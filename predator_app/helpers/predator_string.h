#pragma once

#include <string.h>
#include <stdlib.h>

// Custom string parsing functions to replace strtok
char* predator_get_next_field(const char* str, int field_index, char delimiter);
