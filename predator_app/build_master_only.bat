@echo off
echo ========================================
echo PREDATOR MOMENTUM - MASTER ORCHESTRATOR
echo Building Single Master App with Multi-App Control
echo ========================================
echo.

REM Clean previous builds
echo [PREP] Cleaning previous builds...
if exist build rmdir /s /q build
if exist *.fap del *.fap

echo.
echo [MASTER] Building Master Orchestrator...

REM Copy the master orchestrator .fam file
copy application_master_orchestrator.fam application.fam

REM Build with specific app ID
ufbt build APPID=predator_master_orchestrator

if exist build\predator_master_orchestrator.fap (
    echo SUCCESS: Master Orchestrator built successfully!
    echo File: build\predator_master_orchestrator.fap
    echo.
    echo FEATURES:
    echo - Multi-App Synchronization
    echo - IPC Communication System
    echo - Professional UI
    echo - Memory Optimized
    echo.
) else (
    echo FAILED: Master Orchestrator build failed
    echo Check the build output above for errors
)

echo.
pause
