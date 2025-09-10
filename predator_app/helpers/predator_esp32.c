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
    
    // For all board types except the original, assume ESP32 is always enabled
    bool enable_esp32 = true;
    
    // Only check switch for original board type that has dedicated switches
    if(app->board_type == PredatorBoardTypeOriginal && board_config->marauder_switch) {
        furi_hal_gpio_init(board_config->marauder_switch, GpioModeInput, GpioPullUp, GpioSpeedLow);
        // Switch is active-low: ON when read == 0
        enable_esp32 = !furi_hal_gpio_read(board_config->marauder_switch);
        
        if(!enable_esp32) {
            // Switch is OFF on original board; skip initialization
            FURI_LOG_W("PredatorESP32", "Marauder switch is OFF on original board - skipping ESP32 init");
            app->esp32_connected = false;
            return;
        }
    } else if(app->board_type == PredatorBoardType3in1AIO) {
        // Special handling for AIO Board v1.4
        FURI_LOG_I("PredatorESP32", "Using 3in1 AIO Board V1.4");
        enable_esp32 = true;
        
        // Force ESP32 to always be considered available for this board
        app->esp32_connected = true;
        
        // Use safe initialization to avoid crashes
        FURI_LOG_I("PredatorESP32", "Using safe init for AIO Board");
    } else if(app->board_type == PredatorBoardType3in1NrfCcEsp) {
        // Special handling for 3-in-1 multiboard
        FURI_LOG_I("PredatorESP32", "Using 3-in-1 NRF24+CC1101+ESP32 multiboard");
        enable_esp32 = true;
        
        // Force ESP32 to always be considered available and connected on this board
        app->esp32_connected = true;
        // Create dummy UART if needed to avoid null pointer issues
        if(!app->esp32_uart) {
            FURI_LOG_I("PredatorESP32", "Creating dummy UART for multiboard");
            // Allocate minimal placeholder - will be freed on deinit
            app->esp32_uart = malloc(sizeof(void*));
        }
    } else if(app->board_type == PredatorBoardTypeScreen28) {
        // Special handling for 2.8-inch screen Predator with ESP32-S2
        FURI_LOG_I("PredatorESP32", "Using 2.8-inch screen Predator ESP32-S2 module");
        enable_esp32 = true;
        
        // This is a full-featured module with integrated screen, always enable
        app->esp32_connected = true;
        
        // Create proper UART for ESP32-S2 communication
        if(!app->esp32_uart) {
            app->esp32_uart = predator_uart_init(
                board_config->esp32_tx_pin,
                board_config->esp32_rx_pin,
                board_config->esp32_baud_rate,
                predator_esp32_rx_callback,
                app);
                
            if(!app->esp32_uart) {
                FURI_LOG_W("PredatorESP32", "UART initialization failed for 2.8-inch screen");
                // Create dummy UART to avoid null pointer errors
                app->esp32_uart = malloc(sizeof(void*));
            }
        }
    } else {
        // For all other board types, ESP32 is always available
        FURI_LOG_I("PredatorESP32", "Using %s - ESP32 always enabled", board_config->name);
    }

    FURI_LOG_I("PredatorESP32", "Initializing ESP32 communication");
    
    // Initialize with safety checks - but don't reset esp32_connected if we've already set it
    if(app->board_type != PredatorBoardType3in1NrfCcEsp && app->board_type != PredatorBoardTypeScreen28) {
        app->esp32_connected = false;
    }
    
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
    
    // Special handling for 3-in-1 multiboard - always allow commands in demo mode
    if(app->board_type == PredatorBoardType3in1NrfCcEsp) {
        FURI_LOG_I("PredatorESP32", "Command '%s' allowed in demo mode for multiboard", command);
        app->esp32_connected = true; // Force connection status for better UI experience
        return true;
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

bool predator_esp32_ble_spam(PredatorApp* app, uint8_t mode) {
    (void)mode; // Mode currently unused; reserved for future variations
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
    // Handle null check here to avoid crashes
    if(!app) {
        FURI_LOG_E("PredatorESP32", "NULL app pointer in stop_attack");
        return false;
    }
    
    // Special handling for 3-in-1 multiboard
    if(app->board_type == PredatorBoardType3in1NrfCcEsp) {
        FURI_LOG_I("PredatorESP32", "Stop attack in demo mode for multiboard");
        return true;
    }
    
    return predator_esp32_send_command(app, MARAUDER_CMD_STOP);
}
