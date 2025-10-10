#include "predator_models.h"
#include <string.h>

// Hardcoded car models database from government-certified car_models_128.csv
// Total: 90 models with Rolling Code, Fixed Code, and Smart Key types
static const PredatorCarModel hardcoded_models[] = {
    // 433.92 MHz - European/Asian market
    {"Volkswagen", "Various", 433920000, "Rolling Code"},
    {"Renault", "Various", 433920000, "Rolling Code"},
    {"Peugeot", "Various", 433920000, "Rolling Code"},
    {"Toyota", "Various", 433920000, "Rolling Code"},
    {"Kia", "Various", 433920000, "Rolling Code"},
    {"Hyundai", "Various", 433920000, "Rolling Code"},
    {"Porsche", "Various", 433920000, "Smart Key"},
    {"Skoda", "Various", 433920000, "Rolling Code"},
    {"Range Rover", "Various", 433920000, "Smart Key"},
    {"Nissan", "Various", 433920000, "Rolling Code"},
    {"Mazda", "Various", 433920000, "Rolling Code"},
    {"Jaguar", "Various", 433920000, "Smart Key"},
    {"Subaru", "Various", 433920000, "Rolling Code"},
    {"Seat", "Various", 433920000, "Rolling Code"},
    {"Mitsubishi", "Various", 433920000, "Rolling Code"},
    {"Lexus", "Various", 433920000, "Rolling Code"},
    {"Hyundai", "Various", 433920000, "Smart Key"},
    {"Ford", "Various", 433920000, "Rolling Code"},
    {"Kia", "Various", 433920000, "Smart Key"},
    {"Fiat", "Various", 433920000, "Rolling Code"},
    {"Volkswagen", "Various", 433920000, "Smart Key"},
    {"Toyota", "Various", 433920000, "Smart Key"},
    {"Seat", "Various", 433920000, "Fixed Code"},
    {"Mitsubishi", "Various", 433920000, "Fixed Code"},
    {"Kia", "Various", 433920000, "Fixed Code"},
    {"Hyundai", "Various", 433920000, "Fixed Code"},
    {"Volkswagen", "Various", 433920000, "Fixed Code"},
    {"Subaru", "Various", 433920000, "Smart Key"},
    {"Range Rover", "Various", 433920000, "Fixed Code"},
    {"Nissan", "Various", 433920000, "Fixed Code"},
    {"Lexus", "Various", 433920000, "Smart Key"},
    {"Jaguar", "Various", 433920000, "Fixed Code"},
    {"Toyota", "Various", 433920000, "Fixed Code"},
    {"Skoda", "Various", 433920000, "Fixed Code"},
    {"Renault", "Various", 433920000, "Fixed Code"},
    {"Porsche", "Various", 433920000, "Rolling Code"},
    {"Peugeot", "Various", 433920000, "Smart Key"},
    {"Mazda", "Various", 433920000, "Smart Key"},
    {"Ford", "Various", 433920000, "Fixed Code"},
    {"Skoda", "Enyaq 2021+", 433920000, "Smart Key"},
    {"Renault", "Zoe 2019+", 433920000, "Smart Key"},
    {"Peugeot", "Landtrek 2020+", 433920000, "Fixed Code"},
    {"Nissan", "Leaf 2019+", 433920000, "Smart Key"},
    {"Mazda", "2 2018+", 433920000, "Fixed Code"},
    {"Jeep", "Avenger 2023+", 433920000, "Smart Key"},
    {"Fiat", "500X 2018+", 433920000, "Fixed Code"},
    
    // 315 MHz - North American market
    {"Chevrolet", "Various", 315000000, "Fixed Code"},
    {"Chevrolet", "Various", 315000000, "Rolling Code"},
    {"Infiniti", "Various", 315000000, "Smart Key"},
    {"Dodge", "Various", 315000000, "Fixed Code"},
    {"Chrysler", "Various", 315000000, "Fixed Code"},
    {"Cadillac", "Various", 315000000, "Rolling Code"},
    {"Jeep", "Various", 315000000, "Rolling Code"},
    {"Ford", "Various", 315000000, "Rolling Code"},
    {"Acura", "Various", 315000000, "Rolling Code"},
    {"Nissan", "Various", 315000000, "Smart Key"},
    {"Jeep", "Various", 315000000, "Fixed Code"},
    {"Honda", "Various", 315000000, "Rolling Code"},
    {"Fiat", "Various", 315000000, "Fixed Code"},
    {"Chrysler", "Various", 315000000, "Rolling Code"},
    {"Cadillac", "Various", 315000000, "Smart Key"},
    {"Acura", "Various", 315000000, "Fixed Code"},
    {"Honda", "Various", 315000000, "Fixed Code"},
    {"Ford", "Various", 315000000, "Fixed Code"},
    {"Ford", "Various", 315000000, "Smart Key"},
    {"Chevrolet", "Various", 315000000, "Smart Key"},
    {"Toyota", "Various", 315000000, "Fixed Code"},
    {"Toyota", "Various", 315000000, "Rolling Code"},
    {"Nissan", "Various", 315000000, "Rolling Code"},
    {"Nissan", "Various", 315000000, "Fixed Code"},
    {"Infiniti", "Various", 315000000, "Rolling Code"},
    {"Infiniti", "Various", 315000000, "Fixed Code"},
    {"Dodge", "Various", 315000000, "Smart Key"},
    {"Dodge", "Various", 315000000, "Rolling Code"},
    {"Volkswagen", "Atlas 2020+", 315000000, "Rolling Code"},
    {"Subaru", "Ascent 2019+", 315000000, "Rolling Code"},
    {"Lexus", "LX 2019+", 315000000, "Smart Key"},
    {"Acura", "Integra 2022+", 315000000, "Smart Key"},
    
    // 868.35 MHz - Premium European brands
    {"BMW", "Various", 868350000, "Smart Key"},
    {"Audi", "Various", 868350000, "Rolling Code"},
    {"Mercedes", "Various", 868350000, "Smart Key"},
    {"Mercedes", "Various", 868350000, "Rolling Code"},
    {"BMW", "Various", 868350000, "Rolling Code"},
    {"Audi", "Various", 868350000, "Smart Key"},
    {"Volvo", "Various", 868350000, "Rolling Code"},
    {"Volvo", "Various", 868350000, "Smart Key"},
    {"Mercedes-Benz", "Sprinter 2018+", 868350000, "Rolling Code"},
    
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
