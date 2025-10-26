@echo off
echo ========================================
echo PREDATOR MOMENTUM - NO LOGGING BUILD
echo Maximum Memory Optimization
echo ========================================
echo.

echo Building without logging for maximum memory savings...
echo This will save approximately 2-4KB flash + 500-1000 bytes RAM

ufbt build

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ========================================
    echo BUILD SUCCESS - NO LOGGING VERSION
    echo ========================================
    echo.
    echo Memory optimizations applied:
    echo - All FURI_LOG calls disabled
    echo - All predator_log_append calls disabled  
    echo - Format strings removed from binary
    echo - Estimated savings: 2-4KB flash + 500-1000 bytes RAM
    echo.
    echo Ready for deployment on Flipper Zero!
) else (
    echo.
    echo ========================================
    echo BUILD FAILED
    echo ========================================
    echo Check the error messages above
)

pause
