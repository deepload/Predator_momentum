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

    view_dispatcher_set_queue_enabled(app->view_dispatcher, true);
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

    // Initialize hardware modules
    app->esp32_uart = predator_uart_init(PREDATOR_ESP32_UART_TX_PIN, PREDATOR_ESP32_UART_RX_PIN, PREDATOR_ESP32_UART_BAUD, predator_esp32_rx_callback, app);
    app->gps_uart = predator_uart_init(PREDATOR_GPS_UART_TX_PIN, PREDATOR_GPS_UART_RX_PIN, PREDATOR_GPS_UART_BAUD, predator_gps_rx_callback, app);
    
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
