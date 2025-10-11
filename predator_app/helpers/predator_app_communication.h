#pragma once

#include <furi.h>
#include <storage/storage.h>

// Professional Inter-App Communication System
// Enables seamless communication between Master Control and specialized apps

#define PREDATOR_IPC_VERSION "1.0"
#define PREDATOR_IPC_STORAGE_PATH "/ext/predator_ipc"
#define PREDATOR_IPC_STATUS_FILE "/ext/predator_ipc/status.json"
#define PREDATOR_IPC_COMMANDS_FILE "/ext/predator_ipc/commands.json"
#define PREDATOR_IPC_RESULTS_FILE "/ext/predator_ipc/results.json"
#define PREDATOR_IPC_SHARED_CONFIG "/ext/predator_ipc/config.json"

// Professional App Status Types
typedef enum {
    PredatorAppStatusIdle,
    PredatorAppStatusRunning,
    PredatorAppStatusCompleted,
    PredatorAppStatusError,
    PredatorAppStatusWaiting,
    PredatorAppStatusCommunicating
} PredatorAppStatus;

// Professional App Types
typedef enum {
    PredatorAppTypeMaster,
    PredatorAppTypeGovernment,
    PredatorAppTypeWifi,
    PredatorAppTypeCar,
    PredatorAppTypeTesla,
    PredatorAppTypeBluetooth,
    PredatorAppTypeRfid,
    PredatorAppTypeSubghz,
    PredatorAppTypeInfrastructure,
    PredatorAppTypeCasino,
    PredatorAppTypeNetwork,
    PredatorAppTypeCrypto
} PredatorAppType;

// Professional Command Types
typedef enum {
    PredatorCommandLaunchApp,
    PredatorCommandStopApp,
    PredatorCommandGetStatus,
    PredatorCommandSetConfig,
    PredatorCommandGetResults,
    PredatorCommandSyncData,
    PredatorCommandEmergencyMode,
    PredatorCommandGovernmentMode,
    PredatorCommandVipMode
} PredatorCommandType;

// Professional App Information Structure
typedef struct {
    PredatorAppType app_type;
    PredatorAppStatus status;
    char app_name[64];
    char version[16];
    uint32_t pid;
    uint32_t memory_usage;
    uint32_t cpu_usage;
    uint32_t last_activity;
    bool vip_mode;
    bool government_mode;
    bool emergency_mode;
    char current_operation[128];
    uint32_t operations_count;
    uint32_t success_count;
    uint32_t error_count;
} PredatorAppInfo;

// Professional Command Structure
typedef struct {
    PredatorCommandType command;
    PredatorAppType target_app;
    PredatorAppType source_app;
    uint32_t command_id;
    uint32_t timestamp;
    char parameters[256];
    char response[512];
    bool processed;
    bool success;
} PredatorCommand;

// Professional Shared Configuration
typedef struct {
    // Global Settings
    uint32_t region;
    bool vip_mode_global;
    bool government_mode_global;
    bool emergency_mode_global;
    
    // Hardware Configuration
    uint32_t board_type;
    bool esp32_enabled;
    bool gps_enabled;
    bool subghz_enabled;
    bool nfc_enabled;
    
    // Attack Configuration
    uint32_t max_power_level;
    uint32_t default_frequency;
    uint32_t attack_timeout;
    bool logging_enabled;
    
    // Professional Features
    char operator_name[64];
    char organization[64];
    char contract_id[32];
    uint32_t license_level;
} PredatorSharedConfig;

// Professional Communication Functions
bool predator_ipc_init(void);
void predator_ipc_deinit(void);

// App Registration and Status
bool predator_ipc_register_app(PredatorAppType app_type, const char* app_name, const char* version);
bool predator_ipc_unregister_app(PredatorAppType app_type);
bool predator_ipc_update_status(PredatorAppType app_type, PredatorAppStatus status, const char* operation);
bool predator_ipc_get_app_info(PredatorAppType app_type, PredatorAppInfo* info);
bool predator_ipc_get_all_apps(PredatorAppInfo* apps, uint32_t* count);

// Command System
bool predator_ipc_send_command(PredatorCommandType command, PredatorAppType target, const char* parameters);
bool predator_ipc_get_command(PredatorAppType app_type, PredatorCommand* command);
bool predator_ipc_send_response(uint32_t command_id, bool success, const char* response);
bool predator_ipc_wait_for_response(uint32_t command_id, char* response, uint32_t timeout_ms);

// Shared Configuration
bool predator_ipc_set_shared_config(const PredatorSharedConfig* config);
bool predator_ipc_get_shared_config(PredatorSharedConfig* config);
bool predator_ipc_sync_config_to_app(PredatorAppType app_type);

// Professional Monitoring
bool predator_ipc_update_performance(PredatorAppType app_type, uint32_t memory, uint32_t cpu);
bool predator_ipc_log_operation(PredatorAppType app_type, const char* operation, bool success);
bool predator_ipc_get_statistics(PredatorAppType app_type, uint32_t* operations, uint32_t* success, uint32_t* errors);

// Emergency and VIP Modes
bool predator_ipc_activate_emergency_mode(void);
bool predator_ipc_activate_government_mode(void);
bool predator_ipc_activate_vip_mode(void);
bool predator_ipc_broadcast_mode_change(void);

// Professional App Launcher
bool predator_ipc_launch_app(PredatorAppType app_type, const char* parameters);
bool predator_ipc_is_app_running(PredatorAppType app_type);
bool predator_ipc_stop_app(PredatorAppType app_type);
bool predator_ipc_restart_app(PredatorAppType app_type);

// Data Synchronization
bool predator_ipc_sync_attack_results(PredatorAppType app_type, const char* results);
bool predator_ipc_get_attack_results(PredatorAppType app_type, char* results, size_t max_size);
bool predator_ipc_clear_results(PredatorAppType app_type);

// Professional Utilities
const char* predator_ipc_app_type_to_string(PredatorAppType app_type);
const char* predator_ipc_status_to_string(PredatorAppStatus status);
const char* predator_ipc_command_to_string(PredatorCommandType command);
uint32_t predator_ipc_generate_command_id(void);
bool predator_ipc_validate_app_communication(void);
