@echo off
echo ========================================
echo PREDATOR MOMENTUM - COMPLETE PRODUCT SUITE
echo Building 11 Specialized Security Apps
echo ========================================
echo.

REM Clean previous builds
echo [1/11] Cleaning previous builds...
if exist build rmdir /s /q build
if exist *.fap del *.fap

echo.
echo ========================================
echo BUILDING GOVERNMENT SUITE
echo ========================================

REM 1. Government App (ONU Ready)
echo [1/11] Building Predator Government (ONU Presentation)...
copy application_government.fam application.fam >nul
ufbt build
if exist build\predator_gov.fap (
    copy build\predator_gov.fap predator_government.fap >nul
    echo ✅ SUCCESS: predator_government.fap
) else (
    echo ❌ FAILED: predator_government.fap
)

REM 2. WiFi App
echo [2/11] Building Predator WiFi...
copy application_wifi.fam application.fam >nul
ufbt build
if exist build\predator_wifi.fap (
    copy build\predator_wifi.fap predator_wifi.fap >nul
    echo ✅ SUCCESS: predator_wifi.fap
) else (
    echo ❌ FAILED: predator_wifi.fap
)

REM 3. Car App
echo [3/11] Building Predator Car...
copy application_car.fam application.fam >nul
ufbt build
if exist build\predator_car.fap (
    copy build\predator_car.fap predator_car.fap >nul
    echo ✅ SUCCESS: predator_car.fap
) else (
    echo ❌ FAILED: predator_car.fap
)

REM 4. Tesla App
echo [4/11] Building Predator Tesla...
copy application_tesla.fam application.fam >nul
ufbt build
if exist build\predator_tesla.fap (
    copy build\predator_tesla.fap predator_tesla.fap >nul
    echo ✅ SUCCESS: predator_tesla.fap
) else (
    echo ❌ FAILED: predator_tesla.fap
)

echo.
echo ========================================
echo BUILDING SPECIALIZED SUITE
echo ========================================

REM 5. Bluetooth App
echo [5/11] Building Predator Bluetooth...
copy application_bluetooth.fam application.fam >nul
ufbt build
if exist build\predator_bluetooth.fap (
    copy build\predator_bluetooth.fap predator_bluetooth.fap >nul
    echo ✅ SUCCESS: predator_bluetooth.fap
) else (
    echo ❌ FAILED: predator_bluetooth.fap
)

REM 6. RFID App
echo [6/11] Building Predator RFID...
copy application_rfid.fam application.fam >nul
ufbt build
if exist build\predator_rfid.fap (
    copy build\predator_rfid.fap predator_rfid.fap >nul
    echo ✅ SUCCESS: predator_rfid.fap
) else (
    echo ❌ FAILED: predator_rfid.fap
)

REM 7. SubGHz App
echo [7/11] Building Predator SubGHz...
copy application_subghz.fam application.fam >nul
ufbt build
if exist build\predator_subghz.fap (
    copy build\predator_subghz.fap predator_subghz.fap >nul
    echo ✅ SUCCESS: predator_subghz.fap
) else (
    echo ❌ FAILED: predator_subghz.fap
)

REM 8. Infrastructure App
echo [8/11] Building Predator Infrastructure...
copy application_infrastructure.fam application.fam >nul
ufbt build
if exist build\predator_infra.fap (
    copy build\predator_infra.fap predator_infrastructure.fap >nul
    echo ✅ SUCCESS: predator_infrastructure.fap
) else (
    echo ❌ FAILED: predator_infrastructure.fap
)

echo.
echo ========================================
echo BUILDING ENTERPRISE SUITE
echo ========================================

REM 9. Casino App
echo [9/11] Building Predator Casino...
copy application_casino.fam application.fam >nul
ufbt build
if exist build\predator_casino.fap (
    copy build\predator_casino.fap predator_casino.fap >nul
    echo ✅ SUCCESS: predator_casino.fap
) else (
    echo ❌ FAILED: predator_casino.fap
)

REM 10. Network App
echo [10/11] Building Predator Network...
copy application_network.fam application.fam >nul
ufbt build
if exist build\predator_network.fap (
    copy build\predator_network.fap predator_network.fap >nul
    echo ✅ SUCCESS: predator_network.fap
) else (
    echo ❌ FAILED: predator_network.fap
)

REM 11. Crypto App
echo [11/11] Building Predator Crypto...
copy application_crypto.fam application.fam >nul
ufbt build
if exist build\predator_crypto.fap (
    copy build\predator_crypto.fap predator_crypto.fap >nul
    echo ✅ SUCCESS: predator_crypto.fap
) else (
    echo ❌ FAILED: predator_crypto.fap
)

echo.
echo ========================================
echo BUILD SUMMARY
echo ========================================
echo.
echo GOVERNMENT SUITE:
if exist predator_government.fap (echo ✅ predator_government.fap - ONU Ready) else (echo ❌ predator_government.fap - FAILED)

echo.
echo CORE ATTACK SUITE:
if exist predator_wifi.fap (echo ✅ predator_wifi.fap) else (echo ❌ predator_wifi.fap - FAILED)
if exist predator_car.fap (echo ✅ predator_car.fap) else (echo ❌ predator_car.fap - FAILED)
if exist predator_tesla.fap (echo ✅ predator_tesla.fap) else (echo ❌ predator_tesla.fap - FAILED)
if exist predator_bluetooth.fap (echo ✅ predator_bluetooth.fap) else (echo ❌ predator_bluetooth.fap - FAILED)
if exist predator_rfid.fap (echo ✅ predator_rfid.fap) else (echo ❌ predator_rfid.fap - FAILED)
if exist predator_subghz.fap (echo ✅ predator_subghz.fap) else (echo ❌ predator_subghz.fap - FAILED)

echo.
echo ENTERPRISE SUITE:
if exist predator_infrastructure.fap (echo ✅ predator_infrastructure.fap) else (echo ❌ predator_infrastructure.fap - FAILED)
if exist predator_casino.fap (echo ✅ predator_casino.fap) else (echo ❌ predator_casino.fap - FAILED)
if exist predator_network.fap (echo ✅ predator_network.fap) else (echo ❌ predator_network.fap - FAILED)
if exist predator_crypto.fap (echo ✅ predator_crypto.fap) else (echo ❌ predator_crypto.fap - FAILED)

echo.
echo ========================================
echo DEPLOYMENT READY
echo ========================================
echo.
echo 🏆 PREDATOR MOMENTUM - COMPLETE PRODUCT SUITE
echo 📦 11 Specialized Security Applications Built
echo 🎯 Ready for Market Domination
echo 🏛️ ONU Presentation: predator_government.fap
echo.
echo Copy all predator_*.fap files to Flipper Zero /apps/Tools/
echo.
pause
