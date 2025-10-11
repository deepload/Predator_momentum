@echo off
echo ========================================
echo 🚀 PREDATOR MOMENTUM - FLIPPER INSTALL
echo Quick Installation to Flipper Zero
echo ========================================
echo.

REM Check if the .fap file exists
if not exist "working_toolkit\predator_working_toolkit.fap" (
    echo ❌ ERROR: predator_working_toolkit.fap not found!
    echo Please run build_working_toolkit.bat first.
    pause
    exit /b 1
)

echo 📁 Found: predator_working_toolkit.fap
echo.

REM Get file size and show info
for %%A in (working_toolkit\predator_working_toolkit.fap) do set filesize=%%~zA
echo 📊 File size: %filesize% bytes

if %filesize% LSS 184320 (
    echo ✅ Size OK: Under 180KB limit
) else (
    echo ⚠️ WARNING: Over 180KB limit
)

echo.
echo ========================================
echo 🔧 INSTALLATION OPTIONS
echo ========================================
echo.
echo Choose your installation method:
echo.
echo [1] qFlipper (Recommended - GUI)
echo [2] SD Card (Manual copy)
echo [3] CLI (Advanced users)
echo [4] Show file location only
echo.
set /p choice="Enter your choice (1-4): "

if "%choice%"=="1" goto qflipper
if "%choice%"=="2" goto sdcard  
if "%choice%"=="3" goto cli
if "%choice%"=="4" goto location
goto invalid

:qflipper
echo.
echo ========================================
echo 📱 qFlipper Installation
echo ========================================
echo.
echo STEPS:
echo 1. Connect Flipper Zero via USB
echo 2. Open qFlipper application
echo 3. Click "File Manager"
echo 4. Navigate to: /apps/Tools/
echo 5. Click "Upload" and select:
echo    %cd%\working_toolkit\predator_working_toolkit.fap
echo 6. Wait for upload to complete
echo 7. Restart Flipper Zero
echo 8. Find app in: Applications > Tools > Predator Working Toolkit
echo.
echo 🚀 Ready to upload!
goto end

:sdcard
echo.
echo ========================================
echo 💾 SD Card Installation  
echo ========================================
echo.
echo STEPS:
echo 1. Remove SD card from Flipper Zero
echo 2. Insert SD card into computer
echo 3. Copy this file:
echo    %cd%\working_toolkit\predator_working_toolkit.fap
echo 4. To SD card location:
echo    /apps/Tools/predator_working_toolkit.fap
echo 5. Safely eject SD card
echo 6. Insert SD card back into Flipper Zero
echo 7. Restart Flipper Zero
echo 8. Find app in: Applications > Tools > Predator Working Toolkit
echo.
echo 💾 Ready to copy!
goto end

:cli
echo.
echo ========================================
echo 💻 CLI Installation (Advanced)
echo ========================================
echo.
echo STEPS:
echo 1. Connect Flipper Zero via USB
echo 2. Open terminal/command prompt
echo 3. Connect to Flipper CLI
echo 4. Run this command:
echo.
echo    storage write /ext/apps/Tools/predator_working_toolkit.fap "%cd%\working_toolkit\predator_working_toolkit.fap"
echo.
echo 5. Wait for transfer to complete
echo 6. Run: power reboot
echo 7. Find app in: Applications > Tools > Predator Working Toolkit
echo.
echo 💻 CLI commands ready!
goto end

:location
echo.
echo ========================================
echo 📁 File Location
echo ========================================
echo.
echo Source file location:
echo %cd%\working_toolkit\predator_working_toolkit.fap
echo.
echo Target location on Flipper Zero:
echo /apps/Tools/predator_working_toolkit.fap
echo.
echo Use any method you prefer to copy the file!
goto end

:invalid
echo.
echo ❌ Invalid choice. Please run the script again.
goto end

:end
echo.
echo ========================================
echo 🏆 INSTALLATION COMPLETE
echo ========================================
echo.
echo After installation, launch the app from:
echo Applications > Tools > Predator Working Toolkit
echo.
echo 🎯 FEATURES READY:
echo ✅ WiFi Security Tools (ESP32 Marauder)
echo ✅ Car Security Tools (90+ Models, Tesla VIP++)  
echo ✅ Bluetooth Security Tools (BLE Scan, Spam)
echo ✅ RFID/NFC Security Tools (Clone, Bruteforce)
echo ✅ Government Contract Tools (Swiss, California)
echo ✅ Emergency Features (Renault Demo, Walking Mode)
echo.
echo 🚀 READY FOR ELON'S STARTUP!
echo.
pause
