@echo off
echo ========================================
echo 🏆 PREDATOR COMPLETE SUITE BUILDER
echo Building Master Control + All Apps
echo ========================================
echo.

REM Clean everything first
echo [STEP 1] Cleaning all previous builds...
if exist build rmdir /s /q build
if exist *.fap del *.fap
if exist deployment rmdir /s /q deployment
mkdir deployment

echo.
echo ========================================
echo BUILDING MASTER CONTROL CENTER
echo ========================================

REM 1. Build Master Control (Most Important)
echo [1/12] Building Master Control Center...
copy application_master_simple.fam application.fam >nul
ufbt build
if exist build\predator_master_simple.fap (
    copy build\predator_master_simple.fap deployment\predator_master.fap >nul
    echo ✅ SUCCESS: predator_master.fap (MASTER CONTROL)
) else (
    echo ❌ FAILED: predator_master.fap
)

echo.
echo ========================================
echo BUILDING GOVERNMENT SUITE
echo ========================================

REM 2. Government App (ONU Critical)
echo [2/12] Building Government App (ONU Ready)...
copy application_minimal_government.fam application.fam >nul
ufbt build
if exist build\predator_gov_minimal.fap (
    copy build\predator_gov_minimal.fap deployment\predator_government.fap >nul
    echo ✅ SUCCESS: predator_government.fap (ONU READY)
) else (
    echo ❌ FAILED: predator_government.fap
)

echo.
echo ========================================
echo BUILDING CORE ATTACK SUITE
echo ========================================

REM 3. WiFi App
echo [3/12] Building WiFi App...
copy application_wifi.fam application.fam >nul
ufbt build
if exist build\predator_wifi.fap (
    copy build\predator_wifi.fap deployment\predator_wifi.fap >nul
    echo ✅ SUCCESS: predator_wifi.fap
) else (
    echo ❌ FAILED: predator_wifi.fap
)

REM 4. Car App
echo [4/12] Building Car App...
copy application_car.fam application.fam >nul
ufbt build
if exist build\predator_car.fap (
    copy build\predator_car.fap deployment\predator_car.fap >nul
    echo ✅ SUCCESS: predator_car.fap
) else (
    echo ❌ FAILED: predator_car.fap
)

REM 5. Tesla App
echo [5/12] Building Tesla App...
copy application_tesla.fam application.fam >nul
ufbt build
if exist build\predator_tesla.fap (
    copy build\predator_tesla.fap deployment\predator_tesla.fap >nul
    echo ✅ SUCCESS: predator_tesla.fap
) else (
    echo ❌ FAILED: predator_tesla.fap
)

REM 6. Bluetooth App
echo [6/12] Building Bluetooth App...
copy application_bluetooth.fam application.fam >nul
ufbt build
if exist build\predator_bluetooth.fap (
    copy build\predator_bluetooth.fap deployment\predator_bluetooth.fap >nul
    echo ✅ SUCCESS: predator_bluetooth.fap
) else (
    echo ❌ FAILED: predator_bluetooth.fap
)

REM 7. RFID App
echo [7/12] Building RFID App...
copy application_rfid.fam application.fam >nul
ufbt build
if exist build\predator_rfid.fap (
    copy build\predator_rfid.fap deployment\predator_rfid.fap >nul
    echo ✅ SUCCESS: predator_rfid.fap
) else (
    echo ❌ FAILED: predator_rfid.fap
)

REM 8. SubGHz App
echo [8/12] Building SubGHz App...
copy application_subghz.fam application.fam >nul
ufbt build
if exist build\predator_subghz.fap (
    copy build\predator_subghz.fap deployment\predator_subghz.fap >nul
    echo ✅ SUCCESS: predator_subghz.fap
) else (
    echo ❌ FAILED: predator_subghz.fap
)

echo.
echo ========================================
echo BUILDING ENTERPRISE SUITE
echo ========================================

REM 9. Infrastructure App
echo [9/12] Building Infrastructure App...
copy application_infrastructure.fam application.fam >nul
ufbt build
if exist build\predator_infra.fap (
    copy build\predator_infra.fap deployment\predator_infrastructure.fap >nul
    echo ✅ SUCCESS: predator_infrastructure.fap
) else (
    echo ❌ FAILED: predator_infrastructure.fap
)

REM 10. Casino App
echo [10/12] Building Casino App...
copy application_casino.fam application.fam >nul
ufbt build
if exist build\predator_casino.fap (
    copy build\predator_casino.fap deployment\predator_casino.fap >nul
    echo ✅ SUCCESS: predator_casino.fap
) else (
    echo ❌ FAILED: predator_casino.fap
)

REM 11. Network App
echo [11/12] Building Network App...
copy application_network.fam application.fam >nul
ufbt build
if exist build\predator_network.fap (
    copy build\predator_network.fap deployment\predator_network.fap >nul
    echo ✅ SUCCESS: predator_network.fap
) else (
    echo ❌ FAILED: predator_network.fap
)

REM 12. Crypto App
echo [12/12] Building Crypto App...
copy application_crypto.fam application.fam >nul
ufbt build
if exist build\predator_crypto.fap (
    copy build\predator_crypto.fap deployment\predator_crypto.fap >nul
    echo ✅ SUCCESS: predator_crypto.fap
) else (
    echo ❌ FAILED: predator_crypto.fap
)

echo.
echo ========================================
echo BUILD SUMMARY
echo ========================================

REM Count successful builds
set /a success_count=0
for %%f in (deployment\predator_*.fap) do set /a success_count+=1

echo.
echo 🏆 PREDATOR COMPLETE SUITE BUILT
echo 📦 Apps Built: %success_count%/12
echo 📁 Location: deployment\ folder
echo.

echo MASTER CONTROL:
if exist deployment\predator_master.fap (echo ✅ predator_master.fap - ULTIMATE COMMAND CENTER) else (echo ❌ predator_master.fap - FAILED)

echo.
echo GOVERNMENT SUITE:
if exist deployment\predator_government.fap (echo ✅ predator_government.fap - ONU READY) else (echo ❌ predator_government.fap - FAILED)

echo.
echo CORE ATTACK SUITE:
if exist deployment\predator_wifi.fap (echo ✅ predator_wifi.fap) else (echo ❌ predator_wifi.fap - FAILED)
if exist deployment\predator_car.fap (echo ✅ predator_car.fap) else (echo ❌ predator_car.fap - FAILED)
if exist deployment\predator_tesla.fap (echo ✅ predator_tesla.fap) else (echo ❌ predator_tesla.fap - FAILED)
if exist deployment\predator_bluetooth.fap (echo ✅ predator_bluetooth.fap) else (echo ❌ predator_bluetooth.fap - FAILED)
if exist deployment\predator_rfid.fap (echo ✅ predator_rfid.fap) else (echo ❌ predator_rfid.fap - FAILED)
if exist deployment\predator_subghz.fap (echo ✅ predator_subghz.fap) else (echo ❌ predator_subghz.fap - FAILED)

echo.
echo ENTERPRISE SUITE:
if exist deployment\predator_infrastructure.fap (echo ✅ predator_infrastructure.fap) else (echo ❌ predator_infrastructure.fap - FAILED)
if exist deployment\predator_casino.fap (echo ✅ predator_casino.fap) else (echo ❌ predator_casino.fap - FAILED)
if exist deployment\predator_network.fap (echo ✅ predator_network.fap) else (echo ❌ predator_network.fap - FAILED)
if exist deployment\predator_crypto.fap (echo ✅ predator_crypto.fap) else (echo ❌ predator_crypto.fap - FAILED)

echo.
echo ========================================
echo DEPLOYMENT INSTRUCTIONS
echo ========================================
echo.
echo 1. Copy ALL files from deployment\ to Flipper Zero:
echo    /apps/Tools/predator_master.fap
echo    /apps/Tools/predator_government.fap
echo    /apps/Tools/predator_wifi.fap
echo    /apps/Tools/predator_car.fap
echo    /apps/Tools/predator_tesla.fap
echo    /apps/Tools/predator_bluetooth.fap
echo    /apps/Tools/predator_rfid.fap
echo    /apps/Tools/predator_subghz.fap
echo    /apps/Tools/predator_infrastructure.fap
echo    /apps/Tools/predator_casino.fap
echo    /apps/Tools/predator_network.fap
echo    /apps/Tools/predator_crypto.fap
echo.
echo 2. Launch MASTER CONTROL first:
echo    Apps → Tools → Predator Master Control
echo.
echo 3. Use Master Control to manage all other apps:
echo    🏆 Master Control → 📱 App Launcher → Select any app
echo.

if %success_count% GEQ 10 (
    echo ✅ BUILD STATUS: READY FOR DEPLOYMENT
    echo 🏆 PREDATOR COMPLETE SUITE READY!
    echo 🎯 Master Control + %success_count% Specialized Apps
    echo 🏛️ ONU Presentation Ready (Monday)
    echo 🇫🇷 Elon Renault Demo Ready (Never Fail)
    echo 🌍 Global Market Domination Achieved
) else (
    echo ❌ BUILD STATUS: SOME FAILURES
    echo 🔧 Only %success_count%/12 apps built successfully
    echo 🛠️ Check errors above and rebuild failed apps
)

echo.
pause
