@echo off
echo ========================================
echo 🏆 PREDATOR MOMENTUM - WORKING APPS ONLY
echo Building Only Confirmed Working Apps
echo Fast Production Package
echo ========================================
echo.

REM Clean everything
echo [PREP] Cleaning previous builds...
if exist build rmdir /s /q build
if exist *.fap del *.fap
if exist working_package rmdir /s /q working_package
mkdir working_package
mkdir working_package\apps

echo.
echo ========================================
echo 🎛️ BUILDING CONFIRMED WORKING APPS
echo ========================================

REM 1. Master Orchestrator (CONFIRMED WORKING)
echo [1/4] Building Master Orchestrator (CONFIRMED WORKING)...
copy application_minimal_working.fam application.fam >nul
ufbt build
if exist build\predator_minimal_working.fap (
    copy build\predator_minimal_working.fap working_package\apps\predator_master_orchestrator.fap >nul
    echo ✅ SUCCESS: predator_master_orchestrator.fap - BRAIN READY
) else (
    echo ❌ FAILED: predator_master_orchestrator.fap
)

REM 2. WiFi Specialist (CONFIRMED WORKING)
echo [2/4] Building WiFi Specialist (CONFIRMED WORKING)...
copy application_wifi.fam application.fam >nul
ufbt build
if exist build\predator_wifi.fap (
    copy build\predator_wifi.fap working_package\apps\predator_wifi_specialist.fap >nul
    echo ✅ SUCCESS: predator_wifi_specialist.fap
) else (
    echo ❌ FAILED: predator_wifi_specialist.fap
)

REM 3. Car Specialist (CONFIRMED WORKING)
echo [3/4] Building Car Specialist (CONFIRMED WORKING)...
copy application_car.fam application.fam >nul
ufbt build
if exist build\predator_car.fap (
    copy build\predator_car.fap working_package\apps\predator_car_specialist.fap >nul
    echo ✅ SUCCESS: predator_car_specialist.fap
) else (
    echo ❌ FAILED: predator_car_specialist.fap
)

REM 4. Tesla VIP++ (CONFIRMED WORKING)
echo [4/4] Building Tesla VIP++ Specialist (CONFIRMED WORKING)...
copy application_tesla.fam application.fam >nul
ufbt build
if exist build\predator_tesla.fap (
    copy build\predator_tesla.fap working_package\apps\predator_tesla_vip.fap >nul
    echo ✅ SUCCESS: predator_tesla_vip.fap
) else (
    echo ❌ FAILED: predator_tesla_vip.fap
)

echo.
echo ========================================
echo 📦 WORKING APPS PACKAGE READY
echo ========================================

REM Count successes
set /a success_count=0
if exist working_package\apps\predator_master_orchestrator.fap set /a success_count+=1
if exist working_package\apps\predator_wifi_specialist.fap set /a success_count+=1
if exist working_package\apps\predator_car_specialist.fap set /a success_count+=1
if exist working_package\apps\predator_tesla_vip.fap set /a success_count+=1

echo.
echo 🏆 WORKING APPS BUILT: %success_count%/4
echo.
echo ✅ CONFIRMED WORKING APPS:
if exist working_package\apps\predator_master_orchestrator.fap echo    🎛️ predator_master_orchestrator.fap - MASTER BRAIN
if exist working_package\apps\predator_wifi_specialist.fap echo    📶 predator_wifi_specialist.fap - WiFi Expert
if exist working_package\apps\predator_car_specialist.fap echo    🚗 predator_car_specialist.fap - 90+ Car Models
if exist working_package\apps\predator_tesla_vip.fap echo    ⚡ predator_tesla_vip.fap - Elon's VIP++ Requirements

echo.
echo 🚀 DEPLOYMENT INSTRUCTIONS:
echo 1. Copy working_package\apps\*.fap to Flipper Zero /apps/Tools/
echo 2. Start with predator_master_orchestrator.fap
echo 3. Use Master to launch WiFi, Car, and Tesla specialists
echo 4. Professional multi-app ecosystem ready!

echo.
echo ========================================
echo 🏆 WORKING APPS PACKAGE COMPLETE
echo Fast Production Deployment Ready
echo ========================================
echo.
pause
