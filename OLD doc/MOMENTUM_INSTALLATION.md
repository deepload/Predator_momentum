# Installing Predator on Momentum Firmware

If you're getting "Error: Invalid file" with all attempted formats, this suggests the Momentum firmware may require:

1. **Internal App Compilation**: The app may need to be compiled as part of the firmware itself, not as an external FAP

2. **Custom App Registry**: Momentum may have a modified app registry that only accepts apps built specifically for it

3. **Alternative Installation Method**: You may need to use a different approach for app installation

## Recommended Approach

### Option 1: Build Predator as Internal App in Momentum Firmware

1. Clone Momentum firmware repository
   ```bash
   git clone https://github.com/Next-Flip/Momentum-Firmware.git
   cd Momentum-Firmware
   ```

2. Copy Predator source to applications directory (not applications_user)
   ```bash
   cp -r path/to/predator_app applications/main/predator
   ```

3. Add Predator to the firmware build by modifying `applications/main/application.fam`
   ```python
   # Add entry for Predator
   App(
       appid="predator",
       name="Predator",
       # ... other properties ...
   )
   ```

4. Build complete firmware with Predator included
   ```bash
   ./fbt firmware_all
   ```

5. Flash the firmware to your Flipper Zero

### Option 2: Use Momentum's Official App Catalog

1. Visit https://momentum-fw.dev/apps
2. Check if Predator is available in the catalog
3. If not, find a similar tool or alternative that is compatible

## Understanding the Issue

The "Invalid file" error typically means:
1. The app format doesn't match what the firmware expects
2. The firmware has custom validation rules for apps
3. The Momentum fork might restrict external apps for security

For apps to work in custom firmwares like Momentum, they typically need to be:
1. Built against the exact same SDK version
2. Compiled with the same toolchain
3. Possibly signed or registered in the firmware's app database
