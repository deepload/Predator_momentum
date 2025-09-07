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

#include "scenes/predator_scene.h"

static bool predator_custom_event_callback(void* context, uint32_t event) {
    furi_assert(context);
    PredatorApp* app = context;
    return scene_manager_handle_custom_event(app->scene_manager, event);
}

static bool predator_back_event_callback(void* context) {
    furi_assert(context);
    PredatorApp* app = context;
    return scene_manager_handle_back_event(app->scene_manager);
}

static void predator_tick_event_callback(void* context) {
    furi_assert(context);
    PredatorApp* app = context;
    scene_manager_handle_tick_event(app->scene_manager);
}

PredatorApp* predator_app_alloc() {
    PredatorApp* app = malloc(sizeof(PredatorApp));

    app->gui = furi_record_open(RECORD_GUI);
    app->notifications = furi_record_open(RECORD_NOTIFICATION);
    app->dialogs = furi_record_open(RECORD_DIALOGS);
    app->storage = furi_record_open(RECORD_STORAGE);

    app->view_dispatcher = view_dispatcher_alloc();
    app->scene_manager = scene_manager_alloc(&predator_scene_handlers, app);

    // Note: using default queue management
    // The deprecated view_dispatcher_enable_queue would have been here
    view_dispatcher_set_event_callback_context(app->view_dispatcher, app);
    view_dispatcher_set_custom_event_callback(app->view_dispatcher, predator_custom_event_callback);
    view_dispatcher_set_navigation_event_callback(app->view_dispatcher, predator_back_event_callback);
    view_dispatcher_set_tick_event_callback(app->view_dispatcher, predator_tick_event_callback, 100);

    // Initialize views
    app->submenu = submenu_alloc();
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewSubmenu, submenu_get_view(app->submenu));

    app->text_input = text_input_alloc();
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewTextInput, text_input_get_view(app->text_input));

    app->popup = popup_alloc();
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewPopup, popup_get_view(app->popup));

    app->loading = loading_alloc();
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewLoading, loading_get_view(app->loading));

    app->widget = widget_alloc();
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewWidget, widget_get_view(app->widget));

    view_dispatcher_attach_to_gui(app->view_dispatcher, app->gui, ViewDispatcherTypeFullscreen);

    // Check if Predator module is connected using multiple detection methods
    
    // Method 1: Try to access the GPIO pins used for module communication
    furi_hal_gpio_init(&gpio_ext_pc0, GpioModeInput, GpioPullUp, GpioSpeedLow);
    furi_hal_gpio_init(&gpio_ext_pc1, GpioModeInput, GpioPullUp, GpioSpeedLow);
    
    // Method 2: Test if Predator identification pins have expected values
    // Real detection would check hardware-specific pins that have known values
    furi_hal_gpio_init(&gpio_ext_pa7, GpioModeInput, GpioPullUp, GpioSpeedLow);
    bool id_pin_state = furi_hal_gpio_read(&gpio_ext_pa7);
    
    // Determine module connection status
    app->module_connected = id_pin_state; // Use pin state for detection
    
    FURI_LOG_I("Predator", "Module detection: %s", app->module_connected ? "Connected" : "Not connected");
    
    // Initialize hardware modules only if module is detected
    if(app->module_connected) {
        app->esp32_uart = predator_uart_init(PREDATOR_ESP32_UART_TX_PIN, PREDATOR_ESP32_UART_RX_PIN, PREDATOR_ESP32_UART_BAUD, predator_esp32_rx_callback, app);
        app->gps_uart = predator_uart_init(PREDATOR_GPS_UART_TX_PIN, PREDATOR_GPS_UART_RX_PIN, PREDATOR_GPS_UART_BAUD, predator_gps_rx_callback, app);
    } else {
        // Safe fallbacks when module isn't connected
        app->esp32_uart = NULL;
        app->gps_uart = NULL;
        FURI_LOG_I("Predator", "Running in limited mode without hardware access");
        
        // Show popup notification for module not detected
        Popup* popup = popup_alloc();
        popup_set_header(popup, "Predator Module", 64, 10, AlignCenter, AlignTop);
        popup_set_text(popup, "Module not detected\n\nRunning in LIMITED MODE\nSome features unavailable", 64, 32, AlignCenter, AlignTop);
        popup_set_timeout(popup, 3000); // 3 seconds
        popup_set_context(popup, app);
        
        // Setup popup callback
        popup_set_callback(popup, NULL);
        popup_enable_timeout(popup);
        
        // Show popup
        view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
        
        // Give time to show popup
        furi_delay_ms(3100);
        
        // Clean up popup
        popup_free(popup);
    }
    
    // Initialize connection status
    app->esp32_connected = false;
    app->gps_connected = false;
    app->targets_found = 0;
    app->packets_sent = 0;
    app->latitude = 0.0f;
    app->longitude = 0.0f;
    app->satellites = 0;

    scene_manager_next_scene(app->scene_manager, PredatorSceneStart);

    return app;
}

void predator_app_free(PredatorApp* app) {
    furi_assert(app);

    // Free UART connections
    if(app->esp32_uart) {
        predator_uart_deinit(app->esp32_uart);
    }
    if(app->gps_uart) {
        predator_uart_deinit(app->gps_uart);
    }

    view_dispatcher_remove_view(app->view_dispatcher, PredatorViewSubmenu);
    view_dispatcher_remove_view(app->view_dispatcher, PredatorViewTextInput);
    view_dispatcher_remove_view(app->view_dispatcher, PredatorViewPopup);
    view_dispatcher_remove_view(app->view_dispatcher, PredatorViewLoading);
    view_dispatcher_remove_view(app->view_dispatcher, PredatorViewWidget);

    submenu_free(app->submenu);
    text_input_free(app->text_input);
    popup_free(app->popup);
    loading_free(app->loading);
    widget_free(app->widget);

    view_dispatcher_free(app->view_dispatcher);
    scene_manager_free(app->scene_manager);

    furi_record_close(RECORD_GUI);
    furi_record_close(RECORD_NOTIFICATION);
    furi_record_close(RECORD_DIALOGS);
    furi_record_close(RECORD_STORAGE);

    free(app);
}

int32_t predator_app(void* p) {
    UNUSED(p);
    PredatorApp* app = predator_app_alloc();
    view_dispatcher_run(app->view_dispatcher);
    predator_app_free(app);
    return 0;
}
