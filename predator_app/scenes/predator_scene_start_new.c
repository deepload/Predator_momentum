#include "../predator_i.h"
#include "predator_scene.h"
#include <furi.h>

void predator_scene_start_new_on_enter(void* context) {
    PredatorApp* app = context;
    
    if(!app) {
        FURI_LOG_E("StartMenu", "App context is NULL on enter");
        return;
    }
    
    if(!app->scene_manager) {
        FURI_LOG_E("StartMenu", "Scene manager is NULL, cannot proceed");
        return;
    }
    
    FURI_LOG_I("StartMenu", "Main menu initialized");
}

bool predator_scene_start_new_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(!app) {
        FURI_LOG_E("StartMenu", "App context is NULL in event handler");
        return false;
    }
    
    return consumed;
}

void predator_scene_start_new_on_exit(void* context) {
    PredatorApp* app = context;
    
    if(!app) {
        FURI_LOG_E("StartMenu", "App context is NULL on exit");
        return;
    }
    
    FURI_LOG_I("StartMenu", "Exited main menu scene");
}
