# Board Selection System - Complete Improvement Plan

## Current Issues

### 1. **No Real Hardware Detection** ❌
- Hardware test is just a 500ms delay (fake)
- No actual verification of ESP32, GPS, or expansion boards
- Can select boards that aren't connected

### 2. **No Persistent Storage** ❌
- Board selection not saved between sessions
- Must reconfigure every time app starts
- No settings file integration

### 3. **Limited Visual Feedback** ⚠️
- Basic text-only interface
- No board images or detailed schematics
- Hard to differentiate boards visually

### 4. **No Auto-Detection** ❌
- Must manually select board every time
- No automatic hardware scanning
- Can't detect what's actually connected

### 5. **Poor Hardware Initialization** ⚠️
- Doesn't reinitialize hardware after board change
- ESP32/GPS may not activate after selection
- Requires app restart to take effect

### 6. **Limited Board Information** ⚠️
- Basic capability list only
- No pinout information
- No troubleshooting help

---

## Comprehensive Improvement Plan

### Phase 1: Real Hardware Detection ✅

#### Add Hardware Testing Functions:
```c
typedef struct {
    bool esp32_detected;
    bool gps_detected;
    bool cc1101_detected;
    bool nrf24_detected;
    int8_t esp32_uart_status;  // 0=none, 1=ok, -1=error
    int8_t gps_uart_status;
    uint8_t satellites_visible;
    char esp32_version[32];
} HardwareTestResult;

HardwareTestResult test_board_hardware(PredatorBoardType board_type);
```

#### Detection Methods:
1. **ESP32 Detection**:
   - Send UART test command
   - Wait for response with timeout
   - Check firmware version
   - Status: Connected/Disconnected/Error

2. **GPS Detection**:
   - Check UART communication
   - Read GPS status
   - Count visible satellites
   - Status: Active/Searching/Error

3. **SubGHz/CC1101 Detection**:
   - Try to initialize
   - Read chip ID
   - Verify communication
   - Status: Ready/Error

4. **NRF24 Detection**:
   - SPI communication test
   - Read chip register
   - Verify response

---

### Phase 2: Persistent Storage ✅

#### Create Board Settings File:
```c
typedef struct {
    uint32_t magic;  // 0xBD000001
    PredatorBoardType selected_board;
    bool auto_detect_enabled;
    uint32_t last_detection_time;
    HardwareTestResult last_test_result;
    uint32_t crc32;
} BoardSettings;

// Save to: /ext/apps_data/predator/board_settings.dat
bool save_board_settings(BoardSettings* settings);
bool load_board_settings(BoardSettings* settings);
```

#### Features:
- ✅ Save selected board to storage
- ✅ Auto-load on startup
- ✅ Remember last hardware test results
- ✅ CRC32 validation for data integrity

---

### Phase 3: Auto-Detection ✅

#### Intelligent Board Detection:
```c
PredatorBoardType auto_detect_board(void) {
    // 1. Test ESP32 on UART
    if(test_esp32_uart()) {
        // 2. Test GPS on secondary UART
        if(test_gps_uart()) {
            // Has ESP32 + GPS = AIO or 2.8" Screen
            if(test_screen_28_specific()) {
                return PredatorBoardTypeScreen28;
            }
            return PredatorBoardType3in1AIO;
        }
        // Has ESP32 only = DrB0rk or 3in1 NRF/CC/ESP
        if(test_nrf24_spi()) {
            return PredatorBoardTypeDrB0rkMultiV2;
        }
        return PredatorBoardType3in1NrfCcEsp;
    }
    // No ESP32 = Original Predator
    return PredatorBoardTypeOriginal;
}
```

#### Detection Priority:
1. Test for ESP32 presence (UART response)
2. Test for GPS presence (UART response)
3. Test for screen size (if applicable)
4. Test for NRF24 (SPI communication)
5. Default to Original if nothing detected

---

### Phase 4: Visual Improvements ✅

#### Enhanced Board Cards:

**Main Screen**:
```
┌────────────────────────────┐
│  🔧 BOARD SELECTION       │
├────────────────────────────┤
│                            │
│  ▶ Predator Original       │
│  📻 SubGHz + NFC           │
│  [●○○○○] Confidence: 40%   │
│                            │
│  Hardware Status:          │
│  ○ ESP32  ○ GPS  ● SubGHz  │
│                            │
│  ↑↓=Browse OK=Auto-Detect  │
└────────────────────────────┘
```

**Details Screen**:
```
┌────────────────────────────┐
│  PREDATOR 3IN1 AIO        │
├────────────────────────────┤
│  Hardware:                 │
│  ● ESP32  ● GPS  ● SubGHz  │
│                            │
│  Capabilities:             │
│  ✓ WiFi Attacks (Marauder)│
│  ✓ GPS Wardriving         │
│  ✓ BT/BLE Spam            │
│  ✓ SubGHz Car Attacks     │
│                            │
│  Pinout: UART1=ESP, GPIO5  │
│  OK=Select  Back=Return    │
└────────────────────────────┘
```

**Testing Screen**:
```
┌────────────────────────────┐
│  TESTING HARDWARE...       │
├────────────────────────────┤
│                            │
│  [████████░░] 80%          │
│                            │
│  ✓ ESP32: OK (v2.0.1)      │
│  ✓ GPS: 8 satellites       │
│  ⏳ Testing SubGHz...       │
│                            │
│  Please wait...            │
└────────────────────────────┘
```

---

### Phase 5: Hardware Reinitialization ✅

#### After Board Selection:
```c
void reinitialize_hardware_for_board(PredatorApp* app, PredatorBoardType board) {
    FURI_LOG_I("Board", "Reinitializing hardware for %s", PREDATOR_BOARD_NAMES[board]);
    
    // 1. Stop any running attacks
    if(app->attack_running) {
        predator_subghz_stop_attack(app);
    }
    
    // 2. Deinitialize current hardware
    if(app->esp32_uart) {
        predator_esp32_deinit(app);
    }
    if(app->gps_uart) {
        predator_gps_deinit(app);
    }
    
    // 3. Set new board type
    app->board_type = board;
    
    // 4. Initialize for new board
    const PredatorBoardConfig* config = predator_boards_get_config(board);
    if(config) {
        // Initialize ESP32 if board has it
        if(config->has_esp32) {
            predator_esp32_init(app);
        }
        // Initialize GPS if board has it
        if(config->has_gps) {
            predator_gps_init(app);
        }
        // SubGHz is always available
        predator_subghz_init(app);
    }
    
    FURI_LOG_I("Board", "Hardware reinitialization complete");
}
```

---

### Phase 6: Enhanced Board Information ✅

#### Detailed Board Specs:

**Original Predator**:
- SubGHz: Internal CC1101 (433MHz)
- NFC: Internal NFC reader
- No WiFi/BT/GPS
- Best for: Car attacks, SubGHz, RFID

**3in1 AIO**:
- SubGHz: External CC1101
- WiFi/BT: ESP32 Marauder (UART1)
- GPS: Neo-6M/7M (UART2)
- NFC: Internal
- Pinout: GPIO pins 5,6,7
- Best for: Everything!

**DrB0rk Multi V2**:
- SubGHz: Internal CC1101
- WiFi/BT: ESP32
- NRF24: Separate module
- Pinout: Custom
- Best for: Advanced RF

**2.8" Screen**:
- SubGHz: Internal CC1101
- WiFi/BT: ESP32
- GPS: Integrated
- Screen: 2.8" TFT
- Best for: Field operations

**3in1 NRF/CC/ESP**:
- SubGHz: External CC1101
- WiFi/BT: ESP32
- NRF24: Module
- Best for: Multi-protocol

---

### Phase 7: Troubleshooting Guide ✅

#### Built-in Help Screen:

```
┌────────────────────────────┐
│  TROUBLESHOOTING           │
├────────────────────────────┤
│  ESP32 Not Detected?       │
│  • Check UART connections  │
│  • Try board power-cycle   │
│  • Verify TX/RX not swapped│
│                            │
│  GPS Not Working?          │
│  • Move outside for signal │
│  • Wait 30s for satellite  │
│  • Check antenna connected │
│                            │
│  SubGHz Issues?            │
│  • Check frequency region  │
│  • Verify CC1101 powered   │
│                            │
│  OK=Back                   │
└────────────────────────────┘
```

---

## Implementation Priority

### Must Have (Week 1): 🔴
1. ✅ Real hardware detection for ESP32/GPS
2. ✅ Persistent board settings storage
3. ✅ Hardware reinitialization after selection
4. ✅ Auto-detection feature

### Should Have (Week 2): 🟡
5. ✅ Enhanced visual board cards
6. ✅ Real-time hardware status indicators
7. ✅ Detailed board information screens
8. ✅ Testing progress bar

### Nice to Have (Week 3): 🟢
9. ⏳ Troubleshooting guide
10. ⏳ Pinout diagrams
11. ⏳ Board comparison screen
12. ⏳ Hardware benchmark tests

---

## Technical Specifications

### Hardware Detection Timeouts:
- ESP32 UART test: 500ms
- GPS UART test: 1000ms
- CC1101 init test: 200ms
- NRF24 SPI test: 100ms
- Total auto-detection: <2 seconds

### Storage Format:
```c
File: /ext/apps_data/predator/board_settings.dat
Size: 128 bytes
Format:
  [0-3]   Magic: 0xBD000001
  [4]     Board Type (0-4)
  [5]     Auto-detect enabled (bool)
  [6-9]   Last detection timestamp
  [10-41] Hardware test results (32 bytes)
  [42-125] Reserved
  [126-129] CRC32 checksum
```

### Error Handling:
- Invalid board type → Default to Original
- Storage corruption → Reconfigure on startup
- Hardware test timeout → Mark as "Unknown"
- Detection failure → Allow manual selection

---

## Expected User Experience

### First Time Setup:
1. User opens Board Selection
2. Sees "Auto-Detect" button prominently
3. Clicks Auto-Detect
4. Sees hardware testing progress (2s)
5. System detects: "3in1 AIO" with 95% confidence
6. Shows detected hardware: ✓ ESP32 ✓ GPS ✓ SubGHz
7. User confirms selection
8. Board saved to settings
9. Hardware reinitialized
10. Ready to use!

### Subsequent Uses:
1. Board auto-loaded from settings on startup
2. Hardware status shown in Module Status scene
3. Can change board anytime from menu
4. Re-detects on board change confirmation

---

## Success Metrics

### Before Improvement:
- ❌ No hardware validation
- ❌ Manual configuration every session
- ❌ Requires app restart for changes
- ❌ Can select wrong board
- ⚠️ Basic UI

### After Improvement:
- ✅ Real hardware validation
- ✅ Persistent storage (one-time setup)
- ✅ Immediate hardware re-init
- ✅ Auto-detection prevents errors
- ✅ Professional UI with visual feedback
- ✅ Troubleshooting help included
- ✅ Government-grade quality

---

## Implementation Files

### New Files:
1. `helpers/predator_board_detection.c/h` - Hardware detection logic
2. `helpers/predator_board_storage.c/h` - Persistent settings
3. `scenes/predator_scene_board_help_ui.c` - Troubleshooting guide

### Modified Files:
1. `scenes/predator_scene_board_selection_ui.c` - Enhanced UI
2. `helpers/predator_boards.c` - Add detection functions
3. `predator_app.c` - Load board on startup
4. `helpers/predator_esp32.c` - Detection support
5. `helpers/predator_gps.c` - Detection support

---

## Status: READY FOR IMPLEMENTATION

**Next Steps**:
1. Implement hardware detection functions
2. Add persistent storage system
3. Enhance UI with visual improvements
4. Add auto-detection feature
5. Test with all board types
6. Create troubleshooting guide
7. Swiss Government demo ready!

**Estimated Time**: 2-3 days for complete implementation
**Priority**: HIGH - Critical for multi-board support and government demos
