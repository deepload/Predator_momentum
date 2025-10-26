#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Aggressive scene cleanup - resets all static variables in scenes
 * This prevents memory fragmentation between app runs by forcing
 * all scene static variables and views to be reallocated fresh
 */
void predator_scene_cleanup_all_statics(void);

#ifdef __cplusplus
}
#endif
