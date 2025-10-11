@echo off
echo ========================================
echo 🏆 PREDATOR MASTER CONTROL CENTER
echo Building Ultimate Command & Control App
echo ========================================
echo.

REM Clean previous builds
echo [STEP 1] Cleaning previous builds...
if exist build rmdir /s /q build
if exist predator_master.fap del predator_master.fap

echo.
echo ========================================
echo BUILDING MASTER CONTROL CENTER
echo ========================================

echo [STEP 2] Building Master Control App...
copy application_master.fam application.fam >nul
ufbt build

if exist build\predator_master.fap (
    copy build\predator_master.fap predator_master.fap >nul
    echo ✅ SUCCESS: predator_master.fap
    echo.
    echo ========================================
    echo MASTER CONTROL CENTER READY
    echo ========================================
    echo.
    echo 🏆 PREDATOR MASTER CONTROL CENTER
    echo 📱 Manages all 11 specialized apps
    echo 📊 Real-time system monitoring
    echo 🚨 Emergency controls included
    echo 🇫🇷 Elon Renault demo integrated
    echo 🏛️ Government mode ready
    echo.
    echo FEATURES:
    echo ✅ Master Control Dashboard
    echo ✅ App Launcher (11 specialized apps)
    echo ✅ System Monitor (real-time)
    echo ✅ Emergency Mode (max power)
    echo ✅ Elon Renault Demo (never fail)
    echo ✅ Government Suite (ONU ready)
    echo ✅ All Attack Suites (WiFi, Car, Tesla, etc.)
    echo.
    echo DEPLOYMENT:
    echo Copy predator_master.fap to Flipper Zero /apps/Tools/
    echo.
    echo 🏆 ULTIMATE SECURITY COMMAND CENTER READY!
) else (
    echo ❌ FAILED: predator_master.fap
    echo.
    echo BUILD FAILED - Check for errors above
)

echo.
pause
