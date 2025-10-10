#include "../predator_i.h"
#include "predator_scene.h"
#include "predator_submenu_index.h"

// Cryptography Tools Menu
static void crypto_tools_submenu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    if(!app || !app->view_dispatcher) return;
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void predator_scene_crypto_tools_ui_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app || !app->submenu) return;
    
    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "Crypto Tools");
    
    submenu_add_item(app->submenu, "Entropy Analysis", SubmenuIndexEntropyAnalysis, crypto_tools_submenu_callback, app);
    submenu_add_item(app->submenu, "Frequency Analysis", SubmenuIndexFreqAnalysis, crypto_tools_submenu_callback, app);
    submenu_add_item(app->submenu, "XOR Key Detect", SubmenuIndexXORDetect, crypto_tools_submenu_callback, app);
    submenu_add_item(app->submenu, "Brute Force", SubmenuIndexBruteForce, crypto_tools_submenu_callback, app);
    submenu_add_item(app->submenu, "Dictionary Attack", SubmenuIndexDictAttack, crypto_tools_submenu_callback, app);
    submenu_add_item(app->submenu, "Hash Calculator", SubmenuIndexHashCalc, crypto_tools_submenu_callback, app);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

bool predator_scene_crypto_tools_ui_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(event.type == SceneManagerEventTypeCustom) {
        consumed = true;
        FURI_LOG_I("CryptoTools", "Selected: %lu", event.event);
    }
    
    UNUSED(app);
    return consumed;
}

void predator_scene_crypto_tools_ui_on_exit(void* context) {
    PredatorApp* app = context;
    if(app && app->submenu) {
        submenu_reset(app->submenu);
    }
}
