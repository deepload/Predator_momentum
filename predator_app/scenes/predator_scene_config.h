// PROFESSIONAL SCENE CONFIGURATION - ESSENTIAL SCENES ONLY
ADD_SCENE(predator, main_menu_ui, MainMenuUI)

// Tesla & Car Security (CORE BUSINESS)
ADD_SCENE(predator, car_tesla_ui, CarTeslaUI)
ADD_SCENE(predator, car_continent_ui, CarContinentUI)     // Continent picker (Europe/Asia/America)
ADD_SCENE(predator, car_models_ui, CarModelsUI)           // Simple model picker
ADD_SCENE(predator, car_model_attacks_ui, CarModelAttacksUI) // Attacks for selected model
ADD_SCENE(predator, protocol_test_ui, ProtocolTestUI)     // Protocol testing (Keeloq/Hitag2/AES)
// ADD_SCENE(predator, walking_open_ui, WalkingOpenUI)      // REMOVED: Replaced by Konami code easter egg 🎮
ADD_SCENE(predator, car_key_bruteforce_ui, CarKeyBruteforceUI)
ADD_SCENE(predator, car_jamming_ui, CarJammingUI)
ADD_SCENE(predator, car_passive_opener_ui, CarPassiveOpenerUI)
ADD_SCENE(predator, parking_barriers_ui, ParkingBarriersUI)  // RE-ADDED: Swiss Gov requirement - Testing stability
ADD_SCENE(predator, barrier_manufacturer_select_ui, BarrierManufacturerSelectUI)  // SWISS KKS: Choose manufacturer or try all
ADD_SCENE(predator, barrier_attack_ui, BarrierAttackUI)      // SWISS KKS: Dedicated barrier opening with real manufacturer keys

// WiFi Attacks - REDUCED FOR MEMORY (Keep essentials only)
ADD_SCENE(predator, wifi_attacks_ui, WifiAttacksUI)
ADD_SCENE(predator, wifi_scan_ui, WifiScanUI)
ADD_SCENE(predator, wifi_deauth_ui, WifiDeauthUI)
ADD_SCENE(predator, wifi_evil_twin_ui, WifiEvilTwinUI)   // RE-ADDED: Testing stability
// ADD_SCENE(predator, wifi_handshake_ui, WifiHandshakeUI)   // REMOVED: Out of memory
ADD_SCENE(predator, wifi_pmkid_ui, WifiPmkidUI)           // KEPT: More efficient than handshake

// Bluetooth Attacks - REDUCED FOR MEMORY
ADD_SCENE(predator, bluetooth_attacks_ui, BluetoothAttacksUI)
ADD_SCENE(predator, ble_scan_ui, BleScanUI)
ADD_SCENE(predator, ble_spam_ui, BleSpamUI)               // KEPT: Memory optimized via on-the-fly filtering

// RFID Attacks - REDUCED FOR MEMORY
ADD_SCENE(predator, rfid_attacks_ui, RfidAttacksUI)
ADD_SCENE(predator, rfid_clone_ui, RfidCloneUI)
ADD_SCENE(predator, rfid_bruteforce_ui, RfidBruteforceUI) // RE-ADDED: Testing stability
// ADD_SCENE(predator, rfid_fuzzing_ui, RfidFuzzingUI)       // REMOVED: Memory optimization

// SubGHz Attacks - REDUCED FOR MEMORY
ADD_SCENE(predator, subghz_attacks_ui, SubGhzAttacksUI)
ADD_SCENE(predator, subghz_jamming_ui, SubGhzJammingUI)   // RE-ADDED: Testing stability
// ADD_SCENE(predator, subghz_raw_send_ui, SubGhzRawSendUI)  // REMOVED: Memory optimization

// GPS & Navigation (TEMPORARILY DISABLED - memory optimization)
// ADD_SCENE(predator, gps_tracker_ui, GpsTrackerUI)
// ADD_SCENE(predator, wardriving_ui, WardrivingUI)

// Social Engineering (TEMPORARILY DISABLED - memory optimization)
// ADD_SCENE(predator, social_engineering_ui, SocialEngineeringUI)

// System & Utility (ESSENTIAL)
ADD_SCENE(predator, module_status_ui, ModuleStatusUI)
ADD_SCENE(predator, board_selection_ui, BoardSelectionUI)
ADD_SCENE(predator, settings_ui, SettingsUI)
ADD_SCENE(predator, about_ui, AboutUI)
ADD_SCENE(predator, live_monitor_ui, LiveMonitorUI)
