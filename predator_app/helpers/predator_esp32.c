#include "predator_esp32.h"
#include "../predator_i.h"
#include "../predator_uart.h"
#include <furi.h>
#include <furi_hal.h>
#include <string.h>

void predator_esp32_rx_callback(uint8_t* buf, size_t len, void* context) {
    PredatorApp* app = (PredatorApp*)context;
    
    // Process ESP32 response
    if(len > 0) {
        // Check for connection status
        if(strstr((char*)buf, "ESP32") || strstr((char*)buf, "Marauder")) {
            app->esp32_connected = true;
        }
        
        // Parse scan results, attack status, etc.
        if(strstr((char*)buf, "AP Found:")) {
            app->targets_found++;
        }
        
        if(strstr((char*)buf, "Deauth sent:")) {
            app->packets_sent++;
        }
    }
}

void predator_esp32_init(PredatorApp* app) {
    // Initialize ESP32 UART communication
    app->esp32_uart = predator_uart_init(PREDATOR_ESP32_UART_TX_PIN, PREDATOR_ESP32_UART_RX_PIN, PREDATOR_ESP32_UART_BAUD, predator_esp32_rx_callback, app);
    app->esp32_connected = false;
    
    // Send status command to check connection
    predator_esp32_send_command(app, MARAUDER_CMD_STATUS);
}

void predator_esp32_deinit(PredatorApp* app) {
    if(app->esp32_uart) {
        predator_uart_deinit(app->esp32_uart);
        app->esp32_uart = NULL;
    }
    app->esp32_connected = false;
}

bool predator_esp32_send_command(PredatorApp* app, const char* command) {
    if(!app || !app->esp32_uart || !command) return false;
    
    size_t len = strlen(command);
    predator_uart_tx(app->esp32_uart, (uint8_t*)command, len);
    predator_uart_tx(app->esp32_uart, (uint8_t*)"\r\n", 2);
    
    return true;
}

bool predator_esp32_is_connected(PredatorApp* app) {
    return app->esp32_connected;
}

// WiFi Attack Functions
bool predator_esp32_wifi_scan(PredatorApp* app) {
    return predator_esp32_send_command(app, MARAUDER_CMD_WIFI_SCAN);
}

bool predator_esp32_wifi_deauth(PredatorApp* app, int channel) {
    char cmd[64];
    snprintf(cmd, sizeof(cmd), "%s %d", MARAUDER_CMD_WIFI_DEAUTH, channel);
    return predator_esp32_send_command(app, cmd);
}

bool predator_esp32_wifi_evil_twin(PredatorApp* app) {
    return predator_esp32_send_command(app, MARAUDER_CMD_WIFI_EVIL_TWIN);
}

// Bluetooth Attack Functions
bool predator_esp32_ble_scan(PredatorApp* app) {
    return predator_esp32_send_command(app, MARAUDER_CMD_BLE_SCAN);
}

bool predator_esp32_ble_spam(PredatorApp* app) {
    return predator_esp32_send_command(app, MARAUDER_CMD_BLE_SPAM);
}

// Wardriving Function
bool predator_esp32_wardrive(PredatorApp* app) {
    return predator_esp32_send_command(app, MARAUDER_CMD_WARDRIVE);
}

// Status and Control
bool predator_esp32_get_status(PredatorApp* app) {
    return predator_esp32_send_command(app, MARAUDER_CMD_STATUS);
}

bool predator_esp32_stop_attack(PredatorApp* app) {
    return predator_esp32_send_command(app, MARAUDER_CMD_STOP);
}
