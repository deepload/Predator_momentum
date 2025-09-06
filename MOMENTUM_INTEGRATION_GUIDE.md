# Predator App Integration Guide for Momentum Firmware

This guide provides detailed instructions for integrating the Predator app with Flipper Zero Momentum firmware.

## Method 1: Direct FAP Installation (Recommended)

This method builds the Predator app as a standalone FAP file that can be installed on a Flipper Zero running Momentum firmware.

### Building Steps

1. **Ensure you have the required tools**
   ```powershell
   python -m pip install --upgrade ufbt
   ```

2. **Navigate to the Predator app directory**
   ```powershell
   cd C:\Projects\Predator_momentum\predator_app
   ```

3. **Update ufbt configuration**
   
   The `.ufbt` and `.ufbt.user` files should point to Momentum firmware:
   
   `.ufbt` contents:
   ```
   FIRMWARE_API_VERSION=af67a99d
   SDK_BRANCH=dev
   SDK_ORIGIN=https://github.com/Next-Flip/Momentum-Firmware.git
   ```
   
   `.ufbt.user` contents:
   ```
   UFBT_FIRMWARE_ORIGIN = "https://github.com/Next-Flip/Momentum-Firmware.git"
   UFBT_FIRMWARE_BRANCH = "dev"
   UFBT_FIRMWARE_VERSION = "latest"
   ```

4. **Initialize ufbt environment**
   ```powershell
   ufbt update
   ```

5. **Build the app**
   ```powershell
   ufbt
   ```

6. **Install to Flipper**
   
   The built FAP file will be in `dist/` directory. 
   
   You can install it directly with:
   ```powershell
   ufbt launch
   ```
   
   Or copy it manually to the SD card:
   ```powershell
   copy dist\predator.fap <SD_CARD_PATH>\apps\Tools\
   ```

## Method 2: Full Firmware Integration

This method integrates the Predator app directly into the Momentum firmware build.

### Integration Steps

1. **Clone the Momentum firmware repository**
   ```powershell
   git clone https://github.com/Next-Flip/Momentum-Firmware.git
   cd Momentum-Firmware
   ```

2. **Create applications_user directory**
   ```powershell
   mkdir -p applications_user
   ```

3. **Copy Predator app to the applications directory**
   ```powershell
   xcopy /E /I C:\Projects\Predator_momentum\predator_app applications_user\predator
   ```

4. **Configure the build**
   
   Edit `applications/meta/application.fam` to include the Predator app.

5. **Build the full firmware**
   ```powershell
   .\fbt COMPACT=1
   ```

6. **Flash the firmware**
   
   Use qFlipper or the web updater to flash the built firmware to your Flipper Zero.

## Momentum-Specific Configuration

To ensure the Predator app works properly with the Momentum firmware, configure the following:

### Hardware Pin Mapping

1. **ESP32 Module**
   - Navigate to: **Momentum** > **Protocol Settings** > **GPIO Pin Settings** > **ESP32**
   - Set TX pin to: **15**
   - Set RX pin to: **16**

2. **GPS Module**
   - Navigate to: **Momentum** > **Protocol Settings** > **GPIO Pin Settings** > **GPS**
   - Set TX pin to: **13**
   - Set RX pin to: **14**

3. **RF Module**
   - Enter: **Sub GHz** menu
   - Click: **Advanced Settings**
   - Set module to: **External**

### Troubleshooting Momentum Integration

1. **"Invalid file" error**
   - Rebuild the FAP with the exact SDK version matching your Momentum firmware
   - Use the `latest` tag in UFBT_FIRMWARE_VERSION to get the current version

2. **App not appearing in menu**
   - Verify the app category is set to "Tools" in application.fam
   - Check that the FAP file is in the correct location on SD card

3. **App crashes on launch**
   - Check Flipper console logs for errors
   - Ensure all required API permissions are granted in application.fam
   - Rebuild with the latest Momentum SDK

## Testing the Integration

After installation, test the following functionality:

1. **Basic app launch**
   - App should start without crashing
   - Main menu should display all attack categories

2. **Hardware detection**
   - ESP32 connection status should be shown
   - GPS status should update when satellites are detected
   - RF module should be recognized in SubGHz attacks

## Momentum Firmware Advantages

The Momentum firmware offers several advantages for the Predator app:

1. **Enhanced GPIO control** - More reliable pin mapping and control
2. **Better UART stability** - Improved serial communication for ESP32 and GPS
3. **SubGHz improvements** - Enhanced external RF module support
4. **Battery optimization** - Extended runtime for wardriving and GPS tracking
