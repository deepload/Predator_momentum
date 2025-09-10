#pragma once

#include <gui/scene_manager.h>

// Scene IDs are already defined in predator_i.h
// Use predefined enum values instead of redefining
#define ADD_SCENE(prefix, name, id)
#include "predator_scene_config.h"
#undef ADD_SCENE

// Explicitly reference RFID scenes to ensure they're recognized
#define PredatorSceneRfidClone 17
#define PredatorSceneRfidBruteforce 18

extern const SceneManagerHandlers predator_scene_handlers;

// Generate scene on_enter handlers declaration
#define ADD_SCENE(prefix, name, id) void prefix##_scene_##name##_on_enter(void*);
#include "predator_scene_config.h"
#undef ADD_SCENE

// Generate scene on_event handlers declaration
#define ADD_SCENE(prefix, name, id) \
    bool prefix##_scene_##name##_on_event(void* context, SceneManagerEvent event);
#include "predator_scene_config.h"
#undef ADD_SCENE

// Generate scene on_exit handlers declaration
#define ADD_SCENE(prefix, name, id) void prefix##_scene_##name##_on_exit(void* context);
#include "predator_scene_config.h"
#undef ADD_SCENE
