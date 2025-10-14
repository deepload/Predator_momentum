#include "predator_models.h"
#include <string.h>

// COMPLETE WORLDWIDE CAR MODELS DATABASE - Production Ready! 🚗
// Total: 178 models with Rolling Code, Fixed Code, and Smart Key types
// Coverage: US (315MHz), EU (433/868MHz), China (433.92MHz), Honda (433.42MHz)
// ORDER: EUROPEAN BRANDS FIRST (VW, Audi, BMW, Mercedes, Renault, etc.), then global brandsrest of world
static const PredatorCarModel hardcoded_models[] = {
    // ========================================================================
    // EUROPEAN BRANDS FIRST - Most Classic & Popular in Europe 🇪🇺
    // ========================================================================
    
    // VOLKSWAGEN (German Engineering - #1 in Europe)
    {"Volkswagen", "Atlas 2020+", 315000000, "Rolling Code", CarContinentEurope},
    {"Volkswagen", "Various Fixed", 433920000, "Fixed Code", CarContinentEurope},
    {"Volkswagen", "Various Rolling", 433920000, "Rolling Code", CarContinentEurope},
    {"Volkswagen", "Various Smart", 433920000, "Smart Key", CarContinentEurope},
    
    // AUDI (Premium German Engineering)
    {"Audi", "Various Rolling", 868350000, "Rolling Code", CarContinentEurope},
    {"Audi", "Various Smart", 868350000, "Smart Key", CarContinentEurope},
    
    // BMW (Bavarian Motor Works)
    {"BMW", "Various Rolling", 868350000, "Rolling Code", CarContinentEurope},
    {"BMW", "Various Smart", 868350000, "Smart Key", CarContinentEurope},
    
    // MERCEDES (German Luxury)
    {"Mercedes", "Various Rolling", 868350000, "Rolling Code", CarContinentEurope},
    {"Mercedes", "Various Smart", 868350000, "Smart Key", CarContinentEurope},
    {"Mercedes-Benz", "Sprinter 2018+", 868350000, "Rolling Code", CarContinentEurope},
    
    // RENAULT (French Innovation)
    {"Renault", "Various Fixed", 433920000, "Fixed Code", CarContinentEurope},
    {"Renault", "Various Rolling", 433920000, "Rolling Code", CarContinentEurope},
    {"Renault", "Zoe 2019+", 433920000, "Smart Key", CarContinentEurope},
    
    // PEUGEOT (French Style)
    {"Peugeot", "Landtrek 2020+", 433920000, "Fixed Code", CarContinentEurope},
    {"Peugeot", "Various Rolling", 433920000, "Rolling Code", CarContinentEurope},
    {"Peugeot", "Various Smart", 433920000, "Smart Key", CarContinentEurope},
    
    // FIAT (Italian Style)
    {"Fiat", "500X 2018+", 433920000, "Fixed Code", CarContinentEurope},
    {"Fiat", "Various Fixed", 315000000, "Fixed Code", CarContinentEurope},
    {"Fiat", "Various Rolling", 433920000, "Rolling Code", CarContinentEurope},
    
    // PORSCHE (German Performance)
    {"Porsche", "Various Rolling", 433920000, "Rolling Code", CarContinentEurope},
    {"Porsche", "Various Smart", 433920000, "Smart Key", CarContinentEurope},
    
    // SKODA (Czech Engineering - VW Group)
    {"Skoda", "Enyaq 2021+", 433920000, "Smart Key", CarContinentEurope},
    {"Skoda", "Various Fixed", 433920000, "Fixed Code", CarContinentEurope},
    {"Skoda", "Various Rolling", 433920000, "Rolling Code", CarContinentEurope},
    
    // SEAT (Spanish Design - VW Group)
    {"Seat", "Various Fixed", 433920000, "Fixed Code", CarContinentEurope},
    {"Seat", "Various Rolling", 433920000, "Rolling Code", CarContinentEurope},
    
    // VOLVO (Swedish Safety)
    {"Volvo", "Various Rolling", 868350000, "Rolling Code", CarContinentEurope},
    {"Volvo", "Various Smart", 868350000, "Smart Key", CarContinentEurope},
    
    // JAGUAR (British Elegance)
    {"Jaguar", "Various Fixed", 433920000, "Fixed Code", CarContinentEurope},
    {"Jaguar", "Various Smart", 433920000, "Smart Key", CarContinentEurope},
    
    // RANGE ROVER (British Luxury)
    {"Range Rover", "Various Fixed", 433920000, "Fixed Code", CarContinentEurope},
    {"Range Rover", "Various Smart", 433920000, "Smart Key", CarContinentEurope},
    
    // === PREMIUM EUROPEAN SUPERCARS ===
    
    // FERRARI (Italian Supercar)
    {"Ferrari", "488 GTB 2015+", 433920000, "Smart Key", CarContinentEurope},
    {"Ferrari", "F8 Tributo 2019+", 433920000, "Smart Key", CarContinentEurope},
    {"Ferrari", "SF90 Stradale 2019+", 433920000, "Smart Key", CarContinentEurope},
    {"Ferrari", "Roma 2020+", 433920000, "Smart Key", CarContinentEurope},
    {"Ferrari", "Portofino 2017+", 433920000, "Smart Key", CarContinentEurope},
    {"Ferrari", "812 Superfast 2017+", 433920000, "Smart Key", CarContinentEurope},
    {"Ferrari", "296 GTB 2022+", 433920000, "Smart Key", CarContinentEurope},
    
    // LAMBORGHINI (Italian Supercar)
    {"Lamborghini", "Huracan 2014+", 433920000, "Smart Key", CarContinentEurope},
    {"Lamborghini", "Aventador 2011+", 433920000, "Smart Key", CarContinentEurope},
    {"Lamborghini", "Urus 2018+", 433920000, "Smart Key", CarContinentEurope},
    {"Lamborghini", "Gallardo 2003-2013", 433920000, "Rolling Code", CarContinentEurope},
    {"Lamborghini", "Murcielago 2001-2010", 433920000, "Fixed Code", CarContinentEurope},
    
    // MASERATI (Italian Luxury)
    {"Maserati", "Ghibli 2014+", 433920000, "Smart Key", CarContinentEurope},
    {"Maserati", "Quattroporte 2013+", 433920000, "Smart Key", CarContinentEurope},
    {"Maserati", "Levante 2016+", 433920000, "Smart Key", CarContinentEurope},
    {"Maserati", "MC20 2021+", 433920000, "Smart Key", CarContinentEurope},
    {"Maserati", "GranTurismo 2007-2019", 433920000, "Rolling Code", CarContinentEurope},
    
    // BENTLEY (British Ultra-Luxury)
    {"Bentley", "Continental GT 2018+", 433920000, "Smart Key", CarContinentEurope},
    {"Bentley", "Flying Spur 2019+", 433920000, "Smart Key", CarContinentEurope},
    {"Bentley", "Bentayga 2016+", 433920000, "Smart Key", CarContinentEurope},
    {"Bentley", "Mulsanne 2010-2020", 433920000, "Smart Key", CarContinentEurope},
    
    // ROLLS-ROYCE (British Ultra-Luxury)
    {"Rolls-Royce", "Phantom 2017+", 433920000, "Smart Key", CarContinentEurope},
    {"Rolls-Royce", "Ghost 2020+", 433920000, "Smart Key", CarContinentEurope},
    {"Rolls-Royce", "Cullinan 2018+", 433920000, "Smart Key", CarContinentEurope},
    {"Rolls-Royce", "Wraith 2013+", 433920000, "Smart Key", CarContinentEurope},
    
    // ASTON MARTIN (British Luxury)
    {"Aston Martin", "DB11 2016+", 433920000, "Smart Key", CarContinentEurope},
    {"Aston Martin", "DBS Superleggera 2018+", 433920000, "Smart Key", CarContinentEurope},
    {"Aston Martin", "Vantage 2018+", 433920000, "Smart Key", CarContinentEurope},
    {"Aston Martin", "DBX 2020+", 433920000, "Smart Key", CarContinentEurope},
    
    // McLAREN (British Supercar)
    {"McLaren", "720S 2017+", 433920000, "Smart Key", CarContinentEurope},
    {"McLaren", "765LT 2020+", 433920000, "Smart Key", CarContinentEurope},
    {"McLaren", "Artura 2021+", 433920000, "Smart Key", CarContinentEurope},
    {"McLaren", "GT 2019+", 433920000, "Smart Key", CarContinentEurope},
    
    // BUGATTI (French Hypercar)
    {"Bugatti", "Chiron 2016+", 433920000, "Smart Key", CarContinentEurope},
    {"Bugatti", "Veyron 2005-2015", 433920000, "Smart Key", CarContinentEurope},
    
    // ========================================================================
    // AMERICAN BRANDS 🇺🇸
    // ========================================================================
    
    // FORD (Built Ford Tough)
    {"Ford", "Various Fixed 315", 315000000, "Fixed Code", CarContinentAmerica},
    {"Ford", "Various Fixed 433", 433920000, "Fixed Code", CarContinentAmerica},
    {"Ford", "Various Rolling 315", 315000000, "Rolling Code", CarContinentAmerica},
    {"Ford", "Various Rolling 433", 433920000, "Rolling Code", CarContinentAmerica},
    {"Ford", "Various Smart", 315000000, "Smart Key", CarContinentAmerica},
    
    // CHEVROLET (American Classic)
    {"Chevrolet", "Various Fixed", 315000000, "Fixed Code", CarContinentAmerica},
    {"Chevrolet", "Various Rolling", 315000000, "Rolling Code", CarContinentAmerica},
    {"Chevrolet", "Various Smart", 315000000, "Smart Key", CarContinentAmerica},
    
    // JEEP (Go Anywhere)
    {"Jeep", "Avenger 2023+", 433920000, "Smart Key", CarContinentAmerica},
    {"Jeep", "Various Fixed", 315000000, "Fixed Code", CarContinentAmerica},
    {"Jeep", "Various Rolling", 315000000, "Rolling Code", CarContinentAmerica},
    
    // DODGE (American Muscle)
    {"Dodge", "Various Fixed", 315000000, "Fixed Code", CarContinentAmerica},
    {"Dodge", "Various Rolling", 315000000, "Rolling Code", CarContinentAmerica},
    {"Dodge", "Various Smart", 315000000, "Smart Key", CarContinentAmerica},
    
    // CHRYSLER (American Heritage)
    {"Chrysler", "Various Fixed", 315000000, "Fixed Code", CarContinentAmerica},
    {"Chrysler", "Various Rolling", 315000000, "Rolling Code", CarContinentAmerica},
    
    // CADILLAC (American Luxury)
    {"Cadillac", "Various Rolling", 315000000, "Rolling Code", CarContinentAmerica},
    {"Cadillac", "Various Smart", 315000000, "Smart Key", CarContinentAmerica},
    
    // TESLA (American EV Leader) - CRITICAL!
    {"Tesla", "Model S 2012-2016", 315000000, "Rolling Code", CarContinentAmerica},
    {"Tesla", "Model S 2017+", 315000000, "Smart Key", CarContinentAmerica},
    {"Tesla", "Model 3 2018+", 315000000, "Smart Key", CarContinentAmerica},
    {"Tesla", "Model X 2016-2020", 315000000, "Rolling Code", CarContinentAmerica},
    {"Tesla", "Model X 2021+", 315000000, "Smart Key", CarContinentAmerica},
    {"Tesla", "Model Y 2020+", 315000000, "Smart Key", CarContinentAmerica},
    {"Tesla", "Cybertruck 2024+", 315000000, "Smart Key", CarContinentAmerica},
    {"Tesla", "Roadster 2008-2012", 315000000, "Fixed Code", CarContinentAmerica},
    {"Tesla", "Roadster 2023+", 315000000, "Smart Key", CarContinentAmerica},
    {"Tesla", "Semi 2023+", 315000000, "Smart Key", CarContinentAmerica},
    
    // ========================================================================
    // JAPANESE BRANDS 
    // ========================================================================
    
    // TOYOTA (Japanese Quality)
    {"Toyota", "Various Fixed 315", 315000000, "Fixed Code", CarContinentAsia},
    {"Toyota", "Various Fixed 433", 433920000, "Fixed Code", CarContinentAsia},
    {"Toyota", "Various Rolling 315", 315000000, "Rolling Code", CarContinentAsia},
    {"Toyota", "Various Rolling 433", 433920000, "Rolling Code", CarContinentAsia},
    {"Toyota", "Various Smart", 433920000, "Smart Key", CarContinentAsia},
    
    // HONDA (Japanese Reliability)
    {"Honda", "Various Fixed", 315000000, "Fixed Code", CarContinentAsia},
    {"Honda", "Various Rolling", 315000000, "Rolling Code", CarContinentAsia},
    {"Honda", "Various Special", 433420000, "Rolling Code", CarContinentAsia},
    {"Honda", "Various", 433420000, "Rolling Code", CarContinentAsia},
    {"Honda", "Various", 433420000, "Fixed Code", CarContinentAsia},
    {"Honda", "Various", 433420000, "Smart Key", CarContinentAsia},
    
    // NISSAN (Japanese Innovation)
    {"Nissan", "Leaf 2019+", 433920000, "Smart Key", CarContinentAsia},
    {"Nissan", "Various Fixed 315", 315000000, "Fixed Code", CarContinentAsia},
    {"Nissan", "Various Fixed 433", 433920000, "Fixed Code", CarContinentAsia},
    {"Nissan", "Various Rolling 315", 315000000, "Rolling Code", CarContinentAsia},
    {"Nissan", "Various Rolling 433", 433920000, "Rolling Code", CarContinentAsia},
    {"Nissan", "Various Smart", 315000000, "Smart Key", CarContinentAsia},
    
    // MAZDA (Japanese Engineering)
    {"Mazda", "2 2018+", 433920000, "Fixed Code", CarContinentAsia},
    {"Mazda", "Various Rolling", 433920000, "Rolling Code", CarContinentAsia},
    {"Mazda", "Various Smart", 433920000, "Smart Key", CarContinentAsia},
    
    // MITSUBISHI (Japanese Reliability)
    {"Mitsubishi", "Various Fixed", 433920000, "Fixed Code", CarContinentAsia},
    {"Mitsubishi", "Various Rolling", 433920000, "Rolling Code", CarContinentAsia},
    
    // SUBARU (Japanese AWD)
    {"Subaru", "Ascent 2019+", 315000000, "Rolling Code", CarContinentAsia},
    {"Subaru", "Various Rolling", 433920000, "Rolling Code", CarContinentAsia},
    {"Subaru", "Various Smart", 433920000, "Smart Key", CarContinentAsia},
    
    // LEXUS (Japanese Luxury)
    {"Lexus", "LX 2019+", 315000000, "Smart Key", CarContinentAsia},
    {"Lexus", "Various Rolling", 433920000, "Rolling Code", CarContinentAsia},
    {"Lexus", "Various Smart", 433920000, "Smart Key", CarContinentAsia},
    
    // INFINITI (Japanese Luxury)
    {"Infiniti", "Various Fixed", 315000000, "Fixed Code", CarContinentAsia},
    {"Infiniti", "Various Rolling", 315000000, "Rolling Code", CarContinentAsia},
    {"Infiniti", "Various Smart", 315000000, "Smart Key", CarContinentAsia},
    
    // ACURA (Japanese Luxury)
    {"Acura", "Integra 2022+", 315000000, "Smart Key", CarContinentAsia},
    {"Acura", "Various Fixed", 315000000, "Fixed Code", CarContinentAsia},
    {"Acura", "Various Rolling", 315000000, "Rolling Code", CarContinentAsia},
    
    // ========================================================================
    // KOREAN BRANDS 
    // ========================================================================
    
    // HYUNDAI (Korean Innovation)
    {"Hyundai", "Various Fixed", 433920000, "Fixed Code", CarContinentAsia},
    {"Hyundai", "Various Rolling", 433920000, "Rolling Code", CarContinentAsia},
    {"Hyundai", "Various Smart", 433920000, "Smart Key", CarContinentAsia},
    
    // KIA (Korean Quality)
    {"Kia", "Various Fixed", 433920000, "Fixed Code", CarContinentAsia},
    {"Kia", "Various Rolling", 433920000, "Rolling Code", CarContinentAsia},
    {"Kia", "Various Smart", 433920000, "Smart Key", CarContinentAsia},
    
    // ========================================================================
    // CHINESE MANUFACTURERS 🇨🇳 (China Standard 433.92MHz)
    // ========================================================================
    
    // BYD (比亚迪) - World's Largest EV Maker
    {"BYD", "Han 2020+", 433920000, "Smart Key", CarContinentAsia},
    {"BYD", "Tang 2018+", 433920000, "Smart Key", CarContinentAsia},
    {"BYD", "Song Plus", 433920000, "Rolling Code", CarContinentAsia},
    {"BYD", "Seal 2022+", 433920000, "Smart Key", CarContinentAsia},
    {"BYD", "Dolphin 2021+", 433920000, "Smart Key", CarContinentAsia},
    {"BYD", "Various Rolling", 433920000, "Rolling Code", CarContinentAsia},
    
    // NIO (蔚来) - Premium EVs
    {"NIO", "ES6 2019+", 433920000, "Smart Key", CarContinentAsia},
    {"NIO", "ES8 2018+", 433920000, "Smart Key", CarContinentAsia},
    {"NIO", "ET5 2022+", 433920000, "Smart Key", CarContinentAsia},
    {"NIO", "ET7 2022+", 433920000, "Smart Key", CarContinentAsia},
    
    // XPENG (小鹏) - Tech-Focused EVs
    {"Xpeng", "P7 2020+", 433920000, "Smart Key", CarContinentAsia},
    {"Xpeng", "P5 2021+", 433920000, "Smart Key", CarContinentAsia},
    {"Xpeng", "G9 2022+", 433920000, "Smart Key", CarContinentAsia},
    {"Xpeng", "G3 2019+", 433920000, "Smart Key", CarContinentAsia},
    
    // GEELY (吉利) - Major Manufacturer
    {"Geely", "Emgrand GT", 433920000, "Rolling Code", CarContinentAsia},
    {"Geely", "Boyue 2018+", 433920000, "Rolling Code", CarContinentAsia},
    {"Geely", "Coolray 2020+", 433920000, "Smart Key", CarContinentAsia},
    {"Geely", "Various Rolling", 433920000, "Rolling Code", CarContinentAsia},
    
    // GREAT WALL (长城) - SUVs & Pickups
    {"Great Wall", "Haval H6", 433920000, "Rolling Code", CarContinentAsia},
    {"Great Wall", "WEY VV7", 433920000, "Smart Key", CarContinentAsia},
    {"Great Wall", "Tank 300", 433920000, "Rolling Code", CarContinentAsia},
    
    // MG (SAIC Motor) - Global Brand
    {"MG", "ZS EV 2019+", 433920000, "Smart Key", CarContinentAsia},
    {"MG", "HS 2019+", 433920000, "Rolling Code", CarContinentAsia},
    {"MG", "Various", 433920000, "Rolling Code", CarContinentAsia},
    
    // ROEWE (上汽荣威) - SAIC Brand
    {"Roewe", "RX5 2016+", 433920000, "Rolling Code", CarContinentAsia},
    {"Roewe", "i6 2017+", 433920000, "Rolling Code", CarContinentAsia},
    
    // CHANGAN (长安)
    {"Changan", "CS75 Plus", 433920000, "Rolling Code", CarContinentAsia},
    {"Changan", "UNI-T 2020+", 433920000, "Smart Key", CarContinentAsia},
    {"Changan", "Eado 2018+", 433920000, "Rolling Code", CarContinentAsia},
    
    // HONGQI (红旗) - Luxury State Cars
    {"Hongqi", "H9 2020+", 433920000, "Smart Key", CarContinentAsia},
    {"Hongqi", "E-HS9 2021+", 433920000, "Smart Key", CarContinentAsia},
    
    // LI AUTO (理想) - Extended-Range EVs
    {"Li Auto", "Li ONE 2019+", 433920000, "Smart Key", CarContinentAsia},
    {"Li Auto", "L7 2023+", 433920000, "Smart Key", CarContinentAsia},
    {"Li Auto", "L8 2022+", 433920000, "Smart Key", CarContinentAsia},
    {"Li Auto", "L9 2022+", 433920000, "Smart Key", CarContinentAsia},
    
    // ZEEKR (极氪) - Geely's Premium EV
    {"Zeekr", "001 2021+", 433920000, "Smart Key", CarContinentAsia},
    {"Zeekr", "X 2023+", 433920000, "Smart Key", CarContinentAsia},
    
    // LYNK & CO (领克) - Geely-Volvo Joint
    {"Lynk & Co", "01 2017+", 433920000, "Rolling Code", CarContinentAsia},
    {"Lynk & Co", "03 2018+", 433920000, "Smart Key", CarContinentAsia},
    
    // JAC (江淮) - Commercial & Passenger
    {"JAC", "iEV7S", 433920000, "Fixed Code", CarContinentAsia},
    {"JAC", "Various", 433920000, "Rolling Code", CarContinentAsia},
    
    // AIWAYS (爱驰) - EV Startup
    {"Aiways", "U5 2019+", 433920000, "Smart Key", CarContinentAsia},
    
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
    return (strcmp(hardcoded_models[index].remote_type, "Rolling Code") == 0);
}

// Check if a model uses smart key (for attack logic)
bool predator_models_uses_smart_key(size_t index) {
    if(index >= hardcoded_models_count) {
        return false;
    }
    return (strcmp(hardcoded_models[index].remote_type, "Smart Key") == 0);
}

// Check if a model uses fixed code (for attack logic)
bool predator_models_uses_fixed_code(size_t index) {
    if(index >= hardcoded_models_count) {
        return false;
    }
    return (strcmp(hardcoded_models[index].remote_type, "Fixed Code") == 0);
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
    if(strcmp(model->remote_type, "Fixed Code") == 0) {
        return CryptoProtocolNone;
    }
    
    // SMART KEY: AES-128 or Tesla-specific
    if(strcmp(model->remote_type, "Smart Key") == 0) {
        // Tesla uses proprietary protocol
        if(strcmp(model->make, "Tesla") == 0) {
            return CryptoProtocolTesla;
        }
        // All other smart keys use AES-128
        return CryptoProtocolAES128;
    }
    
    // ROLLING CODE: Keeloq OR Hitag2 (depends on frequency + manufacturer)
    if(strcmp(model->remote_type, "Rolling Code") == 0) {
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
        case CryptoProtocolNone:    return "Fixed Code (Replay)";
        case CryptoProtocolKeeloq:  return "Keeloq Rolling Code";
        case CryptoProtocolHitag2:  return "Hitag2 (BMW/Audi)";
        case CryptoProtocolAES128:  return "AES-128 Smart Key";
        case CryptoProtocolTesla:   return "Tesla Protocol";
        default:                    return "Unknown";
    }
}

// Check if model uses specific protocol
bool predator_models_uses_protocol(size_t index, CryptoProtocol protocol) {
    return predator_models_get_protocol(index) == protocol;
}
