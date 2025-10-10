# Professional UI Upgrade Plan - All Scenes

## ✅ Build Status: SUCCESSFUL (Target: 7, API: 86.0)

## 🎯 Goal
Apply the same professional, dynamic UI style from the Car Test Results scene to ALL scenes in the Predator Momentum app.

---

## 📊 UI Design Pattern (Proven Successful)

### Core Elements
1. **Header** - Bold title with separator line
2. **Status Line** - Current operation status
3. **Progress Bar** - Visual feedback (animated or static)
4. **Statistics** - Real-time counters (time, items found, packets sent)
5. **Results Display** - Key information (strongest signal, detected type, etc.)
6. **Instructions** - Bottom line with button hints

### Layout Grid (128x64 pixels)
```
Row 1 (0-12px):   BOLD HEADER
Row 2 (12-14px):  ─────────────────
Row 3 (14-24px):  Status: [state]
Row 4 (24-34px):  Transport/Mode info
Row 5 (34-44px):  [████████░░░░░░] Progress bar
Row 6 (44-54px):  Stats (count, time, etc.)
Row 7 (54-64px):  Results/Instructions
```

---

## 🎨 Scenes to Upgrade (Priority Order)

### ✅ COMPLETED
1. **Car Test Results** - ✅ Professional UI with real-time updates
2. **WiFi Scan UI** - ✅ Professional UI with AP count, signal strength

### 🔥 HIGH PRIORITY (Tesla Demo Critical)

#### 3. WiFi Deauth
**Current:** Popup-based
**New UI:**
```
┌──────────────────────────────┐
│ WIFI DEAUTH ATTACK           │
├──────────────────────────────┤
│ Status: Attacking            │
│ Target: NetworkName          │
│ [████████████░░░░░░░░]       │
│ Packets: 1,234  Time: 45s    │
│ Channel: 6    Power: High    │
│                              │
│ OK=Stop  Back=Exit           │
└──────────────────────────────┘
```

#### 4. BLE Spam
**Current:** Custom view with mode selection
**New UI:**
```
┌──────────────────────────────┐
│ BLE SPAM ATTACK              │
├──────────────────────────────┤
│ Status: Spamming             │
│ Mode: iOS Crash              │
│ [████████████████████]       │
│ Packets: 5,678  Time: 1m 23s │
│ Devices Affected: 12         │
│                              │
│ OK=Stop  Back=Exit           │
└──────────────────────────────┘
```

#### 5. GPS Tracker
**Current:** Full GPS integration
**New UI:**
```
┌──────────────────────────────┐
│ GPS TRACKER                  │
├──────────────────────────────┤
│ Status: Tracking             │
│ Satellites: 8/12             │
│ [████████░░░░░░░░░░░]        │
│ Lat: 45.5234  Lon: -122.6762 │
│ Speed: 45 km/h  Alt: 123m    │
│                              │
│ OK=Log  Back=Exit            │
└──────────────────────────────┘
```

#### 6. Car Jamming
**Current:** SubGHz with popup
**New UI:**
```
┌──────────────────────────────┐
│ CAR JAMMING                  │
├──────────────────────────────┤
│ Status: Jamming              │
│ Frequency: 433.92 MHz        │
│ [████████████████████]       │
│ Duration: 2m 15s             │
│ Power: Maximum               │
│                              │
│ OK=Stop  Back=Exit           │
└──────────────────────────────┘
```

### 📱 MEDIUM PRIORITY

#### 7. RFID Clone
```
┌──────────────────────────────┐
│ RFID CLONE                   │
├──────────────────────────────┤
│ Status: Reading              │
│ Type: Mifare Classic         │
│ [████████░░░░░░░░░░░]        │
│ Blocks Read: 12/64           │
│ UID: 04:A1:B2:C3             │
│                              │
│ OK=Clone  Back=Exit          │
└──────────────────────────────┘
```

#### 8. RFID Bruteforce
```
┌──────────────────────────────┐
│ RFID BRUTEFORCE              │
├──────────────────────────────┤
│ Status: Attacking            │
│ Type: EM4100                 │
│ [████████████░░░░░░░]        │
│ Tried: 1,234/10,000          │
│ Time: 5m 23s  ETA: 15m       │
│                              │
│ OK=Stop  Back=Exit           │
└──────────────────────────────┘
```

#### 9. Wardriving
```
┌──────────────────────────────┐
│ WARDRIVING                   │
├──────────────────────────────┤
│ Status: Scanning             │
│ GPS: 8 sats  WiFi: Active    │
│ [████████████████████]       │
│ APs Found: 45  Distance: 2.3km│
│ Strongest: -45dBm            │
│                              │
│ OK=Stop  Back=Exit           │
└──────────────────────────────┘
```

#### 10. WiFi Evil Twin
```
┌──────────────────────────────┐
│ WIFI EVIL TWIN               │
├──────────────────────────────┤
│ Status: Broadcasting         │
│ SSID: FreeWiFi               │
│ [████████████████████]       │
│ Clients: 3  Captures: 1      │
│ Channel: 6  Auth: WPA2       │
│                              │
│ OK=Stop  Back=Exit           │
└──────────────────────────────┘
```

#### 11. BLE Scan
```
┌──────────────────────────────┐
│ BLE SCANNER                  │
├──────────────────────────────┤
│ Status: Scanning             │
│ ESP32: UART OK               │
│ [████████████░░░░░░░]        │
│ Devices: 12  Time: 23s       │
│ Strongest: -55dBm            │
│                              │
│ OK=View  Back=Exit           │
└──────────────────────────────┘
```

### 📊 LOW PRIORITY (Info Screens)

#### 12. Module Status
```
┌──────────────────────────────┐
│ MODULE STATUS                │
├──────────────────────────────┤
│ ESP32: ✓ Connected           │
│ GPS: ✓ 8 satellites          │
│ SubGHz: ✓ Ready              │
│ Board: Original              │
│ Voltage: 3.3V                │
│                              │
│ Back=Exit                    │
└──────────────────────────────┘
```

#### 13. Settings
```
┌──────────────────────────────┐
│ SETTINGS                     │
├──────────────────────────────┤
│ ▸ Board Selection            │
│ ▸ WiFi Power                 │
│ ▸ GPS Settings               │
│ ▸ Logging                    │
│ ▸ Display                    │
│                              │
│ OK=Select  Back=Exit         │
└──────────────────────────────┘
```

---

## 🛠️ Implementation Template

### File Structure
```c
#include "../predator_i.h"
#include "../helpers/[relevant_helper].h"
#include <gui/view.h>
#include <string.h>

// [Scene Name] - Professional UI
// Shows real-time [operation] progress and results

typedef enum {
    [Scene]StatusIdle,
    [Scene]StatusRunning,
    [Scene]StatusComplete,
    [Scene]StatusError
} [Scene]Status;

typedef struct {
    [Scene]Status status;
    uint32_t items_processed;
    uint32_t time_elapsed_ms;
    // Scene-specific fields
    char info_line[32];
    bool hardware_connected;
} [Scene]State;

static [Scene]State scene_state;
static uint32_t start_tick = 0;

// Draw functions
static void draw_header(Canvas* canvas) {
    canvas_set_font(canvas, FontPrimary);
    canvas_draw_str(canvas, 2, 10, "[SCENE NAME]");
    canvas_draw_line(canvas, 0, 12, 128, 12);
}

static void draw_status(Canvas* canvas, [Scene]State* state) {
    canvas_set_font(canvas, FontSecondary);
    
    // Status line
    canvas_draw_str(canvas, 2, 22, "Status:");
    const char* status_text = "Unknown";
    switch(state->status) {
        case [Scene]StatusIdle: status_text = "Ready"; break;
        case [Scene]StatusRunning: status_text = "Running..."; break;
        case [Scene]StatusComplete: status_text = "Complete"; break;
        case [Scene]StatusError: status_text = "Error"; break;
    }
    canvas_draw_str(canvas, 45, 22, status_text);
    
    // Progress bar
    canvas_draw_frame(canvas, 2, 36, 124, 6);
    if(state->status == [Scene]StatusRunning) {
        uint8_t progress = ((state->time_elapsed_ms / 100) % 124);
        canvas_draw_box(canvas, 3, 37, progress, 4);
    }
}

static void draw_results(Canvas* canvas, [Scene]State* state) {
    canvas_set_font(canvas, FontSecondary);
    
    // Statistics
    char stats[32];
    snprintf(stats, sizeof(stats), "Items: %lu", state->items_processed);
    canvas_draw_str(canvas, 2, 48, stats);
    
    // Time
    char time_str[32];
    uint32_t seconds = state->time_elapsed_ms / 1000;
    snprintf(time_str, sizeof(time_str), "Time: %lus", seconds);
    canvas_draw_str(canvas, 70, 48, time_str);
}

// Main draw callback
static void [scene]_draw_callback(Canvas* canvas, void* context) {
    furi_assert(canvas);
    furi_assert(context);
    UNUSED(context);
    
    canvas_clear(canvas);
    canvas_set_color(canvas, ColorBlack);
    
    draw_header(canvas);
    draw_status(canvas, &scene_state);
    draw_results(canvas, &scene_state);
    
    // Instructions
    canvas_set_font(canvas, FontSecondary);
    if(scene_state.status == [Scene]StatusRunning) {
        canvas_draw_str(canvas, 40, 64, "Back=Stop");
    } else {
        canvas_draw_str(canvas, 30, 64, "OK=Start  Back=Exit");
    }
}

// Input callback
static bool [scene]_input_callback(InputEvent* event, void* context) {
    PredatorApp* app = context;
    if(!app) return false;
    
    if(event->type == InputTypeShort) {
        if(event->key == InputKeyBack) {
            scene_state.status = [Scene]StatusIdle;
            // Stop operation
            return false;
        } else if(event->key == InputKeyOk) {
            if(scene_state.status == [Scene]StatusIdle) {
                // Start operation
                scene_state.status = [Scene]StatusRunning;
                start_tick = furi_get_tick();
                return true;
            }
        }
    }
    return true;
}

// Timer callback for updates
static void [scene]_timer_callback(void* context) {
    furi_assert(context);
    PredatorApp* app = context;
    
    if(scene_state.status == [Scene]StatusRunning) {
        scene_state.time_elapsed_ms = furi_get_tick() - start_tick;
        
        // Update scene-specific data
        // ...
        
        // Trigger view update
        if(app->view_dispatcher) {
            view_dispatcher_send_custom_event(app->view_dispatcher, 0);
        }
    }
}

// Scene lifecycle
void predator_scene_[scene]_on_enter(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    memset(&scene_state, 0, sizeof([Scene]State));
    scene_state.status = [Scene]StatusIdle;
    
    // Create view
    View* view = view_alloc();
    view_set_context(view, app);
    view_set_draw_callback(view, [scene]_draw_callback);
    view_set_input_callback(view, [scene]_input_callback);
    
    view_dispatcher_add_view(app->view_dispatcher, PredatorView[Scene], view);
    view_dispatcher_switch_to_view(app->view_dispatcher, PredatorView[Scene]);
    
    // Start timer
    app->timer = furi_timer_alloc([scene]_timer_callback, FuriTimerTypePeriodic, app);
    furi_timer_start(app->timer, 100);
}

bool predator_scene_[scene]_on_event(void* context, SceneManagerEvent event) {
    if(event.type == SceneManagerEventTypeCustom) {
        return true;
    }
    return false;
}

void predator_scene_[scene]_on_exit(void* context) {
    PredatorApp* app = context;
    if(!app) return;
    
    if(app->timer) {
        furi_timer_stop(app->timer);
        furi_timer_free(app->timer);
        app->timer = NULL;
    }
    
    if(app->view_dispatcher) {
        view_dispatcher_remove_view(app->view_dispatcher, PredatorView[Scene]);
    }
}
```

---

## 📋 Checklist for Each Scene

- [ ] Create new `predator_scene_[name]_ui.c` file
- [ ] Add `PredatorView[Name]` to `predator_i.h` enum
- [ ] Add scene to `predator_scene_config.h`
- [ ] Add source file to `application.fam`
- [ ] Implement header with separator line
- [ ] Implement status line with state machine
- [ ] Implement animated progress bar
- [ ] Implement real-time statistics (time, count, etc.)
- [ ] Implement results display
- [ ] Implement input handling (OK=Start, Back=Stop)
- [ ] Implement timer for 100ms updates
- [ ] Add logging integration
- [ ] Test on Flipper Zero hardware
- [ ] Add menu item to access new UI

---

## 🎯 Benefits of Professional UI

1. **Consistency** - All scenes look and feel the same
2. **Real-time Feedback** - Users always know what's happening
3. **Professional Appearance** - Impressive for demos (Elon!)
4. **Better UX** - Clear instructions, visual progress
5. **Debugging** - Easy to see status and errors
6. **Logging Integration** - All events tracked
7. **Responsive** - 100ms update rate, smooth animations

---

## 🚀 Next Steps

1. ✅ WiFi Scan UI - **COMPLETED**
2. 🔄 WiFi Deauth UI - **IN PROGRESS**
3. BLE Spam UI
4. GPS Tracker UI
5. Car Jamming UI
6. RFID Clone UI
7. RFID Bruteforce UI
8. Wardriving UI
9. WiFi Evil Twin UI
10. BLE Scan UI
11. Module Status UI
12. Settings UI

**Estimated Time:** 2-3 hours per scene (design + implementation + testing)
**Total:** ~30 hours for all 13 scenes

---

## 📝 Notes

- Keep existing popup-based scenes as fallback
- Add "(Pro UI)" suffix to menu items for new versions
- Maintain backward compatibility
- Test each scene on real hardware before moving to next
- Document any hardware-specific quirks

**This upgrade will make Predator Momentum the most professional-looking security tool on Flipper Zero!**
