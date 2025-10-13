#include "predator_models.h"
#include <string.h>

// COMPLETE WORLDWIDE CAR MODELS DATABASE - Production Ready! 🚗
// Total: 178 models with Rolling Code, Fixed Code, and Smart Key types
// Coverage: US (315MHz), EU (433/868MHz), China (433.92MHz), Honda (433.42MHz)
// Includes: Standard brands + Chinese EVs + Premium/Luxury (Tesla, Ferrari, Lamborghini, etc.)
static const PredatorCarModel hardcoded_models[] = {
    // ACURA (Alphabetical Order)
    {"Acura", "Integra 2022+", 315000000, "Smart Key"},
    {"Acura", "Various Fixed", 315000000, "Fixed Code"},
    {"Acura", "Various Rolling", 315000000, "Rolling Code"},
    
    // AUDI (Premium German Engineering)
    {"Audi", "Various Rolling", 868350000, "Rolling Code"},
    {"Audi", "Various Smart", 868350000, "Smart Key"},
    
    // BMW (Bavarian Motor Works)
    {"BMW", "Various Rolling", 868350000, "Rolling Code"},
    {"BMW", "Various Smart", 868350000, "Smart Key"},
    
    // CADILLAC (American Luxury)
    {"Cadillac", "Various Rolling", 315000000, "Rolling Code"},
    {"Cadillac", "Various Smart", 315000000, "Smart Key"},
    
    // CHEVROLET (American Classic)
    {"Chevrolet", "Various Fixed", 315000000, "Fixed Code"},
    {"Chevrolet", "Various Rolling", 315000000, "Rolling Code"},
    {"Chevrolet", "Various Smart", 315000000, "Smart Key"},
    
    // CHRYSLER (American Heritage)
    {"Chrysler", "Various Fixed", 315000000, "Fixed Code"},
    {"Chrysler", "Various Rolling", 315000000, "Rolling Code"},
    
    // DODGE (American Muscle)
    {"Dodge", "Various Fixed", 315000000, "Fixed Code"},
    {"Dodge", "Various Rolling", 315000000, "Rolling Code"},
    {"Dodge", "Various Smart", 315000000, "Smart Key"},
    
    // FIAT (Italian Style)
    {"Fiat", "500X 2018+", 433920000, "Fixed Code"},
    {"Fiat", "Various Fixed", 315000000, "Fixed Code"},
    {"Fiat", "Various Rolling", 433920000, "Rolling Code"},
    
    // FORD (Built Ford Tough)
    {"Ford", "Various Fixed 315", 315000000, "Fixed Code"},
    {"Ford", "Various Fixed 433", 433920000, "Fixed Code"},
    {"Ford", "Various Rolling 315", 315000000, "Rolling Code"},
    {"Ford", "Various Rolling 433", 433920000, "Rolling Code"},
    {"Ford", "Various Smart", 315000000, "Smart Key"},
    
    // HONDA (Japanese Reliability)
    {"Honda", "Various Fixed", 315000000, "Fixed Code"},
    {"Honda", "Various Rolling", 315000000, "Rolling Code"},
    {"Honda", "Various Special", 433420000, "Rolling Code"},
    
    // HYUNDAI (Korean Innovation)
    {"Hyundai", "Various Fixed", 433920000, "Fixed Code"},
    {"Hyundai", "Various Rolling", 433920000, "Rolling Code"},
    {"Hyundai", "Various Smart", 433920000, "Smart Key"},
    
    // INFINITI (Japanese Luxury)
    {"Infiniti", "Various Fixed", 315000000, "Fixed Code"},
    {"Infiniti", "Various Rolling", 315000000, "Rolling Code"},
    {"Infiniti", "Various Smart", 315000000, "Smart Key"},
    
    // JAGUAR (British Elegance)
    {"Jaguar", "Various Fixed", 433920000, "Fixed Code"},
    {"Jaguar", "Various Smart", 433920000, "Smart Key"},
    
    // JEEP (Go Anywhere)
    {"Jeep", "Avenger 2023+", 433920000, "Smart Key"},
    {"Jeep", "Various Fixed", 315000000, "Fixed Code"},
    {"Jeep", "Various Rolling", 315000000, "Rolling Code"},
    
    // KIA (Korean Quality)
    {"Kia", "Various Fixed", 433920000, "Fixed Code"},
    {"Kia", "Various Rolling", 433920000, "Rolling Code"},
    {"Kia", "Various Smart", 433920000, "Smart Key"},
    
    // LEXUS (Japanese Luxury)
    {"Lexus", "LX 2019+", 315000000, "Smart Key"},
    {"Lexus", "Various Rolling", 433920000, "Rolling Code"},
    {"Lexus", "Various Smart", 433920000, "Smart Key"},
    
    // MAZDA (Japanese Engineering)
    {"Mazda", "2 2018+", 433920000, "Fixed Code"},
    {"Mazda", "Various Rolling", 433920000, "Rolling Code"},
    {"Mazda", "Various Smart", 433920000, "Smart Key"},
    
    // MERCEDES (German Luxury)
    {"Mercedes", "Various Rolling", 868350000, "Rolling Code"},
    {"Mercedes", "Various Smart", 868350000, "Smart Key"},
    {"Mercedes-Benz", "Sprinter 2018+", 868350000, "Rolling Code"},
    
    // MITSUBISHI (Japanese Reliability)
    {"Mitsubishi", "Various Fixed", 433920000, "Fixed Code"},
    {"Mitsubishi", "Various Rolling", 433920000, "Rolling Code"},
    
    // NISSAN (Japanese Innovation)
    {"Nissan", "Leaf 2019+", 433920000, "Smart Key"},
    {"Nissan", "Various Fixed 315", 315000000, "Fixed Code"},
    {"Nissan", "Various Fixed 433", 433920000, "Fixed Code"},
    {"Nissan", "Various Rolling 315", 315000000, "Rolling Code"},
    {"Nissan", "Various Rolling 433", 433920000, "Rolling Code"},
    {"Nissan", "Various Smart", 315000000, "Smart Key"},
    
    // PEUGEOT (French Style)
    {"Peugeot", "Landtrek 2020+", 433920000, "Fixed Code"},
    {"Peugeot", "Various Rolling", 433920000, "Rolling Code"},
    {"Peugeot", "Various Smart", 433920000, "Smart Key"},
    
    // PORSCHE (German Performance)
    {"Porsche", "Various Rolling", 433920000, "Rolling Code"},
    {"Porsche", "Various Smart", 433920000, "Smart Key"},
    
    // RANGE ROVER (British Luxury)
    {"Range Rover", "Various Fixed", 433920000, "Fixed Code"},
    {"Range Rover", "Various Smart", 433920000, "Smart Key"},
    
    // RENAULT (French Innovation)
    {"Renault", "Various Fixed", 433920000, "Fixed Code"},
    {"Renault", "Various Rolling", 433920000, "Rolling Code"},
    {"Renault", "Zoe 2019+", 433920000, "Smart Key"},
    
    // SEAT (Spanish Design)
    {"Seat", "Various Fixed", 433920000, "Fixed Code"},
    {"Seat", "Various Rolling", 433920000, "Rolling Code"},
    
    // SKODA (Czech Engineering)
    {"Skoda", "Enyaq 2021+", 433920000, "Smart Key"},
    {"Skoda", "Various Fixed", 433920000, "Fixed Code"},
    {"Skoda", "Various Rolling", 433920000, "Rolling Code"},
    
    // SUBARU (Japanese AWD)
    {"Subaru", "Ascent 2019+", 315000000, "Rolling Code"},
    {"Subaru", "Various Rolling", 433920000, "Rolling Code"},
    {"Subaru", "Various Smart", 433920000, "Smart Key"},
    
    // TOYOTA (Japanese Quality)
    {"Toyota", "Various Fixed 315", 315000000, "Fixed Code"},
    {"Toyota", "Various Fixed 433", 433920000, "Fixed Code"},
    {"Toyota", "Various Rolling 315", 315000000, "Rolling Code"},
    {"Toyota", "Various Rolling 433", 433920000, "Rolling Code"},
    {"Toyota", "Various Smart", 433920000, "Smart Key"},
    
    // VOLKSWAGEN (German Engineering)
    {"Volkswagen", "Atlas 2020+", 315000000, "Rolling Code"},
    {"Volkswagen", "Various Fixed", 433920000, "Fixed Code"},
    {"Volkswagen", "Various Rolling", 433920000, "Rolling Code"},
    {"Volkswagen", "Various Smart", 433920000, "Smart Key"},
    
    // VOLVO (Swedish Safety)
    {"Volvo", "Various Rolling", 868350000, "Rolling Code"},
    {"Volvo", "Various Smart", 868350000, "Smart Key"},
    
    // === CHINESE MANUFACTURERS (China Standard 433.92MHz) ===
    
    // BYD (比亚迪) - World's Largest EV Maker
    {"BYD", "Han 2020+", 433920000, "Smart Key"},
    {"BYD", "Tang 2018+", 433920000, "Smart Key"},
    {"BYD", "Song Plus", 433920000, "Rolling Code"},
    {"BYD", "Seal 2022+", 433920000, "Smart Key"},
    {"BYD", "Dolphin 2021+", 433920000, "Smart Key"},
    {"BYD", "Various Rolling", 433920000, "Rolling Code"},
    
    // NIO (蔚来) - Premium EVs
    {"NIO", "ES6 2019+", 433920000, "Smart Key"},
    {"NIO", "ES8 2018+", 433920000, "Smart Key"},
    {"NIO", "ET5 2022+", 433920000, "Smart Key"},
    {"NIO", "ET7 2022+", 433920000, "Smart Key"},
    
    // XPENG (小鹏) - Tech-Focused EVs
    {"Xpeng", "P7 2020+", 433920000, "Smart Key"},
    {"Xpeng", "P5 2021+", 433920000, "Smart Key"},
    {"Xpeng", "G9 2022+", 433920000, "Smart Key"},
    {"Xpeng", "G3 2019+", 433920000, "Smart Key"},
    
    // GEELY (吉利) - Major Manufacturer
    {"Geely", "Emgrand GT", 433920000, "Rolling Code"},
    {"Geely", "Boyue 2018+", 433920000, "Rolling Code"},
    {"Geely", "Coolray 2020+", 433920000, "Smart Key"},
    {"Geely", "Various Rolling", 433920000, "Rolling Code"},
    
    // GREAT WALL (长城) - SUVs & Pickups
    {"Great Wall", "Haval H6", 433920000, "Rolling Code"},
    {"Great Wall", "WEY VV7", 433920000, "Smart Key"},
    {"Great Wall", "Tank 300", 433920000, "Rolling Code"},
    
    // MG (SAIC Motor) - Global Brand
    {"MG", "ZS EV 2019+", 433920000, "Smart Key"},
    {"MG", "HS 2019+", 433920000, "Rolling Code"},
    {"MG", "Various", 433920000, "Rolling Code"},
    
    // ROEWE (上汽荣威) - SAIC Brand
    {"Roewe", "RX5 2016+", 433920000, "Rolling Code"},
    {"Roewe", "i6 2017+", 433920000, "Rolling Code"},
    
    // CHANGAN (长安)
    {"Changan", "CS75 Plus", 433920000, "Rolling Code"},
    {"Changan", "UNI-T 2020+", 433920000, "Smart Key"},
    {"Changan", "Eado 2018+", 433920000, "Rolling Code"},
    
    // HONGQI (红旗) - Luxury State Cars
    {"Hongqi", "H9 2020+", 433920000, "Smart Key"},
    {"Hongqi", "E-HS9 2021+", 433920000, "Smart Key"},
    
    // LI AUTO (理想) - Extended-Range EVs
    {"Li Auto", "Li ONE 2019+", 433920000, "Smart Key"},
    {"Li Auto", "L7 2023+", 433920000, "Smart Key"},
    {"Li Auto", "L8 2022+", 433920000, "Smart Key"},
    {"Li Auto", "L9 2022+", 433920000, "Smart Key"},
    
    // ZEEKR (极氪) - Geely's Premium EV
    {"Zeekr", "001 2021+", 433920000, "Smart Key"},
    {"Zeekr", "X 2023+", 433920000, "Smart Key"},
    
    // LYNK & CO (领克) - Geely-Volvo Joint
    {"Lynk & Co", "01 2017+", 433920000, "Rolling Code"},
    {"Lynk & Co", "03 2018+", 433920000, "Smart Key"},
    
    // JAC (江淮) - Commercial & Passenger
    {"JAC", "iEV7S", 433920000, "Fixed Code"},
    {"JAC", "Various", 433920000, "Rolling Code"},
    
    // AIWAYS (爱驰) - EV Startup
    {"Aiways", "U5 2019+", 433920000, "Smart Key"},
    
    // === END CHINESE MANUFACTURERS ===
    
    // === PREMIUM/LUXURY BRANDS (Expanded Database) ===
    
    // ASTON MARTIN (British Luxury)
    {"Aston Martin", "DB11 2016+", 433920000, "Smart Key"},
    {"Aston Martin", "DBS Superleggera 2018+", 433920000, "Smart Key"},
    {"Aston Martin", "Vantage 2018+", 433920000, "Smart Key"},
    {"Aston Martin", "DBX 2020+", 433920000, "Smart Key"},
    
    // BENTLEY (British Ultra-Luxury)
    {"Bentley", "Continental GT 2018+", 433920000, "Smart Key"},
    {"Bentley", "Flying Spur 2019+", 433920000, "Smart Key"},
    {"Bentley", "Bentayga 2016+", 433920000, "Smart Key"},
    {"Bentley", "Mulsanne 2010-2020", 433920000, "Smart Key"},
    
    // BUGATTI (Hypercar Manufacturer)
    {"Bugatti", "Chiron 2016+", 433920000, "Smart Key"},
    {"Bugatti", "Veyron 2005-2015", 433920000, "Smart Key"},
    
    // FERRARI (Italian Supercar)
    {"Ferrari", "488 GTB 2015+", 433920000, "Smart Key"},
    {"Ferrari", "F8 Tributo 2019+", 433920000, "Smart Key"},
    {"Ferrari", "SF90 Stradale 2019+", 433920000, "Smart Key"},
    {"Ferrari", "Roma 2020+", 433920000, "Smart Key"},
    {"Ferrari", "Portofino 2017+", 433920000, "Smart Key"},
    {"Ferrari", "812 Superfast 2017+", 433920000, "Smart Key"},
    {"Ferrari", "296 GTB 2022+", 433920000, "Smart Key"},
    
    // LAMBORGHINI (Italian Supercar)
    {"Lamborghini", "Huracan 2014+", 433920000, "Smart Key"},
    {"Lamborghini", "Aventador 2011+", 433920000, "Smart Key"},
    {"Lamborghini", "Urus 2018+", 433920000, "Smart Key"},
    {"Lamborghini", "Gallardo 2003-2013", 433920000, "Rolling Code"},
    {"Lamborghini", "Murcielago 2001-2010", 433920000, "Fixed Code"},
    
    // MASERATI (Italian Luxury)
    {"Maserati", "Ghibli 2014+", 433920000, "Smart Key"},
    {"Maserati", "Quattroporte 2013+", 433920000, "Smart Key"},
    {"Maserati", "Levante 2016+", 433920000, "Smart Key"},
    {"Maserati", "MC20 2021+", 433920000, "Smart Key"},
    {"Maserati", "GranTurismo 2007-2019", 433920000, "Rolling Code"},
    
    // McLAREN (British Supercar)
    {"McLaren", "720S 2017+", 433920000, "Smart Key"},
    {"McLaren", "765LT 2020+", 433920000, "Smart Key"},
    {"McLaren", "Artura 2021+", 433920000, "Smart Key"},
    {"McLaren", "GT 2019+", 433920000, "Smart Key"},
    
    // ROLLS-ROYCE (British Ultra-Luxury)
    {"Rolls-Royce", "Phantom 2017+", 433920000, "Smart Key"},
    {"Rolls-Royce", "Ghost 2020+", 433920000, "Smart Key"},
    {"Rolls-Royce", "Cullinan 2018+", 433920000, "Smart Key"},
    {"Rolls-Royce", "Wraith 2013+", 433920000, "Smart Key"},
    
    // TESLA (American EV Leader) - CRITICAL!
    {"Tesla", "Model S 2012-2016", 315000000, "Rolling Code"},
    {"Tesla", "Model S 2017+", 315000000, "Smart Key"},
    {"Tesla", "Model 3 2018+", 315000000, "Smart Key"},
    {"Tesla", "Model X 2016-2020", 315000000, "Rolling Code"},
    {"Tesla", "Model X 2021+", 315000000, "Smart Key"},
    {"Tesla", "Model Y 2020+", 315000000, "Smart Key"},
    {"Tesla", "Cybertruck 2024+", 315000000, "Smart Key"},
    {"Tesla", "Roadster 2008-2012", 315000000, "Fixed Code"},
    {"Tesla", "Roadster 2023+", 315000000, "Smart Key"},
    {"Tesla", "Semi 2023+", 315000000, "Smart Key"},
    
    // === END PREMIUM/LUXURY BRANDS ===
    
    // 433.42 MHz - Honda-specific frequency
    {"Honda", "Various", 433420000, "Rolling Code"},
    {"Honda", "Various", 433420000, "Fixed Code"},
    {"Honda", "Various", 433420000, "Smart Key"},
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
