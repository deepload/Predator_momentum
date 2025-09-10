@echo off
echo Running update script for scene files...

REM Remove previous predator_view_dispatcher_patch references from all scene files
cd C:\Projects\Predator_momentum\predator_app

REM First, let's backup all scene files
echo Creating backups of scene files...
mkdir backup_scenes 2>nul
for %%f in (scenes\predator_scene_*.c) do copy "%%f" "backup_scenes\%%~nxf" >nul

REM Now update all files to use predator_view_helpers.h
echo Updating scene files to use predator_view_helpers.h...
for %%f in (scenes\predator_scene_*.c) do (
    echo Processing %%f
    powershell -Command "(Get-Content '%%f') -replace '#include \""\.\./helpers/predator_view_dispatcher_patch\.h\""', '#include \"../helpers/predator_view_helpers.h\"' | Set-Content '%%f'"
    
    REM Replace view_set_model calls
    powershell -Command "(Get-Content '%%f') -replace 'view_set_model\(', 'predator_view_set_model(' | Set-Content '%%f'"
    
    REM Replace view_set_model_free_callback calls
    powershell -Command "(Get-Content '%%f') -replace 'view_set_model_free_callback\(', 'predator_view_set_model_free_callback(' | Set-Content '%%f'"
    
    REM Replace view_dispatcher_get_current_view calls
    powershell -Command "(Get-Content '%%f') -replace 'view_dispatcher_get_current_view\(', 'predator_view_dispatcher_get_current_view(' | Set-Content '%%f'"
    
    REM Replace direct view model access pattern with our helper macro
    powershell -Command "$content = Get-Content '%%f' -Raw; $pattern = '(\w+)\* state = view_get_model\(app->view_dispatcher->current_view\);'; $replacement = '$1* state = PREDATOR_GET_MODEL(app->view_dispatcher, $1);'; $content = $content -replace $pattern, $replacement; Set-Content -Path '%%f' -Value $content"
)

echo Update complete. Please compile to check for any remaining issues.
