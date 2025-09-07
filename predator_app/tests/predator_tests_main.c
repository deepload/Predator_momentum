#include <furi.h>
#include <furi_hal.h>
#include <stdio.h>
#include "predator_test_framework.h"

// Forward declarations for test suites
bool predator_run_gps_tests();
bool predator_run_esp32_tests();

// Main test entry point
int32_t predator_run_tests(void* p) {
    UNUSED(p);
    
    FURI_LOG_I("TEST", "==== Predator Unit Tests ====");
    
    bool all_passed = true;
    
    // Run GPS tests
    FURI_LOG_I("TEST", "Running GPS module tests...");
    all_passed &= predator_run_gps_tests();
    
    // Run ESP32 tests
    FURI_LOG_I("TEST", "Running ESP32 module tests...");
    all_passed &= predator_run_esp32_tests();
    
    // Report final status
    if (all_passed) {
        FURI_LOG_I("TEST", "==== All tests PASSED! ====");
    } else {
        FURI_LOG_E("TEST", "==== Some tests FAILED! ====");
    }
    
    return all_passed ? 0 : 1;
}
