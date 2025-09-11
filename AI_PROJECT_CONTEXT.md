# Predator Momentum — AI Project Context

This document provides an AI-friendly overview of the Predator Momentum project to accelerate code understanding, navigation, and development.

## 1) Purpose
Predator Momentum is a Flipper Zero application (FAP) targeting the Momentum firmware. It turns the Flipper Zero into a multi-capability penetration testing platform with support for several expansion boards (ESP32-based WiFi/Bluetooth, GPS, SubGHz/RF, car attacks, RFID/NFC, wardriving, etc.).

## 2) High-Level Architecture
- Application type: External Flipper app (FAP)
- Entry point: `predator_app()` in `predator_app/predator.c`
- UI: Scene-based architecture using Flipper GUI (`ViewDispatcher`, `SceneManager`, custom views)
- Hardware abstraction: Helpers for ESP32 (Marauder), GPS (NMEA), SubGHz, boards
- Multi-board support: Configurable, with optional auto-detect and persistent selection

## 3) Key Files and Directories
- `predator_app/application.fam`
  - App manifest (appid, name, entry point, required subsystems, sources)
- `predator_app/predator.c`
  - Main lifecycle: allocation, view dispatcher, scene manager, safe mode, watchdog, error handling
  - Defers UART init to specific scenes to avoid early HAL crashes on Momentum
- `predator_app/predator_i.h`
  - Core types, enums, pins, baud rates, app state, custom events, ESP32/GPS macros
- `predator_app/scenes/`
  - Scene router: `predator_scene.c`, `predator_scene.h`, `predator_scene_config.h`
  - New UX scenes: e.g., `predator_scene_start_new.c`, `predator_scene_wifi_*_new.c`, `predator_scene_ble_*_new.c`, `predator_scene_car_*_new.c`, `predator_scene_rfid_*_new.c`, `predator_scene_gps_*`, `predator_scene_settings_new.c`, etc.
- `predator_app/helpers/`
  - `predator_boards.[ch]`: board catalog, storage, (basic) auto-detect, pin mapping
  - `predator_esp32.[ch]`: Marauder/ESP32 interactions and commands
  - `predator_gps.[ch]`: GPS UART + NMEA parsing, status, debug
  - `predator_subghz.c`: SubGHz utilities
  - `predator_error.c`, `predator_watchdog.c`, `predator_ui_*`, `predator_view_helpers.c`, `predator_scene_utils.c`, `predator_string.c`
  - `predator_uart.c`: UART abstraction used by ESP32/GPS
- `predator_app/data/`
  - `car_models_500.csv` (≈410 entries used by car attack features)

## 4) Build and Install (Momentum firmware)
- Tooling: `ufbt` (Flipper build tool)
- Quick steps (from `MOMENTUM_INTEGRATION_GUIDE.md`):
  1. `python -m pip install --upgrade ufbt`
  2. From `predator_app/`: `ufbt update` then `ufbt`
  3. Install to device: `ufbt launch` (or copy FAP from `predator_app/dist/` to `/apps/Tools/` on SD)
- Momentum SDK pointers (example):
  - `.ufbt`: `FIRMWARE_API_VERSION=af67a99d`, `SDK_BRANCH=dev`, `SDK_ORIGIN=https://github.com/Next-Flip/Momentum-Firmware.git`
  - `.ufbt.user`: origin/branch/version as above (`latest`)

## 5) Runtime Overview
- App entry: `predator_app()`
  - Safe mode detection via crash counter file `/ext/predator_safe_mode`
  - Alloc/initialize GUI, `ViewDispatcher`, `SceneManager`
  - Initialize error tracking, watchdog; defer UART init to scenes
  - Start at `PredatorSceneStart` (see `predator_scene_start_new.c`)
- Event flow:
  - Custom events in `PredatorCustomEvent` (`predator_i.h`), e.g., `PredatorCustomEventError`, `PredatorCustomEventRecovery`, `PredatorCustomEventGpsUpdate`, etc.
  - Navigation handled via `SceneManager`

## 6) Board & Hardware Mapping
- Board enum: `PredatorBoardType` in `helpers/predator_boards.h`
  - Supported: Original Predator, 3in1 AIO V1.4, DrB0rk Multi V2, 3-in-1 NRF24+CC1101+ESP32, 2.8-inch Predator Screen
- Storage for selection: `/ext/predator/board_config.bin`
- Detection: `predator_boards_detect()` is conservative (often returns Unknown → defaults to Original unless user selects board in settings)
- Core pins (from `predator_i.h` and `predator_boards.c`):
  - ESP32 UART: TX `gpio_ext_pc0` (Pin 15), RX `gpio_ext_pc1` (Pin 16), `115200`
  - GPS UART: TX `gpio_ext_pb2` (Pin 13), RX `gpio_ext_pb3` (Pin 14), `9600`
  - Switches (board-dependent): GPS power `gpio_ext_pa4`, Marauder enable `gpio_ext_pa7`
  - SubGHz: external module (A07) typically used; power ~10 dBm

## 7) Modules of Interest
- ESP32 (Marauder)
  - API: `predator_esp32_*` (`helpers/predator_esp32.h/.c`)
  - Commands: see macros in `predator_i.h` (e.g., `MARAUDER_CMD_WIFI_SCAN`, `MARAUDER_CMD_WIFI_DEAUTH`, `MARAUDER_CMD_BLE_SPAM`, `MARAUDER_CMD_WARDRIVE`, `MARAUDER_CMD_STOP`)
  - Data Rx callback: `predator_esp32_rx_callback()`
- GPS
  - API: `predator_gps_*` (`helpers/predator_gps.h/.c`), NMEA parsing, connectivity/status
  - Debug scene: `predator_scene_gps_debug.c`
- SubGHz
  - Utilities in `helpers/predator_subghz.c`; RF features under SubGHz scenes
- Error/Watchdog
  - `helpers/predator_error.c`: `predator_error_show_popup()` for UI feedback
  - `helpers/predator_watchdog.c`: keepalive via `predator_watchdog_tick()`

## 8) UI/Scenes
- Router: `scenes/predator_scene.c`, `scenes/predator_scene.h`, `scenes/predator_scene_config.h`
- Start scene: `scenes/predator_scene_start_new.c`
  - Custom menu view (icons + scrolling), dispatches to submenus via custom events
- Key categories (new UX scenes):
  - WiFi: `predator_scene_wifi_attacks_new.c`, `predator_scene_wifi_scan_new.c`, `predator_scene_wifi_deauth_new.c`, `predator_scene_wifi_evil_twin_new.c`
  - Bluetooth: `predator_scene_bluetooth_attacks_new.c`, `predator_scene_ble_scan_new.c`, `predator_scene_ble_spam_new.c`
  - SubGHz: `predator_scene_subghz_attacks_new.c`
  - Car: `predator_scene_car_*_new.c` (models, brute-force, jamming, passive opener, Tesla)
  - RFID: `predator_scene_rfid_*_new.c`
  - GPS/Wardrive: `predator_scene_gps_tracker_new.c`, `predator_scene_wardriving_new.c`, `predator_scene_gps_debug.c`
  - Settings/Board: `predator_scene_settings_new.c`, `predator_scene_board_selection_new.c`, `predator_scene_module_status_new.c`, `predator_scene_about_new.c`

## 9) Data
- `predator_app/data/car_models_500.csv`
  - Format: `Make,Model,FrequencyHz,SecurityType` (e.g., `Toyota,Camry 2016+,433920000,Rolling Code`)
  - Used for car attack scenes and model selection

## 10) Permissions and Manifest
- `predator_app/application.fam` highlights:
  - `entry_point="predator_app"`
  - `requires=["gui","dialogs","storage","notification","subghz","nfc","bt","infrared","gpio","power","music_player"]`
  - `fap_category="Tools"`, `targets=["f7"]`, `cdefines=["HEAP_SIZE=12000"]`
  - Explicit `sources=[ ... ]` pin the build to new UX scenes and helper modules

## 11) Momentum-Specific Setup (Device)
- Set pin mappings in Momentum menus:
  - ESP32: TX=15, RX=16
  - GPS: TX=13, RX=14
  - SubGHz: SubGHz → Advanced Settings → Module = External

## 12) Common Issues
- App not in menu: ensure FAP copied to `/apps/Tools/` and `application.fam` category is "Tools"
- "Invalid file": mismatch between FAP and SDK version → rebuild with matching Momentum SDK
- Early crashes: UART initialization is scene-deferred to avoid HAL crashes when hardware isn’t ready
- No GPS fix: check antenna, power switch position, allow time for first fix

## 13) Legal/Ethical Use
Predator is intended for authorized security testing, research, and education only. Users must follow applicable laws and obtain authorization before testing.

---
If you need deeper details, see:
- `MOMENTUM_INTEGRATION_GUIDE.md`
- `MULTI_BOARD_GUIDE.md`
- `PREDATOR_USER_GUIDE.md`
- Code entry point: `predator_app/predator.c`
- Manifest: `predator_app/application.fam`
