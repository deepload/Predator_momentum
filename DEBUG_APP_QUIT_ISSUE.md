# Debug: App Quitting After Card Selection

## Current Issue:
After selecting a card and pressing BACK, the app quits instead of returning to the main menu.

---

## Debugging Steps - CRITICAL

### **Step 1: Connect Flipper via USB**
```powershell
# In terminal, run:
ufbt cli
```

### **Step 2: Deploy the App**
```powershell
# In another terminal:
ufbt launch
```

### **Step 3: Reproduce the Issue**
1. Start the Predator app
2. Go to "Card Selection"
3. Select a board (e.g., "Original Predator Module")
4. Press OK → Details → OK → Confirm
5. Wait for success screen
6. **Press BACK**
7. Watch the logs!

---

## What to Look For in Logs:

### **Expected Flow (CORRECT)**:
```
[MainMenu] ========== MAIN MENU ENTERED ==========
[MainMenu] Menu enter time: 12345, grace period: 500 ms
[MainMenu] Menu item selected: 12
[BoardSelection] Scene entered
[BoardSelection] Board selected...
[BoardSelection] ========== BOARD SELECTION EXITING ==========
[BoardSelection] Current screen: 3, Confirmed: 1
[BoardSelection] ========== BOARD SELECTION EXIT COMPLETE ==========
[MainMenu] ========== MAIN MENU ENTERED ==========  ← Should re-enter!
[MainMenu] Menu enter time: 67890, grace period: 500 ms
```

### **Broken Flow (WRONG)**:
```
[MainMenu] ========== MAIN MENU ENTERED ==========
[MainMenu] Menu enter time: 12345, grace period: 500 ms
[MainMenu] Menu item selected: 12
[BoardSelection] Scene entered
[BoardSelection] Board selected...
[BoardSelection] ========== BOARD SELECTION EXITING ==========
[BoardSelection] Current screen: 3, Confirmed: 1
[BoardSelection] ========== BOARD SELECTION EXIT COMPLETE ==========
[MainMenu] !!! EXITING MAIN MENU - THIS SHOULD ONLY HAPPEN ON APP CLOSE !!!  ← WRONG!
(App exits)
```

---

## Possible Causes:

### 1. **Main Menu Not in Scene Stack**
If main menu exits after board selection exits, it means there's no scene below board selection.

**Fix**: Ensure board selection is always opened FROM main menu using `scene_manager_next_scene()`

### 2. **Double Scene Pop**
Board selection might be popping TWO scenes instead of one.

**Fix**: Check board selection exit logic - should only pop itself

### 3. **Scene Manager Corruption**
Scene stack might be getting corrupted somewhere.

**Fix**: Add scene stack logging

### 4. **View Dispatcher Issue**
The view dispatcher might be calling stop() somewhere.

**Fix**: Search for `view_dispatcher_stop()` calls

---

## Debug Logging Added:

### **Main Menu**:
- `on_enter()`: "========== MAIN MENU ENTERED =========="
- `on_event()`: Shows every event type and timing
- `on_exit()`: "!!! EXITING MAIN MENU - THIS SHOULD ONLY HAPPEN ON APP CLOSE !!!"

### **Board Selection**:
- `on_exit()`: "========== BOARD SELECTION EXITING =========="
- Shows current screen state and confirmation status
- "========== BOARD SELECTION EXIT COMPLETE =========="

---

## Next Steps Based on Logs:

### If Main Menu on_exit is Called:
**Problem**: Scene is being popped when it shouldn't be
**Solution**: Check what's calling `scene_manager_previous_scene()` or returning `false` from on_event

### If Main Menu on_enter NOT Called After Board Exit:
**Problem**: Main menu wasn't in scene stack
**Solution**: Check how board selection is being opened

### If No Logs Appear:
**Problem**: USB connection issue or app not deploying
**Solution**: Check connection, try `ufbt cli` again

---

## Code Changes Made:

### 1. **Main Menu** (`predator_scene_main_menu_ui.c`):
- Added comprehensive event logging
- ALL code paths return `true` (impossible to exit accidentally)
- 500ms grace period after entering
- Double-press protection

### 2. **Board Selection** (`predator_scene_board_selection_ui.c`):
- Simplified exit logic (just let framework handle it)
- Added detailed exit logging
- Removed custom event 998 navigation

---

## Test Commands:

```powershell
# Terminal 1: View logs
ufbt cli

# Terminal 2: Deploy and run
ufbt launch

# After reproducing issue, save logs
# (Copy paste from terminal)
```

---

## Critical Questions to Answer:

1. **Does "MAIN MENU ENTERED" appear after board selection exits?**
   - YES → Main menu is being re-entered (grace period should work)
   - NO → Main menu not in stack (scene navigation problem)

2. **Does "EXITING MAIN MENU" appear unexpectedly?**
   - YES → Something is popping main menu scene
   - NO → Main menu is staying active

3. **What's the time between board exit and any back press?**
   - < 500ms → Grace period should block it
   - > 500ms → Should be treated as first press

---

## Success Criteria:

When working correctly, pressing BACK after card selection should:
1. Exit board selection scene
2. Re-enter main menu scene
3. Grace period active (500ms)
4. First BACK press → vibrate + "Press again to exit"
5. Second BACK press (within 2s) → Exit app

---

**BUILD STATUS**: ✅ Compiled successfully with debug logging
**NEXT STEP**: Run with `ufbt cli` and send me the complete logs!
