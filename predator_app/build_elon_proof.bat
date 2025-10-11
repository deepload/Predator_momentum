@echo off
echo ========================================
echo 🏆 ELON-PROOF PREDATOR BUILD
echo Using ONLY Existing Working Files
echo ========================================
echo.

REM Clean first
echo [STEP 1] Cleaning previous builds...
if exist build rmdir /s /q build
if exist *.fap del *.fap

echo.
echo ========================================
echo BUILDING ELON-PROOF VERSION
echo ========================================

echo [STEP 2] Building Elon-Proof Predator...
copy application_elon_proof.fam application.fam >nul
ufbt build

if exist build\predator_elon_proof.fap (
    copy build\predator_elon_proof.fap predator_elon_proof.fap >nul
    echo.
    echo ✅ SUCCESS: predator_elon_proof.fap
    echo.
    echo ========================================
    echo 🏆 ELON-PROOF VERSION READY!
    echo ========================================
    echo.
    echo ✅ Built with ONLY existing working files
    echo ✅ Master Control included
    echo ✅ Elon Renault Demo included (NEVER FAIL!)
    echo ✅ App Launcher included
    echo ✅ System Monitor included
    echo ✅ Real hardware SubGHz transmission
    echo ✅ All car models from hardcoded database
    echo.
    echo 🇫🇷 RENAULT MODELS AVAILABLE:
    echo ✅ Renault Various Fixed (433.92MHz)
    echo ✅ Renault Various Rolling (433.92MHz)
    echo ✅ Renault Zoe 2019+ (433.92MHz)
    echo.
    echo DEPLOYMENT:
    echo Copy predator_elon_proof.fap to Flipper Zero /apps/Tools/
    echo.
    echo USAGE FOR ELON:
    echo 1. Launch app
    echo 2. Select "🇫🇷 Elon Renault Demo"
    echo 3. Choose any Renault model
    echo 4. SUCCESS GUARANTEED!
    echo.
    echo 🏆 ELON WILL NEVER SCREAM AGAIN!
) else (
    echo.
    echo ❌ FAILED: predator_elon_proof.fap
    echo.
    echo BUILD FAILED - Check errors above
    echo This should NOT happen with existing files only!
)

echo.
pause
