@echo off
echo ========================================
echo 🏆 PREDATOR MOMENTUM - COMPLETE ECOSYSTEM
echo Building Master Orchestrator + 11 Specialized Apps
echo Professional Inter-App Communication System
echo ========================================
echo.

REM Clean previous builds
echo [PREP] Cleaning previous builds...
if exist build rmdir /s /q build
if exist *.fap del *.fap
if exist deploy mkdir deploy
if exist deploy rmdir /s /q deploy
mkdir deploy

echo.
echo ========================================
echo 🏆 BUILDING MASTER ORCHESTRATOR
echo ========================================

REM Master Control App (The Orchestrator)
echo [MASTER] Building Predator Master Control (Orchestrator)...
copy application_master.fam application.fam >nul
ufbt build
if exist build\predator_master.fap (
    copy build\predator_master.fap deploy\predator_master.fap >nul
    echo ✅ SUCCESS: predator_master.fap - ORCHESTRATOR READY
) else (
    echo ❌ FAILED: predator_master.fap - CRITICAL ERROR
    echo ERROR: Master orchestrator failed to build!
    pause
    exit /b 1
)

echo.
echo ========================================
echo 🏛️ BUILDING GOVERNMENT SUITE
echo ========================================

REM 1. Government App (ONU Ready)
echo [1/11] Building Predator Government (ONU Presentation)...
copy application_government.fam application.fam >nul
ufbt build
if exist build\predator_gov.fap (
    copy build\predator_gov.fap deploy\predator_government.fap >nul
    echo ✅ SUCCESS: predator_government.fap
) else (
    echo ❌ FAILED: predator_government.fap
)

echo.
echo ========================================
echo 🚀 BUILDING CORE ATTACK SUITE
echo ========================================

REM 2. WiFi App
echo [2/11] Building Predator WiFi...
copy application_wifi.fam application.fam >nul
ufbt build
if exist build\predator_wifi.fap (
    copy build\predator_wifi.fap deploy\predator_wifi.fap >nul
    echo ✅ SUCCESS: predator_wifi.fap
) else (
    echo ❌ FAILED: predator_wifi.fap
)

REM 3. Car App
echo [3/11] Building Predator Car...
copy application_car.fam application.fam >nul
ufbt build
if exist build\predator_car.fap (
    copy build\predator_car.fap deploy\predator_car.fap >nul
    echo ✅ SUCCESS: predator_car.fap
) else (
    echo ❌ FAILED: predator_car.fap
)

REM 4. Tesla App
echo [4/11] Building Predator Tesla...
copy application_tesla.fam application.fam >nul
ufbt build
if exist build\predator_tesla.fap (
    copy build\predator_tesla.fap deploy\predator_tesla.fap >nul
    echo ✅ SUCCESS: predator_tesla.fap
) else (
    echo ❌ FAILED: predator_tesla.fap
)

REM 5. Bluetooth App
echo [5/11] Building Predator Bluetooth...
copy application_bluetooth.fam application.fam >nul
ufbt build
if exist build\predator_bluetooth.fap (
    copy build\predator_bluetooth.fap deploy\predator_bluetooth.fap >nul
    echo ✅ SUCCESS: predator_bluetooth.fap
) else (
    echo ❌ FAILED: predator_bluetooth.fap
)

REM 6. RFID App
echo [6/11] Building Predator RFID...
copy application_rfid.fam application.fam >nul
ufbt build
if exist build\predator_rfid.fap (
    copy build\predator_rfid.fap deploy\predator_rfid.fap >nul
    echo ✅ SUCCESS: predator_rfid.fap
) else (
    echo ❌ FAILED: predator_rfid.fap
)

REM 7. SubGHz App
echo [7/11] Building Predator SubGHz...
copy application_subghz.fam application.fam >nul
ufbt build
if exist build\predator_subghz.fap (
    copy build\predator_subghz.fap deploy\predator_subghz.fap >nul
    echo ✅ SUCCESS: predator_subghz.fap
) else (
    echo ❌ FAILED: predator_subghz.fap
)

echo.
echo ========================================
echo 🏢 BUILDING ENTERPRISE SUITE
echo ========================================

REM 8. Infrastructure App
echo [8/11] Building Predator Infrastructure...
copy application_infrastructure.fam application.fam >nul
ufbt build
if exist build\predator_infra.fap (
    copy build\predator_infra.fap deploy\predator_infrastructure.fap >nul
    echo ✅ SUCCESS: predator_infrastructure.fap
) else (
    echo ❌ FAILED: predator_infrastructure.fap
)

REM 9. Casino App
echo [9/11] Building Predator Casino...
copy application_casino.fam application.fam >nul
ufbt build
if exist build\predator_casino.fap (
    copy build\predator_casino.fap deploy\predator_casino.fap >nul
    echo ✅ SUCCESS: predator_casino.fap
) else (
    echo ❌ FAILED: predator_casino.fap
)

REM 10. Network App
echo [10/11] Building Predator Network...
copy application_network.fam application.fam >nul
ufbt build
if exist build\predator_network.fap (
    copy build\predator_network.fap deploy\predator_network.fap >nul
    echo ✅ SUCCESS: predator_network.fap
) else (
    echo ❌ FAILED: predator_network.fap
)

REM 11. Crypto App
echo [11/11] Building Predator Crypto...
copy application_crypto.fam application.fam >nul
ufbt build
if exist build\predator_crypto.fap (
    copy build\predator_crypto.fap deploy\predator_crypto.fap >nul
    echo ✅ SUCCESS: predator_crypto.fap
) else (
    echo ❌ FAILED: predator_crypto.fap
)

echo.
echo ========================================
echo 📊 ECOSYSTEM BUILD SUMMARY
echo ========================================
echo.

REM Count successful builds
set /a success_count=0
set /a total_count=12

echo 🏆 MASTER ORCHESTRATOR:
if exist deploy\predator_master.fap (
    echo ✅ predator_master.fap - ORCHESTRATOR READY
    set /a success_count+=1
) else (
    echo ❌ predator_master.fap - CRITICAL FAILURE
)

echo.
echo 🏛️ GOVERNMENT SUITE:
if exist deploy\predator_government.fap (
    echo ✅ predator_government.fap - ONU Ready
    set /a success_count+=1
) else (
    echo ❌ predator_government.fap - FAILED
)

echo.
echo 🚀 CORE ATTACK SUITE:
if exist deploy\predator_wifi.fap (
    echo ✅ predator_wifi.fap
    set /a success_count+=1
) else (
    echo ❌ predator_wifi.fap - FAILED
)
if exist deploy\predator_car.fap (
    echo ✅ predator_car.fap
    set /a success_count+=1
) else (
    echo ❌ predator_car.fap - FAILED
)
if exist deploy\predator_tesla.fap (
    echo ✅ predator_tesla.fap
    set /a success_count+=1
) else (
    echo ❌ predator_tesla.fap - FAILED
)
if exist deploy\predator_bluetooth.fap (
    echo ✅ predator_bluetooth.fap
    set /a success_count+=1
) else (
    echo ❌ predator_bluetooth.fap - FAILED
)
if exist deploy\predator_rfid.fap (
    echo ✅ predator_rfid.fap
    set /a success_count+=1
) else (
    echo ❌ predator_rfid.fap - FAILED
)
if exist deploy\predator_subghz.fap (
    echo ✅ predator_subghz.fap
    set /a success_count+=1
) else (
    echo ❌ predator_subghz.fap - FAILED
)

echo.
echo 🏢 ENTERPRISE SUITE:
if exist deploy\predator_infrastructure.fap (
    echo ✅ predator_infrastructure.fap
    set /a success_count+=1
) else (
    echo ❌ predator_infrastructure.fap - FAILED
)
if exist deploy\predator_casino.fap (
    echo ✅ predator_casino.fap
    set /a success_count+=1
) else (
    echo ❌ predator_casino.fap - FAILED
)
if exist deploy\predator_network.fap (
    echo ✅ predator_network.fap
    set /a success_count+=1
) else (
    echo ❌ predator_network.fap - FAILED
)
if exist deploy\predator_crypto.fap (
    echo ✅ predator_crypto.fap
    set /a success_count+=1
) else (
    echo ❌ predator_crypto.fap - FAILED
)

echo.
echo ========================================
echo 🎯 ECOSYSTEM DEPLOYMENT STATUS
echo ========================================
echo.
echo 📊 BUILD RESULTS: %success_count%/%total_count% apps built successfully
echo.

if %success_count% == %total_count% (
    echo 🏆 PERFECT BUILD! ALL APPS READY FOR DEPLOYMENT
    echo.
    echo 🚀 MASTER ORCHESTRATOR CAPABILITIES:
    echo    ✅ Inter-App Communication System
    echo    ✅ Real-Time App Coordination
    echo    ✅ Professional Workflow Management
    echo    ✅ Government-Grade Orchestration
    echo    ✅ Emergency Mode Coordination
    echo    ✅ VIP Mode Multi-App Control
    echo.
    echo 📁 All apps available in deploy\ directory
    echo 📋 Copy all predator_*.fap files to Flipper Zero /apps/Tools/
    echo.
    echo 🎯 DEPLOYMENT INSTRUCTIONS:
    echo 1. Start with predator_master.fap (The Orchestrator)
    echo 2. Install all specialized apps in /apps/Tools/
    echo 3. Use Master Control to orchestrate all operations
    echo 4. Enjoy professional inter-app communication!
) else (
    echo ⚠️  PARTIAL BUILD - Some apps failed to compile
    echo 🔧 Check build logs for failed apps
    echo 💡 Master orchestrator can still coordinate available apps
)

echo.
echo ========================================
echo 🏆 PREDATOR MOMENTUM ECOSYSTEM READY
echo Professional Inter-App Communication
echo Government-Grade Multi-App Orchestration
echo ========================================
echo.
pause
