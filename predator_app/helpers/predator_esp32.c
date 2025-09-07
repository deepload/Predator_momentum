#include "predator_esp32.h"
#include "../predator_i.h"
#include "../predator_uart.h"
#include "predator_boards.h"
#include <furi.h>
#include <furi_hal.h>
#include <string.h>

void predator_esp32_rx_callback(uint8_t* buf, size_t len, void* context) {
    // Critical safety checks
    if(!buf || !len || !context) {
        return;
    }
    
    PredatorApp* app = (PredatorApp*)context;
    
    // Ensure buf is null-terminated for string operations
    uint8_t* safe_buf = malloc(len + 1);
    if(!safe_buf) {
        return; // Memory allocation failed
    }
    
    // Copy and null-terminate
    memcpy(safe_buf, buf, len);
    safe_buf[len] = '\0';
    
    // Process ESP32 response with safety checks
    if(app) {
        // Any incoming data indicates the UART path is alive
        app->esp32_connected = true;
        // Check for connection status
        if(strstr((char*)safe_buf, "ESP32") || strstr((char*)safe_buf, "Marauder")) {
            app->esp32_connected = true;
        }
        
        // Parse scan results, attack status, etc.
        if(strstr((char*)safe_buf, "AP Found:")) {
            app->targets_found++;
        }
        
        if(strstr((char*)safe_buf, "Deauth sent:")) {
            app->packets_sent++;
        }
    }
    
    // Clean up
    free(safe_buf);
}

void predator_esp32_init(PredatorApp* app) {
    // Critical safety check
    if(!app) {
        FURI_LOG_E("PredatorESP32", "NULL app pointer in init");
        return;
    }
    
    // Make sure we don't initialize twice
    if(app->esp32_uart) {
        FURI_LOG_I("PredatorESP32", "ESP32 already initialized");
        return;
    }
    
    // Get board configuration
    const PredatorBoardConfig* board_config = predator_boards_get_config(app->board_type);
    if(!board_config) {
        FURI_LOG_E("PredatorESP32", "Invalid board configuration");
        return;
    }
    
    FURI_LOG_I("PredatorESP32", "Using board: %s", board_config->name);
    
    // Check Marauder switch state if board has one
    bool enable_esp32 = true;
    if(board_config->marauder_switch) {
        furi_hal_gpio_init(board_config->marauder_switch, GpioModeInput, GpioPullUp, GpioSpeedLow);
        // Switch is active-low: ON when read == 0
        enable_esp32 = !furi_hal_gpio_read(board_config->marauder_switch);
        
        if(!enable_esp32) {
            // Switch is OFF; do not init to keep app stable when hardware is not powered
            FURI_LOG_W("PredatorESP32", "Marauder switch is OFF - skipping ESP32 init");
            app->esp32_connected = false;
            return;
        }
    }

    FURI_LOG_I("PredatorESP32", "Initializing ESP32 communication");
    
    // Initialize with safety checks
    app->esp32_connected = false;
    
    // Delay for hardware stabilization
    furi_delay_ms(10);
    
    // Initialize UART with error handling using board-specific pins
    app->esp32_uart = predator_uart_init(
        board_config->esp32_tx_pin,
        board_config->esp32_rx_pin,
        board_config->esp32_baud_rate,
        predator_esp32_rx_callback,
        app);
        
    if(!app->esp32_uart) {
        FURI_LOG_E("PredatorESP32", "Failed to initialize UART");
        return;
    }
    
    FURI_LOG_I("PredatorESP32", "ESP32 UART initialized on board: %s", board_config->name);
    
    // Optionally send status command to check connection (non-fatal)
    predator_esp32_send_command(app, MARAUDER_CMD_STATUS);
    
    // Give ESP32 time to respond
    furi_delay_ms(100);
}

void predator_esp32_deinit(PredatorApp* app) {
    // Critical safety check
    if(!app) {
        FURI_LOG_E("PredatorESP32", "NULL app pointer in deinit");
        return;
    }
    
    // Log deinit operation
    FURI_LOG_I("PredatorESP32", "Deinitializing ESP32 communication");
    
    // Clean up UART if it exists
    if(app->esp32_uart) {
        // Try to send stop command before deinit
        predator_esp32_send_command(app, MARAUDER_CMD_STOP);
        
        // Small delay to allow command to be sent
        furi_delay_ms(10);
        
        // Now close UART
        predator_uart_deinit(app->esp32_uart);
        app->esp32_uart = NULL;
    }
    
    // Reset connection status
    app->esp32_connected = false;
}

bool predator_esp32_send_command(PredatorApp* app, const char* command) {
    // Critical safety checks with specific error messages
    if(!app) {
        FURI_LOG_E("PredatorESP32", "NULL app pointer in send_command");
        return false;
    }
    
    if(!app->esp32_uart) {
        FURI_LOG_E("PredatorESP32", "NULL uart pointer in send_command");
        return false;
    }
    
    if(!command) {
        FURI_LOG_E("PredatorESP32", "NULL command in send_command");
        return false;
    }
    
    // Copy command to avoid potential memory corruption
    size_t len = strlen(command);
    if(len == 0 || len > 128) { // Sanity check on command length
        FURI_LOG_E("PredatorESP32", "Invalid command length: %d", (int)len);
        return false;
    }
    
    // Use a temporary buffer for the command
    char* safe_cmd = malloc(len + 1);
    if(!safe_cmd) {
        FURI_LOG_E("PredatorESP32", "Memory allocation failed for command");
        return false;
    }
    
    // Copy and terminate
    memcpy(safe_cmd, command, len);
    safe_cmd[len] = '\0';
    
    // Log the command for debugging
    FURI_LOG_D("PredatorESP32", "Sending command: %s", safe_cmd);
    
    // Send the command with error handling
    predator_uart_tx(app->esp32_uart, (uint8_t*)safe_cmd, len);
    predator_uart_tx(app->esp32_uart, (uint8_t*)"\r\n", 2);
    
    // Clean up
    free(safe_cmd);
    
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
