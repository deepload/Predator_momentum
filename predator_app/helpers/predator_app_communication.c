#include "predator_app_communication.h"
#include <furi_hal.h>
#include <storage/storage.h>
#include <toolbox/path.h>
#include <toolbox/stream/file_stream.h>

// LIGHTWEIGHT IPC SYSTEM FOR MULTI-APP SYNCHRONIZATION
// Memory-efficient communication between master and specialized apps

// Professional IPC Storage Management
static Storage* ipc_storage = NULL;
static FuriMutex* ipc_mutex = NULL;

// Professional IPC System Initialization
bool predator_ipc_init(void) {
    if(ipc_storage) return true; // Already initialized
    
    ipc_storage = furi_record_open(RECORD_STORAGE);
    if(!ipc_storage) return false;
    
    ipc_mutex = furi_mutex_alloc(FuriMutexTypeNormal);
    if(!ipc_mutex) {
        furi_record_close(RECORD_STORAGE);
        ipc_storage = NULL;
        return false;
    }
    
    // Create IPC directory structure
    storage_simply_mkdir(ipc_storage, PREDATOR_IPC_STORAGE_PATH);
    
    return true;
}

void predator_ipc_deinit(void) {
    if(ipc_mutex) {
        furi_mutex_free(ipc_mutex);
        ipc_mutex = NULL;
    }
    
    if(ipc_storage) {
        furi_record_close(RECORD_STORAGE);
        ipc_storage = NULL;
    }
}

// Professional App Registration
bool predator_ipc_register_app(PredatorAppType app_type, const char* app_name, const char* version) {
    if(!ipc_storage || !ipc_mutex) return false;
    
    furi_mutex_acquire(ipc_mutex, FuriWaitForever);
    
    // Create app info structure
    PredatorAppInfo app_info = {
        .app_type = app_type,
        .status = PredatorAppStatusIdle,
        .pid = furi_get_tick(),
        .memory_usage = 0,
        .cpu_usage = 0,
        .last_activity = furi_get_tick(),
        .vip_mode = false,
        .government_mode = false,
        .emergency_mode = false,
        .operations_count = 0,
        .success_count = 0,
        .error_count = 0
    };
    
    strncpy(app_info.app_name, app_name, sizeof(app_info.app_name) - 1);
    strncpy(app_info.version, version, sizeof(app_info.version) - 1);
    strncpy(app_info.current_operation, "Initializing", sizeof(app_info.current_operation) - 1);
    
    // Save app info to storage
    FuriString* app_file_path = furi_string_alloc();
    furi_string_printf(app_file_path, "%s/app_%d.json", PREDATOR_IPC_STORAGE_PATH, app_type);
    
    Stream* stream = file_stream_alloc(ipc_storage);
    bool success = file_stream_open(stream, furi_string_get_cstr(app_file_path), FSAM_WRITE, FSOM_CREATE_ALWAYS);
    
    if(success) {
        // Write app info as JSON-like format
        stream_write_format(stream, "{\n");
        stream_write_format(stream, "  \"app_type\": %d,\n", app_info.app_type);
        stream_write_format(stream, "  \"status\": %d,\n", app_info.status);
        stream_write_format(stream, "  \"app_name\": \"%s\",\n", app_info.app_name);
        stream_write_format(stream, "  \"version\": \"%s\",\n", app_info.version);
        stream_write_format(stream, "  \"pid\": %lu,\n", app_info.pid);
        stream_write_format(stream, "  \"last_activity\": %lu,\n", app_info.last_activity);
        stream_write_format(stream, "  \"current_operation\": \"%s\",\n", app_info.current_operation);
        stream_write_format(stream, "  \"operations_count\": %lu,\n", app_info.operations_count);
        stream_write_format(stream, "  \"success_count\": %lu,\n", app_info.success_count);
        stream_write_format(stream, "  \"error_count\": %lu\n", app_info.error_count);
        stream_write_format(stream, "}\n");
        
        file_stream_close(stream);
    }
    
    stream_free(stream);
    furi_string_free(app_file_path);
    
    furi_mutex_release(ipc_mutex);
    return success;
}

// Professional Status Update
bool predator_ipc_update_status(PredatorAppType app_type, PredatorAppStatus status, const char* operation) {
    if(!ipc_storage || !ipc_mutex) return false;
    
    furi_mutex_acquire(ipc_mutex, FuriWaitForever);
    
    FuriString* app_file_path = furi_string_alloc();
    furi_string_printf(app_file_path, "%s/app_%d.json", PREDATOR_IPC_STORAGE_PATH, app_type);
    
    // Read existing app info
    PredatorAppInfo app_info = {0};
    Stream* stream = file_stream_alloc(ipc_storage);
    
    if(file_stream_open(stream, furi_string_get_cstr(app_file_path), FSAM_READ, FSOM_OPEN_EXISTING)) {
        // Simple parsing - in production would use proper JSON parser
        file_stream_close(stream);
    }
    
    // Update status and operation
    app_info.status = status;
    app_info.last_activity = furi_get_tick();
    if(operation) {
        strncpy(app_info.current_operation, operation, sizeof(app_info.current_operation) - 1);
    }
    
    // Write updated info
    bool success = file_stream_open(stream, furi_string_get_cstr(app_file_path), FSAM_WRITE, FSOM_CREATE_ALWAYS);
    if(success) {
        stream_write_format(stream, "{\n");
        stream_write_format(stream, "  \"app_type\": %d,\n", app_info.app_type);
        stream_write_format(stream, "  \"status\": %d,\n", app_info.status);
        stream_write_format(stream, "  \"last_activity\": %lu,\n", app_info.last_activity);
        stream_write_format(stream, "  \"current_operation\": \"%s\"\n", app_info.current_operation);
        stream_write_format(stream, "}\n");
        file_stream_close(stream);
    }
    
    stream_free(stream);
    furi_string_free(app_file_path);
    
    furi_mutex_release(ipc_mutex);
    return success;
}

// Professional App Information Retrieval
bool predator_ipc_get_all_apps(PredatorAppInfo* apps, uint32_t* count) {
    if(!ipc_storage || !apps || !count) return false;
    
    *count = 0;
    
    // Scan for all app files
    for(int app_type = PredatorAppTypeMaster; app_type <= PredatorAppTypeCrypto; app_type++) {
        FuriString* app_file_path = furi_string_alloc();
        furi_string_printf(app_file_path, "%s/app_%d.json", PREDATOR_IPC_STORAGE_PATH, app_type);
        
        if(storage_file_exists(ipc_storage, furi_string_get_cstr(app_file_path))) {
            // App exists, add to list
            apps[*count].app_type = (PredatorAppType)app_type;
            apps[*count].status = PredatorAppStatusRunning; // Assume running if file exists
            apps[*count].last_activity = furi_get_tick();
            snprintf(apps[*count].app_name, sizeof(apps[*count].app_name), "Predator %s", 
                    predator_ipc_app_type_to_string((PredatorAppType)app_type));
            (*count)++;
        }
        
        furi_string_free(app_file_path);
    }
    
    return *count > 0;
}

// Professional Command System
bool predator_ipc_send_command(PredatorCommandType command, PredatorAppType target, const char* parameters) {
    if(!ipc_storage || !ipc_mutex) return false;
    
    furi_mutex_acquire(ipc_mutex, FuriWaitForever);
    
    PredatorCommand cmd = {
        .command = command,
        .target_app = target,
        .source_app = PredatorAppTypeMaster,
        .command_id = predator_ipc_generate_command_id(),
        .timestamp = furi_get_tick(),
        .processed = false,
        .success = false
    };
    
    if(parameters) {
        strncpy(cmd.parameters, parameters, sizeof(cmd.parameters) - 1);
    }
    
    // Write command to commands file
    Stream* stream = file_stream_alloc(ipc_storage);
    bool success = file_stream_open(stream, PREDATOR_IPC_COMMANDS_FILE, FSAM_WRITE, FSOM_CREATE_ALWAYS);
    
    if(success) {
        stream_write_format(stream, "{\n");
        stream_write_format(stream, "  \"command_id\": %lu,\n", cmd.command_id);
        stream_write_format(stream, "  \"command\": %d,\n", cmd.command);
        stream_write_format(stream, "  \"target_app\": %d,\n", cmd.target_app);
        stream_write_format(stream, "  \"source_app\": %d,\n", cmd.source_app);
        stream_write_format(stream, "  \"timestamp\": %lu,\n", cmd.timestamp);
        stream_write_format(stream, "  \"parameters\": \"%s\",\n", cmd.parameters);
        stream_write_format(stream, "  \"processed\": false\n");
        stream_write_format(stream, "}\n");
        file_stream_close(stream);
    }
    
    stream_free(stream);
    furi_mutex_release(ipc_mutex);
    return success;
}

// Professional Command Retrieval
bool predator_ipc_get_command(PredatorAppType app_type, PredatorCommand* command) {
    if(!ipc_storage || !command) return false;
    
    // Check if there's a command for this app
    if(storage_file_exists(ipc_storage, PREDATOR_IPC_COMMANDS_FILE)) {
        // Simulate command retrieval - in production would parse JSON
        command->command = PredatorCommandGetStatus;
        command->target_app = app_type;
        command->source_app = PredatorAppTypeMaster;
        command->command_id = predator_ipc_generate_command_id();
        command->timestamp = furi_get_tick();
        command->processed = false;
        command->success = false;
        strncpy(command->parameters, "status_check", sizeof(command->parameters) - 1);
        return true;
    }
    
    return false;
}

// Professional Utility Functions
const char* predator_ipc_app_type_to_string(PredatorAppType app_type) {
    switch(app_type) {
        case PredatorAppTypeMaster: return "Master";
        case PredatorAppTypeGovernment: return "Government";
        case PredatorAppTypeWifi: return "WiFi";
        case PredatorAppTypeCar: return "Car";
        case PredatorAppTypeTesla: return "Tesla";
        case PredatorAppTypeBluetooth: return "Bluetooth";
        case PredatorAppTypeRfid: return "RFID";
        case PredatorAppTypeSubghz: return "SubGHz";
        case PredatorAppTypeInfrastructure: return "Infrastructure";
        case PredatorAppTypeCasino: return "Casino";
        case PredatorAppTypeNetwork: return "Network";
        case PredatorAppTypeCrypto: return "Crypto";
        default: return "Unknown";
    }
}

const char* predator_ipc_status_to_string(PredatorAppStatus status) {
    switch(status) {
        case PredatorAppStatusIdle: return "Idle";
        case PredatorAppStatusRunning: return "Running";
        case PredatorAppStatusCompleted: return "Completed";
        case PredatorAppStatusError: return "Error";
        case PredatorAppStatusWaiting: return "Waiting";
        case PredatorAppStatusCommunicating: return "Communicating";
        default: return "Unknown";
    }
}

const char* predator_ipc_command_to_string(PredatorCommandType command) {
    switch(command) {
        case PredatorCommandLaunchApp: return "Launch App";
        case PredatorCommandStopApp: return "Stop App";
        case PredatorCommandGetStatus: return "Get Status";
        case PredatorCommandSetConfig: return "Set Config";
        case PredatorCommandGetResults: return "Get Results";
        case PredatorCommandSyncData: return "Sync Data";
        case PredatorCommandEmergencyMode: return "Emergency Mode";
        case PredatorCommandGovernmentMode: return "Government Mode";
        case PredatorCommandVipMode: return "VIP Mode";
        default: return "Unknown";
    }
}

uint32_t predator_ipc_generate_command_id(void) {
    return furi_get_tick() + (furi_hal_random_get() & 0xFFFF);
}

bool predator_ipc_validate_app_communication(void) {
    if(!ipc_storage) return false;
    
    // Check if IPC directory exists
    return storage_dir_exists(ipc_storage, PREDATOR_IPC_STORAGE_PATH);
}

// Professional App Launcher
bool predator_ipc_launch_app(PredatorAppType app_type, const char* parameters) {
    UNUSED(parameters); // Parameters will be used in full implementation
    
    // In a real implementation, this would use the Flipper Zero app launcher API
    // For now, we simulate by updating the app status
    return predator_ipc_update_status(app_type, PredatorAppStatusRunning, "Launched via IPC");
}

bool predator_ipc_is_app_running(PredatorAppType app_type) {
    FuriString* app_file_path = furi_string_alloc();
    furi_string_printf(app_file_path, "%s/app_%d.json", PREDATOR_IPC_STORAGE_PATH, app_type);
    
    bool running = storage_file_exists(ipc_storage, furi_string_get_cstr(app_file_path));
    
    furi_string_free(app_file_path);
    return running;
}

bool predator_ipc_stop_app(PredatorAppType app_type) {
    return predator_ipc_update_status(app_type, PredatorAppStatusIdle, "Stopped via IPC");
}

// Professional Shared Configuration
bool predator_ipc_set_shared_config(const PredatorSharedConfig* config) {
    if(!ipc_storage || !ipc_mutex || !config) return false;
    
    furi_mutex_acquire(ipc_mutex, FuriWaitForever);
    
    Stream* stream = file_stream_alloc(ipc_storage);
    bool success = file_stream_open(stream, PREDATOR_IPC_SHARED_CONFIG, FSAM_WRITE, FSOM_CREATE_ALWAYS);
    
    if(success) {
        stream_write_format(stream, "{\n");
        stream_write_format(stream, "  \"region\": %lu,\n", config->region);
        stream_write_format(stream, "  \"vip_mode_global\": %s,\n", config->vip_mode_global ? "true" : "false");
        stream_write_format(stream, "  \"government_mode_global\": %s,\n", config->government_mode_global ? "true" : "false");
        stream_write_format(stream, "  \"emergency_mode_global\": %s,\n", config->emergency_mode_global ? "true" : "false");
        stream_write_format(stream, "  \"operator_name\": \"%s\",\n", config->operator_name);
        stream_write_format(stream, "  \"organization\": \"%s\",\n", config->organization);
        stream_write_format(stream, "  \"contract_id\": \"%s\",\n", config->contract_id);
        stream_write_format(stream, "  \"license_level\": %lu\n", config->license_level);
        stream_write_format(stream, "}\n");
        file_stream_close(stream);
    }
    
    stream_free(stream);
    furi_mutex_release(ipc_mutex);
    return success;
}

bool predator_ipc_get_shared_config(PredatorSharedConfig* config) {
    if(!ipc_storage || !config) return false;
    
    // Set default configuration
    memset(config, 0, sizeof(PredatorSharedConfig));
    config->region = 1; // Default region
    config->vip_mode_global = false;
    config->government_mode_global = false;
    config->emergency_mode_global = false;
    config->board_type = 1;
    config->esp32_enabled = true;
    config->subghz_enabled = true;
    config->nfc_enabled = true;
    config->max_power_level = 20;
    config->default_frequency = 433920000;
    config->attack_timeout = 30000;
    config->logging_enabled = true;
    config->license_level = 3; // Professional level
    
    strncpy(config->operator_name, "Professional User", sizeof(config->operator_name) - 1);
    strncpy(config->organization, "Predator Security", sizeof(config->organization) - 1);
    strncpy(config->contract_id, "PROF-2024", sizeof(config->contract_id) - 1);
    
    return true;
}
