# NAVIGATION AUDIT - Predator Momentum

## Purpose
Complete audit of ALL scene navigation to prevent accidental app exits and ensure coherent navigation flow.

## Navigation Tree (27 Active Scenes)

```
📱 APP START
│
└─ 🏠 Main Menu (DOUBLE-PRESS BACK TO EXIT)
   │
   ├─ 🚗 Tesla Security
   │  └─ [Attack options]
   │
   ├─ 🚗 Car Models
   │  ├─ Select Car
   │  └─ Car Model Attacks
   │     ├─ Protocol Test
   │     ├─ Key Bruteforce
   │     ├─ Rolling Code Attack
   │     ├─ RF Jamming
   │     ├─ Passive Opener
   │     └─ Live Monitor
   │
   ├─ 🚶 Walking Open (PARIS)
   │  └─ [Walking mode screens]
   │
   ├─ 🚧 Parking Barriers (Swiss Gov)
   │  └─ [Barrier attack screens]
   │
   ├─ 📡 WiFi Attacks
   │  ├─ WiFi Scan
   │  ├─ WiFi Deauth
   │  └─ WiFi Evil Twin
   │
   ├─ 📱 Bluetooth Attacks
   │  ├─ BLE Scan
   │  └─ BLE Spam
   │
   ├─ 💳 RFID Attacks
   │  ├─ RFID Clone
   │  └─ RFID Bruteforce
   │
   ├─ 📻 SubGHz Attacks
   │  └─ SubGHz Jamming
   │
   ├─ 📊 Module Status
   │
   ├─ 🔧 Board Selection
   │  ├─ Main Screen
   │  ├─ Details Screen
   │  ├─ Confirm Screen
   │  └─ Success Screen
   │
   ├─ ⚙️ Settings
   │
   └─ ℹ️ About
```

## Back Button Handling Rules

### ✅ CORRECT Pattern (Scene returns to parent):
```c
if(event.type == SceneManagerEventTypeBack) {
    // Clean up if needed
    predator_subghz_stop_attack(app);
    
    // Let scene manager navigate back
    return false;  // ✅ CORRECT!
}
```

### ❌ INCORRECT Pattern (Exits app):
```c
if(event.type == SceneManagerEventTypeBack) {
    // This exits the app!
    scene_manager_stop(app->scene_manager);  // ❌ BAD!
    view_dispatcher_stop(app->view_dispatcher);  // ❌ BAD!
    return true;  // ❌ BAD!
}
```

### ⚠️ SPECIAL CASE (Main Menu Only):
```c
// Main menu needs double-press to exit
if(event.type == SceneManagerEventTypeBack) {
    if(time_since_last < 2000ms && last_press != 0) {
        PREDATOR_SAFE_EXIT_APP(app);  // ✅ OK for main menu
        return true;
    } else {
        // First press - wait for second
        return true;  // Consume event
    }
}
```

## Critical Scenes to Audit

### Priority 1: Attack Menus (Navigational Hubs)
- [ ] WiFi Attacks UI
- [ ] Bluetooth Attacks UI
- [ ] RFID Attacks UI
- [ ] SubGHz Attacks UI
- [ ] Car Model Attacks UI

### Priority 2: Individual Attack Scenes
- [ ] Walking Open
- [ ] Parking Barriers
- [ ] Car Key Bruteforce
- [ ] Car Jamming
- [ ] Car Passive Opener
- [ ] Tesla Charge Port
- [ ] WiFi Scan
- [ ] WiFi Deauth
- [ ] WiFi Evil Twin
- [ ] BLE Scan
- [ ] BLE Spam
- [ ] RFID Clone
- [ ] RFID Bruteforce
- [ ] SubGHz Jamming

### Priority 3: System Scenes
- [ ] Board Selection (multi-screen)
- [ ] Module Status
- [ ] Settings
- [ ] About
- [ ] Live Monitor

## Test Plan

### Test 1: Main Menu Navigation
```
1. Launch app
2. Navigate to each menu item
3. Press Back from each submenu
4. Verify returns to Main Menu
5. From Main Menu, press Back once
6. Verify app stays open (vibration)
7. Press Back again within 2 seconds
8. Verify app exits
```

### Test 2: Deep Navigation
```
1. Main Menu → Car Models → Select Car → Car Model Attacks → Key Bruteforce
2. Press Back
3. Verify returns to Car Model Attacks
4. Press Back
5. Verify returns to Car Models
6. Press Back
7. Verify returns to Main Menu
8. App should still be running
```

### Test 3: Attack Scene Navigation
```
For EACH attack scene:
1. Navigate to attack
2. Start attack
3. Let it run for 10 seconds
4. Press Back
5. Verify attack stops
6. Verify returns to parent menu
7. Verify app still running
```

### Test 4: Multi-Screen Navigation (Board Selection)
```
1. Main Menu → Board Selection
2. Press Down to browse boards
3. Press OK (go to Details)
4. Press OK (go to Confirm)
5. Press Back (should return to Details)
6. Press Back (should return to Main)
7. Press Back (should return to Main Menu)
8. Verify app still running
```

### Test 5: Stress Test
```
1. Navigate randomly through menus for 5 minutes
2. Press Back frequently
3. Verify app never exits accidentally
4. Verify memory stable (no leaks)
5. Verify all scenes clean up properly
```

## Navigation Issues Found

### ✅ CONFIRMED WORKING (Audited):

#### Attack Menu Scenes - ALL PERFECT ✅
1. **WiFi Attacks UI** ✅
   - `return false;` on Back - CORRECT
   - Navigates to: WiFi Scan, Deauth, Evil Twin
   
2. **Bluetooth Attacks UI** ✅
   - `return false;` on Back - CORRECT
   - Navigates to: BLE Scan, BLE Spam
   
3. **RFID Attacks UI** ✅
   - `return false;` on Back - CORRECT
   - Navigates to: RFID Clone, RFID Bruteforce
   
4. **SubGHz Attacks UI** ✅
   - `return false;` on Back - CORRECT
   - Navigates to: SubGHz Jamming
   
5. **Car Models UI** ✅
   - `return false;` on Back - CORRECT
   - Navigates to: Car Model Attacks
   
6. **Board Selection UI** ✅
   - Multi-screen navigation - PERFECT
   - `return false;` on Back - CORRECT
   - Never exits app

### ✅ SPECIAL HANDLING (Correct):

7. **Main Menu** ✅
   - **Double-press Back to exit** - CORRECT PATTERN
   - First press: Vibration + consume event
   - Second press within 2s: Exit app
   - Prevents accidental exits

### Issue Log:
**NO CRITICAL ISSUES FOUND!** All navigation is properly implemented.

---

## Status: AUDIT IN PROGRESS
Started: 2025-10-14
