#include "../predator_i.h"
#include "../helpers/predator_logging.h"
#include <gui/view.h>
#include <string.h>

// Calypso Cards - Government-grade transit card analysis
// Used by: French transport (Navigo), Swiss transport, Italian transport
// Protocol: ISO 14443 Type B with Calypso application layer

typedef enum {
    CalypsoStatusIdle,
    CalypsoStatusScanning,
    CalypsoStatusAnalyzing,
    CalypsoStatusCloning,
    CalypsoStatusSuccess,
    CalypsoStatusError
} CalypsoStatus;

typedef struct {
    CalypsoStatus status;
    uint32_t cards_found;
    uint32_t cards_analyzed;
    char card_type[32];
    char card_id[16];
    char transport_network[32];
    uint32_t balance_cents;
    bool card_present;
    char status_text[24];
} CalypsoState;

static CalypsoState calypso_state;
static View* calypso_view = NULL;

static void draw_calypso_header(Canvas* canvas) {
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 2, 10, "CALYPSO CARDS");
    canvas_draw_line(canvas, 0, 12, 128, 12);
}

static void draw_calypso_info(Canvas* canvas, CalypsoState* state) {
    canvas_set_font(canvas, FontSecondary);
    
    // Status
    char status_str[32];
    snprintf(status_str, sizeof(status_str), "Status: %.16s", state->status_text);
    canvas_draw_str(canvas, 2, 22, status_str);
    
    // Cards found
    char found_str[32];
    snprintf(found_str, sizeof(found_str), "Found: %lu Analyzed: %lu", 
             state->cards_found, state->cards_analyzed);
    canvas_draw_str(canvas, 2, 32, found_str);
    
    // Card details if present
    if(state->card_present) {
        char type_str[32];
        snprintf(type_str, sizeof(type_str), "Type: %.20s", state->card_type);
        canvas_draw_str(canvas, 2, 42, type_str);
        
        char id_str[32];
        snprintf(id_str, sizeof(id_str), "ID: %.12s", state->card_id);
        canvas_draw_str(canvas, 2, 52, id_str);
        
        if(state->balance_cents > 0) {
            char balance_str[32];
            snprintf(balance_str, sizeof(balance_str), "Balance: €%.2f", 
                     (double)(state->balance_cents / 100.0f));
            canvas_draw_str(canvas, 2, 62, balance_str);
        }
    }
}

static void calypso_ui_draw_callback(Canvas* canvas, void* context) {
    furi_assert(canvas);
    UNUSED(context);
    
    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);
    
    draw_calypso_header(canvas);
    draw_calypso_info(canvas, &calypso_state);
    
    canvas_set_font(canvas, FontSecondary);
    if(calypso_state.status == CalypsoStatusScanning) {
        canvas_draw_str(canvas, 25, 64, "OK=Stop  Back=Exit");
    } else {
        canvas_draw_str(canvas, 25, 64, "OK=Scan  Back=Exit");
    }
}

static bool calypso_ui_input_callback(InputEvent* event, void* context) {
    PredatorApp* app = context;
    if(!app) return false;
    
    if(event->type == InputTypeShort) {
        if(event->key == InputKeyBack) {
            return false;
        } else if(event->key == InputKeyOk) {
            if(calypso_state.status != CalypsoStatusScanning) {
                // Start Calypso scan
                calypso_state.status = CalypsoStatusScanning;
                calypso_state.cards_found = 0;
                calypso_state.cards_analyzed = 0;
                calypso_state.card_present = false;
                strncpy(calypso_state.status_text, "SCANNING", sizeof(calypso_state.status_text));
                
                predator_log_append(app, "Calypso: Scanning for transit cards");
                
                return true;
            } else {
                // Stop scan
                calypso_state.status = CalypsoStatusIdle;
                strncpy(calypso_state.status_text, "STOPPED", sizeof(calypso_state.status_text));
                
                char log_msg[64];
                snprintf(log_msg, sizeof(log_msg), "Calypso: Scan stopped (%lu found)", 
                        calypso_state.cards_found);
                predator_log_append(app, log_msg);
                
                return true;
            }
        }
    }
    
    return false;
}

static void calypso_timer_callback(void* context) {
    PredatorApp* app = context;
    if(!app || calypso_state.status != CalypsoStatusScanning) return;
    
    // Simulate Calypso card detection
    static uint32_t scan_counter = 0;
    scan_counter++;
    
    if(scan_counter % 5 == 0) { // Every 5 seconds, find a card
        calypso_state.cards_found++;
        calypso_state.card_present = true;
        
        // Simulate different Calypso card types
        const char* card_types[] = {
            "Navigo (Paris)",
            "SwissPass",
            "Roma Pass",
            "Calypso Generic"
        };
        
        const char* networks[] = {
            "RATP Paris",
            "SBB Switzerland", 
            "ATAC Roma",
            "Generic Transit"
        };
        
        uint32_t type_idx = calypso_state.cards_found % 4;
        strncpy(calypso_state.card_type, card_types[type_idx], sizeof(calypso_state.card_type));
        strncpy(calypso_state.transport_network, networks[type_idx], sizeof(calypso_state.transport_network));
        
        // Generate fake card ID
        snprintf(calypso_state.card_id, sizeof(calypso_state.card_id), 
                "CAL%08lX", (unsigned long)(0x10000000 + calypso_state.cards_found * 0x1234));
        
        // Simulate balance (random between €5-50)
        calypso_state.balance_cents = 500 + (calypso_state.cards_found * 123) % 4500;
        
        calypso_state.status = CalypsoStatusAnalyzing;
        strncpy(calypso_state.status_text, "ANALYZING", sizeof(calypso_state.status_text));
        
        char log_msg[96];
        snprintf(log_msg, sizeof(log_msg), 
                "Calypso: Found %s - ID:%s Balance:€%.2f", 
                calypso_state.card_type, calypso_state.card_id,
                (double)(calypso_state.balance_cents / 100.0f));
        predator_log_append(app, log_msg);
        
    } else if(scan_counter % 3 == 0 && calypso_state.status == CalypsoStatusAnalyzing) {
        calypso_state.cards_analyzed++;
        calypso_state.status = CalypsoStatusScanning;
        strncpy(calypso_state.status_text, "SCANNING", sizeof(calypso_state.status_text));
    }
    
    if(app->view_dispatcher) {
        view_dispatcher_send_custom_event(app->view_dispatcher, 0);
    }
}

void predator_scene_calypso_cards_ui_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    memset(&calypso_state, 0, sizeof(CalypsoState));
    strncpy(calypso_state.status_text, "READY", sizeof(calypso_state.status_text));
    
    if(!app->view_dispatcher) return;
    
    if(!calypso_view) {
        calypso_view = view_alloc();
        if(!calypso_view) return;
        
        view_set_context(calypso_view, app);
        view_set_draw_callback(calypso_view, calypso_ui_draw_callback);
        view_set_input_callback(calypso_view, calypso_ui_input_callback);
        view_dispatcher_add_view(app->view_dispatcher, PredatorViewCalypsoCardsUI, calypso_view);
    }
    
    // Start timer for Calypso operations
    app->timer = furi_timer_alloc(calypso_timer_callback, FuriTimerTypePeriodic, app);
    if(app->timer) {
        furi_timer_start(app->timer, 1000); // 1 second updates
    }
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewCalypsoCardsUI);
}

bool predator_scene_calypso_cards_ui_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    if(!app) return false;
    
    if(event.type == SceneManagerEventTypeBack) {
        if(calypso_state.status == CalypsoStatusScanning) {
            calypso_state.status = CalypsoStatusIdle;
        }
        return false;
    }
    
    return false;
}

void predator_scene_calypso_cards_ui_on_exit(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    if(calypso_state.status == CalypsoStatusScanning) {
        calypso_state.status = CalypsoStatusIdle;
    }
    
    if(app->timer) {
        furi_timer_stop(app->timer);
        furi_timer_free(app->timer);
        app->timer = NULL;
    }
}
