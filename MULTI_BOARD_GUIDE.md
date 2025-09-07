# Predator App Multi-Board Support Guide

## Overview

The Predator app now supports multiple expansion boards for the Flipper Zero:

1. **Original Predator Module** - The default configuration
2. **3in1 AIO Board V1.4** - SecureTechware's 3in1-AIO-Expansion-Board
3. **DrB0rk Multi Board V2** - DrB0rk's Multi-board with ESP32 and NRF24
4. **3-in-1 NRF24+CC1101+ESP32** - Generic 3-in-1 multiboard combining NRF24, CC1101 and ESP32

This guide explains how to configure and use the Predator app with these different expansion boards.

## Board Selection

You can select or change your board type through the settings menu:

1. Open the Predator app
2. Navigate to **Settings**
3. Select **Board Selection**
4. Choose your expansion board from the list:
   - Original Predator Module
   - 3in1 AIO Board V1.4
   - DrB0rk Multi Board V2
   - 3-in-1 NRF24+CC1101+ESP32
   - Auto Detect (tries to automatically detect your board)

Your selection will be saved and remembered for future sessions.

## Board Details

### Original Predator Module

The original Predator module uses the following pin configuration:

- ESP32 TX/RX: Pins 15,16
- GPS TX/RX: Pins 13,14
- Marauder Switch: Active-low on Pin PA7
- GPS Power Switch: Active-low on Pin PA4

### 3in1 AIO Board V1.4

The 3in1 AIO Board uses the following pin configuration:

- ESP32 TX/RX: Pins 15,16
- GPS TX/RX: Pins 13,14
- No dedicated power switches (always powered)
- RF module accessible via external antenna

![AIO Board](images/aio_board.png)

### DrB0rk Multi Board V2

The DrB0rk Multi Board V2 uses the following pin configuration:

- ESP32 TX/RX: Pins 15,16
- GPS TX/RX: Pins 13,14
- NRF24 module for additional functionality
- No dedicated power switches

## Troubleshooting

If the app doesn't detect your board correctly, try these steps:

1. Ensure the board is properly connected to your Flipper Zero
2. Try manually selecting your board type from the Board Selection menu
3. Check for any loose connections or damaged pins
4. Verify that you're using the latest firmware for both Flipper Zero and the expansion board

### Module Status Screen

You can check the current status of your expansion board:

1. Go to **Settings** → **Module Status**
2. The screen will display:
   - Board type
   - Connection status
   - ESP32 Marauder status
   - GPS status
   - RF module status
   - Hardware details

## Notes on Specific Boards

### 3in1 AIO Board

- The 3in1 AIO Board does not have dedicated power switches, as the components are always powered when connected.
- The board has multiple antenna connections for different frequencies.
- Use the external SubGHz setting in the Flipper Zero for RF functionality.

### DrB0rk Multi Board

- The DrB0rk board features an additional NRF24 module.
- No dedicated power switches, components are always powered when connected.
- Uses the same pin mapping as the other boards for core functionality.

### 3-in-1 NRF24+CC1101+ESP32 Multiboard

- Combines NRF24, CC1101, and ESP32 modules on a single board.
- Features two external antennas for enhanced signal reception.
- Functions include NRF24 sniffing, mousejacking, extended SubGHz range via CC1101, and ESP32 Marauder capabilities.
- Uses pins 15/16 for ESP32 communication and pins 13/14 for GPS (if connected).
- No dedicated power switches, components are always powered when connected.

## Adding Support for New Boards

Developers can add support for additional expansion boards by modifying the board configuration in:

`predator_app/helpers/predator_boards.c`

Define a new board configuration with the appropriate pin mappings and add it to the `predator_board_configs` array.
