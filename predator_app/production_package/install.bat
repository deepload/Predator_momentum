@echo off
echo Installing Predator Momentum Production Suite...
echo.
echo Copying apps to Flipper Zero /apps/Tools/...
copy apps\*.fap "%FLIPPER_PATH%\apps\Tools\"
echo.
echo Installation complete!
echo Start with: predator_master_orchestrator.fap
pause
