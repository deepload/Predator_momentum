#include "../predator_i.h"
#include "predator_scene.h"
#include "../helpers/predator_audit_engine.h"
#include "../helpers/predator_logging.h"

// COMPREHENSIVE AUDIT SCENE - MAXIMUM POWER ASSESSMENT
// Professional audit interface for Tesla demonstrations and government contracts

typedef enum {
    ComprehensiveAuditStateRunning,
    ComprehensiveAuditStateComplete,
    ComprehensiveAuditStateError
} ComprehensiveAuditState;

typedef struct {
    ComprehensiveAuditState state;
    ComprehensiveAuditReport audit_report;
    uint32_t audit_start_time;
    uint32_t current_phase;
    bool audit_in_progress;
} ComprehensiveAuditSceneState;

static ComprehensiveAuditSceneState audit_state;

void predator_scene_comprehensive_audit_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app || !app->popup) return;
    
    memset(&audit_state, 0, sizeof(ComprehensiveAuditSceneState));
    
    // Initialize audit engine
    if(!predator_audit_init(app)) {
        FURI_LOG_E("Audit", "Failed to initialize audit engine");
        audit_state.state = ComprehensiveAuditStateError;
        popup_set_header(app->popup, "❌ Audit Error", 64, 10, AlignCenter, AlignTop);
        popup_set_text(app->popup, "Failed to initialize\naudit engine", 64, 32, AlignCenter, AlignCenter);
        view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
        return;
    }
    
    // Start comprehensive audit
    audit_state.state = ComprehensiveAuditStateRunning;
    audit_state.audit_start_time = furi_get_tick();
    audit_state.audit_in_progress = true;
    audit_state.current_phase = 0;
    
    popup_set_header(app->popup, "🔍 COMPREHENSIVE AUDIT", 64, 10, AlignCenter, AlignTop);
    popup_set_text(app->popup, "Running system audit...\nPlease wait", 64, 32, AlignCenter, AlignCenter);
    // popup_set_icon(app->popup, 10, 10, &I_DolphinFirstStart8x8);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
    
    predator_log_append(app, "AUDIT: Starting comprehensive system assessment");
}

bool predator_scene_comprehensive_audit_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(event.type == SceneManagerEventTypeTick && audit_state.audit_in_progress) {
        consumed = true;
        
        // Update audit progress
        audit_state.current_phase++;
        
        char progress_text[128];
        
        switch(audit_state.current_phase) {
        case 1:
            snprintf(progress_text, sizeof(progress_text), 
                    "Phase 1/8: Hardware\nCapabilities Assessment");
            break;
        case 2:
            snprintf(progress_text, sizeof(progress_text), 
                    "Phase 2/8: Attack Vector\nFunctionality Check");
            break;
        case 3:
            snprintf(progress_text, sizeof(progress_text), 
                    "Phase 3/8: Security Gap\nIdentification");
            break;
        case 4:
            snprintf(progress_text, sizeof(progress_text), 
                    "Phase 4/8: Tesla Compliance\nValidation");
            break;
        case 5:
            snprintf(progress_text, sizeof(progress_text), 
                    "Phase 5/8: Government Contract\nRequirements Check");
            break;
        case 6:
            snprintf(progress_text, sizeof(progress_text), 
                    "Phase 6/8: Real Transmission\nValidation");
            break;
        case 7:
            snprintf(progress_text, sizeof(progress_text), 
                    "Phase 7/8: Walking Mode\nEffectiveness Test");
            break;
        case 8:
            snprintf(progress_text, sizeof(progress_text), 
                    "Phase 8/8: Performance\nBottleneck Analysis");
            break;
        case 15:
            // Run the actual comprehensive audit
            if(predator_audit_run_comprehensive(app, &audit_state.audit_report)) {
                audit_state.state = ComprehensiveAuditStateComplete;
                audit_state.audit_in_progress = false;
                
                // Display results
                char results_text[256];
                if(audit_state.audit_report.critical_issues == 0 && 
                   audit_state.audit_report.overall_score >= 80) {
                    snprintf(results_text, sizeof(results_text), 
                            "✅ AUDIT PASSED\n\nScore: %lu/100\nCoverage: %.1f%%\n\nReady for Tesla demo!",
                            (unsigned long)audit_state.audit_report.overall_score,
                            (double)audit_state.audit_report.security_coverage_percentage);
                    popup_set_header(app->popup, "✅ AUDIT COMPLETE", 64, 10, AlignCenter, AlignTop);
                } else {
                    snprintf(results_text, sizeof(results_text), 
                            "⚠️ ISSUES FOUND\n\nScore: %lu/100\nCritical: %lu\nHigh Priority: %lu\n\nReview required!",
                            (unsigned long)audit_state.audit_report.overall_score,
                            (unsigned long)audit_state.audit_report.critical_issues,
                            (unsigned long)audit_state.audit_report.high_priority_issues);
                    popup_set_header(app->popup, "⚠️ AUDIT ISSUES", 64, 10, AlignCenter, AlignTop);
                }
                
                popup_set_text(app->popup, results_text, 64, 32, AlignCenter, AlignCenter);
                
                // Log detailed findings
                predator_audit_log_findings(app, &audit_state.audit_report);
                
                char summary_log[256];
                snprintf(summary_log, sizeof(summary_log), 
                        "AUDIT SUMMARY: %lu/100 score, %lu critical, %lu high priority issues",
                        (unsigned long)audit_state.audit_report.overall_score,
                        (unsigned long)audit_state.audit_report.critical_issues,
                        (unsigned long)audit_state.audit_report.high_priority_issues);
                predator_log_append(app, summary_log);
                
            } else {
                audit_state.state = ComprehensiveAuditStateError;
                audit_state.audit_in_progress = false;
                popup_set_header(app->popup, "❌ Audit Failed", 64, 10, AlignCenter, AlignTop);
                popup_set_text(app->popup, "Audit execution failed\nCheck system status", 64, 32, AlignCenter, AlignCenter);
            }
            break;
        default:
            if(audit_state.current_phase < 15) {
                snprintf(progress_text, sizeof(progress_text), 
                        "Running audit phase %lu/8\nAnalyzing system...", 
                        (unsigned long)((audit_state.current_phase > 8) ? 8 : audit_state.current_phase));
            }
            break;
        }
        
        if(audit_state.audit_in_progress && audit_state.current_phase < 15) {
            popup_set_text(app->popup, progress_text, 64, 32, AlignCenter, AlignCenter);
        }
    }
    
    if(event.type == SceneManagerEventTypeBack) {
        consumed = true;
        scene_manager_previous_scene(app->scene_manager);
    }
    
    return consumed;
}

void predator_scene_comprehensive_audit_on_exit(void* context) {
    PredatorApp* app = context;
    if(!app || !app->popup) return;
    
    // Log final audit status
    if(audit_state.state == ComprehensiveAuditStateComplete) {
        if(audit_state.audit_report.critical_issues == 0 && 
           audit_state.audit_report.overall_score >= 80) {
            predator_log_append(app, "AUDIT COMPLETE: System ready for Tesla demonstration");
        } else {
            predator_log_append(app, "AUDIT COMPLETE: Critical issues require attention before demo");
        }
    }
    
    popup_reset(app->popup);
}
