# CRITICAL: We Need USB Logs to Fix This Issue!

## Current Problem:
After selecting a board and pressing BACK, the app quits to the Flipper desktop instead of returning to the app's main menu.

## Why We Need Logs:
Without seeing the actual logs, we can't determine which of these issues is happening:

### Scenario 1: Main Menu Not in Scene Stack
```
[BoardSelection] ==== BACK EVENT ==== Screen: 3
[BoardSelection] Success screen back - searching for main menu
[BoardSelection] ❌ Main menu not found - letting framework handle
```
**This means**: Board selection was opened without main menu being in the stack.

### Scenario 2: Scene Manager Failing
```
[BoardSelection] ==== BACK EVENT ==== Screen: 3
[BoardSelection] Success screen back - searching for main menu
[BoardSelection] ✅ Switched to main menu
[MainMenu] ========== MAIN MENU ENTERED ==========
[MainMenu] BACK EVENT - enter:50 ms ago
```
**This means**: It IS returning to main menu, but then immediately exiting.

### Scenario 3: Back Event Not Reaching Handler
```
(No logs at all)
```
**This means**: The back event is being consumed before reaching our handlers.

---

## How to Get the Logs:

### Step 1: Connect Flipper via USB

### Step 2: Open First Terminal - View Logs
```powershell
cd c:\Projects\Predator_momentum\predator_app
ufbt cli
```
This will show live logs. **Keep this terminal open!**

### Step 3: Open Second Terminal - Deploy App
```powershell
cd c:\Projects\Predator_momentum\predator_app
# Copy FAP file manually to SD card via qFlipper or:
# ufbt launch  (if USB not busy)
```

### Step 4: Run the App and Reproduce Issue
1. Start Predator app on Flipper
2. Navigate to "Card Selection"
3. Select "Original Predator Module"
4. Press OK → Details → OK → Confirm
5. Success screen appears
6. **Press BACK**
7. **Watch the logs in terminal 1**

### Step 5: Copy and Send the Logs
Copy everything from the terminal and send it here.

---

## What the Logs Will Show:

### **Expected (Working)**:
```
[MainMenu] ========== MAIN MENU ENTERED ==========
[MainMenu] Menu enter time: 1000, grace period: 1000 ms, exit BLOCKED
[MainMenu] Event received - type: 1
[MainMenu] Menu item selected: 12
[BoardSelection] Scene entered
[BoardSelection] Board confirmed
[BoardSelection] Success screen - BACK button, NOT consuming
[BoardSelection] ==== BACK EVENT ==== Screen: 3
[BoardSelection] Success screen back - searching for main menu
[BoardSelection] ✅ Switched to main menu
[MainMenu] ========== MAIN MENU ENTERED ==========
[MainMenu] Menu enter time: 5000, grace period: 1000 ms
```

### **Broken (Main Menu Missing)**:
```
[MainMenu] ========== MAIN MENU ENTERED ==========
[MainMenu] Menu item selected: 12
[BoardSelection] Scene entered
[BoardSelection] Success screen - BACK button, NOT consuming
[BoardSelection] ==== BACK EVENT ==== Screen: 3
[BoardSelection] Success screen back - searching for main menu
[BoardSelection] ❌ Main menu not found - letting framework handle
[BoardSelection] ========== BOARD SELECTION EXITING ==========
(App exits - no more logs)
```

### **Broken (Event Not Reaching Handler)**:
```
[MainMenu] ========== MAIN MENU ENTERED ==========
[MainMenu] Menu item selected: 12
[BoardSelection] Scene entered
[BoardSelection] Board confirmed
(No "BACK EVENT" log)
[BoardSelection] ========== BOARD SELECTION EXITING ==========
(App exits)
```

---

## Alternative: Check SD Card Logs

If USB doesn't work, check for log files on SD card:
```
/ext/logs/
```

---

## Current Build Status:

✅ **Latest version compiled**: `dist/predator_professional.fap`

**Changes in this build**:
1. Input callback doesn't consume BACK on success screen
2. Scene on_event explicitly searches for main menu
3. Comprehensive logging at every step
4. View dispatcher always prevents app exit
5. Main menu has 1000ms grace period + double-press protection

---

## Without Logs, We're Guessing:

We've tried 15+ different fixes, but without seeing the actual behavior, we can't determine the root cause. The logs will instantly show us exactly where the issue is.

---

## Quick Test Without Logs:

If you can't get USB logs, try this:

### Test 1: Other Menu Items
1. Go to "Module Status" from main menu
2. Press BACK
3. Does it return to main menu? Or quit?

### Test 2: Settings
1. Go to "Settings" from main menu
2. Press BACK
3. Does it return to main menu? Or quit?

If those work but Card Selection doesn't, it confirms the issue is specific to board selection scene.

---

## Deployment:

**Copy to Flipper**:
```
Source: dist/predator_professional.fap
Destination: /ext/apps/Tools/predator_professional.fap
```

**Methods**:
1. qFlipper - drag and drop
2. SD card reader - copy directly
3. `ufbt launch` - if USB available

---

**Please run with USB logs so we can see exactly what's failing!** 🔍
