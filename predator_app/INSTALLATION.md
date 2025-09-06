# Predator App for Flipper Zero Momentum - Installation Guide

## Installation Methods

### Method 1: Direct Installation with ufbt (Recommended)

If your Flipper Zero is connected to your PC:

```bash
cd C:\Projects\Predator_momentum\predator_app
ufbt launch
```

This will automatically install and launch the app on your device.

### Method 2: Manual Installation via qFlipper

1. Connect your Flipper Zero to your computer
2. Open qFlipper
3. Navigate to the File Manager
4. Copy `C:\Projects\Predator_momentum\predator_app\dist\predator.fap` to `/ext/apps/Tools/` on your Flipper Zero
5. Safely disconnect your device

### Method 3: SD Card Installation

1. Copy `C:\Projects\Predator_momentum\predator_app\dist\predator.fap` to the `/apps/Tools/` directory on your SD card
2. Insert the SD card into your Flipper Zero

## Configuring the Hardware for Momentum Firmware

Once installed, you'll need to configure the hardware properly for the Momentum firmware:

### ESP32 Module Configuration
1. Navigate to: **Momentum** → **Protocol Settings** → **GPIO Pin Settings** → **ESP32**
2. Set TX pin to: **15**
3. Set RX pin to: **16**

### GPS Module Configuration
1. Navigate to: **Momentum** → **Protocol Settings** → **GPIO Pin Settings** → **GPS Pin Settings**
2. Set TX pin to: **13**
3. Set RX pin to: **14**

### RF Module Configuration
1. Enter the **Sub GHz** menu
2. Select **Advanced Settings**
3. Change module to: **External**

## Using the Predator App

After installation and configuration, you can access the Predator app from the Apps > Tools menu on your Flipper Zero.

The main menu provides access to all the penetration testing tools:
- WiFi Attacks
- Bluetooth Attacks
- SubGHz/RF Attacks
- Car Attacks
- RFID/NFC Attacks
- GPS Tracker
- Wardriving
- Social Engineering

## Hardware Switches

Refer to the Predator module documentation (predator.md) for details on the hardware switches:
- Left side switch controls GPS power (down = Flipper Zero power, up = internal battery)
- Right side switch controls the Marauder display

## Support

For issues or questions, refer to the documentation in the Predator_momentum repository or contact the Anthrobic support team.
