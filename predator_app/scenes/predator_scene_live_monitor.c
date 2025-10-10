#include "../predator_i.h"
#include <furi.h>
#include <gui/elements.h>
#include <storage/storage.h>
#include <string.h>

// Live Monitor using the existing popup view: shows the last few lines of the log

#define LOG_PATH "/ext/apps_data/predator/predator_logs.txt"
#define MAX_LINES 6
#define MAX_LINE_LEN 64

static void live_monitor_refresh_popup(PredatorApp* app) {
    if(!app || !app->popup) return;
    Storage* storage = app->storage ? app->storage : furi_record_open(RECORD_STORAGE);
    bool close_storage = (storage != app->storage);

    File* file = storage_file_alloc(storage);
    char lines[MAX_LINES][MAX_LINE_LEN];
    for(size_t i = 0; i < MAX_LINES; i++) lines[i][0] = '\0';

    if(storage_file_open(file, LOG_PATH, FSAM_READ, FSOM_OPEN_EXISTING)) {
        int32_t size = storage_file_size(file);
        int32_t pos = size;
        int collected = 0;
        char buf[128];
        while(pos > 0 && collected < MAX_LINES) {
            int32_t chunk = (pos >= (int32_t)sizeof(buf)) ? (int32_t)sizeof(buf) : pos;
            pos -= chunk;
            storage_file_seek(file, pos, false);
            uint16_t br = storage_file_read(file, buf, chunk);
            for(int i = (int)br - 1; i >= 0 && collected < MAX_LINES; --i) {
                if(buf[i] == '\n') {
                    int32_t start = pos + i + 1;
                    storage_file_seek(file, start, false);
                    size_t idx = 0; char ch = 0; uint16_t r = 0;
                    while(idx + 1 < MAX_LINE_LEN && (r = storage_file_read(file, &ch, 1)) == 1) {
                        if(ch == '\n' || ch == '\r') break;
                        lines[collected][idx++] = ch;
                    }
                    lines[collected][idx] = '\0';
                    collected++;
                    storage_file_seek(file, pos, false);
                }
            }
        }
        storage_file_close(file);
    }
    storage_file_free(file);
    if(close_storage) furi_record_close(RECORD_STORAGE);

    // Build popup text body (oldest at top)
    char body[256]; body[0] = '\0';
    int write = 0;
    for(int i = MAX_LINES - 1; i >= 0; --i) {
        if(lines[i][0] == '\0') continue;
        write += snprintf(body + write, sizeof(body) - write, "%s\n", lines[i]);
        if(write >= (int)sizeof(body)) break;
    }
    if(body[0] == '\0') {
        snprintf(body, sizeof(body), "No logs yet. Actions will appear here.\nPress Back to return");
    }
    popup_set_header(app->popup, "Live Monitor", 64, 10, AlignCenter, AlignTop);
    popup_set_text(app->popup, body, 64, 28, AlignCenter, AlignTop);
}

void predator_scene_live_monitor_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app || !app->view_dispatcher || !app->popup) return;
    popup_reset(app->popup);
    popup_set_context(app->popup, app);
    popup_set_timeout(app->popup, 0);
    popup_enable_timeout(app->popup);
    // First render
    live_monitor_refresh_popup(app);
    // Start periodic refresh
    app->attack_running = true;
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
}

bool predator_scene_live_monitor_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    if(!app) return false;
    if(event.type == SceneManagerEventTypeBack) {
        app->attack_running = false;
        scene_manager_previous_scene(app->scene_manager);
        return true;
    } else if(event.type == SceneManagerEventTypeTick) {
        if(app->attack_running) {
            live_monitor_refresh_popup(app);
            return true;
        }
    }
    return false;
}

void predator_scene_live_monitor_on_exit(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    app->attack_running = false;
}
