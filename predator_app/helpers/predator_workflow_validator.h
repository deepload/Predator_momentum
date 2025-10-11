#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct PredatorApp PredatorApp;

// Professional workflow validation
bool predator_workflow_validate_scene_transition(PredatorApp* app, uint32_t from_scene, uint32_t to_scene);
bool predator_workflow_validate_submenu_callback(PredatorApp* app, uint32_t index);
bool predator_workflow_validate_back_navigation(PredatorApp* app);

// Professional error handling
void predator_workflow_show_error(PredatorApp* app, const char* title, const char* message);
void predator_workflow_show_success(PredatorApp* app, const char* title, const char* message);
void predator_workflow_show_warning(PredatorApp* app, const char* title, const char* message);

// Professional navigation helpers
bool predator_workflow_safe_scene_transition(PredatorApp* app, uint32_t scene_id);
void predator_workflow_add_breadcrumb(PredatorApp* app, const char* scene_name);

// Workflow coherence checks
typedef enum {
    WORKFLOW_CHECK_NAVIGATION = 0,
    WORKFLOW_CHECK_CALLBACKS,
    WORKFLOW_CHECK_MEMORY,
    WORKFLOW_CHECK_ERROR_HANDLING,
    WORKFLOW_CHECK_COUNT
} WorkflowCheckType;

bool predator_workflow_run_coherence_check(PredatorApp* app, WorkflowCheckType check_type);
