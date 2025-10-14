# 📦 PRODUCTION CLEANUP - OPTION 3: ARCHIVE
# Moves unused files to ARCHIVED directory instead of deleting
# Safest approach - keeps everything but organized

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  PREDATOR PRODUCTION CLEANUP - ARCHIVE" -ForegroundColor Cyan
Write-Host "  100K SALES CONSOLIDATION" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""
Write-Host "This will MOVE 96 files to ARCHIVED directory" -ForegroundColor Yellow
Write-Host "Nothing will be deleted - safest approach" -ForegroundColor Green
Write-Host ""

$confirmation = Read-Host "Type 'YES' to proceed with archiving"
if ($confirmation -ne 'YES') {
    Write-Host "Archiving cancelled." -ForegroundColor Red
    exit
}

# Create archive directories
$archiveRoot = "C:\Projects\Predator_momentum\predator_app\ARCHIVED"
$archiveHelpers = "$archiveRoot\helpers"
$archiveScenes = "$archiveRoot\scenes"

Write-Host ""
Write-Host "Creating archive directories..." -ForegroundColor Yellow
New-Item -ItemType Directory -Path $archiveHelpers -Force | Out-Null
New-Item -ItemType Directory -Path $archiveScenes -Force | Out-Null
Write-Host "✅ Archive directories created" -ForegroundColor Green
Write-Host ""

# Navigate to predator_app directory
Set-Location "C:\Projects\Predator_momentum\predator_app"

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "PHASE 1: Archiving HELPERS (66 files)" -ForegroundColor Yellow
Write-Host "========================================" -ForegroundColor Cyan

$helpersToArchive = @(
    # Duplicates
    "helpers/predator_advanced_monitoring_compact.c",
    "helpers/predator_advanced_sigint_compact.c",
    "helpers/predator_ai_engine_compact.c",
    "helpers/predator_hardware_manager_simple.c",
    "helpers/predator_models_expanded.c",
    "helpers/predator_models_link.c",
    
    # Advanced/Monitoring
    "helpers/predator_advanced_monitoring.c",
    "helpers/predator_advanced_monitoring.h",
    "helpers/predator_advanced_sigint.c",
    "helpers/predator_advanced_sigint.h",
    "helpers/predator_ai_engine.c",
    "helpers/predator_ai_engine.h",
    
    # Optimizers/Engines
    "helpers/predator_attack_optimizer.c",
    "helpers/predator_attack_optimizer.h",
    "helpers/predator_audit_engine.c",
    "helpers/predator_audit_engine.h",
    "helpers/predator_enhanced_attack_vectors.c",
    "helpers/predator_enhanced_attack_vectors.h",
    "helpers/predator_hardware_optimizer.c",
    "helpers/predator_hardware_optimizer.h",
    "helpers/predator_performance_optimizer.c",
    "helpers/predator_performance_optimizer.h",
    "helpers/predator_signal_intelligence.c",
    "helpers/predator_signal_intelligence.h",
    "helpers/predator_maximum_power_engine.c",
    "helpers/predator_maximum_power_engine.h",
    
    # Detection/Hardware
    "helpers/predator_full_detection.c",
    "helpers/predator_full_detection.h",
    "helpers/predator_full_detection_working.c",
    "helpers/predator_hardware_manager.c",
    "helpers/predator_hardware_manager.h",
    "helpers/predator_simple_detection.c",
    "helpers/predator_view_dispatcher_patch.c",
    "helpers/predator_view_dispatcher_patch.h",
    
    # Car/Models
    "helpers/predator_car_attacks.c",
    "helpers/predator_car_attacks.h",
    "helpers/predator_models.c",
    "helpers/predator_models_seed.c",
    "helpers/predator_models_seed.h",
    
    # Navigation
    "helpers/predator_navigation_fix.h",
    "helpers/predator_navigation_safe.h",
    
    # Scene Utils
    "helpers/predator_scene_utils.c",
    "helpers/predator_scene_utils.h",
    
    # UI Utilities
    "helpers/predator_ui_callbacks.c",
    "helpers/predator_ui_clean.c",
    "helpers/predator_ui_clean.h",
    "helpers/predator_ui_status.c",
    "helpers/predator_ui_status.h",
    "helpers/predator_ui_math.c",
    "helpers/predator_view_helpers.c",
    "helpers/predator_view_helpers.h",
    
    # String Utils
    "helpers/predator_string.c",
    "helpers/predator_string.h",
    
    # Documentation
    "helpers/update_scene_files.md"
)

$archivedHelpers = 0
foreach ($file in $helpersToArchive) {
    if (Test-Path $file) {
        $fileName = Split-Path $file -Leaf
        Move-Item $file "$archiveHelpers\$fileName" -Force
        Write-Host "  📦 Archived: $file" -ForegroundColor Gray
        $archivedHelpers++
    }
}
Write-Host "✅ Archived $archivedHelpers helper files" -ForegroundColor Green
Write-Host ""

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "PHASE 2: Archiving SCENES (30 files)" -ForegroundColor Yellow
Write-Host "========================================" -ForegroundColor Cyan

$scenesToArchive = @(
    # Config files
    "scenes/predator_scene_config_complete.h",
    "scenes/predator_scene_config_expanded.h",
    "scenes/predator_scene_config_optimized.h",
    "scenes/predator_scene_config_professional.h",
    "scenes/predator_scene_config_ultimate.h",
    "scenes/predator_scene_quick_poc.c",
    
    # AI/Audit scenes
    "scenes/predator_scene_ai_attack_coordinator.c",
    "scenes/predator_scene_ai_powered_audit.c",
    "scenes/predator_scene_comprehensive_audit.c",
    "scenes/predator_scene_enhanced_audit.c",
    "scenes/predator_scene_system_audit.c",
    "scenes/predator_scene_maximum_power.c",
    
    # Alternative implementations
    "scenes/predator_scene_tesla_security_suite.c",
    "scenes/predator_scene_universal_car_hacker.c",
    "scenes/predator_scene_user_friendly_ui.c",
    "scenes/predator_scene_bluetooth_main_menu.c",
    "scenes/predator_scene_usb_main_menu.c",
    "scenes/predator_scene_car_brand_security_suite.c",
    "scenes/predator_scene_car_model_selector.c",
    "scenes/predator_scene_car_model_tests.c",
    "scenes/predator_scene_car_test_results.c",
    
    # Disabled scenes
    "scenes/predator_scene_rfid_fuzzing_ui.c",
    "scenes/predator_scene_subghz_raw_send_ui.c",
    
    # Commented out in build
    "scenes/predator_scene_wifi_handshake_ui.c",
    "scenes/predator_scene_gps_tracker_ui.c",
    "scenes/predator_scene_wardriving_ui.c",
    "scenes/predator_scene_social_engineering_ui.c",
    "scenes/predator_scene_traffic_light_security_ui.c"
)

$archivedScenes = 0
foreach ($file in $scenesToArchive) {
    if (Test-Path $file) {
        $fileName = Split-Path $file -Leaf
        Move-Item $file "$archiveScenes\$fileName" -Force
        Write-Host "  📦 Archived: $file" -ForegroundColor Gray
        $archivedScenes++
    }
}
Write-Host "✅ Archived $archivedScenes scene files" -ForegroundColor Green
Write-Host ""

# Create README in archive
$readmeContent = @"
# ARCHIVED FILES - Predator Momentum

These files were archived during production consolidation after 100K sales milestone.

## Archive Date
$(Get-Date -Format "yyyy-MM-dd HH:mm:ss")

## Why Archived?
- Not included in production build (application.fam)
- Superseded by newer implementations
- Duplicates or old versions
- Experimental features not used in production

## Contents
- **Helpers**: $archivedHelpers files (~300KB)
- **Scenes**: $archivedScenes files (~250KB)
- **Total**: $($archivedHelpers + $archivedScenes) files (~550KB)

## Usage
These files can be restored if needed for future features or reference.
To restore a file, simply copy it back to the original directory.

## Production Build
The production build uses only 48 essential files:
- 15 helper modules
- 30 scene implementations
- 2 core application files
- 1 UART implementation

All production files remain in the main helpers/ and scenes/ directories.
"@

Set-Content -Path "$archiveRoot\README.md" -Value $readmeContent

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "ARCHIVING COMPLETE!" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""
Write-Host "📊 SUMMARY:" -ForegroundColor Cyan
Write-Host "  • Helpers archived: $archivedHelpers" -ForegroundColor White
Write-Host "  • Scenes archived: $archivedScenes" -ForegroundColor White
Write-Host "  • Total archived: $($archivedHelpers + $archivedScenes)" -ForegroundColor White
Write-Host "  • Archive location: $archiveRoot" -ForegroundColor Yellow
Write-Host ""
Write-Host "✅ All files safely archived!" -ForegroundColor Green
Write-Host "   Production directory is clean" -ForegroundColor White
Write-Host "   Archive available for future reference" -ForegroundColor White
Write-Host ""
Write-Host "🚀 NEXT STEPS:" -ForegroundColor Cyan
Write-Host "  1. Test build: ufbt" -ForegroundColor White
Write-Host "  2. Verify FAP deployment on hardware" -ForegroundColor White
Write-Host "  3. Commit to version control" -ForegroundColor White
Write-Host ""
