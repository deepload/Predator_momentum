#pragma once

// This file is for debugging the scene enums
#include "predator_scene.h"
#include "../predator_i.h"

// Generate an enumeration of all scene IDs for reference
enum {
#define ADD_SCENE(prefix, name, id) PredatorScene##id##_Val = PredatorScene##id,
#include "predator_scene_config.h"
#undef ADD_SCENE
};
