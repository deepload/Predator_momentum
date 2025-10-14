# CRITICAL: Algorithm-to-Model Mapping Verification

## Problem Identified

The crypto algorithm selection is based on **brand name string matching** instead of using the **`remote_type`** field that's already in the database!

### Current Approach (SUBOPTIMAL):
```c
// Protocol test scene uses string matching
if(strstr(app->selected_model_make, "BMW") || strstr(app->selected_model_make, "Audi")) {
    uses_hitag2 = true;
}
```

### Database Has Better Data:
```c
// Each model already specifies its type!
{"BMW", "Various Rolling", 868350000, "Rolling Code", CarContinentEurope}
{"Tesla", "Model 3 2018+", 315000000, "Smart Key", CarContinentAmerica}
{"Honda", "Various Rolling", 315000000, "Rolling Code", CarContinentAsia}
```

## ⚠️ Why This Matters for Real Demonstrations

1. **BMW has multiple types**:
   - Older BMW: Fixed Code
   - Mid BMW: Rolling Code (Keeloq)
   - New BMW: Rolling Code (Hitag2)
   - Latest BMW: Smart Key

2. **Brand-based detection FAILS** when:
   - Same brand uses different protocols across years
   - Example: "BMW 3-series 2010" vs "BMW 3-series 2020"

3. **For government demos**, we need:
   - ✅ Exact protocol for each model/year
   - ✅ Correct crypto algorithm
   - ✅ Real attack success

## 🎯 Required Improvements

### 1. Protocol Selection Based on `remote_type`

**Mapping:**
```
"Fixed Code"    → Simple replay attack (no crypto)
"Rolling Code"  → Keeloq 528-round encryption
"Smart Key"     → AES-128 challenge-response
```

**Additional frequency-based refinement:**
```
Rolling Code + 868MHz + German brand → Hitag2 (not Keeloq)
Rolling Code + 315/433MHz            → Keeloq
Smart Key     + Tesla                → Tesla-specific protocol
```

### 2. Enhanced Database with Protocol Field

**Suggestion:** Add explicit `protocol` field:

```c
typedef struct {
    const char* make;
    const char* model;
    uint32_t frequency;
    const char* remote_type;     // "Fixed Code", "Rolling Code", "Smart Key"
    CarContinent continent;
    CryptoProtocol protocol;     // NEW: Explicit protocol
} PredatorCarModel;

typedef enum {
    ProtocolNone,        // Fixed code replay
    ProtocolKeeloq,      // 528-round rolling code
    ProtocolHitag2,      // BMW/Audi LFSR cipher
    ProtocolAES128,      // Modern smart key
    ProtocolTeslaSmart,  // Tesla-specific
    ProtocolCount
} CryptoProtocol;
```

### 3. Automatic Protocol Detection Function

```c
CryptoProtocol predator_models_get_protocol(size_t index) {
    const PredatorCarModel* model = predator_models_get_hardcoded(index);
    if(!model) return ProtocolNone;
    
    // Use remote_type field!
    if(strcmp(model->remote_type, "Fixed Code") == 0) {
        return ProtocolNone;
    }
    
    if(strcmp(model->remote_type, "Smart Key") == 0) {
        // Check for Tesla-specific
        if(strcmp(model->make, "Tesla") == 0) {
            return ProtocolTeslaSmart;
        }
        return ProtocolAES128;
    }
    
    if(strcmp(model->remote_type, "Rolling Code") == 0) {
        // Check frequency and brand for Hitag2
        if(model->frequency >= 868000000 && 
           (strcmp(model->make, "BMW") == 0 || 
            strcmp(model->make, "Audi") == 0 ||
            strcmp(model->make, "Volkswagen") == 0 ||
            strcmp(model->make, "Porsche") == 0)) {
            return ProtocolHitag2;
        }
        return ProtocolKeeloq;
    }
    
    return ProtocolNone;
}
```

## 🚨 Impact on Demonstrations

### Current Risk:
- ❌ Wrong algorithm for specific model/year
- ❌ Attack fails on real hardware
- ❌ Embarrassment during government demo

### With Improvements:
- ✅ Perfect algorithm match per model
- ✅ Attacks work on real hardware
- ✅ Professional demonstration
- ✅ Shows technical superiority

## 📊 Verification Needed

**Test each attack type:**

| Model Example | remote_type | Frequency | Expected Protocol | Current Selection | Status |
|--------------|-------------|-----------|-------------------|-------------------|---------|
| BMW 3-series 2020 | "Rolling Code" | 868MHz | Hitag2 | ✅ Correct | OK |
| Honda Civic 2018 | "Rolling Code" | 315MHz | Keeloq | ✅ Correct | OK |
| Tesla Model 3 | "Smart Key" | 315MHz | Tesla Smart | ⚠️ Generic AES | NEEDS FIX |
| Ford F-150 2015 | "Fixed Code" | 315MHz | None (Replay) | ❌ Might use Keeloq | NEEDS FIX |
| BYD Han 2020+ | "Smart Key" | 433MHz | AES-128 | ✅ Correct | OK |
| Ferrari SF90 | "Smart Key" | 433MHz | AES-128 | ✅ Correct | OK |

## ✅ Recommended Actions

1. **Immediate**: Add `predator_models_get_protocol()` helper function
2. **Update**: Protocol test scene to use model data instead of brand matching
3. **Verify**: Test all 315+ models have correct `remote_type` values
4. **Optional**: Add explicit `protocol` field for even more precision

## 🎯 For Best Real Functionality

**The algorithm MUST match the actual car model specification, not just the brand name.**

This ensures:
- ✅ Real attacks work on real cars
- ✅ Government demos are flawless
- ✅ Technical credibility maintained
- ✅ Market superiority demonstrated
