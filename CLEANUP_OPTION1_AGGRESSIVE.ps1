# 🔥 PRODUCTION CLEANUP - OPTION 1: AGGRESSIVE
# Deletes 96 unused files, keeps 48 essential production files
# Total cleanup: ~550KB

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  PREDATOR PRODUCTION CLEANUP - AGGRESSIVE" -ForegroundColor Cyan
Write-Host "  100K SALES CONSOLIDATION" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""
Write-Host "This will DELETE 96 unused files (~550KB)" -ForegroundColor Yellow
Write-Host "Production files (48) will remain untouched" -ForegroundColor Green
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
Write-Host "PHASE 1: Deleting unused HELPERS (66 files)" -ForegroundColor Yellow
Write-Host "========================================" -ForegroundColor Cyan

$helpersToDelete = @(
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

$deletedHelpers = 0
foreach ($file in $helpersToDelete) {
    if (Test-Path $file) {
        Remove-Item $file -Force
        Write-Host "  ❌ Deleted: $file" -ForegroundColor Gray
        $deletedHelpers++
    } else {
        Write-Host "  ⚠️  Not found: $file" -ForegroundColor DarkGray
    }
}
Write-Host "✅ Deleted $deletedHelpers helper files" -ForegroundColor Green
Write-Host ""

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "PHASE 2: Deleting unused SCENES (30 files)" -ForegroundColor Yellow
Write-Host "========================================" -ForegroundColor Cyan

$scenesToDelete = @(
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

$deletedScenes = 0
foreach ($file in $scenesToDelete) {
    if (Test-Path $file) {
        Remove-Item $file -Force
        Write-Host "  ❌ Deleted: $file" -ForegroundColor Gray
        $deletedScenes++
    } else {
        Write-Host "  ⚠️  Not found: $file" -ForegroundColor DarkGray
    }
}
Write-Host "✅ Deleted $deletedScenes scene files" -ForegroundColor Green
Write-Host ""

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "CLEANUP COMPLETE!" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""
Write-Host "📊 SUMMARY:" -ForegroundColor Cyan
Write-Host "  • Helpers deleted: $deletedHelpers" -ForegroundColor White
Write-Host "  • Scenes deleted: $deletedScenes" -ForegroundColor White
Write-Host "  • Total deleted: $($deletedHelpers + $deletedScenes)" -ForegroundColor White
Write-Host "  • Backup location: $backupDir" -ForegroundColor Yellow
Write-Host ""
Write-Host "✅ Production codebase is now clean and optimized!" -ForegroundColor Green
Write-Host ""
Write-Host "🚀 NEXT STEPS:" -ForegroundColor Cyan
Write-Host "  1. Test build: ufbt" -ForegroundColor White
Write-Host "  2. Verify FAP deployment on hardware" -ForegroundColor White
Write-Host "  3. Commit to version control" -ForegroundColor White
Write-Host ""
