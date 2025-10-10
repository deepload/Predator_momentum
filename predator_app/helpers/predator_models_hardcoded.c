#include "predator_models.h"
#include <string.h>

// ALPHABETICALLY ORDERED car models database - Elon Approved! 🚗
// Total: 90 models with Rolling Code, Fixed Code, and Smart Key types
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
