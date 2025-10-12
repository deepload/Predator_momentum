#include "../predator_i.h"
#include "predator_scene.h"
#include "predator_submenu_index.h"
#include "../helpers/predator_workflow_validator.h"

// USB SPECIALIST MAIN MENU - ONLY USB/HID FEATURES
// Professional UI for USB security testing

static void usb_main_menu_callback(void* context, uint32_t index) {
    PredatorApp* app = context;
    if(!app || !app->view_dispatcher) return;
    
    predator_workflow_add_breadcrumb(app, "USB Menu Selection");
    view_dispatcher_send_custom_event(app->view_dispatcher, index);
}

void predator_scene_usb_main_menu_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app || !app->submenu) return;
    
    submenu_reset(app->submenu);
    submenu_set_header(app->submenu, "💾 USB SPECIALIST");
    
    // 💾 USB ATTACK TOOLS
    submenu_add_item(app->submenu, "💾 USB Attacks", SubmenuIndexUSBAttacks, usb_main_menu_callback, app);
    submenu_add_item(app->submenu, "🦆 BadUSB", SubmenuIndexBadUSB, usb_main_menu_callback, app);
    submenu_add_item(app->submenu, "⌨️ HID Keyboard", SubmenuIndexHIDKeyboard, usb_main_menu_callback, app);
    submenu_add_item(app->submenu, "🖱️ HID Mouse", SubmenuIndexHIDMouse, usb_main_menu_callback, app);
    submenu_add_item(app->submenu, "🦆 Rubber Ducky", SubmenuIndexRubberDucky, usb_main_menu_callback, app);
    
    // 🔧 USB SYSTEM TOOLS
    submenu_add_item(app->submenu, "📊 Module Status", SubmenuIndexModuleStatus, usb_main_menu_callback, app);
    submenu_add_item(app->submenu, "🔧 Board Selection", SubmenuIndexBoardSelection, usb_main_menu_callback, app);
    submenu_add_item(app->submenu, "⚙️ Settings", SubmenuIndexSettings, usb_main_menu_callback, app);
    submenu_add_item(app->submenu, "📖 About", SubmenuIndexAbout, usb_main_menu_callback, app);
    
    // 🔙 RETURN TO MASTER
    submenu_add_item(app->submenu, "🔙 Return to Master", SubmenuIndexReturnToMaster, usb_main_menu_callback, app);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewSubmenu);
}

bool predator_scene_usb_main_menu_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(event.type == SceneManagerEventTypeCustom) {
        switch(event.event) {
            case SubmenuIndexUSBAttacks:
                scene_manager_next_scene(app->scene_manager, PredatorSceneUSBAttacksUI);
                consumed = true;
                break;
            case SubmenuIndexBadUSB:
                scene_manager_next_scene(app->scene_manager, PredatorSceneBadUSBUI);
                consumed = true;
                break;
            case SubmenuIndexHIDKeyboard:
                scene_manager_next_scene(app->scene_manager, PredatorSceneHIDKeyboardUI);
                consumed = true;
                break;
            case SubmenuIndexHIDMouse:
                scene_manager_next_scene(app->scene_manager, PredatorSceneHIDMouseUI);
                consumed = true;
                break;
            case SubmenuIndexRubberDucky:
                scene_manager_next_scene(app->scene_manager, PredatorSceneRubberDuckyUI);
                consumed = true;
                break;
            case SubmenuIndexModuleStatus:
                scene_manager_next_scene(app->scene_manager, PredatorSceneModuleStatusUI);
                consumed = true;
                break;
            case SubmenuIndexBoardSelection:
                scene_manager_next_scene(app->scene_manager, PredatorSceneBoardSelectionUI);
                consumed = true;
                break;
            case SubmenuIndexSettings:
                scene_manager_next_scene(app->scene_manager, PredatorSceneSettingsUI);
                consumed = true;
                break;
            case SubmenuIndexAbout:
                scene_manager_next_scene(app->scene_manager, PredatorSceneAboutUI);
                consumed = true;
                break;
            case SubmenuIndexReturnToMaster:
                // IPC call to return to master orchestrator
                consumed = true;
                break;
        }
    }
    
    return consumed;
}

void predator_scene_usb_main_menu_on_exit(void* context) {
    PredatorApp* app = context;
    if(!app || !app->submenu) return;
    submenu_reset(app->submenu);
}
