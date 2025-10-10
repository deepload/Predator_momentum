#pragma once

#include "../predator_i.h"

// Automated reporting system
typedef enum {
    ReportFormatHTML,
    ReportFormatPDF,
    ReportFormatMarkdown,
    ReportFormatJSON,
} ReportFormat;

typedef enum {
    ReportTypeVulnerability,
    ReportTypePenetrationTest,
    ReportTypeExecutiveSummary,
    ReportTypeTechnicalDetails,
} ReportType;

typedef struct {
    char title[128];
    char description[512];
    uint8_t severity;
    char remediation[512];
    char evidence[256];
} VulnerabilityEntry;

typedef struct {
    char project_name[64];
    char client_name[64];
    char tester_name[64];
    uint32_t timestamp;
    VulnerabilityEntry* vulnerabilities;
    size_t vulnerability_count;
    uint32_t total_tests;
    uint32_t successful_tests;
    uint32_t failed_tests;
} SecurityReport;

// Report generation
bool predator_report_init(SecurityReport* report, const char* project_name);
bool predator_report_add_vulnerability(SecurityReport* report, const VulnerabilityEntry* vuln);
bool predator_report_generate(const SecurityReport* report, ReportFormat format, const char* output_path);
bool predator_report_export(const SecurityReport* report, const char* output_path);

// Statistics
void predator_report_update_stats(SecurityReport* report, bool test_success);
float predator_report_calculate_risk_score(const SecurityReport* report);
