# Predator App for Flipper Zero Momentum

A comprehensive penetration testing toolkit designed for Flipper Zero with the Predator module.

## Features

- **WiFi Attacks**: Scanning, deauthentication, evil twin access points
- **Bluetooth Attacks**: BLE scanning, Bluetooth spam
- **SubGHz/RF Attacks**: Signal analysis and replay
- **Car Attacks**: Key bruteforce, jamming, Tesla-specific attacks, passive entry
- **RFID/NFC Attacks**: Card cloning, bruteforce
- **GPS Tracking**: Real-time location tracking
- **Wardriving**: WiFi network mapping with GPS
- **Social Engineering**: Captive portals, phishing tools

## Hardware Requirements

The Predator app is designed to work with the Predator 4-in-1 module which includes:
- 433MHz RF module with A07 (10dBm)
- ESP32S2 module with Marauder firmware
- GPS module
- 2.8-inch display with Marauder interface

## Building the App

### Prerequisites

- Python 3.6+
- Git
- ufbt (Flipper Build Tool)

### Installation Steps

1. **Install ufbt**
   ```
   pip install --upgrade ufbt
   ```

2. **Clone the repository** (if you haven't already)
   ```
   git clone https://github.com/your-repo/predator.git
   cd predator
   ```

3. **Build the app**
   ```
   cd predator_app
   ufbt
   ```

4. **Install to Flipper Zero**
   ```
   ufbt launch
   ```
   
   Alternatively, copy the generated `.fap` file from `dist/` directory to your Flipper Zero's SD card at `/ext/apps/Tools/`.

### Building for Momentum Firmware

The app has been configured to work specifically with Momentum firmware. There are two ways to install it:

#### Option 1: Direct FAP Installation

1. Build with ufbt as described above
2. Copy the `.fap` file to `/ext/apps/Tools/` on your Flipper Zero's SD card
3. Run the app from Apps > Tools > Predator

#### Option 2: Integration with Momentum Firmware

For tighter integration with Momentum:

1. Clone the Momentum firmware repository
   ```
   git clone https://github.com/Next-Flip/Momentum-Firmware.git
   cd Momentum-Firmware
   ```

2. Copy the Predator app to the applications directory
   ```
   mkdir -p applications_user
   cp -r /path/to/predator/predator_app applications_user/predator
   ```

3. Build the firmware with Predator integrated
   ```
   ./fbt COMPACT=1
   ```

4. Flash the firmware to your Flipper Zero

## Using the App

### Initial Setup

1. Connect your Predator module to the Flipper Zero
2. Make sure the GPS switch (left side) is in the DOWN position before connecting
3. Launch the Predator app from the Apps > Tools menu

### Module Configuration

#### ESP32 Module
- Navigate to Momentum > Protocol Settings > GPIO Pin Settings > ESP32
- Set pins to 15, 16

#### GPS Module
- Navigate to Momentum > Protocol Settings > GPIO Pin Settings > GPS Pin Settings
- Change GPS pins to 13, 14

#### RF Module
- Enter Sub GHz menu
- Click on Advanced Settings to set the module to external

### Basic Operation

- **WiFi Scanning**: Navigate to WiFi Attacks > WiFi Scan
- **GPS Tracking**: Navigate to GPS Tracker
- **Car Attacks**: Navigate to Car Attacks menu for various options

## Troubleshooting

### Common Issues

1. **Module Not Detected**
   - Check physical connections
   - Verify power switch positions
   - Ensure GPIO pins are configured correctly

2. **GPS Not Acquiring Satellites**
   - Make sure you have a clear view of the sky
   - Wait 30-90 seconds for initial acquisition
   - Check GPS debug screen for signal strength

3. **ESP32 Commands Not Working**
   - Try rebooting the Flipper Zero
   - Verify UART connections
   - Update Marauder firmware if needed

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Credits

Developed by Anthrobic for Flipper Zero Momentum.

## Legal Notice

This tool is provided for educational and testing purposes only. Users are responsible for ensuring all usage complies with local laws and regulations. The developers do not condone or support illegal activities.
