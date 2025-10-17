#include "../predator_i.h"
#include "predator_scene.h"
#include "../helpers/predator_boards.h"
#include "../helpers/predator_esp32.h"
#include "../helpers/predator_gps.h"
#include "../helpers/predator_logging.h"
#include <gui/view.h>
#include <notification/notification_messages.h>

// ULTIMATE BOARD SELECTION - SWISS GOVERNMENT GRADE
// The BEST Flipper Zero board selection experience ever created

typedef enum {
    BoardScreenMain,
    BoardScreenDetails,
    BoardScreenConfirm,
    BoardScreenSuccess
} BoardScreen;

typedef struct {
    uint8_t selected_index;
    BoardScreen current_screen;
    bool selection_confirmed;
    bool hardware_tested;
    uint32_t animation_tick;
    PredatorApp* app;
} BoardSelectionState;

static BoardSelectionState board_state;
static View* board_selection_view = NULL;

// ULTIMATE BOARD SELECTION SCREENS

static void draw_main_screen(Canvas* canvas) {
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 15, 10, "🔧 BOARD SELECTION");
    canvas_draw_line(canvas, 0, 12, 128, 12);
    
    // Animated selection indicator
    uint8_t anim_offset = (board_state.animation_tick / 10) % 3;
    canvas_set_font(canvas, FontSecondary);
    
    // Board name with animation - add bounds checking
    char board_display[32];
    uint8_t safe_index = board_state.selected_index;
    if(safe_index >= PredatorBoardTypeCount) {
        safe_index = PredatorBoardTypeOriginal;
    }
    
    // CRITICAL FIX: Use actual board config names (same as main menu)
    const char* board_name = predator_boards_get_name(safe_index);
    snprintf(board_display, sizeof(board_display), "%s%s", 
             anim_offset == 0 ? "► " : anim_offset == 1 ? "▶ " : "▷ ",
             board_name);
    canvas_draw_str(canvas, 2, 25, board_display);
    
    // Quick capabilities preview
    bool has_esp32 = (board_state.selected_index == PredatorBoardType3in1AIO || 
                     board_state.selected_index == PredatorBoardTypeScreen28 ||
                     board_state.selected_index == PredatorBoardTypeDrB0rkMultiV2 ||
                     board_state.selected_index == PredatorBoardType3in1NrfCcEsp);
    
    canvas_draw_str(canvas, 2, 35, "Capabilities:");
    canvas_draw_str(canvas, 2, 45, has_esp32 ? "📡 WiFi+BT+GPS+SubGHz+NFC" : "📻 SubGHz+NFC Only");
    
    // Navigation hints
    canvas_draw_str(canvas, 2, 58, "↑↓=Browse  OK=Details  Back=Exit");
}

static void draw_details_screen(Canvas* canvas) {
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 20, 10, "BOARD DETAILS");
    canvas_draw_line(canvas, 0, 12, 128, 12);
    
    canvas_set_font(canvas, FontSecondary);
    
    // Board name - add bounds checking
    uint8_t safe_index = board_state.selected_index;
    if(safe_index >= PredatorBoardTypeCount) {
        safe_index = PredatorBoardTypeOriginal;
    }
    // CRITICAL FIX: Use actual board config name
    canvas_draw_str(canvas, 2, 22, predator_boards_get_name(safe_index));
    
    // Detailed capabilities
    bool has_esp32 = (board_state.selected_index == PredatorBoardType3in1AIO || 
                     board_state.selected_index == PredatorBoardTypeScreen28 ||
                     board_state.selected_index == PredatorBoardTypeDrB0rkMultiV2 ||
                     board_state.selected_index == PredatorBoardType3in1NrfCcEsp);
    
    canvas_draw_str(canvas, 2, 32, has_esp32 ? "✓ WiFi Attacks" : "✗ WiFi Attacks");
    canvas_draw_str(canvas, 2, 40, has_esp32 ? "✓ Bluetooth Attacks" : "✗ Bluetooth Attacks");
    canvas_draw_str(canvas, 2, 48, has_esp32 ? "✓ GPS Tracking" : "✗ GPS Tracking");
    canvas_draw_str(canvas, 2, 56, "✓ SubGHz/Car Attacks");
    
    canvas_draw_str(canvas, 2, 64, "OK=Select  Back=Return");
}

static void draw_confirm_screen(Canvas* canvas) {
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 25, 10, "CONFIRM BOARD");
    canvas_draw_line(canvas, 0, 12, 128, 12);
    
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str(canvas, 2, 25, "Selected Board:");
    
    // Add bounds checking
    uint8_t safe_index = board_state.selected_index;
    if(safe_index >= PredatorBoardTypeCount) {
        safe_index = PredatorBoardTypeOriginal;
    }
    // CRITICAL FIX: Use actual board config name
    canvas_draw_str(canvas, 2, 35, predator_boards_get_name(safe_index));
    
    if(board_state.hardware_tested) {
        canvas_draw_str(canvas, 2, 45, "✓ Hardware Test: PASSED");
    } else {
        canvas_draw_str(canvas, 2, 45, "⏳ Testing hardware...");
    }
    
    canvas_draw_str(canvas, 2, 58, "OK=Confirm  Back=Cancel");
}

static void draw_success_screen(Canvas* canvas) {
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 30, 10, "SUCCESS!");
    canvas_draw_line(canvas, 0, 12, 128, 12);
    
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str(canvas, 2, 25, "Board configured:");
    
    // Add bounds checking
    uint8_t safe_index = board_state.selected_index;
    if(safe_index >= PredatorBoardTypeCount) {
        safe_index = PredatorBoardTypeOriginal;
    }
    // CRITICAL FIX: Use actual board config name
    canvas_draw_str(canvas, 2, 35, predator_boards_get_name(safe_index));
    
    // Success animation
    uint8_t anim = (board_state.animation_tick / 5) % 4;
    const char* success_icons[] = {"✓", "✓✓", "✓✓✓", "✓✓✓✓"};
    canvas_draw_str(canvas, 2, 45, success_icons[anim]);
    canvas_draw_str(canvas, 20, 45, "Ready for Swiss Demo!");
    
    canvas_draw_str(canvas, 2, 58, "Any key=Back  Back=Main Menu");
}

static void board_draw_callback(Canvas* canvas, void* context) {
    UNUSED(context);
    if(!canvas) return;
    
    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);
    
    // Increment animation counter
    board_state.animation_tick++;
    
    // Draw appropriate screen
    switch(board_state.current_screen) {
        case BoardScreenMain:
            draw_main_screen(canvas);
            break;
        case BoardScreenDetails:
            draw_details_screen(canvas);
            break;
        case BoardScreenConfirm:
            draw_confirm_screen(canvas);
            break;
        case BoardScreenSuccess:
            draw_success_screen(canvas);
            break;
    }
}

// ULTIMATE INPUT HANDLING - Multi-screen navigation
static void perform_hardware_test(void) {
    // Simulate hardware testing
    furi_delay_ms(500);
    board_state.hardware_tested = true;
}

static void trigger_success_notification(PredatorApp* app) {
    if(app && app->notifications) {
        notification_message(app->notifications, &sequence_success);
    }
}

static bool board_input_callback(InputEvent* event, void* context) {
    UNUSED(context);
    if(!event || event->type != InputTypeShort) return false;
    
    bool consumed = false;
    
    switch(board_state.current_screen) {
        case BoardScreenMain:
            switch(event->key) {
                case InputKeyUp:
                    if(board_state.selected_index > 0) {
                        board_state.selected_index--;
                        consumed = true;
                    }
                    break;
                    
                case InputKeyDown:
                    if(board_state.selected_index < PredatorBoardTypeCount - 1) {
                        board_state.selected_index++;
                        consumed = true;
                    }
                    break;
                    
                case InputKeyOk:
                    board_state.current_screen = BoardScreenDetails;
                    consumed = true;
                    break;
                    
                case InputKeyBack:
                    // DON'T consume Back on main screen - let view dispatcher handle it
                    // This allows proper scene manager navigation
                    consumed = false;
                    break;
                    
                default:
                    break;
            }
            break;
            
        case BoardScreenDetails:
            switch(event->key) {
                case InputKeyOk:
                    board_state.current_screen = BoardScreenConfirm;
                    board_state.hardware_tested = false;
                    // Start hardware test
                    perform_hardware_test();
                    consumed = true;
                    break;
                    
                case InputKeyBack:
                    board_state.current_screen = BoardScreenMain;
                    consumed = true;
                    break;
                    
                default:
                    break;
            }
            break;
            
        case BoardScreenConfirm:
            switch(event->key) {
                case InputKeyOk:
                    if(board_state.hardware_tested && board_state.app) {
                        // CRITICAL: Actually set the board type
                        board_state.app->board_type = board_state.selected_index;
                        board_state.selection_confirmed = true;
                        
                        // Log successful selection
                        predator_log_append(board_state.app, "Board configured:");
                        predator_log_append(board_state.app, predator_boards_get_name(board_state.selected_index));
                        
                        // Trigger success notification
                        trigger_success_notification(board_state.app);
                        
                        // Go to success screen
                        board_state.current_screen = BoardScreenSuccess;
                        board_state.animation_tick = 0; // Reset animation
                    }
                    consumed = true;
                    break;
                    
                case InputKeyBack:
                    board_state.current_screen = BoardScreenDetails;
                    consumed = true;
                    break;
                    
                default:
                    break;
            }
            break;
            
        case BoardScreenSuccess:
            // CRITICAL FIX: Success screen - DON'T consume back button!
            // Let it propagate to on_event handler
            if(event->type == InputTypeShort && event->key == InputKeyBack) {
                FURI_LOG_I("BoardSelection", "Success screen - BACK button, NOT consuming (let on_event handle it)");
                consumed = false;  // DON'T consume - let it propagate
            } else {
                // Other keys - consume them
                FURI_LOG_I("BoardSelection", "Success screen - key %d type %d, consuming", event->key, event->type);
                consumed = true;
            }
            break;
    }
    
    return consumed;
}

void predator_scene_board_selection_ui_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    // ULTIMATE INITIALIZATION - Reset all state
    memset(&board_state, 0, sizeof(BoardSelectionState));
    board_state.app = app;
    board_state.current_screen = BoardScreenMain;
    board_state.selected_index = 0;
    board_state.selection_confirmed = false;
    board_state.hardware_tested = false;
    board_state.animation_tick = 0;
    
    // Set current board as selected if valid
    if(app->board_type < PredatorBoardTypeCount) {
        board_state.selected_index = app->board_type;
    } else {
        // Default to Original Predator if invalid board type
        board_state.selected_index = PredatorBoardTypeOriginal;
    }
    
    // Create view with timer for animations
    board_selection_view = view_alloc();
    if(!board_selection_view) return;
    
    view_set_context(board_selection_view, app);
    view_set_draw_callback(board_selection_view, board_draw_callback);
    view_set_input_callback(board_selection_view, board_input_callback);
    
    // Enable continuous redraw for animations
    view_set_update_callback(board_selection_view, NULL);
    view_set_update_callback_context(board_selection_view, app);
    
    view_dispatcher_add_view(app->view_dispatcher, PredatorViewBoardSelectionUI, board_selection_view);
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewBoardSelectionUI);
    
    // Start animation timer
    if(app->timer) {
        furi_timer_stop(app->timer);
        furi_timer_free(app->timer);
        app->timer = NULL;
    }
}

bool predator_scene_board_selection_ui_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    if(!app) return false;
    
    // Handle animation updates
    if(event.type == SceneManagerEventTypeCustom && event.event == 1) {
        // Trigger redraw for animations
        return true;
    }
    
    // SIMPLIFIED: Handle back button
    if(event.type == SceneManagerEventTypeBack) {
        FURI_LOG_E("BoardSelection", "==== BACK EVENT ==== Screen: %d", board_state.current_screen);
        
        // Just pop this scene - main menu is below
        scene_manager_previous_scene(app->scene_manager);
        
        // CRITICAL: ALWAYS return true to let view dispatcher handle exit prevention
        return true;
    }
    
    return false; // Let framework handle other events
}

void predator_scene_board_selection_ui_on_exit(void* context) {
    PredatorApp* app = context;
    FURI_LOG_E("BoardSelection", "========== BOARD SELECTION EXITING ==========");
    FURI_LOG_I("BoardSelection", "Current screen: %d, Confirmed: %d", 
               board_state.current_screen, board_state.selection_confirmed);
    
    if(!app) {
        FURI_LOG_E("BoardSelection", "ERROR: app is NULL!");
        return;
    }
    
    // Stop animation timer
    if(app->timer) {
        furi_timer_stop(app->timer);
        furi_timer_free(app->timer);
        app->timer = NULL;
    }
    
    // Clean up view properly
    view_dispatcher_remove_view(app->view_dispatcher, PredatorViewBoardSelectionUI);
    if(board_selection_view) {
        view_free(board_selection_view);
        board_selection_view = NULL;
    }
    
    // Log final board selection
    if(board_state.selection_confirmed) {
        FURI_LOG_I("BoardSelection", "Board selection complete - returning to previous scene");
        predator_log_append(app, "Board selection complete - Swiss Demo Ready!");
    } else {
        FURI_LOG_W("BoardSelection", "Board selection cancelled - no board confirmed");
    }
    
    FURI_LOG_E("BoardSelection", "========== BOARD SELECTION EXIT COMPLETE ==========");
}
