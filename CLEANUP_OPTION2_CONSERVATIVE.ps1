# 🛡️ PRODUCTION CLEANUP - OPTION 2: CONSERVATIVE
# Deletes only duplicates and confirmed unused (60 files)
# Keeps potential future features for evaluation

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  PREDATOR PRODUCTION CLEANUP - CONSERVATIVE" -ForegroundColor Cyan
Write-Host "  100K SALES CONSOLIDATION" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""
Write-Host "This will DELETE 60 files (duplicates + confirmed unused)" -ForegroundColor Yellow
Write-Host "Potential future features will be kept" -ForegroundColor Green
Write-Host ""

$confirmation = Read-Host "Type 'YES' to proceed with cleanup"
if ($confirmation -ne 'YES') {
    Write-Host "Cleanup cancelled." -ForegroundColor Red
    exit
}

Write-Host ""
Write-Host "Creating backup first..." -ForegroundColor Yellow
$timestamp = Get-Date -Format "yyyyMMdd_HHmmss"
$backupDir = "C:\Projects\Predator_momentum\BACKUP_BEFORE_CLEANUP_$timestamp"
Copy-Item -Path "C:\Projects\Predator_momentum\predator_app" -Destination $backupDir -Recurse -Force
Write-Host "✅ Backup created: $backupDir" -ForegroundColor Green
Write-Host ""

# Navigate to predator_app directory
Set-Location "C:\Projects\Predator_momentum\predator_app"

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "PHASE 1: Deleting duplicate HELPERS (12 files)" -ForegroundColor Yellow
Write-Host "========================================" -ForegroundColor Cyan

$duplicatesToDelete = @(
    "helpers/predator_advanced_monitoring_compact.c",
    "helpers/predator_advanced_sigint_compact.c",
    "helpers/predator_ai_engine_compact.c",
    "helpers/predator_hardware_manager_simple.c",
    "helpers/predator_models_expanded.c",
    "helpers/predator_models_link.c",
    "helpers/predator_ui_clean.c",
    "helpers/predator_ui_clean.h",
    "helpers/predator_ui_callbacks.c",
    "helpers/predator_view_dispatcher_patch.c",
    "helpers/predator_view_dispatcher_patch.h",
    "helpers/update_scene_files.md"
)

$deletedDupes = 0
foreach ($file in $duplicatesToDelete) {
    if (Test-Path $file) {
        Remove-Item $file -Force
        Write-Host "  ❌ Deleted: $file" -ForegroundColor Gray
        $deletedDupes++
    }
}
Write-Host "✅ Deleted $deletedDupes duplicate files" -ForegroundColor Green
Write-Host ""

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "PHASE 2: Deleting confirmed unused HELPERS (28 files)" -ForegroundColor Yellow
Write-Host "========================================" -ForegroundColor Cyan

$confirmedUnused = @(
    "helpers/predator_full_detection.c",
    "helpers/predator_full_detection.h",
    "helpers/predator_full_detection_working.c",
    "helpers/predator_simple_detection.c",
    "helpers/predator_models.c",
    "helpers/predator_models_seed.c",
    "helpers/predator_models_seed.h",
    "helpers/predator_navigation_fix.h",
    "helpers/predator_navigation_safe.h",
    "helpers/predator_scene_utils.c",
    "helpers/predator_scene_utils.h",
    "helpers/predator_ui_status.c",
    "helpers/predator_ui_status.h",
    "helpers/predator_ui_math.c",
    "helpers/predator_view_helpers.c",
    "helpers/predator_view_helpers.h",
    "helpers/predator_string.c",
    "helpers/predator_string.h",
    "helpers/predator_car_attacks.c",
    "helpers/predator_car_attacks.h",
    "helpers/predator_audit_engine.c",
    "helpers/predator_audit_engine.h",
    "helpers/predator_ai_engine.c",
    "helpers/predator_ai_engine.h",
    "helpers/predator_hardware_manager.c",
    "helpers/predator_hardware_manager.h",
    "helpers/predator_advanced_monitoring.c",
    "helpers/predator_advanced_monitoring.h"
)

$deletedUnused = 0
foreach ($file in $confirmedUnused) {
    if (Test-Path $file) {
        Remove-Item $file -Force
        Write-Host "  ❌ Deleted: $file" -ForegroundColor Gray
        $deletedUnused++
    }
}
Write-Host "✅ Deleted $deletedUnused unused files" -ForegroundColor Green
Write-Host ""

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "PHASE 3: Deleting unused SCENES (20 files)" -ForegroundColor Yellow
Write-Host "========================================" -ForegroundColor Cyan

$scenesToDelete = @(
    "scenes/predator_scene_config_complete.h",
    "scenes/predator_scene_config_expanded.h",
    "scenes/predator_scene_config_optimized.h",
    "scenes/predator_scene_config_professional.h",
    "scenes/predator_scene_config_ultimate.h",
    "scenes/predator_scene_quick_poc.c",
    "scenes/predator_scene_ai_attack_coordinator.c",
    "scenes/predator_scene_ai_powered_audit.c",
    "scenes/predator_scene_comprehensive_audit.c",
    "scenes/predator_scene_enhanced_audit.c",
    "scenes/predator_scene_system_audit.c",
    "scenes/predator_scene_bluetooth_main_menu.c",
    "scenes/predator_scene_usb_main_menu.c",
    "scenes/predator_scene_car_model_selector.c",
    "scenes/predator_scene_car_model_tests.c",
    "scenes/predator_scene_car_test_results.c",
    "scenes/predator_scene_rfid_fuzzing_ui.c",
    "scenes/predator_scene_subghz_raw_send_ui.c",
    "scenes/predator_scene_wifi_handshake_ui.c",
    "scenes/predator_scene_traffic_light_security_ui.c"
)

$deletedScenes = 0
foreach ($file in $scenesToDelete) {
    if (Test-Path $file) {
        Remove-Item $file -Force
        Write-Host "  ❌ Deleted: $file" -ForegroundColor Gray
        $deletedScenes++
    }
}
Write-Host "✅ Deleted $deletedScenes scene files" -ForegroundColor Green
Write-Host ""

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "CLEANUP COMPLETE!" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""
Write-Host "📊 SUMMARY:" -ForegroundColor Cyan
Write-Host "  • Helpers deleted: $($deletedDupes + $deletedUnused)" -ForegroundColor White
Write-Host "  • Scenes deleted: $deletedScenes" -ForegroundColor White
Write-Host "  • Total deleted: $($deletedDupes + $deletedUnused + $deletedScenes)" -ForegroundColor White
Write-Host "  • Backup location: $backupDir" -ForegroundColor Yellow
Write-Host ""
Write-Host "✅ Conservative cleanup complete!" -ForegroundColor Green
Write-Host "   Advanced features preserved for future evaluation" -ForegroundColor White
Write-Host ""
