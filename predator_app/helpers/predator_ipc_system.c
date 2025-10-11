#include "predator_ipc_system.h"
#include <furi.h>
#include <storage/storage.h>

// PROFESSIONAL IPC SYSTEM FOR MULTI-APP COMMUNICATION
// Enables Master Orchestrator to control specialist apps

#define IPC_STORAGE_PATH "/ext/predator_ipc"
#define IPC_COMMAND_FILE "/ext/predator_ipc/command.txt"
#define IPC_STATUS_FILE "/ext/predator_ipc/status.txt"
#define IPC_RESPONSE_FILE "/ext/predator_ipc/response.txt"

typedef struct {
    Storage* storage;
    FuriMutex* mutex;
    bool initialized;
} PredatorIPC;

static PredatorIPC* ipc_instance = NULL;

// Initialize IPC system
bool predator_ipc_init(void) {
    if(ipc_instance) return true;
    
    ipc_instance = malloc(sizeof(PredatorIPC));
    if(!ipc_instance) return false;
    
    ipc_instance->storage = furi_record_open(RECORD_STORAGE);
    ipc_instance->mutex = furi_mutex_alloc(FuriMutexTypeNormal);
    ipc_instance->initialized = false;
    
    if(!ipc_instance->storage || !ipc_instance->mutex) {
        predator_ipc_deinit();
        return false;
    }
    
    // Create IPC directory
    storage_common_mkdir(ipc_instance->storage, IPC_STORAGE_PATH);
    
    ipc_instance->initialized = true;
    FURI_LOG_I("PredatorIPC", "IPC system initialized");
    return true;
}

// Deinitialize IPC system
void predator_ipc_deinit(void) {
    if(!ipc_instance) return;
    
    if(ipc_instance->storage) {
        furi_record_close(RECORD_STORAGE);
    }
    
    if(ipc_instance->mutex) {
        furi_mutex_free(ipc_instance->mutex);
    }
    
    free(ipc_instance);
    ipc_instance = NULL;
    
    FURI_LOG_I("PredatorIPC", "IPC system deinitialized");
}

// Send command to specialist app
bool predator_ipc_send_command(PredatorIPCCommand command, const char* app_name, const char* params) {
    if(!ipc_instance || !ipc_instance->initialized) return false;
    
    furi_mutex_acquire(ipc_instance->mutex, FuriWaitForever);
    
    File* file = storage_file_alloc(ipc_instance->storage);
    bool success = false;
    
    if(storage_file_open(file, IPC_COMMAND_FILE, FSAM_WRITE, FSOM_CREATE_ALWAYS)) {
        FuriString* command_str = furi_string_alloc();
        
        // Format: COMMAND:APP_NAME:PARAMS
        furi_string_printf(command_str, "%d:%s:%s\n", 
                          (int)command, 
                          app_name ? app_name : "", 
                          params ? params : "");
        
        size_t bytes_written = storage_file_write(file, 
                                                 furi_string_get_cstr(command_str), 
                                                 furi_string_size(command_str));
        
        success = (bytes_written == furi_string_size(command_str));
        
        furi_string_free(command_str);
        storage_file_close(file);
        
        FURI_LOG_I("PredatorIPC", "Command sent: %d to %s", (int)command, app_name);
    }
    
    storage_file_free(file);
    furi_mutex_release(ipc_instance->mutex);
    
    return success;
}

// Read command from IPC (for specialist apps)
bool predator_ipc_read_command(PredatorIPCCommand* command, char* app_name, char* params, size_t max_len) {
    if(!ipc_instance || !ipc_instance->initialized) return false;
    if(!command || !app_name || !params) return false;
    
    furi_mutex_acquire(ipc_instance->mutex, FuriWaitForever);
    
    File* file = storage_file_alloc(ipc_instance->storage);
    bool success = false;
    
    if(storage_file_open(file, IPC_COMMAND_FILE, FSAM_READ, FSOM_OPEN_EXISTING)) {
        FuriString* command_str = furi_string_alloc();
        
        if(storage_file_read_to_string(file, command_str)) {
            // Parse: COMMAND:APP_NAME:PARAMS
            const char* str = furi_string_get_cstr(command_str);
            int cmd_int;
            
            if(sscanf(str, "%d:%[^:]:%[^\n]", &cmd_int, app_name, params) >= 2) {
                *command = (PredatorIPCCommand)cmd_int;
                success = true;
                
                // Clear command file after reading
                storage_file_close(file);
                storage_common_remove(ipc_instance->storage, IPC_COMMAND_FILE);
                
                FURI_LOG_I("PredatorIPC", "Command read: %d for %s", cmd_int, app_name);
            }
        }
        
        furi_string_free(command_str);
        if(!success) storage_file_close(file);
    }
    
    storage_file_free(file);
    furi_mutex_release(ipc_instance->mutex);
    
    return success;
}

// Send response from specialist app
bool predator_ipc_send_response(PredatorIPCResponse response, const char* data) {
    if(!ipc_instance || !ipc_instance->initialized) return false;
    
    furi_mutex_acquire(ipc_instance->mutex, FuriWaitForever);
    
    File* file = storage_file_alloc(ipc_instance->storage);
    bool success = false;
    
    if(storage_file_open(file, IPC_RESPONSE_FILE, FSAM_WRITE, FSOM_CREATE_ALWAYS)) {
        FuriString* response_str = furi_string_alloc();
        
        // Format: RESPONSE:DATA
        furi_string_printf(response_str, "%d:%s\n", 
                          (int)response, 
                          data ? data : "");
        
        size_t bytes_written = storage_file_write(file, 
                                                 furi_string_get_cstr(response_str), 
                                                 furi_string_size(response_str));
        
        success = (bytes_written == furi_string_size(response_str));
        
        furi_string_free(response_str);
        storage_file_close(file);
        
        FURI_LOG_I("PredatorIPC", "Response sent: %d", (int)response);
    }
    
    storage_file_free(file);
    furi_mutex_release(ipc_instance->mutex);
    
    return success;
}

// Read response (for master orchestrator)
bool predator_ipc_read_response(PredatorIPCResponse* response, char* data, size_t max_len) {
    if(!ipc_instance || !ipc_instance->initialized) return false;
    if(!response || !data) return false;
    
    furi_mutex_acquire(ipc_instance->mutex, FuriWaitForever);
    
    File* file = storage_file_alloc(ipc_instance->storage);
    bool success = false;
    
    if(storage_file_open(file, IPC_RESPONSE_FILE, FSAM_READ, FSOM_OPEN_EXISTING)) {
        FuriString* response_str = furi_string_alloc();
        
        if(storage_file_read_to_string(file, response_str)) {
            // Parse: RESPONSE:DATA
            const char* str = furi_string_get_cstr(response_str);
            int resp_int;
            
            if(sscanf(str, "%d:%[^\n]", &resp_int, data) >= 1) {
                *response = (PredatorIPCResponse)resp_int;
                success = true;
                
                // Clear response file after reading
                storage_file_close(file);
                storage_common_remove(ipc_instance->storage, IPC_RESPONSE_FILE);
                
                FURI_LOG_I("PredatorIPC", "Response read: %d", resp_int);
            }
        }
        
        furi_string_free(response_str);
        if(!success) storage_file_close(file);
    }
    
    storage_file_free(file);
    furi_mutex_release(ipc_instance->mutex);
    
    return success;
}

// Launch specialist app
bool predator_ipc_launch_app(const char* app_name) {
    if(!app_name) return false;
    
    // Send launch command
    bool success = predator_ipc_send_command(PredatorIPCCommandLaunch, app_name, "");
    
    if(success) {
        FURI_LOG_I("PredatorIPC", "Launch request sent for: %s", app_name);
    }
    
    return success;
}

// Check if command is available
bool predator_ipc_has_command(void) {
    if(!ipc_instance || !ipc_instance->initialized) return false;
    
    return storage_common_exists(ipc_instance->storage, IPC_COMMAND_FILE);
}

// Check if response is available
bool predator_ipc_has_response(void) {
    if(!ipc_instance || !ipc_instance->initialized) return false;
    
    return storage_common_exists(ipc_instance->storage, IPC_RESPONSE_FILE);
}
