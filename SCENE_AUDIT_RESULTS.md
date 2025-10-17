# Scene Navigation Audit Results

## ✅ ALL WORKING SCENES

### Attack Scenes with Custom Views (Verified ✅):
1. **CarKeyBruteforceUI** ✅ - View created once, not freed, returns false
2. **BarrierAttackUI** ✅ - View created once, not freed, returns false  
3. **CarJammingUI** ✅ - View created once, not freed, returns false
4. **CarPassiveOpenerUI** ✅ - View created once, not freed, returns false

### Submenu Scenes (Verified ✅):
1. **CarContinentUI** ✅ - First in chain, returns false
2. **CarModelsUI** ✅ - Middle scene, scene_manager_previous_scene() + return true
3. **CarModelAttacksUI** ✅ - Before attack, returns false
4. **ParkingBarriersUI** ✅ - First in chain, returns false
5. **BarrierManufacturerSelectUI** ✅ - Middle scene, scene_manager_previous_scene() + return true

### First-Level Menus (Verified ✅):
1. **WifiAttacksUI** ✅ - scene_manager_previous_scene() + return true
2. **SubGhzAttacksUI** ✅ - scene_manager_previous_scene() + return true
3. **RfidAttacksUI** ✅ - scene_manager_previous_scene() + return true
4. **BluetoothAttacksUI** ✅ - scene_manager_previous_scene() + return true
5. **CarTeslaUI** ✅ - scene_manager_previous_scene() + return true
6. **BoardSelectionUI** ✅ - scene_manager_previous_scene() + return true
7. **SettingsUI** ✅ - scene_manager_previous_scene() + return true
8. **ModuleStatusUI** ✅ - scene_manager_previous_scene() + return true
9. **AboutUI** ✅ - scene_manager_previous_scene() + return true

---

## ❌ 1 SCENE NEEDS FIXING

### **SubGhzJammingUI** - BROKEN VIEW LIFECYCLE

**Problem:**
```c
// on_enter - WRONG! Creates new view every time
View* view = view_alloc();  // Line 252
view_dispatcher_add_view(app->view_dispatcher, PredatorViewSubGhzJammingUI, view);

// on_exit - WRONG! Removes view
view_dispatcher_remove_view(app->view_dispatcher, PredatorViewSubGhzJammingUI);  // Line 314
```

**Should Be:**
```c
// on_enter - Create view ONCE
if(!subghz_jamming_view) {
    subghz_jamming_view = view_alloc();
    view_set_callbacks(...);
    view_dispatcher_add_view(...);
}
view_dispatcher_switch_to_view(...);

// on_exit - DON'T remove/free view
jamming_state.status = RFJammingStatusIdle;
// DON'T remove/free view - we reuse it next time
```

**Impact:** May cause crashes when pressing BACK from RF Jamming scene, similar to the barrier attack bug we just fixed.

---

## 📊 AUDIT SUMMARY

| Category | Total | Working | Need Fix |
|----------|-------|---------|----------|
| **Attack Scenes** | 5 | 4 ✅ | 1 ❌ |
| **Submenu Scenes** | 5 | 5 ✅ | 0 |
| **First-Level Menus** | 9 | 9 ✅ | 0 |
| **TOTAL** | 19 | 18 ✅ | 1 ❌ |

**Success Rate: 94.7%** 🎯

---

## 🔧 FIX REQUIRED

### File: `predator_scene_subghz_jamming_ui.c`

**Lines to Fix:**
- Line 229-268: on_enter function (add view creation guard)
- Line 303-318: on_exit function (remove view_dispatcher_remove_view call)

**Pattern to Follow:** Match **CarKeyBruteforceUI** or **BarrierAttackUI** exactly.

---

## ✅ CONCLUSION

**All scenes follow the correct navigation pattern EXCEPT SubGhzJammingUI.**

Once SubGhzJammingUI is fixed, all navigation flows will be stable and crash-free! 🎉
