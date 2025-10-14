# Update Car Models with Continent Field

## What needs to be done:

All car model entries in `predator_models_hardcoded.c` need to add the continent field.

### Old format:
```c
{"Volkswagen", "Atlas 2020+", 315000000, "Rolling Code"}
```

### New format:
```c
{"Volkswagen", "Atlas 2020+", 315000000, "Rolling Code", CarContinentEurope}
```

## Changes needed:

1. **European brands** → Add `, CarContinentEurope`
   - Volkswagen, Audi, BMW, Mercedes, Renault, Peugeot, Fiat, Porsche
   - Skoda, Seat, Volvo, Jaguar, Range Rover
   - Ferrari, Lamborghini, Maserati, Bentley, Rolls-Royce, Aston Martin, McLaren, Bugatti

2. **Asian brands** → Add `, CarContinentAsia`
   - Toyota, Honda, Nissan, Mazda, Mitsubishi, Subaru, Lexus, Infiniti, Acura
   - Hyundai, Kia
   - BYD, NIO, Xpeng, Geely, Great Wall, MG, Roewe, Changan, Hongqi, Li Auto, Zeekr, Lynk & Co, JAC, Aiways

3. **American brands** → Add `, CarContinentAmerica`
   - Ford, Chevrolet, Jeep, Dodge, Chrysler, Cadillac, Tesla

## Total entries to update: ~178 models

## Implementation:
Due to the large number of entries, this will be done with a Python script or multiple targeted edits.

## Status:
- [x] Added `CarContinent continent;` field to `PredatorCarModel` struct
- [x] Added `selected_continent` to `PredatorApp` struct  
- [ ] Update all 178 model entries with continent field
- [ ] Update `predator_models_get_continent()` to use struct field
- [ ] Update car_models scene to filter by continent
- [ ] Test build
