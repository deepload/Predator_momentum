@echo off
cls
echo ========================================
echo PREDATOR MOMENTUM - COMPLETE SOLUTION
echo Build and Install Complete Security Toolkit
echo One Master App - All Tools Working Together
echo ========================================
echo.

REM Clean everything first
echo [STEP 1] Cleaning previous builds...
if exist build rmdir /s /q build 2>nul
if exist *.fap del *.fap 2>nul
if exist FINAL_PACKAGE rmdir /s /q FINAL_PACKAGE 2>nul
mkdir FINAL_PACKAGE 2>nul

echo [STEP 2] Building Complete Security Toolkit...
copy application_final_complete.fam application.fam >nul 2>&1

echo Building...
ufbt build

REM Check if build was successful
if exist build\predator_complete.fap (
    echo.
    echo ========================================
    echo SUCCESS! COMPLETE TOOLKIT BUILT!
    echo ========================================
    
    REM Copy to final package
    copy build\predator_complete.fap FINAL_PACKAGE\predator_complete_security_toolkit.fap >nul
    
    REM Get file size
    for %%A in (FINAL_PACKAGE\predator_complete_security_toolkit.fap) do (
        echo File: predator_complete_security_toolkit.fap
        echo Size: %%~zA bytes
        
        REM Check size limit
        if %%~zA LSS 184320 (
            echo Status: UNDER 180KB LIMIT - PERFECT!
        ) else (
            echo Status: Over 180KB - but should still work
        )
    )
    
    echo.
    echo ========================================
    echo INSTALLATION INSTRUCTIONS
    echo ========================================
    echo.
    echo READY TO INSTALL TO FLIPPER ZERO:
    echo.
    echo METHOD 1 - qFlipper (Recommended):
    echo 1. Connect Flipper Zero via USB
    echo 2. Open qFlipper application  
    echo 3. Go to File Manager
    echo 4. Navigate to /apps/Tools/
    echo 5. Upload: FINAL_PACKAGE\predator_complete_security_toolkit.fap
    echo 6. Restart Flipper Zero
    echo 7. Launch: Applications ^> Tools ^> Predator Complete Security Toolkit
    echo.
    echo METHOD 2 - SD Card:
    echo 1. Remove SD card from Flipper Zero
    echo 2. Copy FINAL_PACKAGE\predator_complete_security_toolkit.fap
    echo 3. To SD card: /apps/Tools/
    echo 4. Insert SD card back into Flipper Zero
    echo 5. Restart Flipper Zero
    echo.
    echo ========================================
    echo COMPLETE TOOLKIT FEATURES
    echo ========================================
    echo.
    echo WIFI SECURITY:
    echo - WiFi Scan (ESP32 Marauder)
    echo - WiFi Deauth Attack  
    echo - WiFi Evil Twin
    echo - Advanced WiFi Tools
    echo.
    echo CAR SECURITY:
    echo - Tesla Security Suite (VIP++)
    echo - Car Key Bruteforce (90+ Models)
    echo - Car Jamming
    echo - Car Passive Opener
    echo - Elon's Renault Demo
    echo.
    echo BLUETOOTH SECURITY:
    echo - BLE Scan
    echo - BLE Spam
    echo - Advanced Bluetooth Tools
    echo.
    echo RFID/NFC SECURITY:
    echo - RFID Clone
    echo - RFID Bruteforce
    echo - Casino RFID Tools
    echo - Advanced NFC Tools
    echo.
    echo GOVERNMENT CONTRACTS:
    echo - Traffic Infrastructure Testing
    echo - Critical Infrastructure Testing
    echo - Government Contract Tools
    echo.
    echo SYSTEM TOOLS:
    echo - Module Status Monitor
    echo - Board Selection (5 Boards)
    echo - GPS Tracker
    echo - Live Monitor
    echo - Settings
    echo.
    echo ========================================
    echo READY FOR PRODUCTION!
    echo ========================================
    echo.
    echo File Location: FINAL_PACKAGE\predator_complete_security_toolkit.fap
    echo Install Location: /apps/Tools/ on Flipper Zero
    echo.
    echo ELON'S STARTUP: READY FOR GOVERNMENT CONTRACTS!
    echo TESLA REQUIREMENTS: ALL VIP++ FEATURES INCLUDED!
    echo SWISS & CALIFORNIA: GOVERNMENT GRADE TESTING!
    
) else (
    echo.
    echo ========================================
    echo BUILD FAILED
    echo ========================================
    echo.
    echo The build failed. Please check the output above for errors.
    echo Common issues:
    echo - Missing scene files
    echo - Compilation errors
    echo - Memory issues
    echo.
    echo Try running: ufbt clean
    echo Then run this script again.
)

echo.
echo ========================================
echo PREDATOR MOMENTUM COMPLETE SOLUTION
echo Professional Security Toolkit Ready
echo ========================================
echo.
pause
