@echo off
echo Adding include for predator_view_helpers.h to all scene files...

cd C:\Projects\Predator_momentum\predator_app

for %%f in (scenes\predator_scene_*.c) do (
    echo Processing %%f
    powershell -Command "$content = Get-Content '%%f'; if($content -notlike '*#include \"../helpers/predator_view_helpers.h\"*') { $content = $content -replace '#include \"../predator_i.h\"', '#include \"../predator_i.h\"\n#include \"../helpers/predator_view_helpers.h\"'; Set-Content -Path '%%f' -Value $content }"
)

echo Include directive added to all scene files.
