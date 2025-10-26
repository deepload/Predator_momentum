@echo off
echo ========================================
echo PREDATOR MOMENTUM - WITH LOGGING BUILD
echo Development/Debug Version
echo ========================================
echo.

echo Temporarily disabling NO_LOGGING flag...

REM Backup current application.fam
copy application.fam application.fam.backup

REM Remove NO_LOGGING flag for this build
powershell -Command "(Get-Content application.fam) -replace ', \"NO_LOGGING=1\"', '' | Set-Content application.fam"

echo Building with logging enabled for debugging...
ufbt build

REM Restore original application.fam
move application.fam.backup application.fam

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ========================================
    echo BUILD SUCCESS - WITH LOGGING VERSION
    echo ========================================
    echo.
    echo This version includes:
    echo - Full FURI_LOG output for debugging
    echo - predator_log_append for live monitoring
    echo - All format strings included
    echo.
    echo Use for development and debugging only!
) else (
    echo.
    echo ========================================
    echo BUILD FAILED
    echo ========================================
    echo Check the error messages above
)

pause
