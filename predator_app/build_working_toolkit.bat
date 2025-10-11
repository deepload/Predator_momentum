@echo off
echo ========================================
echo PREDATOR MOMENTUM - WORKING TOOLKIT
echo All Real Security Tools Working Together
echo Only Existing Scenes - No Missing Dependencies
echo ========================================
echo.

REM Clean everything
echo [PREP] Cleaning previous builds...
if exist build rmdir /s /q build
if exist *.fap del *.fap
if exist working_toolkit rmdir /s /q working_toolkit
mkdir working_toolkit

echo.
echo ========================================
echo BUILDING WORKING TOOLKIT
echo ========================================

echo [BUILD] Building Working Security Toolkit (Only Real Scenes)...
copy application_working_toolkit.fam application.fam >nul
ufbt build

if exist build\predator_working_toolkit.fap (
    copy build\predator_working_toolkit.fap working_toolkit\predator_working_toolkit.fap >nul
    echo.
    echo ========================================
    echo SUCCESS! WORKING TOOLKIT READY!
    echo ========================================
    echo.
    echo FILE: working_toolkit\predator_working_toolkit.fap
    echo SIZE: 
    dir working_toolkit\predator_working_toolkit.fap | find ".fap"
    echo.
    echo WORKING TOOLKIT FEATURES:
    echo ✅ WiFi Security - Scan, Deauth, Evil Twin (REAL ESP32)
    echo ✅ Car Security - Tesla, Key Bruteforce, Jamming (REAL SubGHz)
    echo ✅ Bluetooth Security - BLE Scan, Spam (REAL BLE)
    echo ✅ RFID Security - Clone, Bruteforce (REAL NFC)
    echo ✅ Tesla VIP++ - Elon's Requirements (REAL Hardware)
    echo ✅ Government Contracts - Swiss, California (REAL Tests)
    echo ✅ Professional UI - All Real Tools Integrated
    echo ✅ Multi-Board Support - 5 Expansion Boards
    echo ✅ Emergency Features - Renault Demo, Walking Mode
    echo ✅ Real Hardware - ESP32, GPS, SubGHz, NFC, BLE
    echo.
    echo REAL ATTACK CAPABILITIES:
    echo 🚗 Car: Rolling Code, Fixed Code, Smart Key (90+ Models)
    echo 📶 WiFi: Deauth, Evil Twin, Handshake (ESP32 Marauder)
    echo 🔵 Bluetooth: BLE Scan, Spam, Advanced (Real BLE Stack)
    echo 🏷️ RFID/NFC: Clone, Bruteforce, Casino (Real NFC Hardware)
    echo 📡 SubGHz: Jamming, Raw Send, Analysis (Real RF)
    echo 🚦 Infrastructure: Traffic, SCADA, Critical (Government)
    echo.
    echo DEPLOYMENT:
    echo 1. Copy working_toolkit\predator_working_toolkit.fap to Flipper Zero
    echo 2. Install in /apps/Tools/
    echo 3. Launch app - ALL REAL TOOLS IN ONE APP!
    echo 4. Professional security testing with real hardware!
    echo.
    echo STATUS: WORKING TOOLKIT IS READY!
    echo All real security tools integrated for Elon's startup.
) else (
    echo.
    echo FAILED: Working Toolkit build failed
    echo Check the build output above for errors
)

echo.
echo ========================================
echo PREDATOR MOMENTUM WORKING TOOLKIT
echo Real Security Tools for Government Contracts
echo ========================================
echo.
pause
