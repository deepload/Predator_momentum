# PowerShell script to update all scene files to use the new predator_view_helpers.h
# Run from the predator_app directory

# Define the scenes directory path
$scenes_dir = ".\scenes\"

# Loop through all scene files and update them
Get-ChildItem -Path $scenes_dir -Filter "predator_scene_*.c" | ForEach-Object {
    $file = $_.FullName
    Write-Host "Updating $file..."
    
    # Read the file content
    $content = Get-Content -Path $file -Raw
    
    # Replace header include
    $content = $content -replace '#include "\.\./helpers/predator_view_dispatcher_patch\.h"', '#include "../helpers/predator_view_helpers.h"'
    
    # Replace direct view model access pattern with our helper macro
    $content = $content -replace '(\w+)\* state = view_get_model\(app->view_dispatcher->current_view\);', '$1* state = PREDATOR_GET_MODEL(app->view_dispatcher, $1);'
    
    # Replace view_set_model calls to use our helper
    $content = $content -replace 'view_set_model\(', 'predator_view_set_model('
    
    # Replace view_set_model_free_callback calls to use our helper
    $content = $content -replace 'view_set_model_free_callback\(', 'predator_view_set_model_free_callback('
    
    # Replace view_dispatcher_get_current_view calls to use our helper
    $content = $content -replace 'view_dispatcher_get_current_view\(', 'predator_view_dispatcher_get_current_view('
    
    # Write the updated content back to the file
    Set-Content -Path $file -Value $content
    
    Write-Host "Updated $file"
}

Write-Host "Update complete. Please compile to check for any remaining issues."
