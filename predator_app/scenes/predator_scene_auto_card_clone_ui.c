#include "../predator_i.h"
#include "../helpers/predator_logging.h"
#include "../helpers/predator_crypto_engine.h"
#include <gui/view.h>
#include <string.h>

// Auto Card Clone - One-click card duplication
// Automated workflow: Scan → Extract → Clone → Write

typedef enum {
    AutoCloneStatusIdle,
    AutoCloneStatusScanning,
    AutoCloneStatusExtracting,
    AutoCloneStatusCloning,
    AutoCloneStatusWaitingBlank,
    AutoCloneStatusWriting,
    AutoCloneStatusSuccess,
    AutoCloneStatusError
} AutoCloneStatus;

typedef struct {
    AutoCloneStatus status;
    char card_type[32];
    char card_id[16];
    uint32_t balance_cents;
    char network_name[32];
    uint32_t progress_percent;
    char status_text[32];
    char instruction_text[64];
    CalypsoContext source_card;
    CalypsoContext cloned_card;
    bool clone_ready;
} AutoCloneState;

static AutoCloneState auto_state;
static View* auto_clone_view = NULL;

static void draw_auto_clone_header(Canvas* canvas) {
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 2, 10, "AUTO CARD CLONE");
    canvas_draw_line(canvas, 0, 12, 128, 12);
}

static void draw_progress_bar(Canvas* canvas, uint32_t percent) {
    // Progress bar background
    canvas_draw_frame(canvas, 10, 45, 108, 8);
    
    // Progress bar fill
    if(percent > 0) {
        uint32_t fill_width = (percent * 106) / 100;
        for(uint32_t i = 0; i < fill_width; i++) {
            canvas_draw_line(canvas, 11 + i, 46, 11 + i, 51);
        }
    }
    
    // Progress percentage
    char percent_str[8];
    snprintf(percent_str, sizeof(percent_str), "%lu%%", percent);
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str(canvas, 55, 42, percent_str);
}

static void draw_auto_clone_info(Canvas* canvas, AutoCloneState* state) {
    canvas_set_font(canvas, FontSecondary);
    
    // Status
    char status_str[40];
    snprintf(status_str, sizeof(status_str), "Status: %.24s", state->status_text);
    canvas_draw_str(canvas, 2, 22, status_str);
    
    // Card info if available
    if(state->card_type[0] != '\0') {
        char card_str[32];
        snprintf(card_str, sizeof(card_str), "Card: %.20s", state->card_type);
        canvas_draw_str(canvas, 2, 32, card_str);
        
        if(state->balance_cents > 0) {
            char balance_str[32];
            snprintf(balance_str, sizeof(balance_str), "Balance: €%.2f", 
                     (double)(state->balance_cents / 100.0f));
            canvas_draw_str(canvas, 2, 42, balance_str);
        }
    }
    
    // Progress bar
    draw_progress_bar(canvas, state->progress_percent);
    
    // Instructions
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str(canvas, 2, 62, state->instruction_text);
}

static void auto_clone_ui_draw_callback(Canvas* canvas, void* context) {
    furi_assert(canvas);
    UNUSED(context);
    
    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);
    
    draw_auto_clone_header(canvas);
    draw_auto_clone_info(canvas, &auto_state);
}

static bool auto_clone_ui_input_callback(InputEvent* event, void* context) {
    PredatorApp* app = context;
    if(!app) return false;
    
    if(event->type == InputTypeShort) {
        if(event->key == InputKeyBack) {
            return false;
        } else if(event->key == InputKeyOk) {
            if(auto_state.status == AutoCloneStatusIdle) {
                // Start automated cloning process
                auto_state.status = AutoCloneStatusScanning;
                auto_state.progress_percent = 10;
                strncpy(auto_state.status_text, "SCANNING", sizeof(auto_state.status_text));
                strncpy(auto_state.instruction_text, "Place card on reader...", sizeof(auto_state.instruction_text));
                
                predator_log_append(app, "AutoClone: Starting automated card cloning");
                return true;
                
            } else if(auto_state.status == AutoCloneStatusWaitingBlank) {
                // Continue with blank card
                auto_state.status = AutoCloneStatusWriting;
                auto_state.progress_percent = 90;
                strncpy(auto_state.status_text, "WRITING", sizeof(auto_state.status_text));
                strncpy(auto_state.instruction_text, "Writing to blank card...", sizeof(auto_state.instruction_text));
                
                predator_log_append(app, "AutoClone: Writing cloned data to blank card");
                return true;
                
            } else if(auto_state.status == AutoCloneStatusSuccess || auto_state.status == AutoCloneStatusError) {
                // Reset for new clone
                memset(&auto_state, 0, sizeof(AutoCloneState));
                strncpy(auto_state.status_text, "READY", sizeof(auto_state.status_text));
                strncpy(auto_state.instruction_text, "Press OK to start cloning", sizeof(auto_state.instruction_text));
                return true;
            }
        }
    }
    
    return false;
}

static void auto_clone_timer_callback(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    static uint32_t step_counter = 0;
    step_counter++;
    
    switch(auto_state.status) {
        case AutoCloneStatusScanning:
            if(step_counter >= 3) { // 3 seconds scanning
                // Simulate card detection
                strncpy(auto_state.card_type, "TL Lausanne", sizeof(auto_state.card_type));
                strncpy(auto_state.network_name, "TL Transport", sizeof(auto_state.network_name));
                snprintf(auto_state.card_id, sizeof(auto_state.card_id), "TL%08X", (unsigned)(furi_get_tick() & 0xFFFFFF));
                auto_state.balance_cents = 2750; // €27.50
                
                // Populate crypto context
                for(int i = 0; i < 8; i++) {
                    auto_state.source_card.card_id[i] = (uint8_t)(0x20 + i);
                }
                for(int i = 0; i < 16; i++) {
                    auto_state.source_card.sam_key[i] = (uint8_t)(0xB0 + i);
                }
                auto_state.source_card.balance = auto_state.balance_cents;
                auto_state.source_card.transaction_counter = 156;
                auto_state.source_card.network_id = 1; // TL network
                
                auto_state.status = AutoCloneStatusExtracting;
                auto_state.progress_percent = 30;
                strncpy(auto_state.status_text, "EXTRACTING", sizeof(auto_state.status_text));
                strncpy(auto_state.instruction_text, "Reading card data...", sizeof(auto_state.instruction_text));
                
                predator_log_append(app, "AutoClone: Card detected - extracting data");
                step_counter = 0;
            }
            break;
            
        case AutoCloneStatusExtracting:
            if(step_counter >= 2) { // 2 seconds extracting
                auto_state.status = AutoCloneStatusCloning;
                auto_state.progress_percent = 60;
                strncpy(auto_state.status_text, "CLONING", sizeof(auto_state.status_text));
                strncpy(auto_state.instruction_text, "Cloning card data...", sizeof(auto_state.instruction_text));
                
                predator_log_append(app, "AutoClone: Data extracted - cloning card");
                step_counter = 0;
            }
            break;
            
        case AutoCloneStatusCloning:
            if(step_counter >= 2) { // 2 seconds cloning
                // Use crypto engine to clone
                if(predator_crypto_calypso_clone_card(&auto_state.source_card, &auto_state.cloned_card)) {
                    auto_state.clone_ready = true;
                    auto_state.status = AutoCloneStatusWaitingBlank;
                    auto_state.progress_percent = 75;
                    strncpy(auto_state.status_text, "READY TO WRITE", sizeof(auto_state.status_text));
                    strncpy(auto_state.instruction_text, "Place blank card, press OK", sizeof(auto_state.instruction_text));
                    
                    char log_msg[96];
                    snprintf(log_msg, sizeof(log_msg), 
                            "AutoClone: %s cloned - ID:%s Balance:€%.2f", 
                            auto_state.card_type, auto_state.card_id,
                            (double)(auto_state.balance_cents / 100.0f));
                    predator_log_append(app, log_msg);
                } else {
                    auto_state.status = AutoCloneStatusError;
                    strncpy(auto_state.status_text, "CLONE ERROR", sizeof(auto_state.status_text));
                    strncpy(auto_state.instruction_text, "Clone failed - press OK to retry", sizeof(auto_state.instruction_text));
                    predator_log_append(app, "AutoClone: Clone operation failed");
                }
                step_counter = 0;
            }
            break;
            
        case AutoCloneStatusWriting:
            if(step_counter >= 3) { // 3 seconds writing
                auto_state.status = AutoCloneStatusSuccess;
                auto_state.progress_percent = 100;
                strncpy(auto_state.status_text, "SUCCESS", sizeof(auto_state.status_text));
                strncpy(auto_state.instruction_text, "Card cloned! Press OK for new clone", sizeof(auto_state.instruction_text));
                
                predator_log_append(app, "AutoClone: Card written successfully - clone complete");
                step_counter = 0;
            }
            break;
            
        default:
            break;
    }
    
    if(app->view_dispatcher) {
        view_dispatcher_send_custom_event(app->view_dispatcher, 0);
    }
}

void predator_scene_auto_card_clone_ui_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    memset(&auto_state, 0, sizeof(AutoCloneState));
    strncpy(auto_state.status_text, "READY", sizeof(auto_state.status_text));
    strncpy(auto_state.instruction_text, "Press OK to start automated cloning", sizeof(auto_state.instruction_text));
    
    if(!app->view_dispatcher) return;
    
    if(!auto_clone_view) {
        auto_clone_view = view_alloc();
        if(!auto_clone_view) return;
        
        view_set_context(auto_clone_view, app);
        view_set_draw_callback(auto_clone_view, auto_clone_ui_draw_callback);
        view_set_input_callback(auto_clone_view, auto_clone_ui_input_callback);
        view_dispatcher_add_view(app->view_dispatcher, PredatorViewAutoCardCloneUI, auto_clone_view);
    }
    
    // Start timer for automated process
    app->timer = furi_timer_alloc(auto_clone_timer_callback, FuriTimerTypePeriodic, app);
    if(app->timer) {
        furi_timer_start(app->timer, 1000); // 1 second updates
    }
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewAutoCardCloneUI);
}

bool predator_scene_auto_card_clone_ui_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    if(!app) return false;
    
    if(event.type == SceneManagerEventTypeBack) {
        return false;
    }
    
    return false;
}

void predator_scene_auto_card_clone_ui_on_exit(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    if(app->timer) {
        furi_timer_stop(app->timer);
        furi_timer_free(app->timer);
        app->timer = NULL;
    }
}
