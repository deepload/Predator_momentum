#include "predator_logging.h"
#include "../predator_i.h"
#include <storage/storage.h>
#include <furi.h>
#include <string.h>

#define LOG_DIR  "/ext/apps_data/predator"
#define LOG_PATH "/ext/apps_data/predator/predator_logs.txt"

bool predator_log_append(struct PredatorApp* app, const char* line) {
    if(!line) return false;
    Storage* storage = app && app->storage ? app->storage : furi_record_open(RECORD_STORAGE);
    bool close_storage = (!app || storage != app->storage);

    File* file = storage_file_alloc(storage);
    bool ok = false;
    // Ensure directory exists (ignore error if already exists)
    storage_common_mkdir(storage, LOG_DIR);
    if(storage_file_open(file, LOG_PATH, FSAM_WRITE, FSOM_OPEN_ALWAYS)) {
        storage_file_seek(file, 0, true); // seek to end
        storage_file_write(file, line, strlen(line));
        storage_file_write(file, "\n", 1);
        storage_file_close(file);
        ok = true;
    }
    storage_file_free(file);
    if(close_storage) furi_record_close(RECORD_STORAGE);
    return ok;
}
