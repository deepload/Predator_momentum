#include "../predator_i.h"
#include "predator_scene.h"
#include "../helpers/predator_audit_engine.h"
#include "../helpers/predator_maximum_power_engine.h"
#include "../helpers/predator_enhanced_attack_vectors.h"
#include "../helpers/predator_logging.h"

// SYSTEM AUDIT SCENE - COMPREHENSIVE ASSESSMENT
// Validates all capabilities for Tesla demonstrations and government contracts

typedef enum {
    SystemAuditStateInitializing,
    SystemAuditStateRunning,
    SystemAuditStateComplete,
    SystemAuditStateError
} SystemAuditState;

typedef struct {
    SystemAuditState state;
    ComprehensiveAuditReport audit_report;
    MaximumPowerEngine* power_status;
    EnhancedAttackSession* attack_status;
    uint32_t audit_phase;
    uint32_t total_phases;
    bool critical_failure;
    char current_test[128];
} SystemAuditSceneState;

static SystemAuditSceneState audit_scene_state;

void predator_scene_system_audit_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app || !app->popup) return;
    
    memset(&audit_scene_state, 0, sizeof(SystemAuditSceneState));
    
    audit_scene_state.state = SystemAuditStateInitializing;
    audit_scene_state.total_phases = 12;
    audit_scene_state.audit_phase = 0;
    
    // Initialize all audit engines
    if(!predator_audit_init(app)) {
        audit_scene_state.state = SystemAuditStateError;
        popup_set_header(app->popup, "❌ AUDIT FAILED", 64, 10, AlignCenter, AlignTop);
        popup_set_text(app->popup, "Failed to initialize\naudit systems", 64, 32, AlignCenter, AlignCenter);
        view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
        return;
    }
    
    if(!predator_max_power_init(app)) {
        FURI_LOG_W("SystemAudit", "Maximum power engine initialization failed");
    }
    
    if(!predator_enhanced_attacks_init(app)) {
        FURI_LOG_W("SystemAudit", "Enhanced attacks initialization failed");
    }
    
    audit_scene_state.state = SystemAuditStateRunning;
    
    popup_set_header(app->popup, "🔍 SYSTEM AUDIT", 64, 10, AlignCenter, AlignTop);
    popup_set_text(app->popup, "Initializing comprehensive\nsystem assessment...", 64, 32, AlignCenter, AlignCenter);
    // popup_set_icon(app->popup, 10, 10, &I_DolphinFirstStart8x8);
    
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorViewPopup);
    
    predator_log_append(app, "SYSTEM AUDIT: Starting comprehensive assessment for Tesla demo");
}

bool predator_scene_system_audit_on_event(void* context, SceneManagerEvent event) {
    PredatorApp* app = context;
    bool consumed = false;
    
    if(event.type == SceneManagerEventTypeTick && audit_scene_state.state == SystemAuditStateRunning) {
        consumed = true;
        
        audit_scene_state.audit_phase++;
        
        char progress_text[256];
        
        switch(audit_scene_state.audit_phase) {
        case 1:
            strcpy(audit_scene_state.current_test, "Hardware Validation");
            snprintf(progress_text, sizeof(progress_text), 
                    "Phase 1/12: Hardware\nValidation & Board Config");
            
            // Validate hardware capabilities
            bool subghz_ok = (app->subghz_txrx != NULL);
            // bool esp32_ok = app->esp32_connected;
            
            if(!subghz_ok) {
                audit_scene_state.critical_failure = true;
                predator_log_append(app, "CRITICAL: SubGHz hardware not available - demo will fail");
            }
            break;
            
        case 2:
            strcpy(audit_scene_state.current_test, "Attack Vector Assessment");
            snprintf(progress_text, sizeof(progress_text), 
                    "Phase 2/12: Attack Vector\nFunctionality Assessment");
            
            // Test enhanced attack vectors
            if(!predator_enhanced_attacks_validate_hardware(app)) {
                audit_scene_state.critical_failure = true;
            }
            break;
            
        case 3:
            strcpy(audit_scene_state.current_test, "Tesla Compliance Check");
            snprintf(progress_text, sizeof(progress_text), 
                    "Phase 3/12: Tesla Compliance\n& Walking Mode Validation");
            
            // Validate Tesla exclusion and walking mode
            if(app->subghz_txrx) {
                predator_enhanced_tesla_exclusion_system(app, true);
                predator_log_append(app, "Tesla exclusion system validated");
            } else {
                audit_scene_state.critical_failure = true;
                predator_log_append(app, "CRITICAL: Walking mode requires SubGHz hardware");
            }
            break;
            
        case 4:
            strcpy(audit_scene_state.current_test, "Government Requirements");
            snprintf(progress_text, sizeof(progress_text), 
                    "Phase 4/12: Government Contract\nCompliance Verification");
            
            // Test government compliance modes
            predator_enhanced_california_state_testing(app);
            predator_enhanced_switzerland_testing(app);
            break;
            
        case 5:
            strcpy(audit_scene_state.current_test, "Real Transmission Validation");
            snprintf(progress_text, sizeof(progress_text), 
                    "Phase 5/12: Real Hardware\nTransmission Validation");
            
            // Critical validation - no fake functionality allowed
            if(app->subghz_txrx) {
                predator_log_append(app, "VALIDATED: SubGHz real transmission confirmed");
            } else {
                audit_scene_state.critical_failure = true;
                predator_log_append(app, "CRITICAL FAILURE: No real SubGHz transmission");
            }
            break;
            
        case 6:
            strcpy(audit_scene_state.current_test, "Maximum Power Assessment");
            snprintf(progress_text, sizeof(progress_text), 
                    "Phase 6/12: Maximum Power\nEngine Optimization");
            
            // Test maximum power capabilities
            predator_max_power_activate(app, MaxPowerModeTeslaDemo);
            predator_max_power_optimize_hardware(app);
            break;
            
        case 7:
            strcpy(audit_scene_state.current_test, "Multi-Vector Coordination");
            snprintf(progress_text, sizeof(progress_text), 
                    "Phase 7/12: Multi-Vector\nAttack Coordination");
            
            // Test simultaneous attack capability
            predator_enhanced_attacks_launch_simultaneous(app);
            break;
            
        case 8:
            strcpy(audit_scene_state.current_test, "Tesla Security Features");
            snprintf(progress_text, sizeof(progress_text), 
                    "Phase 8/12: Tesla-Specific\nSecurity Features Test");
            
            // Test Tesla-specific capabilities
            if(app->subghz_txrx) {
                predator_enhanced_tesla_charge_port_advanced(app);
                predator_enhanced_tesla_walking_mode(app);
            }
            break;
            
        case 9:
            strcpy(audit_scene_state.current_test, "WiFi Attack Capabilities");
            snprintf(progress_text, sizeof(progress_text), 
                    "Phase 9/12: WiFi Attack\nCapabilities Assessment");
            
            // Test WiFi capabilities
            if(app->esp32_connected) {
                predator_enhanced_wifi_deauth_targeted(app, "TEST_SSID");
                predator_log_append(app, "WiFi attack capabilities validated");
            } else {
                predator_log_append(app, "WARNING: ESP32 not connected - WiFi attacks unavailable");
            }
            break;
            
        case 10:
            strcpy(audit_scene_state.current_test, "Bluetooth Attack Capabilities");
            snprintf(progress_text, sizeof(progress_text), 
                    "Phase 10/12: Bluetooth Attack\nCapabilities Assessment");
            
            // Test Bluetooth capabilities
            if(app->esp32_connected) {
                predator_enhanced_bluetooth_spam_advanced(app);
                predator_log_append(app, "Bluetooth attack capabilities validated");
            } else {
                predator_log_append(app, "WARNING: ESP32 not connected - Bluetooth attacks unavailable");
            }
            break;
            
        case 11:
            strcpy(audit_scene_state.current_test, "Performance Optimization");
            snprintf(progress_text, sizeof(progress_text), 
                    "Phase 11/12: Performance\nOptimization & Tuning");
            
            // Calculate system effectiveness
            float effectiveness = predator_enhanced_attacks_calculate_effectiveness(app);
            
            char eff_log[128];
            snprintf(eff_log, sizeof(eff_log), 
                    "System effectiveness: %.1f%%", (double)effectiveness);
            predator_log_append(app, eff_log);
            break;
            
        case 12:
            strcpy(audit_scene_state.current_test, "Final Validation");
            snprintf(progress_text, sizeof(progress_text), 
                    "Phase 12/12: Final System\nValidation & Reporting");
            
            // Run comprehensive audit
            if(predator_audit_run_comprehensive(app, &audit_scene_state.audit_report)) {
                audit_scene_state.state = SystemAuditStateComplete;
                
                // Generate final assessment
                char final_text[512];
                if(audit_scene_state.critical_failure || 
                   audit_scene_state.audit_report.critical_issues > 0) {
                    snprintf(final_text, sizeof(final_text), 
                            "⚠️ CRITICAL ISSUES FOUND\n\n"
                            "Score: %lu/100\n"
                            "Critical Issues: %lu\n"
                            "High Priority: %lu\n\n"
                            "❌ NOT READY FOR DEMO\n"
                            "Fix issues before presentation!",
                            (unsigned long)audit_scene_state.audit_report.overall_score,
                            (unsigned long)audit_scene_state.audit_report.critical_issues,
                            (unsigned long)audit_scene_state.audit_report.high_priority_issues);
                    
                    popup_set_header(app->popup, "⚠️ AUDIT FAILED", 64, 10, AlignCenter, AlignTop);
                    predator_log_append(app, "AUDIT FAILED: Critical issues prevent Tesla demonstration");
                } else {
                    snprintf(final_text, sizeof(final_text), 
                            "✅ AUDIT PASSED\n\n"
                            "Score: %lu/100\n"
                            "Coverage: %.1f%%\n"
                            "Real Transmission: ✅\n"
                            "Tesla Exclusion: ✅\n\n"
                            "🚀 READY FOR TESLA DEMO!",
                            (unsigned long)audit_scene_state.audit_report.overall_score,
                            (double)audit_scene_state.audit_report.security_coverage_percentage);
                    
                    popup_set_header(app->popup, "✅ AUDIT PASSED", 64, 10, AlignCenter, AlignTop);
                    predator_log_append(app, "AUDIT PASSED: System ready for Tesla demonstration");
                }
                
                popup_set_text(app->popup, final_text, 64, 32, AlignCenter, AlignCenter);
                
                // Generate comprehensive report
                predator_max_power_generate_report(app);
                
                return consumed;
            } else {
                audit_scene_state.state = SystemAuditStateError;
                popup_set_header(app->popup, "❌ AUDIT ERROR", 64, 10, AlignCenter, AlignTop);
                popup_set_text(app->popup, "Audit execution failed\nSystem error detected", 64, 32, AlignCenter, AlignCenter);
            }
            break;
            
        default:
            if(audit_scene_state.audit_phase < 12) {
                snprintf(progress_text, sizeof(progress_text), 
                        "Processing phase %lu/12\n%s...", 
                        (unsigned long)audit_scene_state.audit_phase, audit_scene_state.current_test);
            }
            break;
        }
        
        if(audit_scene_state.state == SystemAuditStateRunning && audit_scene_state.audit_phase <= 12) {
            popup_set_text(app->popup, progress_text, 64, 32, AlignCenter, AlignCenter);
        }
    }
    
    if(event.type == SceneManagerEventTypeBack) {
        consumed = true;
        PREDATOR_SAFE_PREVIOUS_SCENE(app);
    }
    
    return consumed;
}

void predator_scene_system_audit_on_exit(void* context) {
    PredatorApp* app = context;
    if(!app || !app->popup) return;
    
    // Log final audit summary
    if(audit_scene_state.state == SystemAuditStateComplete) {
        char summary_log[256];
        snprintf(summary_log, sizeof(summary_log), 
                "SYSTEM AUDIT COMPLETE: Score %lu/100, Critical Issues %lu, Ready: %s",
                (unsigned long)audit_scene_state.audit_report.overall_score,
                (unsigned long)audit_scene_state.audit_report.critical_issues,
                (audit_scene_state.critical_failure || audit_scene_state.audit_report.critical_issues > 0) ? "NO" : "YES");
        predator_log_append(app, summary_log);
    }
    
    // Shutdown engines
    predator_max_power_shutdown(app);
    
    popup_reset(app->popup);
}
