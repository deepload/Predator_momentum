#pragma once

#include "../predator_i.h"

// COMPREHENSIVE AUDIT ENGINE - MAXIMUM POWER ASSESSMENT
// Implements systematic auditing of all attack vectors and capabilities
// Addresses Elon's requirements for complete system assessment

typedef enum {
    AuditTypeSecurityGaps,
    AuditTypeAttackVectors,
    AuditTypeHardwareCapabilities,
    AuditTypePerformanceBottlenecks,
    AuditTypeTeslaCompliance,
    AuditTypeGovernmentRequirements,
    AuditTypeRealTransmissionValidation,
    AuditTypeWalkingModeEffectiveness,
    AuditTypeCount
} AuditType;

typedef enum {
    AuditSeverityCritical,
    AuditSeverityHigh,
    AuditSeverityMedium,
    AuditSeverityLow,
    AuditSeverityInfo
} AuditSeverity;

typedef struct {
    AuditType type;
    AuditSeverity severity;
    char finding[256];
    char recommendation[256];
    bool requires_immediate_action;
    uint32_t impact_score;
    uint32_t timestamp;
} AuditFinding;

typedef struct {
    // Security Assessment
    uint32_t total_attack_vectors;
    uint32_t functional_attack_vectors;
    uint32_t real_transmission_vectors;
    float security_coverage_percentage;
    
    // Hardware Assessment
    bool subghz_functional;
    bool esp32_functional;
    bool gps_functional;
    bool external_rf_available;
    uint32_t max_power_capability;
    
    // Tesla Specific Assessment
    bool tesla_exclusion_active;
    bool walking_mode_functional;
    uint32_t tesla_specific_features;
    
    // Government Contract Compliance
    bool california_state_compliant;
    bool switzerland_compliant;
    uint32_t compliance_score;
    
    // Performance Metrics
    float attack_success_rate;
    uint32_t average_attack_time_ms;
    uint32_t memory_efficiency_score;
    uint32_t power_efficiency_score;
    
    // Audit Results
    AuditFinding findings[32];
    uint32_t finding_count;
    uint32_t critical_issues;
    uint32_t high_priority_issues;
    uint32_t overall_score;
    
    uint32_t audit_timestamp;
    bool audit_complete;
} ComprehensiveAuditReport;

// Audit Engine Functions
bool predator_audit_init(PredatorApp* app);
bool predator_audit_run_comprehensive(PredatorApp* app, ComprehensiveAuditReport* report);
bool predator_audit_security_gaps(PredatorApp* app, ComprehensiveAuditReport* report);
bool predator_audit_attack_vectors(PredatorApp* app, ComprehensiveAuditReport* report);
bool predator_audit_hardware_capabilities(PredatorApp* app, ComprehensiveAuditReport* report);
bool predator_audit_tesla_compliance(PredatorApp* app, ComprehensiveAuditReport* report);
bool predator_audit_government_requirements(PredatorApp* app, ComprehensiveAuditReport* report);
bool predator_audit_real_transmission_validation(PredatorApp* app, ComprehensiveAuditReport* report);
bool predator_audit_walking_mode_effectiveness(PredatorApp* app, ComprehensiveAuditReport* report);
bool predator_audit_performance_bottlenecks(PredatorApp* app, ComprehensiveAuditReport* report);

// Audit Reporting
void predator_audit_generate_report(PredatorApp* app, ComprehensiveAuditReport* report);
void predator_audit_log_findings(PredatorApp* app, ComprehensiveAuditReport* report);
uint32_t predator_audit_calculate_overall_score(ComprehensiveAuditReport* report);

// Audit Remediation
bool predator_audit_apply_security_enhancements(PredatorApp* app, ComprehensiveAuditReport* report);
bool predator_audit_optimize_attack_vectors(PredatorApp* app, ComprehensiveAuditReport* report);
bool predator_audit_enhance_tesla_features(PredatorApp* app, ComprehensiveAuditReport* report);
