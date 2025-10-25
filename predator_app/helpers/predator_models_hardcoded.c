#include "predator_models.h"
#include <string.h>

// Frequency mapping: 0=315MHz, 1=433.92MHz, 2=868MHz, 3=433.42MHz
#define F315 315000000
#define F433 433920000  
#define F868 868350000
#define F434 433420000

// MINIMAL MODEL DATABASE - ESSENTIAL ONLY (Use VIN codes for full database)
static const PredatorCarModel hardcoded_models[] = {
    // ESSENTIAL MODELS ONLY - FULL DATABASE IN predator_vin_codes.c
    // ESSENTIAL MODELS ONLY - MAJOR BRANDS REPRESENTED
    {"Tesla", "Model 3", F315, "SK", CarContinentAmerica},
    {"Tesla", "Model Y", F315, "SK", CarContinentAmerica},
    {"Ford", "F150", F315, "RC", CarContinentAmerica},
    {"Chevrolet", "Silverado", F315, "RC", CarContinentAmerica},
    {"Toyota", "Camry", F315, "RC", CarContinentAsia},
    {"Honda", "Civic", F315, "RC", CarContinentAsia},
    {"VW", "Golf", F433, "RC", CarContinentEurope},
    {"BMW", "3 Series", F868, "RC", CarContinentEurope},
    {"Mercedes", "C-Class", F868, "RC", CarContinentEurope},
    {"Audi", "A4", F868, "RC", CarContinentEurope},
    
    // PEUGEOT (French Style)
    {"Peugeot", "Landtrek 2020+", F433, "FC", CarContinentEurope},
    {"Peugeot", "Roll", F433, "RC", CarContinentEurope},
    {"Peugeot", "Smart", F433, "SK", CarContinentEurope},
    
    // FIAT (Italian Style)
    {"Fiat", "500X 2018+", F433, "FC", CarContinentEurope},
    {"Fiat", "Fix", F315, "FC", CarContinentEurope},
    {"Fiat", "Roll", F433, "RC", CarContinentEurope},
    
    // PORSCHE (German Performance)
    {"Porsche", "Roll", F433, "RC", CarContinentEurope},
    {"Porsche", "Smart", F433, "SK", CarContinentEurope},
    
    // SKODA (Czech Engineering - VW Group)
    {"Skoda", "Enyaq 2021+", F433, "SK", CarContinentEurope},
    {"Skoda", "Fix", F433, "FC", CarContinentEurope},
    {"Skoda", "Roll", F433, "RC", CarContinentEurope},
    
    // SEAT (Spanish Design - VW Group)
    {"Seat", "Fix", F433, "FC", CarContinentEurope},
    {"Seat", "Roll", F433, "RC", CarContinentEurope},
    
    // VOLVO (Swedish Safety)
    {"Volvo", "Roll", F868, "RC", CarContinentEurope},
    {"Volvo", "Smart", F868, "SK", CarContinentEurope},
    
    // JAGUAR (British Elegance)
    {"Jaguar", "Fix", F433, "FC", CarContinentEurope},
    {"Jaguar", "Smart", F433, "SK", CarContinentEurope},
    
    // RANGE ROVER (British Luxury)
    {"Range Rover", "Fix", F433, "FC", CarContinentEurope},
    {"Range Rover", "Smart", F433, "SK", CarContinentEurope},
    
    // COMMENTED OUT LUXURY CARS TO SAVE SPACE - UNCOMMENT IF NEEDED
    /*
    {"Ferrari", "488 GTB 2015+", F433, "SK", CarContinentEurope},
    {"Ferrari", "F8 Tributo 2019+", F433, "SK", CarContinentEurope},
    {"Ferrari", "SF90 Stradale 2019+", F433, "SK", CarContinentEurope},
    {"Ferrari", "Roma 2020+", F433, "SK", CarContinentEurope},
    {"Ferrari", "Portofino 2017+", F433, "SK", CarContinentEurope},
    {"Ferrari", "812 Superfast 2017+", F433, "SK", CarContinentEurope},
    {"Ferrari", "296 GTB 2022+", F433, "SK", CarContinentEurope},
    {"Lamborghini", "Huracan 2014+", F433, "SK", CarContinentEurope},
    {"Lamborghini", "Aventador 2011+", F433, "SK", CarContinentEurope},
    {"Lamborghini", "Urus 2018+", F433, "SK", CarContinentEurope},
    {"Lamborghini", "Gallardo 2003-2013", F433, "RC", CarContinentEurope},
    {"Lamborghini", "Murcielago 2001-2010", F433, "FC", CarContinentEurope},
    {"Maserati", "Ghibli 2014+", F433, "SK", CarContinentEurope},
    {"Maserati", "Quattroporte 2013+", F433, "SK", CarContinentEurope},
    {"Maserati", "Levante 2016+", F433, "SK", CarContinentEurope},
    {"Maserati", "MC20 2021+", F433, "SK", CarContinentEurope},
    {"Maserati", "GranTurismo 2007-2019", F433, "RC", CarContinentEurope},
    {"Bentley", "Continental GT 2018+", F433, "SK", CarContinentEurope},
    {"Bentley", "Flying Spur 2019+", F433, "SK", CarContinentEurope},
    {"Bentley", "Bentayga 2016+", F433, "SK", CarContinentEurope},
    {"Bentley", "Mulsanne 2010-2020", F433, "SK", CarContinentEurope},
    {"Rolls-Royce", "Phantom 2017+", F433, "SK", CarContinentEurope},
    {"Rolls-Royce", "Ghost 2020+", F433, "SK", CarContinentEurope},
    {"Rolls-Royce", "Cullinan 2018+", F433, "SK", CarContinentEurope},
    {"Rolls-Royce", "Wraith 2013+", F433, "SK", CarContinentEurope},
    {"Aston Martin", "DB11 2016+", F433, "SK", CarContinentEurope},
    {"Aston Martin", "DBS Superleggera 2018+", F433, "SK", CarContinentEurope},
    {"Aston Martin", "Vantage 2018+", F433, "SK", CarContinentEurope},
    {"Aston Martin", "DBX 2020+", F433, "SK", CarContinentEurope},
    {"McLaren", "720S 2017+", F433, "SK", CarContinentEurope},
    {"McLaren", "765LT 2020+", F433, "SK", CarContinentEurope},
    {"McLaren", "Artura 2021+", F433, "SK", CarContinentEurope},
    {"McLaren", "GT 2019+", F433, "SK", CarContinentEurope},
    {"Bugatti", "Chiron 2016+", F433, "SK", CarContinentEurope},
    {"Bugatti", "Veyron 2005-2015", F433, "SK", CarContinentEurope},
    */
    
    // ========================================================================
    // AMERICAN BRANDS 🇺🇸
    // ========================================================================
    
    // FORD (Built Ford Tough) - ESSENTIAL ONLY
    {"Ford", "Fix", F315, "FC", CarContinentAmerica},
    {"Ford", "Roll", F315, "RC", CarContinentAmerica},
    {"Ford", "Smart", F315, "SK", CarContinentAmerica},
    
    // CHEVROLET (American Classic)
    {"Chevrolet", "Fix", F315, "FC", CarContinentAmerica},
    {"Chevrolet", "Roll", F315, "RC", CarContinentAmerica},
    {"Chevrolet", "Smart", F315, "SK", CarContinentAmerica},
    
    // JEEP (Go Anywhere)
    {"Jeep", "Avenger 2023+", F433, "SK", CarContinentAmerica},
    {"Jeep", "Fix", F315, "FC", CarContinentAmerica},
    {"Jeep", "Roll", F315, "RC", CarContinentAmerica},
    
    // DODGE (American Muscle)
    {"Dodge", "Fix", F315, "FC", CarContinentAmerica},
    {"Dodge", "Roll", F315, "RC", CarContinentAmerica},
    {"Dodge", "Smart", F315, "SK", CarContinentAmerica},
    
    // CHRYSLER (American Heritage)
    {"Chrysler", "Fix", F315, "FC", CarContinentAmerica},
    {"Chrysler", "Roll", F315, "RC", CarContinentAmerica},
    
    // CADILLAC (American Luxury)
    {"Cadillac", "Roll", F315, "RC", CarContinentAmerica},
    {"Cadillac", "Smart", F315, "SK", CarContinentAmerica},
    
    // TESLA (American EV Leader) - CRITICAL!
    {"Tesla", "Model S 2012-2016", F315, "RC", CarContinentAmerica},
    {"Tesla", "Model S 2017+", F315, "SK", CarContinentAmerica},
    {"Tesla", "Model 3 2018+", F315, "SK", CarContinentAmerica},
    {"Tesla", "Model X 2016-2020", F315, "RC", CarContinentAmerica},
    {"Tesla", "Model X 2021+", F315, "SK", CarContinentAmerica},
    {"Tesla", "Model Y 2020+", F315, "SK", CarContinentAmerica},
    {"Tesla", "Cybertruck 2024+", F315, "SK", CarContinentAmerica},
    {"Tesla", "Roadster 2008-2012", F315, "FC", CarContinentAmerica},
    {"Tesla", "Roadster 2023+", F315, "SK", CarContinentAmerica},
    {"Tesla", "Semi 2023+", F315, "SK", CarContinentAmerica},
    
    // ========================================================================
    // JAPANESE BRANDS 
    // ========================================================================
    
    // TOYOTA (Japanese Quality) - ESSENTIAL ONLY
    {"Toyota", "Fix", F315, "FC", CarContinentAsia},
    {"Toyota", "Roll", F315, "RC", CarContinentAsia},
    {"Toyota", "Smart", F433, "SK", CarContinentAsia},
    
    // HONDA (Japanese Reliability) - ESSENTIAL ONLY
    {"Honda", "Fix", F315, "FC", CarContinentAsia},
    {"Honda", "Roll", F315, "RC", CarContinentAsia},
    {"Honda", "Special", F434, "RC", CarContinentAsia},
    
    // NISSAN (Japanese Innovation) - ESSENTIAL ONLY
    {"Nissan", "Leaf 2019+", F433, "SK", CarContinentAsia},
    {"Nissan", "Fix", F315, "FC", CarContinentAsia},
    {"Nissan", "Roll", F315, "RC", CarContinentAsia},
    
    // MAZDA (Japanese Engineering)
    {"Mazda", "2 2018+", F433, "FC", CarContinentAsia},
    {"Mazda", "Roll", F433, "RC", CarContinentAsia},
    {"Mazda", "Smart", F433, "SK", CarContinentAsia},
    
    // MITSUBISHI (Japanese Reliability)
    {"Mitsubishi", "Fix", F433, "FC", CarContinentAsia},
    {"Mitsubishi", "Roll", F433, "RC", CarContinentAsia},
    
    // SUBARU (Japanese AWD)
    {"Subaru", "Ascent 2019+", F315, "RC", CarContinentAsia},
    {"Subaru", "Roll", F433, "RC", CarContinentAsia},
    {"Subaru", "Smart", F433, "SK", CarContinentAsia},
    
    // LEXUS (Japanese Luxury)
    {"Lexus", "LX 2019+", F315, "SK", CarContinentAsia},
    {"Lexus", "Roll", F433, "RC", CarContinentAsia},
    {"Lexus", "Smart", F433, "SK", CarContinentAsia},
    
    // INFINITI (Japanese Luxury)
    {"Infiniti", "Fix", F315, "FC", CarContinentAsia},
    {"Infiniti", "Roll", F315, "RC", CarContinentAsia},
    {"Infiniti", "Smart", F315, "SK", CarContinentAsia},
    
    // ACURA (Japanese Luxury)
    {"Acura", "Integra 2022+", F315, "SK", CarContinentAsia},
    {"Acura", "Fix", F315, "FC", CarContinentAsia},
    {"Acura", "Roll", F315, "RC", CarContinentAsia},
    
    // ========================================================================
    // KOREAN BRANDS 
    // ========================================================================
    
    // HYUNDAI (Korean Innovation)
    {"Hyundai", "Fix", F433, "FC", CarContinentAsia},
    {"Hyundai", "Roll", F433, "RC", CarContinentAsia},
    {"Hyundai", "Smart", F433, "SK", CarContinentAsia},
    
    // KIA (Korean Quality)
    {"Kia", "Fix", F433, "FC", CarContinentAsia},
    {"Kia", "Roll", F433, "RC", CarContinentAsia},
    {"Kia", "Smart", F433, "SK", CarContinentAsia},
    
    // ========================================================================
    // CHINESE MANUFACTURERS 🇨🇳 (China Standard 433.92MHz) - OPTIMIZED
    // ========================================================================
    
    // Major Chinese EVs - ESSENTIAL ONLY
    {"BYD", "All Models", F433, "SK", CarContinentAsia},
    {"NIO", "All Models", F433, "SK", CarContinentAsia},
    {"Xpeng", "All Models", F433, "SK", CarContinentAsia},
    {"Geely", "All Models", F433, "RC", CarContinentAsia},
    
    // === END CHINESE MANUFACTURERS ===
    
    // AIWAYS (爱驰) - EV Startup
    {"Aiways", "U5 2019+", F433, "SK", CarContinentAsia},
    
    // === END CHINESE MANUFACTURERS ===
};

static const size_t hardcoded_models_count = sizeof(hardcoded_models) / sizeof(hardcoded_models[0]);

// Return hardcoded model count
size_t predator_models_get_hardcoded_count(void) {
    return hardcoded_models_count;
}

// Get hardcoded model by index
const PredatorCarModel* predator_models_get_hardcoded(size_t index) {
    if(index >= hardcoded_models_count) {
        return NULL;
    }
    return &hardcoded_models[index];
}

// Check if a model uses rolling code (for attack logic)
bool predator_models_uses_rolling_code(size_t index) {
    if(index >= hardcoded_models_count) {
        return false;
    }
    return (strcmp(hardcoded_models[index].remote_type, "RC") == 0);
}

// Check if a model uses smart key (for attack logic)
bool predator_models_uses_smart_key(size_t index) {
    if(index >= hardcoded_models_count) {
        return false;
    }
    return (strcmp(hardcoded_models[index].remote_type, "SK") == 0);
}

// Check if a model uses fixed code (for attack logic)
bool predator_models_uses_fixed_code(size_t index) {
    if(index >= hardcoded_models_count) {
        return false;
    }
    return (strcmp(hardcoded_models[index].remote_type, "FC") == 0);
}

// Get continent from model struct (optimized - no string comparisons!)
CarContinent predator_models_get_continent(size_t index) {
    if(index >= hardcoded_models_count) {
        return CarContinentEurope; // Default
    }
    return hardcoded_models[index].continent;
}

// Get continent name as string
const char* predator_models_get_continent_name(CarContinent continent) {
    switch(continent) {
        case CarContinentEurope: return "Europe";
        case CarContinentAsia: return "Asia";
        case CarContinentAmerica: return "America";
        default: return "Unknown";
    }
}

// Check if model belongs to a specific continent
bool predator_models_is_continent(size_t index, CarContinent continent) {
    return predator_models_get_continent(index) == continent;
}

// ===== CRYPTO PROTOCOL DETECTION (INTELLIGENT) =====
// Automatically detects the correct crypto algorithm based on model data

CryptoProtocol predator_models_get_protocol(size_t index) {
    const PredatorCarModel* model = predator_models_get_hardcoded(index);
    if(!model) return CryptoProtocolNone;
    
    // Use the remote_type field from database (NOT just brand name!)
    
    // FIXED CODE: No encryption, simple replay
    if(strcmp(model->remote_type, "FC") == 0) {
        return CryptoProtocolNone;
    }
    
    // SMART KEY: AES-128 or Tesla-specific
    if(strcmp(model->remote_type, "SK") == 0) {
        // Tesla uses proprietary protocol
        if(strcmp(model->make, "Tesla") == 0) {
            return CryptoProtocolTesla;
        }
        // All other smart keys use AES-128
        return CryptoProtocolAES128;
    }
    
    // ROLLING CODE: Keeloq OR Hitag2 (depends on frequency + manufacturer)
    if(strcmp(model->remote_type, "RC") == 0) {
        // Hitag2: German brands at 868MHz
        if(model->frequency >= 868000000 && model->frequency < 869000000) {
            if(strcmp(model->make, "BMW") == 0 ||
               strcmp(model->make, "Audi") == 0 ||
               strcmp(model->make, "Volkswagen") == 0 ||
               strcmp(model->make, "Porsche") == 0 ||
               strcmp(model->make, "Skoda") == 0 ||
               strcmp(model->make, "Seat") == 0) {
                return CryptoProtocolHitag2;
            }
        }
        
        // All other rolling code vehicles use Keeloq
        return CryptoProtocolKeeloq;
    }
    
    // Default: No encryption
    return CryptoProtocolNone;
}

// Get protocol name as human-readable string
const char* predator_models_get_protocol_name(CryptoProtocol protocol) {
    switch(protocol) {
        case CryptoProtocolNone:    return "FC (Replay)";
        case CryptoProtocolKeeloq:  return "Keeloq RC";
        case CryptoProtocolHitag2:  return "Hitag2 (BMW/Audi)";
        case CryptoProtocolAES128:  return "AES-128 SK";
        case CryptoProtocolTesla:   return "Tesla Protocol";
        default:                    return "Unknown";
    }
}

// Check if model uses specific protocol
bool predator_models_uses_protocol(size_t index, CryptoProtocol protocol) {
    return predator_models_get_protocol(index) == protocol;
}

// =====================================================
// WRAPPER FUNCTIONS FOR COMPATIBILITY
// =====================================================

// Wrapper function to match predator_models.h API
size_t predator_models_count(void) {
    return predator_models_get_hardcoded_count();
}

// Wrapper function to match predator_models.h API
const PredatorCarModel* predator_models_get(size_t index) {
    return predator_models_get_hardcoded(index);
}
