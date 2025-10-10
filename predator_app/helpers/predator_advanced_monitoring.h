#pragma once

#include "../predator_i.h"

// ADVANCED MONITORING SYSTEM - REAL-TIME INTELLIGENCE AND REPORTING
// Professional-grade monitoring for Tesla demonstrations and government contracts

typedef enum {
    MonitoringModeBasic,
    MonitoringModeAdvanced,
    MonitoringModeTeslaDemo,
    MonitoringModeGovernmentContract,
    MonitoringModeRealTimeIntelligence
} MonitoringMode;

typedef enum {
    AlertLevelInfo,
    AlertLevelWarning,
    AlertLevelCritical,
    AlertLevelTeslaSpecific,
    AlertLevelGovernmentRequired
} AlertLevel;

typedef struct {
    // System Monitoring
    bool real_time_monitoring_active;
    uint32_t monitoring_sessions;
    uint32_t alerts_generated;
    uint32_t critical_events_detected;
    
    // Performance Metrics
    float system_performance_score;
    uint32_t successful_attacks_monitored;
    float average_attack_success_rate;
    uint32_t hardware_efficiency_score;
    
    // Tesla Monitoring
    bool tesla_monitoring_active;
    uint32_t tesla_attacks_monitored;
    float tesla_success_rate;
    uint32_t tesla_vulnerabilities_found;
    
    // Government Contract Monitoring
    bool california_monitoring_active;
    bool switzerland_monitoring_active;
    uint32_t traffic_light_tests_monitored;
    uint32_t infrastructure_scans_completed;
    
    // Real-time Intelligence
    uint32_t intelligence_reports_generated;
    uint32_t pattern_anomalies_detected;
    uint32_t zero_day_candidates_found;
    float threat_assessment_score;
    
    // Security Monitoring
    uint32_t security_breaches_detected;
    uint32_t unauthorized_access_attempts;
    bool intrusion_detection_active;
    
    // Live Dashboard Metrics
    uint32_t active_connections;
    uint32_t data_throughput_kbps;
    uint32_t cpu_usage_percent;
    uint32_t memory_usage_percent;
    
    uint32_t last_monitoring_update;
} AdvancedMonitoringStatus;

typedef struct {
    AlertLevel level;
    char message[256];
    uint32_t timestamp;
    char category[64];
    bool tesla_related;
    bool government_related;
} MonitoringAlert;

// Core Monitoring Functions
bool predator_advanced_monitoring_init(PredatorApp* app);
bool predator_advanced_monitoring_start_session(PredatorApp* app, MonitoringMode mode);
bool predator_advanced_monitoring_stop_session(PredatorApp* app);
bool predator_advanced_monitoring_set_mode(PredatorApp* app, MonitoringMode mode);

// Real-time Monitoring
bool predator_advanced_monitoring_real_time_update(PredatorApp* app);
bool predator_advanced_monitoring_track_attack_performance(PredatorApp* app, const char* attack_type, bool success);
bool predator_advanced_monitoring_monitor_hardware_status(PredatorApp* app);
bool predator_advanced_monitoring_detect_anomalies(PredatorApp* app);

// Tesla-Specific Monitoring
bool predator_advanced_monitoring_tesla_session(PredatorApp* app);
bool predator_advanced_monitoring_tesla_attack_tracker(PredatorApp* app, const char* tesla_attack, bool success);
bool predator_advanced_monitoring_tesla_vulnerability_scanner(PredatorApp* app);
bool predator_advanced_monitoring_tesla_compliance_check(PredatorApp* app);

// Government Contract Monitoring
bool predator_advanced_monitoring_california_traffic_analysis(PredatorApp* app);
bool predator_advanced_monitoring_switzerland_infrastructure(PredatorApp* app);
bool predator_advanced_monitoring_government_compliance_tracker(PredatorApp* app);
bool predator_advanced_monitoring_critical_infrastructure_scan(PredatorApp* app);

// Intelligence and Reporting
bool predator_advanced_monitoring_generate_intelligence_report(PredatorApp* app);
bool predator_advanced_monitoring_create_performance_dashboard(PredatorApp* app);
bool predator_advanced_monitoring_export_monitoring_data(PredatorApp* app);
bool predator_advanced_monitoring_threat_assessment(PredatorApp* app);

// Alert System
bool predator_advanced_monitoring_create_alert(PredatorApp* app, AlertLevel level, const char* message, const char* category);
bool predator_advanced_monitoring_process_alerts(PredatorApp* app);
bool predator_advanced_monitoring_tesla_alert(PredatorApp* app, const char* tesla_event);
bool predator_advanced_monitoring_government_alert(PredatorApp* app, const char* gov_event);

// Advanced Analytics
bool predator_advanced_monitoring_pattern_analysis(PredatorApp* app);
bool predator_advanced_monitoring_predictive_analytics(PredatorApp* app);
bool predator_advanced_monitoring_behavioral_analysis(PredatorApp* app);
bool predator_advanced_monitoring_zero_day_detection(PredatorApp* app);

// Live Dashboard
bool predator_advanced_monitoring_update_live_dashboard(PredatorApp* app);
bool predator_advanced_monitoring_real_time_statistics(PredatorApp* app);
bool predator_advanced_monitoring_performance_graphs(PredatorApp* app);

// Status and Reporting
AdvancedMonitoringStatus* predator_advanced_monitoring_get_status(PredatorApp* app);
bool predator_advanced_monitoring_generate_comprehensive_report(PredatorApp* app);
float predator_advanced_monitoring_calculate_system_score(PredatorApp* app);
