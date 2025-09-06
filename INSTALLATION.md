# Predator App Installation Guide

This document provides detailed instructions for installing the Predator app on various Flipper Zero firmware versions, including Momentum.

## Supported Firmware Versions

The Predator app is compatible with:
- **Momentum Firmware** (Latest builds)
- **RogueMaster Firmware**
- **Official Firmware** (0.86.0+)
- **Unleashed Firmware**

## Pre-Built Installation

### Method 1: qFlipper Installation (Recommended)

1. Download the latest `predator.fap` file from the project
2. Connect your Flipper Zero to your computer via USB
3. Close any running instances of qFlipper Lab/Web Updater
4. Open qFlipper
5. Navigate to the File Browser by clicking the folder icon
6. Browse to `/ext/apps/Tools/` on your Flipper Zero
   - Create this directory if it doesn't exist
7. Drag and drop `predator.fap` into this folder
8. Safely disconnect your Flipper Zero

### Method 2: SD Card Installation

1. Power off your Flipper Zero
2. Remove the microSD card and connect it to your computer
3. Create or navigate to `/apps/Tools/` directory on the SD card
4. Copy `predator.fap` to this directory
5. Safely eject the SD card
6. Reinsert the SD card into your Flipper Zero and power it on

## Momentum Firmware-Specific Instructions

Momentum firmware is fully compatible with the Predator app. For optimal performance:

1. Use the latest Momentum firmware release
2. Install via qFlipper for best results
3. Update your Momentum firmware using the [Web Updater](https://momentum-fw.dev/update) if needed

```
Important: The Predator app requires specific hardware for full functionality.
Without the hardware module, only simulation features will work.
```

## Building from Source

### Prerequisites
- Python 3.6+
- Git
- ufbt (Flipper Build Tool)

### Building with ufbt

```bash
# Install ufbt if not already installed
pip install --upgrade ufbt

# Clone the repository
git clone https://github.com/example/predator.git
cd predator

# Build the app
cd predator_app
ufbt

# The compiled .fap file will be in dist/
```

### Building with Firmware Integration

If you need to integrate with a specific firmware:

```bash
# Clone the firmware repository
git clone --recursive --jobs 8 https://github.com/Next-Flip/Momentum-Firmware.git
cd Momentum-Firmware

# Copy the Predator app to applications_user
mkdir -p applications_user
cp -r /path/to/predator/predator_app applications_user/predator

# Build the app
./fbt fap_predator

# The compiled .fap file will be in build/f7-C/
```

## Launching the App

1. On your Flipper Zero, go to the main menu
2. Navigate to **Apps** → **Tools**
3. Find and select **Predator**

## Hardware Setup

For full functionality, connect your Predator module to the Flipper Zero:

### Pin Connections
- **ESP32S2**: Pins 15,16 (UART)
- **GPS Module**: Pins 13,14 (UART)
- **A07 RF Module**: External RF connection

### Power Switch Position
- Down position: GPS powered
- Up position: Uses internal battery

## Troubleshooting

### App Not Appearing in Menu
- Verify the FAP file is in the correct location
- Check SD card is properly inserted
- Try reinstalling the app

### Hardware Not Detected
- Check physical connections
- Verify power switch is in correct position
- Ensure hardware module is powered

### Crash on Launch
- Update to the latest firmware
- Rebuild the app with matching SDK version
- Check for error messages in debug mode

## Updating

To update the Predator app:
1. Download the latest version
2. Follow the same installation procedure
3. The new version will replace the previous one

## Feature Compatibility Matrix

| Feature | Momentum | RogueMaster | Official | Unleashed |
|---------|:--------:|:-----------:|:--------:|:---------:|
| GPS Tracking | ✓ | ✓ | ✓ | ✓ |
| Car Models | ✓ | ✓ | ✓ | ✓ |
| Passive Mode | ✓ | ✓ | ✓ | ✓ |
| WiFi Attacks | ✓ | ✓ | ⚠️ | ✓ |
| Debug Features | ✓ | ✓ | ✓ | ✓ |

✓: Fully supported | ⚠️: Limited functionality
