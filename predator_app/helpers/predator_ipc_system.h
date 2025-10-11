#pragma once

#include <furi.h>

// PROFESSIONAL IPC SYSTEM FOR MULTI-APP COMMUNICATION
// Enables Master Orchestrator to control specialist apps

// IPC Command types
typedef enum {
    PredatorIPCCommandLaunch = 1,      // Launch specialist app
    PredatorIPCCommandStop,            // Stop specialist app
    PredatorIPCCommandStatus,          // Get app status
    PredatorIPCCommandExecute,         // Execute specific function
    PredatorIPCCommandSync,            // Synchronize data
    PredatorIPCCommandShutdown         // Shutdown app
} PredatorIPCCommand;

// IPC Response types
typedef enum {
    PredatorIPCResponseOK = 1,         // Command executed successfully
    PredatorIPCResponseError,          // Command failed
    PredatorIPCResponseBusy,           // App is busy
    PredatorIPCResponseNotFound,       // App not found
    PredatorIPCResponseData            // Response contains data
} PredatorIPCResponse;

// IPC System functions
bool predator_ipc_init(void);
void predator_ipc_deinit(void);

// Master Orchestrator functions
bool predator_ipc_send_command(PredatorIPCCommand command, const char* app_name, const char* params);
bool predator_ipc_read_response(PredatorIPCResponse* response, char* data, size_t max_len);
bool predator_ipc_launch_app(const char* app_name);
bool predator_ipc_has_response(void);

// Specialist App functions
bool predator_ipc_read_command(PredatorIPCCommand* command, char* app_name, char* params, size_t max_len);
bool predator_ipc_send_response(PredatorIPCResponse response, const char* data);
bool predator_ipc_has_command(void);

// App names for IPC
#define PREDATOR_APP_WIFI "predator_wifi_specialist"
#define PREDATOR_APP_CAR "predator_car_specialist"
#define PREDATOR_APP_TESLA "predator_tesla_vip"
#define PREDATOR_APP_BLUETOOTH "predator_bluetooth_specialist"
#define PREDATOR_APP_RFID "predator_rfid_specialist"
