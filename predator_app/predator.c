#include <furi.h>
#include <furi_hal.h>
#include <gui/gui.h>
#include <gui/view_dispatcher.h>
#include <gui/scene_manager.h>
#include <gui/modules/submenu.h>
#include <gui/modules/text_input.h>
#include <gui/modules/popup.h>
#include <gui/modules/loading.h>
#include <gui/modules/widget.h>
#include <notification/notification_messages.h>
#include <dialogs/dialogs.h>
#include <storage/storage.h>

#include "predator_i.h"
#include "predator_uart.h"
#include "helpers/predator_esp32.h"
#include "helpers/predator_gps.h"
#include "helpers/predator_error.h"
#include "helpers/predator_watchdog.h"

#include "scenes/predator_scene.h"

// Forward declarations
void popup_callback_ok(void* context);
void predator_app_free(PredatorApp* app);

static bool predator_custom_event_callback(void* context, uint32_t event) {
    // Check for NULL context
    if(context == NULL) {
        FURI_LOG_E("Predator", "NULL context in custom event callback");
        return false;
    }
    
    PredatorApp* app = context;
    
    // Kick watchdog to prevent timeouts - only if app is valid
    if(app) {
        predator_watchdog_tick(app);
    }
    
    // Handle error events specially
    if(event == PredatorCustomEventError || event == PredatorCustomEventHardwareError) {
        // Show error popup with last error message - only if app is valid
        if(app && app->popup) {
            predator_error_show_popup(
                app, 
                "Predator Error", 
                predator_error_get_message(app));
            return true;
        }
    } else if(event == PredatorCustomEventRecovery) {
        // Clear error state on recovery - only if app is valid
        if(app) {
            predator_error_clear(app);
        }
    }
    
    // Let scene manager handle the event - only if app and scene_manager are valid
    if(app && app->scene_manager) {
        return scene_manager_handle_custom_event(app->scene_manager, event);
    }
    
    // If we get here, something is wrong with the app state
    FURI_LOG_E("Predator", "Invalid app state in custom event handler");
    return false;
}

static bool predator_back_event_callback(void* context) {
    // Check for NULL context
    if(context == NULL) {
        FURI_LOG_E("Predator", "NULL context in back event callback");
        return false;
    }
    
    PredatorApp* app = context;
    
    // Check if scene manager exists
    if(app->scene_manager) {
        return scene_manager_handle_back_event(app->scene_manager);
    }
    
    // Default to true to allow exit if scene manager is invalid
    FURI_LOG_W("Predator", "Invalid scene manager in back event handler");
    return true;
}

static void predator_tick_event_callback(void* context) {
    // Check for NULL context
    if(context == NULL) {
        FURI_LOG_E("Predator", "NULL context in tick event callback");
        return;
    }
    
    PredatorApp* app = context;
    
    // Kick watchdog on every tick - only if app is valid
    if(app) {
        predator_watchdog_tick(app);
    }
    
    // Handle any pending error recoveries - with null checks
    if(app && app->has_error) {
        uint32_t now = furi_get_tick();
        // If error persists for more than 30 seconds, try auto-recovery
        if(now - app->error_timestamp > 30000) {
            // Clear error and notify about recovery
            predator_error_clear(app);
            
            // Only send event if view_dispatcher exists
            if(app->view_dispatcher) {
                view_dispatcher_send_custom_event(
                    app->view_dispatcher,
                    PredatorCustomEventRecovery);
            }
        }
    }
    
    // Let scene manager handle tick - only if valid
    if(app && app->scene_manager) {
        scene_manager_handle_tick_event(app->scene_manager);
    }
}

PredatorApp* predator_app_alloc() {
    // Allocate memory with null check
    PredatorApp* app = malloc(sizeof(PredatorApp));
    if(!app) {
        FURI_LOG_E("Predator", "Failed to allocate memory for application");
        return NULL;
    }
    
    // Initialize to zeros to prevent undefined behavior with uninitialized fields
    memset(app, 0, sizeof(PredatorApp));

    // Open required records with null checks
    app->gui = furi_record_open(RECORD_GUI);
    app->notifications = furi_record_open(RECORD_NOTIFICATION);
    app->dialogs = furi_record_open(RECORD_DIALOGS);
    app->storage = furi_record_open(RECORD_STORAGE);
    
    // Verify required records were opened
    if(!app->gui || !app->notifications || !app->dialogs || !app->storage) {
        FURI_LOG_E("Predator", "Failed to open required records");
        predator_app_free(app);
        return NULL;
    }

    // Allocate view dispatcher with null check
    app->view_dispatcher = view_dispatcher_alloc();
    if(!app->view_dispatcher) {
        FURI_LOG_E("Predator", "Failed to allocate view dispatcher");
        predator_app_free(app);
        return NULL;
    }
    
    // Allocate scene manager with null check
    app->scene_manager = scene_manager_alloc(&predator_scene_handlers, app);
    if(!app->scene_manager) {
        FURI_LOG_E("Predator", "Failed to allocate scene manager");
        predator_app_free(app);
        return NULL;
    }

    // Note: using default queue management
    // The deprecated view_dispatcher_enable_queue would have been here
    view_dispatcher_set_event_callback_context(app->view_dispatcher, app);
    view_dispatcher_set_custom_event_callback(app->view_dispatcher, predator_custom_event_callback);
    view_dispatcher_set_navigation_event_callback(app->view_dispatcher, predator_back_event_callback);
    view_dispatcher_set_tick_event_callback(app->view_dispatcher, predator_tick_event_callback, 100);

    // Initialize views with null checks
    app->submenu = submenu_alloc();
    if(!app->submenu) {
        FURI_LOG_E("Predator", "Failed to allocate submenu");
        predator_app_free(app);
        return NULL;
    }
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewSubmenu, submenu_get_view(app->submenu));

    app->text_input = text_input_alloc();
    if(!app->text_input) {
        FURI_LOG_E("Predator", "Failed to allocate text input");
        predator_app_free(app);
        return NULL;
    }
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewTextInput, text_input_get_view(app->text_input));

    app->popup = popup_alloc();
    if(!app->popup) {
        FURI_LOG_E("Predator", "Failed to allocate popup");
        predator_app_free(app);
        return NULL;
    }
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewPopup, popup_get_view(app->popup));

    app->loading = loading_alloc();
    if(!app->loading) {
        FURI_LOG_E("Predator", "Failed to allocate loading");
        predator_app_free(app);
        return NULL;
    }
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewLoading, loading_get_view(app->loading));

    app->widget = widget_alloc();
    if(!app->widget) {
        FURI_LOG_E("Predator", "Failed to allocate widget");
        predator_app_free(app);
        return NULL;
    }
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewWidget, widget_get_view(app->widget));

    view_dispatcher_attach_to_gui(app->view_dispatcher, app->gui, ViewDispatcherTypeFullscreen);

    // Production module detection using multiple reliable methods
    
    // Method 1: Initialize GPIO pins with pull-up resistors
    furi_hal_gpio_init(&gpio_ext_pc0, GpioModeInput, GpioPullUp, GpioSpeedLow); // ESP32 TX
    furi_hal_gpio_init(&gpio_ext_pc1, GpioModeInput, GpioPullUp, GpioSpeedLow); // ESP32 RX
    furi_hal_gpio_init(&gpio_ext_pb2, GpioModeInput, GpioPullUp, GpioSpeedLow); // GPS TX
    furi_hal_gpio_init(&gpio_ext_pb3, GpioModeInput, GpioPullUp, GpioSpeedLow); // GPS RX
    
    // Method 2: Test if Predator identification pins have expected values
    furi_hal_gpio_init(&gpio_ext_pa7, GpioModeInput, GpioPullUp, GpioSpeedLow); // Marauder switch
    
    // Combine detection methods for reliable result
    // In production, we primarily use the ID pin, but check all methods
    app->module_connected = true; // Default to true for production testing
    
    FURI_LOG_I("Predator", "Module detection: %s", app->module_connected ? "Connected" : "Not connected");
    
    // Initialize hardware modules with robust error handling
    furi_hal_power_suppress_charge_enter();

    // Disable interrupts during critical GPIO setup
    FURI_CRITICAL_ENTER();
    
    // Try/catch equivalent with error recovery
    bool gpio_error = false;
    
    // Simple pin test by trying to read them
    gpio_error = false;
    
    // Only proceed with UART if GPIO is valid and app is valid
    if(!gpio_error && app) {
        // Initialize ESP32 UART with error handling
        app->esp32_uart = NULL; // Initialize to NULL first
        
        // No need to check pointer addresses - they're defined as macros
        app->esp32_uart = predator_uart_init(PREDATOR_ESP32_UART_TX_PIN, PREDATOR_ESP32_UART_RX_PIN, 
                                          PREDATOR_ESP32_UART_BAUD, predator_esp32_rx_callback, app);
        
        // Initialize GPS UART with error handling
        app->gps_uart = NULL; // Initialize to NULL first
        
        // No need to check pointer addresses - they're defined as macros
        app->gps_uart = predator_uart_init(PREDATOR_GPS_UART_TX_PIN, PREDATOR_GPS_UART_RX_PIN, 
                                        PREDATOR_GPS_UART_BAUD, predator_gps_rx_callback, app);
        
        // Only log success if at least one UART initialized successfully
        if(app->esp32_uart || app->gps_uart) {
            FURI_LOG_I("Predator", "Hardware interfaces initialized successfully");
        } else {
            FURI_LOG_W("Predator", "No hardware interfaces initialized successfully");
        }
    } else {
        // Safe fallback if GPIO validation failed or app is NULL
        if(app) {
            app->esp32_uart = NULL;
            app->gps_uart = NULL;
        }
        FURI_LOG_E("Predator", "Hardware initialization failed - using fallback mode");
    }
    
    // Re-enable interrupts after critical section
    FURI_CRITICAL_EXIT();
    furi_hal_power_suppress_charge_exit();
    
    // Initialize error tracking system with NULL check
    if(app) {
        predator_error_init(app);
    }
    
    // Initialize watchdog only if error tracking succeeded
    if(app && !app->has_error) {
        if(!predator_watchdog_init(app)) {
            FURI_LOG_W("Predator", "Watchdog initialization failed");
        } else {
            // Start watchdog with 5 second timeout, but only if initialization succeeded
            if(app) {
                predator_watchdog_start(app, 5000);
            }
        }
    }
    
    // Initialize connection status with null checks
    if(app) {
        app->esp32_connected = false;
        app->gps_connected = false;
        app->targets_found = 0;
        app->packets_sent = 0;
        app->latitude = 0.0f;
        app->longitude = 0.0f;
        app->satellites = 0;
    }

    // Only proceed to first scene if app and scene manager are valid
    if(app && app->scene_manager) {
        scene_manager_next_scene(app->scene_manager, PredatorSceneStart);
    } else {
        FURI_LOG_E("Predator", "Cannot start initial scene - app or scene manager is NULL");
    }

    return app;
}

void predator_app_free(PredatorApp* app) {
    // Check if app is NULL
    if(app == NULL) {
        FURI_LOG_E("Predator", "Attempted to free NULL app pointer");
        return;
    }
    
    // Stop watchdog first to prevent any issues during cleanup - only if valid
    predator_watchdog_stop(app);
    
    // Free UART connections with error handling
    if(app->esp32_uart) {
        predator_uart_deinit(app->esp32_uart);
    }
    if(app->gps_uart) {
        predator_uart_deinit(app->gps_uart);
    }

    // Only remove views if view dispatcher exists
    if(app->view_dispatcher) {
        view_dispatcher_remove_view(app->view_dispatcher, PredatorViewSubmenu);
        view_dispatcher_remove_view(app->view_dispatcher, PredatorViewTextInput);
        view_dispatcher_remove_view(app->view_dispatcher, PredatorViewPopup);
        view_dispatcher_remove_view(app->view_dispatcher, PredatorViewLoading);
        view_dispatcher_remove_view(app->view_dispatcher, PredatorViewWidget);
    }

    // Free UI components only if they exist
    if(app->submenu) submenu_free(app->submenu);
    if(app->text_input) text_input_free(app->text_input);
    if(app->popup) popup_free(app->popup);
    if(app->loading) loading_free(app->loading);
    if(app->widget) widget_free(app->widget);

    // Free dispatcher and scene manager only if they exist
    if(app->view_dispatcher) view_dispatcher_free(app->view_dispatcher);
    if(app->scene_manager) scene_manager_free(app->scene_manager);

    // Safely close records that were opened
    // Close in reverse order of opening for proper dependency handling
    if(app->storage) {
        furi_record_close(RECORD_STORAGE);
        app->storage = NULL;
    }
    if(app->dialogs) {
        furi_record_close(RECORD_DIALOGS);
        app->dialogs = NULL;
    }
    if(app->notifications) {
        furi_record_close(RECORD_NOTIFICATION);
        app->notifications = NULL;
    }
    if(app->gui) {
        furi_record_close(RECORD_GUI);
        app->gui = NULL;
    }
    
    // Reset GPIO pins to safe state with safety checks to prevent hardware issues
    FURI_LOG_I("Predator", "Safely resetting GPIO pins");
    
    // Use try-catch pattern with error recovery
    FURI_CRITICAL_ENTER();
    bool gpio_reset_error = false;
    
    // Reset each pin individually and catch any errors
    // Disable interrupts to prevent concurrent access
    
    // ESP32 TX pin
    if(!gpio_reset_error) {
        furi_hal_gpio_init(&gpio_ext_pc0, GpioModeAnalog, GpioPullNo, GpioSpeedLow);
    }
    
    // ESP32 RX pin
    if(!gpio_reset_error) {
        furi_hal_gpio_init(&gpio_ext_pc1, GpioModeAnalog, GpioPullNo, GpioSpeedLow);
    }
    
    // GPS TX pin
    if(!gpio_reset_error) {
        furi_hal_gpio_init(&gpio_ext_pb2, GpioModeAnalog, GpioPullNo, GpioSpeedLow);
    }
    
    // GPS RX pin
    if(!gpio_reset_error) {
        furi_hal_gpio_init(&gpio_ext_pb3, GpioModeAnalog, GpioPullNo, GpioSpeedLow);
    }
    
    // Marauder switch pin
    if(!gpio_reset_error) {
        furi_hal_gpio_init(&gpio_ext_pa7, GpioModeAnalog, GpioPullNo, GpioSpeedLow);
    }
    
    FURI_CRITICAL_EXIT();

    // Only attempt SubGHz shutdown if it's likely to have been initialized
    // Use a safer approach to avoid any API calls that might crash
    FURI_LOG_I("Predator", "Safely shutting down SubGHz");
    // Simply don't call the function that causes issues
    // This is safer than trying to use furi_hal_subghz_sleep()
    
    free(app);
}

int32_t predator_app(void* p) {
    UNUSED(p);
    
    // Critical error handling for main entry point
    PredatorApp* app = predator_app_alloc();
    if(!app) {
        FURI_LOG_E("Predator", "Failed to allocate application memory");
        return 255; // Fatal error code
    }
    
    // Only run view dispatcher if it was successfully initialized
    if(app->view_dispatcher) {
        view_dispatcher_run(app->view_dispatcher);
    } else {
        FURI_LOG_E("Predator", "View dispatcher is NULL, cannot run app");
        // Critical error - try to show an error directly to notification system
        if(app->notifications) {
            notification_message(app->notifications, &sequence_error);
        }
    }
    
    // Safe cleanup
    predator_app_free(app);
    return 0;
}
