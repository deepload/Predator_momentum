# Scene Navigation Pattern Audit

## ✅ CORRECT PATTERNS (Working Scenes)

### Car Model Flow (GOLD STANDARD):
```
Main Menu
  ↓
CarContinentUI: return false ✅
  ↓
CarModelsUI: scene_manager_previous_scene() + return true ✅ (MIDDLE SCENE)
  ↓
CarModelAttacksUI: return false ✅
  ↓
CarKeyBruteforceUI: return false ✅ (view created once, not freed)
```

### Barrier Flow (FIXED):
```
Main Menu
  ↓
ParkingBarriersUI: return false ✅
  ↓
BarrierManufacturerSelectUI: scene_manager_previous_scene() + return true ✅ (MIDDLE SCENE)
  ↓
BarrierAttackUI: return false ✅ (view created once, not freed)
```

---

## ⚠️ SCENES THAT NEED REVIEW

### Category 1: First-Level Menus (Direct from Main Menu)
**Pattern:** Should call `scene_manager_previous_scene()` + return `true`

| Scene | Current Pattern | Status | Action Needed |
|-------|----------------|--------|---------------|
| **WifiAttacksUI** | `scene_manager_previous_scene() + true` | ✅ CORRECT | None |
| **SubGhzAttacksUI** | `scene_manager_previous_scene() + true` | ✅ CORRECT | None |
| **RfidAttacksUI** | `scene_manager_previous_scene() + true` | ✅ CORRECT | None |
| **BluetoothAttacksUI** | `scene_manager_previous_scene() + true` | ✅ CORRECT | None |
| **CarTeslaUI** | `scene_manager_previous_scene() + true` | ✅ CORRECT | None |
| **BoardSelectionUI** | `scene_manager_previous_scene() + true` | ✅ CORRECT | None |
| **SettingsUI** | `scene_manager_previous_scene() + true` | ✅ CORRECT | None |
| **ModuleStatusUI** | `scene_manager_previous_scene() + true` | ✅ CORRECT | None |
| **AboutUI** | `scene_manager_previous_scene() + true` | ✅ CORRECT | None |
| **LiveMonitorUI** | `scene_manager_previous_scene() + true` | ⚠️ CHECK | May need to verify if it's end of chain |
| **ProtocolTestUI** | `scene_manager_previous_scene() + true` | ⚠️ CHECK | May need to verify if it's end of chain |

### Category 2: Attack Scenes (Custom Views)
**Pattern:** Should return `false`, create view once, DON'T free on exit

| Scene | View Created Once? | View Freed on Exit? | Returns false? | Status |
|-------|-------------------|---------------------|----------------|--------|
| **CarKeyBruteforceUI** | ✅ Yes | ✅ No | ✅ Yes | ✅ PERFECT |
| **BarrierAttackUI** | ✅ Yes | ✅ No | ✅ Yes | ✅ PERFECT |
| **CarJammingUI** | ❓ CHECK | ❓ CHECK | ✅ Yes | ⚠️ AUDIT NEEDED |
| **CarPassiveOpenerUI** | ❓ CHECK | ❓ CHECK | ✅ Yes | ⚠️ AUDIT NEEDED |
| **SubGhzJammingUI** | ❓ CHECK | ❓ CHECK | ❓ CHECK | ⚠️ AUDIT NEEDED |

### Category 3: Submenu Scenes (Second Level)
**Pattern:** Depends on position in chain

| Scene | Parent Scene | Pattern Used | Status |
|-------|-------------|-------------|--------|
| **CarContinentUI** | Main Menu | `return false` | ✅ CORRECT (first in chain) |
| **CarModelsUI** | CarContinentUI | `scene_manager_previous_scene() + true` | ✅ CORRECT (middle) |
| **CarModelAttacksUI** | CarModelsUI | `return false` | ✅ CORRECT (before attack) |
| **ParkingBarriersUI** | Main Menu | `return false` | ✅ CORRECT (first in chain) |
| **BarrierManufacturerSelectUI** | ParkingBarriersUI | `scene_manager_previous_scene() + true` | ✅ CORRECT (middle) |

---

## 🔍 HIGH PRIORITY AUDITS NEEDED

### 1. **CarJammingUI** - Custom View Scene
**Check:**
- [ ] Is view created only once?
- [ ] Is view NOT freed on exit?
- [ ] Does BACK return false?

### 2. **CarPassiveOpenerUI** - Custom View Scene
**Check:**
- [ ] Is view created only once?
- [ ] Is view NOT freed on exit?
- [ ] Does BACK return false?

### 3. **SubGhzJammingUI** - Custom View Scene
**Check:**
- [ ] Is view created only once?
- [ ] Is view NOT freed on exit?
- [ ] Does BACK return false?

### 4. **LiveMonitorUI** - May be End of Chain
**Check:**
- [ ] Is it called from multiple places?
- [ ] Should it return false instead of calling scene_manager_previous_scene()?

### 5. **ProtocolTestUI** - May be End of Chain
**Check:**
- [ ] Is it called from CarModelAttacksUI?
- [ ] Should it return false instead of calling scene_manager_previous_scene()?

---

## 📋 NAVIGATION PATTERN RULES

### Rule 1: First Scene in a Flow
```c
if(event.type == SceneManagerEventTypeBack) {
    return false;  // Let scene manager handle natural navigation
}
```
**Examples:** CarContinentUI, ParkingBarriersUI

### Rule 2: Middle Scene in a Flow  
```c
if(event.type == SceneManagerEventTypeBack) {
    scene_manager_previous_scene(app->scene_manager);
    return true;  // Consumed - explicit navigation
}
```
**Examples:** CarModelsUI, BarrierManufacturerSelectUI

### Rule 3: Attack/Custom View Scene
```c
// on_enter:
if(!my_view) {  // Only create once!
    my_view = view_alloc();
    view_set_callbacks(...);
    view_dispatcher_add_view(...);
}
view_dispatcher_switch_to_view(...);

// on_event BACK:
if(event.type == SceneManagerEventTypeBack) {
    // Stop attack if running
    return false;  // Let scene manager handle navigation
}

// on_exit:
void scene_on_exit(void* context) {
    // Stop timer
    // Stop attack
    // Set status to Idle
    // DON'T remove/free view - reuse next time
}
```
**Examples:** CarKeyBruteforceUI, BarrierAttackUI

### Rule 4: Main Menu Scene
```c
if(event.type == SceneManagerEventTypeBack) {
    // Special handling: double-press to exit app
    return true;  // Always consume to prevent exit
}
```

---

## 🎯 NEXT STEPS

1. **Audit CarJammingUI** - Verify view lifecycle
2. **Audit CarPassiveOpenerUI** - Verify view lifecycle
3. **Audit SubGhzJammingUI** - Verify view lifecycle
4. **Review LiveMonitorUI** - Check if it's end of chain
5. **Review ProtocolTestUI** - Check if it's end of chain
6. **Test all navigation flows** - Walk through each menu path

---

## 📝 TESTING CHECKLIST

For each scene audit:
- [ ] Enter scene from parent
- [ ] Press BACK - does app stay open?
- [ ] Enter scene again - does it work?
- [ ] Start attack/action
- [ ] Press BACK during action - does it stop cleanly?
- [ ] Navigate BACK through entire chain - no crashes?
- [ ] Re-enter flow - all menus work?

---

**Status:** Barrier flow fixed ✅ | Car model flow working ✅ | Other attack scenes need audit ⚠️
