#pragma once

#include <furi.h>
#include <stdio.h>
#include <stdbool.h>

/**
 * @brief Simple test framework for Predator app components
 */

// Test result states
typedef enum {
    TestResultPass,
    TestResultFail,
    TestResultSkip
} TestResult;

// Test case structure
typedef struct {
    const char* name;
    TestResult (*test_func)(void* context);
    bool enabled;
} TestCase;

// Test suite structure
typedef struct {
    const char* name;
    TestCase* test_cases;
    size_t test_count;
    void* context;
    void (*setup)(void* context);
    void (*teardown)(void* context);
} TestSuite;

// Test framework stats
typedef struct {
    int total;
    int passed;
    int failed;
    int skipped;
} TestStats;

/**
 * @brief Run a test suite and print results
 * @param suite Test suite to run
 * @return true if all tests passed
 */
bool test_run_suite(TestSuite* suite);

/**
 * @brief Print test result to console
 * @param name Test name
 * @param result Test result
 */
void test_print_result(const char* name, TestResult result);

/**
 * @brief Assert that condition is true
 * @param condition Condition to test
 * @return TestResultPass if true, TestResultFail if false
 */
#define TEST_ASSERT(condition) \
    do { \
        if(!(condition)) { \
            FURI_LOG_E("TEST", "Assertion failed: %s", #condition); \
            return TestResultFail; \
        } \
    } while(0)

/**
 * @brief Assert that two integers are equal
 * @param expected Expected value
 * @param actual Actual value
 * @return TestResultPass if equal, TestResultFail if not
 */
#define TEST_ASSERT_EQUAL_INT(expected, actual) \
    do { \
        if((expected) != (actual)) { \
            FURI_LOG_E("TEST", "Expected %d, got %d", (expected), (actual)); \
            return TestResultFail; \
        } \
    } while(0)

/**
 * @brief Assert that two strings are equal
 * @param expected Expected string
 * @param actual Actual string
 * @return TestResultPass if equal, TestResultFail if not
 */
#define TEST_ASSERT_EQUAL_STRING(expected, actual) \
    do { \
        if(strcmp((expected), (actual)) != 0) { \
            FURI_LOG_E("TEST", "Expected \"%s\", got \"%s\"", (expected), (actual)); \
            return TestResultFail; \
        } \
    } while(0)

/**
 * @brief Assert that pointer is not NULL
 * @param ptr Pointer to check
 * @return TestResultPass if not NULL, TestResultFail if NULL
 */
#define TEST_ASSERT_NOT_NULL(ptr) \
    do { \
        if((ptr) == NULL) { \
            FURI_LOG_E("TEST", "Expected non-NULL pointer"); \
            return TestResultFail; \
        } \
    } while(0)

/**
 * @brief Skip the current test
 * @return TestResultSkip
 */
#define TEST_SKIP() \
    do { \
        return TestResultSkip; \
    } while(0)
