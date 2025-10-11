@echo off
echo ========================================
echo PREDATOR MOMENTUM - FIXED BUILD
echo Building Only Working Apps (No Emojis)
echo ========================================
echo.

REM Clean everything
echo [PREP] Cleaning previous builds...
if exist build rmdir /s /q build
if exist *.fap del *.fap
if exist fixed_package rmdir /s /q fixed_package
mkdir fixed_package

echo.
echo ========================================
echo BUILDING FIXED APPS
echo ========================================

REM 1. Master Orchestrator (Fixed)
echo [1/4] Building Master Orchestrator (Fixed)...
copy application_minimal_working.fam application.fam >nul
ufbt build
if exist build\predator_minimal_working.fap (
    copy build\predator_minimal_working.fap fixed_package\predator_master_orchestrator.fap >nul
    echo ✅ SUCCESS: predator_master_orchestrator.fap
) else (
    echo ❌ FAILED: predator_master_orchestrator.fap
)

REM 2. WiFi Specialist (Fixed - No Emojis)
echo [2/4] Building WiFi Specialist (Fixed - No Emojis)...
copy application_wifi.fam application.fam >nul
ufbt build
if exist build\predator_wifi.fap (
    copy build\predator_wifi.fap fixed_package\predator_wifi_specialist.fap >nul
    echo ✅ SUCCESS: predator_wifi_specialist.fap
) else (
    echo ❌ FAILED: predator_wifi_specialist.fap
)

REM 3. Car Specialist (Fixed - No Emojis)
echo [3/4] Building Car Specialist (Fixed - No Emojis)...
copy application_car.fam application.fam >nul
ufbt build
if exist build\predator_car.fap (
    copy build\predator_car.fap fixed_package\predator_car_specialist.fap >nul
    echo ✅ SUCCESS: predator_car_specialist.fap
) else (
    echo ❌ FAILED: predator_car_specialist.fap
)

REM 4. Tesla VIP (Fixed - Function Signature)
echo [4/4] Building Tesla VIP Specialist (Fixed)...
copy application_tesla.fam application.fam >nul
ufbt build
if exist build\predator_tesla.fap (
    copy build\predator_tesla.fap fixed_package\predator_tesla_vip.fap >nul
    echo ✅ SUCCESS: predator_tesla_vip.fap
) else (
    echo ❌ FAILED: predator_tesla_vip.fap
)

echo.
echo ========================================
echo FIXED BUILD RESULTS
echo ========================================

REM Count successes
set /a success_count=0
if exist fixed_package\predator_master_orchestrator.fap set /a success_count+=1
if exist fixed_package\predator_wifi_specialist.fap set /a success_count+=1
if exist fixed_package\predator_car_specialist.fap set /a success_count+=1
if exist fixed_package\predator_tesla_vip.fap set /a success_count+=1

echo.
echo BUILD RESULTS: %success_count%/4 apps built successfully
echo.
echo FIXED APPS READY:
if exist fixed_package\predator_master_orchestrator.fap echo ✅ predator_master_orchestrator.fap - MASTER BRAIN
if exist fixed_package\predator_wifi_specialist.fap echo ✅ predator_wifi_specialist.fap - WiFi Expert
if exist fixed_package\predator_car_specialist.fap echo ✅ predator_car_specialist.fap - 90+ Car Models
if exist fixed_package\predator_tesla_vip.fap echo ✅ predator_tesla_vip.fap - Tesla VIP Plus

echo.
echo DEPLOYMENT:
echo 1. Copy fixed_package\*.fap to Flipper Zero /apps/Tools/
echo 2. Start with predator_master_orchestrator.fap
echo 3. Use Master to launch specialist apps
echo 4. Multi-app ecosystem ready!

echo.
pause
