// PROFESSIONAL SCENE CONFIGURATION - ESSENTIAL SCENES ONLY
ADD_SCENE(predator, main_menu_ui, MainMenuUI)

// Tesla & Car Security (CORE BUSINESS)
ADD_SCENE(predator, car_tesla_ui, CarTeslaUI)
ADD_SCENE(predator, car_continent_ui, CarContinentUI)     // Continent picker (Europe/Asia/America)
ADD_SCENE(predator, car_models_ui, CarModelsUI)           // Simple model picker
ADD_SCENE(predator, car_model_attacks_ui, CarModelAttacksUI) // Attacks for selected model
ADD_SCENE(predator, protocol_test_ui, ProtocolTestUI)     // Protocol testing (Keeloq/Hitag2/AES)
ADD_SCENE(predator, walking_open_ui, WalkingOpenUI)      // GOVERNMENT APPROVED: Real-world capability
ADD_SCENE(predator, car_key_bruteforce_ui, CarKeyBruteforceUI)
ADD_SCENE(predator, car_passive_opener_ui, CarPassiveOpenerUI)
ADD_SCENE(predator, car_jamming_ui, CarJammingUI)           // RF jamming attacks
ADD_SCENE(predator, advanced_car_hacking_ui, AdvancedCarHackingUI) // CAN/OBD/TPMS/Keyless
ADD_SCENE(predator, barrier_manufacturer_select_ui, BarrierManufacturerSelectUI)  // SWISS KKS: Choose manufacturer or try all
ADD_SCENE(predator, barrier_attack_ui, BarrierAttackUI)      // SWISS KKS: Dedicated barrier opening with real manufacturer keys
ADD_SCENE(predator, parking_barriers_ui, ParkingBarriersUI)  // RE-ADDED: Swiss Gov requirement - Testing stability

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
ADD_SCENE(predator, calypso_cards_ui, CalypsoCardsUI)     // Government-grade transit cards
ADD_SCENE(predator, auto_card_clone_ui, AutoCardCloneUI) // Automated card cloning
// ADD_SCENE(predator, rfid_fuzzing_ui, RfidFuzzingUI)       // REMOVED: Memory optimization

// SubGHz Attacks - REDUCED FOR MEMORY
ADD_SCENE(predator, subghz_attacks_ui, SubGhzAttacksUI)
ADD_SCENE(predator, subghz_jamming_ui, SubGhzJammingUI)   // RE-ADDED: Testing stability
// ADD_SCENE(predator, subghz_raw_send_ui, SubGhzRawSendUI)  // REMOVED: Memory optimization

// GPS & Navigation (GOVERNMENT APPROVED)
ADD_SCENE(predator, gps_tracker_ui, GpsTrackerUI)
ADD_SCENE(predator, wardriving_ui, WardrivingUI)

// Government Contracts (CRITICAL INFRASTRUCTURE)
ADD_SCENE(predator, traffic_light_security_ui, TrafficLightSecurityUI) // Swiss Government NTCIP
ADD_SCENE(predator, industrial_scada_ui, IndustrialScadaUI)           // Industrial control systems
ADD_SCENE(predator, social_engineering_toolkit_ui, SocialEngineeringToolkitUI) // Advanced social engineering
ADD_SCENE(predator, satellite_communication_ui, SatelliteCommunicationUI) // Satellite security

// Social Engineering (TEMPORARILY DISABLED - memory optimization)
// ADD_SCENE(predator, social_engineering_ui, SocialEngineeringUI)

// System & Utility (ESSENTIAL)
ADD_SCENE(predator, module_status_ui, ModuleStatusUI)
ADD_SCENE(predator, board_selection_ui, BoardSelectionUI)
ADD_SCENE(predator, settings_ui, SettingsUI)
ADD_SCENE(predator, about_ui, AboutUI)
ADD_SCENE(predator, live_monitor_ui, LiveMonitorUI)
