#include "../predator_i.h"
#include "../helpers/predator_attack_predictor.h"
#include "../helpers/predator_models_hardcoded.h"
#include "../helpers/predator_logging.h"
#include <gui/view.h>
#include <string.h>

// LIVE ATTACK PREDICTION DASHBOARD
// Shows real-time success predictions for all attack types

typedef struct {
    uint8_t selected_attack;
    uint8_t selected_target;
    AttackPrediction current_prediction;
    bool prediction_valid;
    uint32_t last_update_tick;
} PredictorState;

static PredictorState predictor_state = {0};

static const char* attack_names[] = {
    "Car Rolling Code",
    "Car Fixed Code", 
    "Car Smart Key",
    "WiFi Deauth",
    "WiFi Handshake",
    "RFID Clone",
    "SubGHz Jamming"
};

static void draw_prediction_header(Canvas* canvas) {
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 2, 10, "🎯 ATTACK PREDICTOR");
    canvas_draw_line(canvas, 0, 12, 128, 12);
}

static void draw_attack_selection(Canvas* canvas, PredictorState* state) {
    canvas_set_font(canvas, FontSecondary);
    
    // Current attack type
    canvas_draw_str(canvas, 2, 22, "Attack Type:");
    canvas_draw_str(canvas, 2, 32, attack_names[state->selected_attack]);
    
    // Navigation hint
    canvas_draw_str(canvas, 2, 42, "Up/Down: Change attack");
    canvas_draw_str(canvas, 2, 52, "Left/Right: Change target");
}

static void draw_prediction_results(Canvas* canvas, PredictorState* state) {
    if(!state->prediction_valid) {
        canvas_set_font(canvas, FontSecondary);
        canvas_draw_str(canvas, 2, 62, "Press OK to predict");
        return;
    }
    
    canvas_set_font(canvas, FontSecondary);
    
    // Success probability
    char prob_str[32];
    snprintf(prob_str, sizeof(prob_str), "Success: %.1f%%", 
            state->current_prediction.success_probability * 100);
    canvas_draw_str(canvas, 2, 62, prob_str);
    
    // Time estimate
    char time_str[32];
    if(state->current_prediction.estimated_time_ms < 10000) {
        snprintf(time_str, sizeof(time_str), "Time: %.1fs", 
                state->current_prediction.estimated_time_ms / 1000.0f);
    } else {
        snprintf(time_str, sizeof(time_str), "Time: %lus", 
                state->current_prediction.estimated_time_ms / 1000);
    }
    canvas_draw_str(canvas, 70, 62, time_str);
    
    // Difficulty and confidence
    char diff_str[32];
    snprintf(diff_str, sizeof(diff_str), "Difficulty: %s", 
            predator_predictor_get_difficulty_string(state->current_prediction.difficulty_rating));
    canvas_draw_str(canvas, 2, 72, diff_str);
    
    char conf_str[32];
    snprintf(conf_str, sizeof(conf_str), "Confidence: %s", 
            predator_predictor_get_confidence_string(state->current_prediction.confidence_level));
    canvas_draw_str(canvas, 2, 82, conf_str);
    
    // Recommendation (truncated to fit)
    char rec_str[20];
    strncpy(rec_str, state->current_prediction.recommended_approach, 19);
    rec_str[19] = '\0';
    canvas_draw_str(canvas, 2, 92, rec_str);
}

static void predictor_ui_draw_callback(Canvas* canvas, void* context) {
    PredictorState* state = context;
    
    canvas_clear(canvas);
    draw_prediction_header(canvas);
    draw_attack_selection(canvas, state);
    draw_prediction_results(canvas, state);
}

static bool predictor_ui_input_callback(InputEvent* event, void* context) {
    PredatorApp* app = context;
    if(!app) return false;
    
    if(event->type == InputTypeShort) {
        if(event->key == InputKeyBack) {
            return false; // Let scene manager handle back
        } else if(event->key == InputKeyOk) {
            // Generate new prediction
            uint32_t frequency = 433920000; // Default frequency
            if(predictor_state.selected_attack <= ATTACK_TYPE_CAR_SMART_KEY) {
                // Use car model frequency
                const PredatorCarModel* model = predator_models_get_hardcoded(predictor_state.selected_target);
                if(model) {
                    frequency = model->frequency;
                }
            }
            
            predictor_state.prediction_valid = predator_predict_attack_success(
                app, 
                (PredictiveAttackType)predictor_state.selected_attack,
                frequency,
                predictor_state.selected_target,
                &predictor_state.current_prediction
            );
            
            if(predictor_state.prediction_valid) {
                char log_msg[128];
                snprintf(log_msg, sizeof(log_msg), 
                        "🎯 Prediction: %s - %.1f%% success, %lums",
                        attack_names[predictor_state.selected_attack],
                        predictor_state.current_prediction.success_probability * 100,
                        predictor_state.current_prediction.estimated_time_ms);
                predator_log_append(app, log_msg);
                
                FURI_LOG_I("PredictorUI", "Generated prediction for %s", 
                          attack_names[predictor_state.selected_attack]);
            }
            
            return true;
        } else if(event->key == InputKeyUp) {
            if(predictor_state.selected_attack > 0) {
                predictor_state.selected_attack--;
                predictor_state.prediction_valid = false;
            }
            return true;
        } else if(event->key == InputKeyDown) {
            if(predictor_state.selected_attack < (ATTACK_TYPE_COUNT - 1)) {
                predictor_state.selected_attack++;
                predictor_state.prediction_valid = false;
            }
            return true;
        } else if(event->key == InputKeyLeft) {
            if(predictor_state.selected_target > 0) {
                predictor_state.selected_target--;
                predictor_state.prediction_valid = false;
            }
            return true;
        } else if(event->key == InputKeyRight) {
            size_t max_targets = predator_models_get_hardcoded_count();
            if(predictor_state.selected_target < (max_targets - 1)) {
                predictor_state.selected_target++;
                predictor_state.prediction_valid = false;
            }
            return true;
        }
    }
    
    return false;
}

void predator_scene_attack_predictor_ui_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app || !app->view) return;
    
    // Initialize predictor if not already done
    predator_predictor_init(app);
    
    // Reset state
    predictor_state.selected_attack = 0;
    predictor_state.selected_target = 0;
    predictor_state.prediction_valid = false;
    predictor_state.last_update_tick = furi_get_tick();
    
    // Setup view
    view_set_context(app->view, &predictor_state);
    view_set_draw_callback(app->view, predictor_ui_draw_callback);
    view_set_input_callback(app->view, predictor_ui_input_callback);
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewCustom);
    
    predator_log_append(app, "🎯 Attack Predictor Dashboard opened");
    FURI_LOG_I("PredictorUI", "Attack Predictor UI entered");
}

bool predator_scene_attack_predictor_ui_on_event(void* context, SceneManagerEvent event) {
    UNUSED(context);
    UNUSED(event);
    return false;
}

void predator_scene_attack_predictor_ui_on_exit(void* context) {
    PredatorApp* app = context;
    if(app && app->view) {
        view_set_context(app->view, NULL);
        view_set_draw_callback(app->view, NULL);
        view_set_input_callback(app->view, NULL);
    }
    
    predator_log_append(app, "🎯 Attack Predictor Dashboard closed");
    FURI_LOG_I("PredictorUI", "Attack Predictor UI exited");
}
