# Deployment Guide for Predator Momentum

## Overview
This guide provides instructions for deploying the Predator Momentum application to a Flipper Zero device running Momentum firmware. The application is a comprehensive penetration testing toolkit with support for multiple expansion boards.

## Prerequisites
- Flipper Zero device with Momentum firmware installed
- USB cable for connecting Flipper Zero to a computer
- Built `.fap` file from the Predator Momentum project

## Deployment Steps

### Step 1: Build the Application
1. Ensure you have the `ufbt` tool installed and configured for Momentum firmware development.
2. Navigate to the project directory: `C:\Projects\Predator_momentum\predator_app`
3. Run the build command: `ufbt build`
4. The built `.fap` file will be located at `C:\Users\nchap\.ufbt\build\predator.fap`

### Step 2: Connect Flipper Zero
1. Connect your Flipper Zero device to your computer via USB.
2. Ensure the device is recognized by your system. If not, check the USB connection and drivers.

### Step 3: Flash the Application
1. From the project directory, run the flash command: `ufbt flash`
2. If the automatic interface detection fails, manually specify the interface as needed.
3. Alternatively, manually copy the `.fap` file to the device:
   - Copy `C:\Users\nchap\.ufbt\build\predator.fap` to the `/apps/Tools/` directory on the Flipper Zero.

### Step 4: Verify Installation
1. Disconnect the Flipper Zero from the computer.
2. Navigate to the `Tools` menu on the Flipper Zero.
3. Confirm that `Predator Momentum` appears in the list of applications.

## Troubleshooting
- **Build Errors**: Ensure all dependencies are installed and the `ufbt` environment is correctly set up. Check for unused functions or variables in the code that might cause warnings treated as errors.
- **Flash Errors**: If the `ufbt flash` command fails, manually copy the `.fap` file to the device as described in Step 3.
- **Navigation Issues**: If the application shows white screens or navigation problems, ensure scene navigation uses correct scene IDs or enum values.

## Additional Notes
- The application supports multiple expansion boards. Ensure the correct board is selected in the app settings for proper functionality.
- For Tesla-specific testing, focus on the Car Attacks scene, which includes charge port security and other critical tests.

If you encounter any issues or need further assistance, please refer to the project documentation or contact the development team.
