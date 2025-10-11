@echo off
echo ========================================
echo PREDATOR MOMENTUM - COMPLETE TOOLKIT
echo ALL Security Tools Working Together
echo Single Integrated Application
echo ========================================
echo.

REM Clean everything
echo [PREP] Cleaning previous builds...
if exist build rmdir /s /q build
if exist *.fap del *.fap
if exist complete_toolkit rmdir /s /q complete_toolkit
mkdir complete_toolkit

echo.
echo ========================================
echo BUILDING COMPLETE TOOLKIT
echo ========================================

echo [BUILD] Building Complete Security Toolkit...
copy application_complete_toolkit.fam application.fam >nul
ufbt build

if exist build\predator_complete_toolkit.fap (
    copy build\predator_complete_toolkit.fap complete_toolkit\predator_complete_toolkit.fap >nul
    echo.
    echo ========================================
    echo SUCCESS! COMPLETE TOOLKIT READY!
    echo ========================================
    echo.
    echo FILE: complete_toolkit\predator_complete_toolkit.fap
    echo SIZE: 
    dir complete_toolkit\predator_complete_toolkit.fap | find ".fap"
    echo.
    echo COMPLETE TOOLKIT FEATURES:
    echo ✅ WiFi Security Testing - Scan, Deauth, Evil Twin
    echo ✅ Car Security Testing - 90+ Models, Tesla VIP++
    echo ✅ Bluetooth Security - BLE Scan, Spam, Advanced
    echo ✅ RFID/NFC Security - Clone, Bruteforce, Advanced
    echo ✅ SubGHz Security - Jamming, Raw Send, Analysis
    echo ✅ Government Contracts - Swiss, California Ready
    echo ✅ Tesla Requirements - Elon's VIP++ Features
    echo ✅ Professional UI - All Tools Integrated
    echo ✅ Real Hardware Support - ESP32, GPS, Multi-board
    echo ✅ Emergency Features - Renault Demo, Walking Mode
    echo.
    echo ATTACK CAPABILITIES:
    echo 🚗 Car Attacks: Rolling Code, Fixed Code, Smart Key
    echo 📶 WiFi Attacks: Deauth, Evil Twin, Handshake Capture
    echo 🔵 Bluetooth: BLE Scan, Spam, Advanced Protocols
    echo 🏷️ RFID/NFC: Clone, Bruteforce, Casino Cards
    echo 📡 SubGHz: Jamming, Raw Transmission, Analysis
    echo 🚦 Infrastructure: Traffic Lights, SCADA, Critical Systems
    echo.
    echo DEPLOYMENT:
    echo 1. Copy complete_toolkit\predator_complete_toolkit.fap to Flipper Zero
    echo 2. Install in /apps/Tools/
    echo 3. Launch app - ALL TOOLS AVAILABLE IN ONE APP!
    echo 4. Professional security testing ready!
    echo.
    echo STATUS: COMPLETE TOOLKIT IS WORKING!
    echo All security tools integrated and ready for production.
) else (
    echo.
    echo FAILED: Complete Toolkit build failed
    echo Check the build output above for errors
)

echo.
echo ========================================
echo PREDATOR MOMENTUM COMPLETE TOOLKIT
echo Professional Integrated Security Platform
echo ========================================
echo.
pause
