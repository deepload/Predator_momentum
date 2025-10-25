# 🚗 CAR SCENES MIGRATION GUIDE - REMAINING WORK

**Status:** 4/8 modules complete (Tesla, Core, Shared Header, SubGHz)

---

## ✅ COMPLETED TONIGHT (Success!)

### **1. SubGHz Refactoring** ✅
- **Before:** 1 file (52KB, 1262 lines)
- **After:** 4 modular files
  - `helpers/subghz/predator_subghz_core.c/h` - Hardware management
  - `helpers/subghz/predator_subghz_car.c/h` - Car attacks
  - `helpers/subghz/predator_subghz_rolling.c/h` - Rolling code
  - `helpers/subghz/predator_subghz_jamming.c/h` - Jamming
- **Build:** ✅ SUCCESS

### **2. Car Scenes Foundation** ✅
- `scenes/car/predator_scene_car_shared.h` - All common types/enums
- `scenes/car/predator_scene_car_core.c/h` - Navigation (continent + models)
- `scenes/car/predator_scene_car_tesla.c/h` - Tesla attacks

---

## 📋 REMAINING WORK (Tomorrow - 2 hours)

### **Step 1: Create Attack Module Headers** (15 min)

#### **File:** `scenes/car/predator_scene_car_attacks_bruteforce.h`
```c
#pragma once
#include "predator_scene_car_shared.h"

void predator_scene_car_key_bruteforce_ui_on_enter(void* context);
bool predator_scene_car_key_bruteforce_ui_on_event(void* context, SceneManagerEvent event);
void predator_scene_car_key_bruteforce_ui_on_exit(void* context);
```

#### **File:** `scenes/car/predator_scene_car_attacks_jamming.h`
```c
#pragma once
#include "predator_scene_car_shared.h"

void predator_scene_car_jamming_ui_on_enter(void* context);
bool predator_scene_car_jamming_ui_on_event(void* context, SceneManagerEvent event);
void predator_scene_car_jamming_ui_on_exit(void* context);
```

#### **File:** `scenes/car/predator_scene_car_attacks_passive.h`
```c
#pragma once
#include "predator_scene_car_shared.h"

// Passive opener
void predator_scene_car_passive_opener_ui_on_enter(void* context);
bool predator_scene_car_passive_opener_ui_on_event(void* context, SceneManagerEvent event);
void predator_scene_car_passive_opener_ui_on_exit(void* context);

// Model attacks
void predator_scene_car_model_attacks_ui_on_enter(void* context);
bool predator_scene_car_model_attacks_ui_on_event(void* context, SceneManagerEvent event);
void predator_scene_car_model_attacks_ui_on_exit(void* context);
```

---

### **Step 2: Copy Implementation Files** (30 min)

Simply copy existing files with updated includes:

#### **Bruteforce Module**
- **Source:** `scenes/predator_scene_car_key_bruteforce_ui.c` (22KB)
- **Destination:** `scenes/car/predator_scene_car_attacks_bruteforce.c`
- **Changes:** Update includes:
  ```c
  // OLD:
  #include "../predator_i.h"
  #include "../helpers/predator_subghz.h"
  
  // NEW:
  #include "predator_scene_car_attacks_bruteforce.h"
  ```

#### **Jamming Module**
- **Source:** `scenes/predator_scene_car_jamming_ui.c` (15KB)
- **Destination:** `scenes/car/predator_scene_car_attacks_jamming.c`
- **Changes:** Update includes

#### **Passive Module**
- **Source:** `scenes/predator_scene_car_passive_opener_ui.c` (17KB)
- **Source 2:** `scenes/predator_scene_car_model_attacks_ui.c` (4KB)
- **Destination:** `scenes/car/predator_scene_car_attacks_passive.c`
- **Changes:** Combine both files, update includes

---

### **Step 3: Create Advanced Module Headers** (10 min)

#### **File:** `scenes/car/predator_scene_car_advanced_protocols.h`
```c
#pragma once
#include "predator_scene_car_shared.h"

void predator_scene_protocol_test_ui_on_enter(void* context);
bool predator_scene_protocol_test_ui_on_event(void* context, SceneManagerEvent event);
void predator_scene_protocol_test_ui_on_exit(void* context);
```

#### **File:** `scenes/car/predator_scene_car_advanced_special.h`
```c
#pragma once
#include "predator_scene_car_shared.h"

// Walking Open
void predator_scene_walking_open_ui_on_enter(void* context);
bool predator_scene_walking_open_ui_on_event(void* context, SceneManagerEvent event);
void predator_scene_walking_open_ui_on_exit(void* context);

// Parking Barriers
void predator_scene_parking_barriers_ui_on_enter(void* context);
bool predator_scene_parking_barriers_ui_on_event(void* context, SceneManagerEvent event);
void predator_scene_parking_barriers_ui_on_exit(void* context);
```

---

### **Step 4: Copy Advanced Implementations** (30 min)

#### **Protocols Module**
- **Source:** `scenes/predator_scene_protocol_test_ui.c` (13KB)
- **Destination:** `scenes/car/predator_scene_car_advanced_protocols.c`
- **Changes:** Update includes

#### **Special Features Module**
- **Source:** `scenes/predator_scene_walking_open_ui.c` (19KB)
- **Source 2:** `scenes/predator_scene_parking_barriers_ui.c` (18KB)
- **Destination:** `scenes/car/predator_scene_car_advanced_special.c`
- **Changes:** Combine both, update includes

---

### **Step 5: Update Build System** (15 min)

#### **File:** `application.fam`

Replace:
```python
# Tesla & Car Security (CORE BUSINESS)
"scenes/predator_scene_car_tesla_ui.c",
"scenes/predator_scene_car_continent_ui.c",
"scenes/predator_scene_car_models_ui.c",
"scenes/predator_scene_car_model_attacks_ui.c",
"scenes/predator_scene_protocol_test_ui.c",
"scenes/predator_scene_parking_barriers_ui.c",
"scenes/predator_scene_walking_open_ui.c",
"scenes/predator_scene_car_key_bruteforce_ui.c",
"scenes/predator_scene_car_jamming_ui.c",
"scenes/predator_scene_car_passive_opener_ui.c",
```

With:
```python
# v2.0 REFACTORED: Modular Car Scenes (was 10 files, now 8 organized modules)
"scenes/car/predator_scene_car_core.c",                    # Navigation
"scenes/car/predator_scene_car_tesla.c",                   # Tesla
"scenes/car/predator_scene_car_attacks_bruteforce.c",      # Attacks
"scenes/car/predator_scene_car_attacks_jamming.c",         # Attacks
"scenes/car/predator_scene_car_attacks_passive.c",         # Attacks + Model
"scenes/car/predator_scene_car_advanced_protocols.c",      # Protocols
"scenes/car/predator_scene_car_advanced_special.c",        # Walking/Parking
```

---

### **Step 6: Test Build** (20 min)

```powershell
cd predator_app
ufbt
```

**Expected:** Clean build with all modules compiling

---

## 📊 FINAL STRUCTURE

```
predator_app/
├── helpers/
│   ├── subghz/                          ✅ DONE (v2.0)
│   │   ├── predator_subghz_core.c/h
│   │   ├── predator_subghz_car.c/h
│   │   ├── predator_subghz_rolling.c/h
│   │   └── predator_subghz_jamming.c/h
│   └── ... (other helpers)
│
└── scenes/
    ├── car/                              ⏳ 4/8 DONE
    │   ├── predator_scene_car_shared.h                  ✅ DONE
    │   ├── predator_scene_car_core.c/h                  ✅ DONE
    │   ├── predator_scene_car_tesla.c/h                 ✅ DONE
    │   │
    │   ├── predator_scene_car_attacks_bruteforce.c/h    ⏳ TODO
    │   ├── predator_scene_car_attacks_jamming.c/h       ⏳ TODO
    │   ├── predator_scene_car_attacks_passive.c/h       ⏳ TODO
    │   │
    │   ├── predator_scene_car_advanced_protocols.c/h    ⏳ TODO
    │   └── predator_scene_car_advanced_special.c/h      ⏳ TODO
    │
    └── ... (other scenes)
```

---

## 🎯 TOMORROW'S CHECKLIST

- [ ] Create 5 remaining header files (15 min)
- [ ] Copy/adapt 5 implementation files (1 hour)
- [ ] Update application.fam (15 min)
- [ ] Test build (20 min)
- [ ] **DONE!** Car scenes fully modularized

**Then move to v2.0 features:**
- [ ] Unified error handling
- [ ] Hardware Abstraction Layer
- [ ] Async operations framework
- [ ] Attack history & logging
- [ ] Configuration system

---

## 💡 QUICK SCRIPT FOR TOMORROW

```powershell
# Step 1: Create header files (copy templates from above)

# Step 2: Copy implementations with path updates
cd C:\Projects\Predator_momentum\predator_app\scenes

# Bruteforce
cp predator_scene_car_key_bruteforce_ui.c car/predator_scene_car_attacks_bruteforce.c

# Jamming
cp predator_scene_car_jamming_ui.c car/predator_scene_car_attacks_jamming.c

# Passive (need to combine two files manually)
# - predator_scene_car_passive_opener_ui.c
# - predator_scene_car_model_attacks_ui.c

# Protocols
cp predator_scene_protocol_test_ui.c car/predator_scene_car_advanced_protocols.c

# Special (need to combine two files manually)
# - predator_scene_walking_open_ui.c
# - predator_scene_parking_barriers_ui.c

# Step 3: Update includes in all copied files
# Change: #include "../predator_i.h"
# To: #include "predator_scene_car_attacks_*.h"

# Step 4: Update application.fam

# Step 5: Build
cd ../
ufbt
```

---

## 🎉 WHAT WE ACCOMPLISHED TONIGHT

✅ **SubGHz:** 1 → 4 modular files (COMPLETE)  
✅ **Car Scenes:** Foundation (3/8 modules)  
✅ **Total:** ~8 hours of refactoring work done in 2-3 hours!

**Great progress! Tomorrow:** Finish remaining 5 car modules (2 hours), then start adding v2.0 features!

---

**Sleep well! You've done amazing work tonight! 🌙**
