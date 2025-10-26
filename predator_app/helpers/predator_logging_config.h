#pragma once

// Memory-optimized logging configuration
// When NO_LOGGING is defined, all FURI_LOG calls become no-ops to save memory

#ifdef NO_LOGGING
    // Completely disable all logging to save maximum memory
    #undef FURI_LOG_E
    #undef FURI_LOG_W
    #undef FURI_LOG_I
    #undef FURI_LOG_D
    #undef FURI_LOG_T
    
    // Replace all logging macros with empty statements - MAXIMUM memory optimization
    #define FURI_LOG_E(tag, format, ...) ((void)0)
    #define FURI_LOG_W(tag, format, ...) ((void)0)
    #define FURI_LOG_I(tag, format, ...) ((void)0)
    #define FURI_LOG_D(tag, format, ...) ((void)0)
    #define FURI_LOG_T(tag, format, ...) ((void)0)
    
    // Also disable predator_log_append to save even more memory
    // Note: We can't redefine function declarations, so we'll handle this differently
    
#elif defined(MINIMAL_LOGGING)
    // Keep only error and warning logs for debugging
    #undef FURI_LOG_I
    #undef FURI_LOG_D
    #undef FURI_LOG_T
    
    #define FURI_LOG_I(tag, format, ...) do {} while(0)
    #define FURI_LOG_D(tag, format, ...) do {} while(0)
    #define FURI_LOG_T(tag, format, ...) do {} while(0)
    
#endif

// Memory usage info:
// - Each FURI_LOG call saves ~20-50 bytes (format string + overhead)
// - Total estimated savings with NO_LOGGING: ~2-4KB of flash + ~500-1000 bytes RAM
// - Critical for Flipper Zero's limited 256KB RAM
