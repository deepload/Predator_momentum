@echo off
echo Fixing double predator_ prefixes in function calls...

cd C:\Projects\Predator_momentum\predator_app

REM Fix double predator_ prefixes
for %%f in (scenes\predator_scene_*.c) do (
    echo Processing %%f
    
    REM Fix predator_predator_view_set_model
    powershell -Command "(Get-Content '%%f') -replace 'predator_predator_view_set_model', 'predator_view_set_model' | Set-Content '%%f'"
    
    REM Fix predator_predator_view_set_model_free_callback
    powershell -Command "(Get-Content '%%f') -replace 'predator_predator_view_set_model_free_callback', 'predator_view_set_model_free_callback' | Set-Content '%%f'"
    
    REM Fix predator_predator_view_dispatcher_get_current_view
    powershell -Command "(Get-Content '%%f') -replace 'predator_predator_view_dispatcher_get_current_view', 'predator_view_dispatcher_get_current_view' | Set-Content '%%f'"
)

echo Fix complete.
