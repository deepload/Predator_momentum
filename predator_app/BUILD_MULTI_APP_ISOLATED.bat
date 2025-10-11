@echo off
cls
echo ========================================
echo PREDATOR MOMENTUM - MULTI-APP ISOLATED
echo Proper App Isolation - No Cross Dependencies
echo Master Orchestrator + Specialized Apps
echo ========================================
echo.

REM Clean everything
echo [STEP 1] Cleaning previous builds...
if exist build rmdir /s /q build 2>nul
if exist *.fap del *.fap 2>nul
if exist MULTI_APP_ISOLATED rmdir /s /q MULTI_APP_ISOLATED 2>nul
mkdir MULTI_APP_ISOLATED 2>nul

echo.
echo ========================================
echo BUILDING ISOLATED MULTI-APP SYSTEM
echo ========================================

echo [1/4] Building Master Orchestrator (Isolated)...
copy application_master_orchestrator.fam application.fam >nul 2>&1
ufbt build
if exist build\predator_master_orchestrator.fap (
    copy build\predator_master_orchestrator.fap MULTI_APP_ISOLATED\predator_master_orchestrator.fap >nul
    echo ✅ SUCCESS: Master Orchestrator built
) else (
    echo ❌ FAILED: Master Orchestrator build failed
)

echo.
echo [2/4] Building WiFi Specialist (Isolated)...
copy application_wifi.fam application.fam >nul 2>&1
ufbt build
if exist build\predator_wifi.fap (
    copy build\predator_wifi.fap MULTI_APP_ISOLATED\predator_wifi_specialist.fap >nul
    echo ✅ SUCCESS: WiFi Specialist built
) else (
    echo ❌ FAILED: WiFi Specialist build failed
)

echo.
echo [3/4] Building Car Specialist (Isolated)...
copy application_car.fam application.fam >nul 2>&1
ufbt build
if exist build\predator_car.fap (
    copy build\predator_car.fap MULTI_APP_ISOLATED\predator_car_specialist.fap >nul
    echo ✅ SUCCESS: Car Specialist built
) else (
    echo ❌ FAILED: Car Specialist build failed
)

echo.
echo [4/4] Building Tesla VIP Specialist (Isolated)...
copy application_tesla.fam application.fam >nul 2>&1
ufbt build
if exist build\predator_tesla.fap (
    copy build\predator_tesla.fap MULTI_APP_ISOLATED\predator_tesla_vip.fap >nul
    echo ✅ SUCCESS: Tesla VIP built
) else (
    echo ❌ FAILED: Tesla VIP build failed
)

echo.
echo ========================================
echo MULTI-APP BUILD RESULTS
echo ========================================

REM Count successful builds
set /a success_count=0
if exist MULTI_APP_ISOLATED\predator_master_orchestrator.fap set /a success_count+=1
if exist MULTI_APP_ISOLATED\predator_wifi_specialist.fap set /a success_count+=1
if exist MULTI_APP_ISOLATED\predator_car_specialist.fap set /a success_count+=1
if exist MULTI_APP_ISOLATED\predator_tesla_vip.fap set /a success_count+=1

echo.
echo BUILD RESULTS: %success_count%/4 apps built successfully
echo.

if %success_count% GTR 0 (
    echo ✅ SUCCESSFUL BUILDS:
    if exist MULTI_APP_ISOLATED\predator_master_orchestrator.fap echo    - Master Orchestrator (Controls all apps)
    if exist MULTI_APP_ISOLATED\predator_wifi_specialist.fap echo    - WiFi Specialist (ESP32 Marauder)
    if exist MULTI_APP_ISOLATED\predator_car_specialist.fap echo    - Car Specialist (90+ Models)
    if exist MULTI_APP_ISOLATED\predator_tesla_vip.fap echo    - Tesla VIP (Elon's Requirements)
    
    echo.
    echo ========================================
    echo MULTI-APP DEPLOYMENT INSTRUCTIONS
    echo ========================================
    echo.
    echo INSTALL ALL APPS TO FLIPPER ZERO:
    echo.
    echo 1. Copy ALL .fap files to /apps/Tools/:
    if exist MULTI_APP_ISOLATED\predator_master_orchestrator.fap echo    - predator_master_orchestrator.fap
    if exist MULTI_APP_ISOLATED\predator_wifi_specialist.fap echo    - predator_wifi_specialist.fap
    if exist MULTI_APP_ISOLATED\predator_car_specialist.fap echo    - predator_car_specialist.fap
    if exist MULTI_APP_ISOLATED\predator_tesla_vip.fap echo    - predator_tesla_vip.fap
    echo.
    echo 2. START WITH MASTER ORCHESTRATOR:
    echo    Applications ^> Tools ^> Predator Master Orchestrator
    echo.
    echo 3. USE MASTER TO LAUNCH SPECIALIST APPS:
    echo    - Master controls and coordinates all specialist apps
    echo    - Each specialist app focuses on specific domain
    echo    - Inter-app communication via IPC system
    echo.
    echo ========================================
    echo MULTI-APP ARCHITECTURE READY!
    echo ========================================
    echo.
    echo MASTER ORCHESTRATOR FEATURES:
    echo - 🎛️ Controls all specialist apps
    echo - 📊 System monitoring and status
    echo - 🔄 Inter-app communication
    echo - ⚙️ Global settings management
    echo - 📈 Coordinated attack campaigns
    echo.
    echo SPECIALIST APP FEATURES:
    echo - 📶 WiFi: ESP32 Marauder integration
    echo - 🚗 Car: 90+ models, Tesla VIP++
    echo - ⚡ Tesla: Elon's specific requirements
    echo - 🎯 Each app optimized for specific domain
    echo.
    echo STATUS: MULTI-APP ECOSYSTEM READY FOR ELON'S STARTUP!
    
) else (
    echo ❌ NO SUCCESSFUL BUILDS
    echo All apps failed to build. Check the output above for errors.
    echo.
    echo COMMON ISSUES:
    echo - Missing scene dependencies
    echo - Cross-app references
    echo - Memory allocation problems
    echo.
    echo RECOMMENDATION: Use single app approach as backup
)

echo.
echo ========================================
echo PREDATOR MOMENTUM MULTI-APP SYSTEM
echo Professional Security Ecosystem
echo ========================================
echo.
pause
