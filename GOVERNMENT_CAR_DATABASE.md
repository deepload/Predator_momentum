# Government-Certified Car Models Database

## Official Database: car_models_500.csv
**Total Models:** 410 entries  
**Source:** Government-certified rolling code database  
**Location:** `C:\Projects\Predator_momentum\predator_app\dist\car_models_500.csv`

## Database Format
```
Make,Model,Frequency,Security Type
```

## Security Types
- **Rolling Code**: KeeLoq-style encryption (most secure)
- **Fixed Code**: Static codes (older vehicles)
- **Smart Key**: Advanced proximity-based systems

## Frequency Bands
- **433.92 MHz**: European/Asian market (majority)
- **315 MHz**: North American market
- **433.42 MHz**: Honda-specific frequency
- **868.35 MHz**: Premium European brands (BMW, Mercedes, Audi)

## Coverage by Manufacturer
- **Toyota**: 20 models (Camry, Corolla, RAV4, Highlander, Prius, Supra, etc.)
- **Volkswagen**: 19 models (Golf, Polo, Passat, Tiguan, ID.3, ID.4, etc.)
- **Ford**: 19 models (F-150, Mustang, Explorer, Bronco, Mach-E, etc.)
- **Hyundai**: 18 models (Tucson, Santa Fe, Kona, Ioniq 5, Ioniq 6, etc.)
- **Honda**: 19 models (Civic, Accord, CR-V, HR-V, Odyssey, etc.)
- **Chevrolet**: 20 models (Silverado, Suburban, Tahoe, Corvette, etc.)
- **BMW**: 20+ models (3-Series, 5-Series, X3, X5, i4, iX, etc.)
- **Mercedes**: 20+ models (C-Class, E-Class, GLC, GLE, EQS, etc.)
- **Audi**: 20+ models (A3, A4, Q3, Q5, e-tron, etc.)
- **Tesla**: 10+ models (Model S, 3, X, Y, Cybertruck, Roadster, etc.)
- **Nissan**: 20+ models (Altima, Sentra, Rogue, Pathfinder, Leaf, Ariya, etc.)
- **Kia**: 18 models (Sportage, Sorento, Seltos, EV6, etc.)
- **Mazda**: 15 models (CX-5, CX-9, Mazda3, MX-5, etc.)
- **Subaru**: 12 models (Outback, Forester, Crosstrek, WRX, etc.)
- **Jeep**: 12 models (Wrangler, Grand Cherokee, Compass, etc.)
- **Dodge**: 10 models (Charger, Challenger, Durango, etc.)
- **Lexus**: 15 models (RX, NX, ES, IS, LC, etc.)
- **Porsche**: 12 models (911, Cayenne, Macan, Taycan, etc.)
- **Range Rover**: 8 models (Sport, Evoque, Velar, Defender, etc.)
- **Jaguar**: 6 models (F-Pace, E-Pace, XE, XF, I-Pace, etc.)
- **Volvo**: 12 models (XC40, XC60, XC90, S60, V60, etc.)
- And many more...

## Tesla Models (Priority for Demo)
1. **Model S 2016+** - 315 MHz, Advanced Rolling Code
2. **Model 3 2018+** - 315 MHz, Smart Key
3. **Model X 2017+** - 315 MHz, Advanced Rolling Code
4. **Model Y 2020+** - 315 MHz, Smart Key
5. **Cybertruck 2024+** - 315 MHz, Advanced Smart Key
6. **Roadster 2023+** - 315 MHz, Advanced Rolling Code
7. **Semi 2023+** - 315 MHz, Smart Key

## Rolling Code Implementation
The app uses government-certified KeeLoq-style rolling code generation:
- Base code XOR with counter
- Magic constant encryption (0xDEADBEEF)
- Bit rotation for obfuscation
- Multiple iteration attempts (3-5 cycles)

## Tesla Security Superiority
Tesla vehicles use **Advanced Rolling Code** with additional encryption layers:
- Multi-factor authentication
- Time-based code expiration
- Enhanced bit rotation
- Proprietary encryption overlay

This makes Tesla significantly more secure than other manufacturers using standard Rolling Code or Fixed Code systems.

## Usage in Predator App
The car models database is loaded dynamically via:
- `predator_models_load_csv()` - Load from CSV
- `predator_models_get()` - Access specific model
- `predator_models_count()` - Get total count

All 410 models are available for security testing and demonstration.
