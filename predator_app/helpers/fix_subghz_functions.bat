@echo off
echo Fixing predator_subghz.c return types...

cd C:\Projects\Predator_momentum\predator_app

REM Fix predator_subghz_start_car_bruteforce function (line 173)
powershell -Command "(Get-Content 'helpers\predator_subghz.c') -replace 'if\(frequency \< 300000000 \|\| frequency \> 950000000\) \{\r\n        FURI_LOG_E\(\"PredatorSubGHz\", \"Invalid frequency: %lu\", frequency\);\r\n        return;\r\n    \}', 'if(frequency < 300000000 || frequency > 950000000) {\r\n        FURI_LOG_E(\"PredatorSubGHz\", \"Invalid frequency: %lu\", frequency);\r\n        return false;\r\n    }' | Set-Content 'helpers\predator_subghz.c'"

REM Fix predator_subghz_start_jamming function (void to bool)
powershell -Command "(Get-Content 'helpers\predator_subghz.c') -replace 'void predator_subghz_start_jamming\(PredatorApp\* app, uint32_t frequency\) \{', 'bool predator_subghz_start_jamming(PredatorApp* app, uint32_t frequency) {' | Set-Content 'helpers\predator_subghz.c'"

REM Fix return values in predator_subghz_start_jamming
powershell -Command "(Get-Content 'helpers\predator_subghz.c') | ForEach-Object { $_ -replace 'if\(!app->subghz_txrx\) \{\r\n        FURI_LOG_E\(\"PredatorSubGHz\", \"SubGHz not initialized for jamming\"\);\r\n        return;\r\n    \}', 'if(!app->subghz_txrx) {\r\n        FURI_LOG_E(\"PredatorSubGHz\", \"SubGHz not initialized for jamming\");\r\n        return false;\r\n    }' } | Set-Content 'helpers\predator_subghz.c'"

powershell -Command "(Get-Content 'helpers\predator_subghz.c') | ForEach-Object { $_ -replace 'if\(frequency \< 300000000 \|\| frequency \> 950000000\) \{\r\n        FURI_LOG_E\(\"PredatorSubGHz\", \"Invalid frequency: %lu\", frequency\);\r\n        return;\r\n    \}', 'if(frequency < 300000000 || frequency > 950000000) {\r\n        FURI_LOG_E(\"PredatorSubGHz\", \"Invalid frequency: %lu\", frequency);\r\n        return false;\r\n    }' } | Set-Content 'helpers\predator_subghz.c'"

REM Fix predator_subghz_start_rolling_code_attack function (void to bool)
powershell -Command "(Get-Content 'helpers\predator_subghz.c') -replace 'void predator_subghz_start_rolling_code_attack\(PredatorApp\* app, uint32_t frequency\) \{', 'bool predator_subghz_start_rolling_code_attack(PredatorApp* app, uint32_t frequency) {' | Set-Content 'helpers\predator_subghz.c'"

REM Add return values throughout file
powershell -Command "(Get-Content 'helpers\predator_subghz.c') | ForEach-Object { $_ -replace 'FURI_LOG_I\(\"PredatorSubGHz\", \"Bruteforce started\"\);\r\n\r\n    return;', 'FURI_LOG_I(\"PredatorSubGHz\", \"Bruteforce started\");\r\n\r\n    return true;' } | Set-Content 'helpers\predator_subghz.c'"

powershell -Command "(Get-Content 'helpers\predator_subghz.c') | ForEach-Object { $_ -replace 'FURI_LOG_I\(\"PredatorSubGHz\", \"Jamming started\"\);\r\n\r\n    return;', 'FURI_LOG_I(\"PredatorSubGHz\", \"Jamming started\");\r\n\r\n    return true;' } | Set-Content 'helpers\predator_subghz.c'"

powershell -Command "(Get-Content 'helpers\predator_subghz.c') | ForEach-Object { $_ -replace 'FURI_LOG_I\(\"PredatorSubGHz\", \"Rolling code attack started\"\);\r\n\r\n    return;', 'FURI_LOG_I(\"PredatorSubGHz\", \"Rolling code attack started\");\r\n\r\n    return true;' } | Set-Content 'helpers\predator_subghz.c'"

echo Fix complete.
