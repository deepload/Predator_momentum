# Flipper Zero Predator Module Firmware Builder

A comprehensive build script for integrating the Predator module with Flipper Zero firmware, supporting both full app installation and firmware-only modifications.

## 🚀 Quick Start

```bash
# Build with Predator app (default)
python build_predator_firmware.py

# Build firmware with modifications only
python build_predator_firmware.py --no-app

.\fbt.cmd firmware_all 2>&1 | tee build.log

# Build just the Predator app (recommended)
cd predator_app
ufbt build

# Copy app to firmware and build complete firmware
python copy_predator.py
cd flipperzero-firmware-wPlugins

# Windows build commands
.\fbt.cmd COMPACT=1 DEBUG=0
# OR
python scripts\fbt.py COMPACT=1 DEBUG=0

# Linux/macOS build commands  
./fbt COMPACT=1 DEBUG=0
```

## 📋 Prerequisites

- **Git** - Download from [git-scm.com](https://git-scm.com)
- **ufbt** - Install with `pip3 install ufbt`
- **Python 3.6+**

## 🛠️ Build Modes

### Full Build (Default)
```bash
python build_predator_firmware.py --install-app
```
- Copies Predator app to firmware
- Applies GPIO and CC1101 patches
- Builds `predator.fap` file
- Ready for installation via qFlipper

### Firmware Only
```bash
python build_predator_firmware.py --no-app
```
- Applies only hardware modifications
- Builds standard firmware with Predator support
- No app installation required
- Smaller firmware size

## 🔧 Hardware Configuration

The script automatically configures these pin mappings:

### ESP32 UART
- **TX Pin**: PA7 (Pin 2)
- **RX Pin**: PA6 (Pin 3)
- **Baud**: 115200

### GPS UART
- **TX Pin**: PA4 (Pin 7)
- **RX Pin**: PB3 (Pin 8)
- **Baud**: 9600

### CC1101 SPI
- **CS Pin**: PA4 (Pin 7)
- **SCK Pin**: PA5 (Pin 6)
- **MISO Pin**: PA6 (Pin 3)
- **MOSI Pin**: PA7 (Pin 2)
- **GDO0 Pin**: PB2 (Pin 5)

## 📁 Project Structure

```
Predator/
├── build_predator_firmware.py     # Main build script
├── predator_app/                  # Predator application source
│   ├── helpers/                   # Helper modules
│   ├── scenes/                    # UI scenes
│   └── application.fam            # App manifest
├── flipperzero-firmware-wPlugins/ # Firmware repository (auto-cloned)
└── README.md                      # This file
```

## 🎯 Build Process

1. **Dependency Check** - Verifies Git and ufbt installation
2. **Repository Setup** - Clones/updates firmware repository
3. **App Integration** - Copies Predator app (if enabled)
4. **Configuration** - Creates Predator-specific config files
5. **Patching** - Applies GPIO and CC1101 modifications
6. **Building** - Compiles firmware/app using ufbt

## 📦 Output Files

### With App Installation
- **File**: `predator.fap`
- **Location**: `applications_user/predator/dist/`
- **Installation**: Copy to `/ext/apps/Tools/` on Flipper

### Firmware Only
- **File**: `firmware.upd`
- **Location**: `dist/f7-C/`
- **Installation**: Flash via qFlipper or flipper-cli

## 🔍 Command Line Options

| Option | Description | Default |
|--------|-------------|---------|
| `--install-app` | Install Predator app | ✅ True |
| `--no-app` | Skip app, firmware only | ❌ False |
| `--offline` | Skip network operations | ❌ False |
| `--help` | Show help message | - |

## 🏗️ Manual Firmware Building with ufbt

After running the Predator build script, you can manually build the firmware using ufbt:

### Building Complete Firmware
```bash
# Navigate to firmware directory
cd flipperzero-firmware-wPlugins

# Update ufbt SDK (requires internet)
ufbt update

# Build complete firmware
ufbt

# Build specific targets
ufbt firmware_flash    # Build and flash firmware
ufbt flash_usb        # Flash via USB
ufbt flash_usb_full   # Flash full firmware via USB
```

### Building Individual Apps
```bash
# Build Predator app specifically
cd flipperzero-firmware-wPlugins/applications_user/predator
ufbt

# Launch app directly on connected Flipper
ufbt launch

# Build and install app
ufbt fap_dist
```

### Advanced ufbt Commands
```bash
# Clean build artifacts
ufbt -c

# Verbose build output
ufbt -v

# Build with specific target
ufbt COMPACT=1          # Compact build
ufbt DEBUG=1           # Debug build
ufbt FORCE=1           # Force rebuild

# Flash operations
ufbt flash             # Flash firmware
ufbt flash_blackmagic  # Flash via Black Magic Probe
ufbt jflash            # Flash via J-Link

# Development tools
ufbt cli               # Open Flipper CLI
ufbt lint              # Run code linting
ufbt format            # Format code
ufbt copro_dist        # Build coprocessor firmware
```

## 🚨 Troubleshooting

### Build Failures
```bash
# Manual build in app directory
cd flipperzero-firmware-wPlugins/applications_user/predator
ufbt

# Manual firmware build
cd flipperzero-firmware-wPlugins
ufbt

# Clean and rebuild
ufbt -c
ufbt
```

### Missing Dependencies
```bash
# Install ufbt
pip3 install ufbt

# Update ufbt
ufbt update

# Install build dependencies (Linux/macOS)
sudo apt-get install build-essential git python3 python3-pip
# or
brew install git python3
```

### Git Issues
```bash
# Initialize git repository if missing
cd flipperzero-firmware-wPlugins
git init
git remote add origin https://github.com/RogueMaster/flipperzero-firmware-wPlugins.git

# Clean repository state
git clean -fd
git reset --hard origin/main

# Fix submodules
git submodule update --init --recursive
```

### Network Issues
```bash
# Use offline mode
python build_predator_firmware.py --no-app --offline

# Manual ufbt update with timeout
timeout 120 ufbt update

# Skip ufbt update and use existing SDK
# (patches will still be applied)
```

### Common ufbt Errors

#### "No targets specified"
```bash
# Ensure you're in the right directory
cd flipperzero-firmware-wPlugins
ls application.fam  # Should exist for app builds

# For firmware builds, run from root:
ufbt firmware
```

#### "App folder missing manifest"
```bash
# Check for application.fam file
ls applications_user/predator/application.fam

# If missing, re-run Predator build script
python build_predator_firmware.py --install-app
```

#### "SDK not found"
```bash
# Update ufbt SDK
ufbt update

# Check ufbt installation
ufbt --help
pip3 show ufbt
```

## 🔧 Alternative Build Methods

### Using Shell Script (Linux/macOS)
```bash
cd flipperzero-firmware-wPlugins
chmod +x buildRelease.sh
./buildRelease.sh
```

### Using Docker
```bash
# Build firmware in Docker container
docker run --rm -v $(pwd):/project flipperdevices/flipperzero-toolchain
```

### Cross-Platform Build
```bash
# Windows (PowerShell)
cd flipperzero-firmware-wPlugins
python -m pip install --user pipenv
pipenv install
pipenv run python scripts/fbt.py

# Linux/macOS
cd flipperzero-firmware-wPlugins
./fbt
```

## 📱 Installation Guide

### FAP Installation (App Mode)
1. Connect Flipper Zero via USB
2. Copy `predator.fap` to `/ext/apps/Tools/`
3. Launch from **Apps > Tools > Predator**

### Firmware Installation (Firmware Mode)
1. Connect Flipper Zero via USB
2. Use qFlipper to flash `firmware.upd`
3. Predator modifications are built into firmware

### Using ufbt for Installation
```bash
# Install app directly
cd applications_user/predator
ufbt launch

# Flash firmware directly
cd flipperzero-firmware-wPlugins
ufbt flash_usb
```

## ⚡ Hardware Requirements

- **Predator Module** with ESP32S2, GPS, and RF components
- **Flipper Zero** with external GPIO access
- **Proper pin connections** as specified in configuration

## 🔧 Advanced Usage

### Custom Builds
The script supports modification of pin configurations in `create_predator_config()` function.

### Development Mode
For development, use the `--no-app` mode to quickly test firmware modifications without rebuilding the entire app.

### Continuous Integration
```bash
# Automated build script
#!/bin/bash
python build_predator_firmware.py --no-app --offline
cd flipperzero-firmware-wPlugins
ufbt -c
ufbt firmware
```

## 🐛 Debug and Development

### Debug Build
```bash
cd flipperzero-firmware-wPlugins
ufbt DEBUG=1
```

### Code Analysis
```bash
# Run linting
ufbt lint

# Format code
ufbt format

# Generate documentation
ufbt doxygen
```

### Testing
```bash
# Run unit tests
ufbt test

# Run on device
ufbt launch_app APPID=predator
```

## 📄 License

This project integrates with the RogueMaster firmware distribution. Please respect all applicable licenses and terms of use.

---

**⚠️ Important**: Always ensure your Predator module is properly connected before using any Predator functionality on your Flipper Zero.
