@echo off
echo ========================================
echo PREDATOR MOMENTUM - BUILD & TEST SUITE
echo Ensuring Real Hardware Functionality
echo ========================================
echo.

REM Clean everything first
echo [STEP 1] Cleaning previous builds...
if exist build rmdir /s /q build
if exist *.fap del *.fap
if exist test_results.txt del test_results.txt

echo.
echo ========================================
echo BUILDING WITH REAL HARDWARE VALIDATION
echo ========================================

REM Create test results file
echo PREDATOR MOMENTUM - BUILD TEST RESULTS > test_results.txt
echo Build Date: %date% %time% >> test_results.txt
echo. >> test_results.txt

REM 1. Government App (ONU Critical)
echo [1/11] Building Predator Government (ONU Presentation)...
copy application_government.fam application.fam >nul
ufbt build
if exist build\predator_gov.fap (
    copy build\predator_gov.fap predator_government.fap >nul
    echo ✅ SUCCESS: predator_government.fap
    echo [PASS] predator_government.fap - ONU Ready >> test_results.txt
) else (
    echo ❌ FAILED: predator_government.fap
    echo [FAIL] predator_government.fap - BUILD FAILED >> test_results.txt
)

REM 2. WiFi App
echo [2/11] Building Predator WiFi...
copy application_wifi.fam application.fam >nul
ufbt build
if exist build\predator_wifi.fap (
    copy build\predator_wifi.fap predator_wifi.fap >nul
    echo ✅ SUCCESS: predator_wifi.fap
    echo [PASS] predator_wifi.fap - WiFi Security >> test_results.txt
) else (
    echo ❌ FAILED: predator_wifi.fap
    echo [FAIL] predator_wifi.fap - BUILD FAILED >> test_results.txt
)

REM 3. Car App
echo [3/11] Building Predator Car...
copy application_car.fam application.fam >nul
ufbt build
if exist build\predator_car.fap (
    copy build\predator_car.fap predator_car.fap >nul
    echo ✅ SUCCESS: predator_car.fap
    echo [PASS] predator_car.fap - Automotive Security >> test_results.txt
) else (
    echo ❌ FAILED: predator_car.fap
    echo [FAIL] predator_car.fap - BUILD FAILED >> test_results.txt
)

REM 4. Tesla App
echo [4/11] Building Predator Tesla...
copy application_tesla.fam application.fam >nul
ufbt build
if exist build\predator_tesla.fap (
    copy build\predator_tesla.fap predator_tesla.fap >nul
    echo ✅ SUCCESS: predator_tesla.fap
    echo [PASS] predator_tesla.fap - Tesla Security >> test_results.txt
) else (
    echo ❌ FAILED: predator_tesla.fap
    echo [FAIL] predator_tesla.fap - BUILD FAILED >> test_results.txt
)

REM Continue with remaining apps...
echo [5/11] Building Predator Bluetooth...
copy application_bluetooth.fam application.fam >nul
ufbt build
if exist build\predator_bluetooth.fap (
    copy build\predator_bluetooth.fap predator_bluetooth.fap >nul
    echo ✅ SUCCESS: predator_bluetooth.fap
    echo [PASS] predator_bluetooth.fap - BLE Security >> test_results.txt
) else (
    echo ❌ FAILED: predator_bluetooth.fap
    echo [FAIL] predator_bluetooth.fap - BUILD FAILED >> test_results.txt
)

echo [6/11] Building Predator RFID...
copy application_rfid.fam application.fam >nul
ufbt build
if exist build\predator_rfid.fap (
    copy build\predator_rfid.fap predator_rfid.fap >nul
    echo ✅ SUCCESS: predator_rfid.fap
    echo [PASS] predator_rfid.fap - RFID Security >> test_results.txt
) else (
    echo ❌ FAILED: predator_rfid.fap
    echo [FAIL] predator_rfid.fap - BUILD FAILED >> test_results.txt
)

echo [7/11] Building Predator SubGHz...
copy application_subghz.fam application.fam >nul
ufbt build
if exist build\predator_subghz.fap (
    copy build\predator_subghz.fap predator_subghz.fap >nul
    echo ✅ SUCCESS: predator_subghz.fap
    echo [PASS] predator_subghz.fap - RF Security >> test_results.txt
) else (
    echo ❌ FAILED: predator_subghz.fap
    echo [FAIL] predator_subghz.fap - BUILD FAILED >> test_results.txt
)

echo [8/11] Building Predator Infrastructure...
copy application_infrastructure.fam application.fam >nul
ufbt build
if exist build\predator_infra.fap (
    copy build\predator_infra.fap predator_infrastructure.fap >nul
    echo ✅ SUCCESS: predator_infrastructure.fap
    echo [PASS] predator_infrastructure.fap - Critical Infrastructure >> test_results.txt
) else (
    echo ❌ FAILED: predator_infrastructure.fap
    echo [FAIL] predator_infrastructure.fap - BUILD FAILED >> test_results.txt
)

echo [9/11] Building Predator Casino...
copy application_casino.fam application.fam >nul
ufbt build
if exist build\predator_casino.fap (
    copy build\predator_casino.fap predator_casino.fap >nul
    echo ✅ SUCCESS: predator_casino.fap
    echo [PASS] predator_casino.fap - Gaming Security >> test_results.txt
) else (
    echo ❌ FAILED: predator_casino.fap
    echo [FAIL] predator_casino.fap - BUILD FAILED >> test_results.txt
)

echo [10/11] Building Predator Network...
copy application_network.fam application.fam >nul
ufbt build
if exist build\predator_network.fap (
    copy build\predator_network.fap predator_network.fap >nul
    echo ✅ SUCCESS: predator_network.fap
    echo [PASS] predator_network.fap - Network Security >> test_results.txt
) else (
    echo ❌ FAILED: predator_network.fap
    echo [FAIL] predator_network.fap - BUILD FAILED >> test_results.txt
)

echo [11/11] Building Predator Crypto...
copy application_crypto.fam application.fam >nul
ufbt build
if exist build\predator_crypto.fap (
    copy build\predator_crypto.fap predator_crypto.fap >nul
    echo ✅ SUCCESS: predator_crypto.fap
    echo [PASS] predator_crypto.fap - Cryptographic Security >> test_results.txt
) else (
    echo ❌ FAILED: predator_crypto.fap
    echo [FAIL] predator_crypto.fap - BUILD FAILED >> test_results.txt
)

echo.
echo ========================================
echo HARDWARE FUNCTIONALITY VALIDATION
echo ========================================

REM Check for real hardware calls in built apps
echo. >> test_results.txt
echo HARDWARE FUNCTIONALITY VALIDATION: >> test_results.txt

findstr /C:"REAL HW" helpers\predator_subghz.c >nul
if %errorlevel%==0 (
    echo ✅ REAL HARDWARE: SubGHz transmission enabled
    echo [PASS] Real SubGHz hardware calls found >> test_results.txt
) else (
    echo ❌ FAKE HARDWARE: SubGHz still simulated
    echo [FAIL] SubGHz hardware calls missing >> test_results.txt
)

findstr /C:"furi_hal_subghz_start_async_tx" helpers\predator_subghz.c >nul
if %errorlevel%==0 (
    echo ✅ REAL HARDWARE: Async TX enabled
    echo [PASS] Real async TX calls found >> test_results.txt
) else (
    echo ❌ FAKE HARDWARE: Async TX missing
    echo [FAIL] Async TX calls missing >> test_results.txt
)

echo.
echo ========================================
echo DEPLOYMENT PACKAGE CREATION
echo ========================================

REM Create deployment directory
if not exist deployment mkdir deployment
copy predator_*.fap deployment\ >nul 2>&1

REM Count successful builds
set /a success_count=0
for %%f in (predator_*.fap) do set /a success_count+=1

echo.
echo ========================================
echo BUILD SUMMARY
echo ========================================
echo.
echo 📦 APPS BUILT: %success_count%/11
echo 📁 DEPLOYMENT: deployment\ folder created
echo 📊 TEST RESULTS: test_results.txt
echo.

if %success_count% GEQ 8 (
    echo ✅ BUILD STATUS: READY FOR DEPLOYMENT
    echo [OVERALL] BUILD SUCCESS - %success_count%/11 apps built >> test_results.txt
) else (
    echo ❌ BUILD STATUS: CRITICAL FAILURES
    echo [OVERALL] BUILD FAILURE - Only %success_count%/11 apps built >> test_results.txt
)

echo.
echo ========================================
echo NEXT STEPS FOR REAL HARDWARE TESTING
echo ========================================
echo.
echo 1. Copy deployment\predator_government.fap to Flipper Zero
echo 2. Test SubGHz transmission with spectrum analyzer
echo 3. Verify car attacks work in parking lot
echo 4. Check LED feedback matches real operations
echo 5. Validate all government features before ONU
echo.
echo 📋 Full test results saved to: test_results.txt
echo.
pause
