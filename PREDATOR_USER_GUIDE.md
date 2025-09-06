# Predator Module User Guide

## Table of Contents
1. [Introduction](#introduction)
2. [Hardware Setup](#hardware-setup)
3. [Switch Configuration](#switch-configuration)
4. [Antenna Setup](#antenna-setup)
5. [Menu Navigation](#menu-navigation)
6. [WiFi Attacks](#wifi-attacks)
7. [Bluetooth Attacks](#bluetooth-attacks)
8. [SubGHz/RF Operations](#subghzrf-operations)
9. [Car Attacks](#car-attacks)
10. [GPS Tracking](#gps-tracking)
11. [RFID/NFC Functions](#rfidnfc-functions)
12. [Wardriving](#wardriving)
13. [Social Engineering Tools](#social-engineering-tools)
14. [Settings](#settings)
15. [Troubleshooting](#troubleshooting)
16. [Legal Disclaimer](#legal-disclaimer)

## Introduction

The Predator module is a comprehensive penetration testing expansion for the Flipper Zero device. It adds advanced capabilities including WiFi attacks, Bluetooth scanning, SubGHz operations, GPS tracking, and more. This guide covers all aspects of using the Predator module with your Flipper Zero.

## Hardware Setup

### Installing the Predator Module

1. Power off your Flipper Zero completely
2. Align the Predator module with the GPIO pins on your Flipper Zero
3. Gently press down until the module is fully seated
4. Attach the required antennas (see Antenna Setup section)
5. Power on your Flipper Zero

### Physical Components

- **ESP32-S2 Marauder** processor (Pins 15,16)
- **GPS Module** (Pins 13,14)
- **A07 433MHz RF Module** (10dBm)
- **2.8" Display** with 800mAh battery
- **Two control switches** (front left and right)
- **Multiple antenna ports**

## Switch Configuration

The Predator module has two important switches that control different hardware components:

### Left Switch (GPS Power Switch)
- **Position DOWN**: Powers the GPS module using the Flipper Zero's power
- **Position UP**: Uses the Predator module's internal battery for GPS power
- **Function**: Set to DOWN when you want to save the Predator's internal battery and use Flipper's power instead.

### Right Switch (Marauder Enable Switch)
- **Position DOWN**: Disables the ESP32 Marauder module
- **Position UP**: Enables the ESP32 Marauder module for WiFi/Bluetooth attacks
- **Function**: Set to UP when you want to use WiFi scanning, deauth attacks, or Bluetooth functions.

## Antenna Setup

The Predator module includes several antenna ports, each designed for specific operations:

![Antenna Layout](antenna_layout.jpg)

- **433M**: Connect the 433MHz antenna here for SubGHz operations (car keys, RF)
- **A**: Main WiFi antenna port (2.4GHz) - This is the primary antenna for WiFi operations
- **GPS**: Connect the GPS antenna here for location tracking functions
- **B/C**: Secondary 2.4GHz antenna ports for WiFi operations (enhances performance)
- **D**: Optional diversity antenna port
- **2.4G**: Additional 2.4GHz ports marked as 1 and 2 for specialized operations

For best results:
- Always use the correct antenna for each frequency range
- Hand-tighten antennas only - do not overtighten
- Keep antennas perpendicular when in use
- For GPS functions, ensure the GPS antenna has a clear view of the sky

## Menu Navigation

The Predator app offers a comprehensive menu system accessed through the Flipper Zero interface:

1. Navigate to **Apps** on your Flipper Zero
2. Select the **Predator** application
3. Use the directional pad to navigate through menus
4. Press the center button to select options
5. Press the back button to return to previous menus

Main menu options include:
- 📡 WiFi Attacks
- 📱 Bluetooth Attacks
- 📻 SubGHz/RF Attacks
- 🚗 Car Attacks
- 💳 RFID/NFC Attacks
- 🛰️ GPS Tracker
- 🗺️ Wardriving
- 🎭 Social Engineering
- ⚙️ Settings
- ℹ️ About

## WiFi Attacks

Before using WiFi functions, ensure the right switch (Marauder Enable) is in the UP position.

### WiFi Scanning
1. Select **WiFi Attacks** from the main menu
2. Choose **WiFi Scan**
3. Wait as nearby networks are discovered
4. View details including SSID, BSSID, Channel, and Encryption
5. Press Back when finished

### Deauth Attack
1. Select **WiFi Attacks** from the main menu
2. Choose **Deauth Attack**
3. Select a target network from the scan results
4. Choose broadcast (all clients) or specific client
5. Set number of packets to send (default: 100)
6. Press Start to begin the attack
7. Press Back to stop

### Evil Twin
1. Select **WiFi Attacks** from the main menu
2. Choose **Evil Twin**
3. Select a target network or enter a custom SSID
4. Configure portal settings
5. Start the attack
6. Monitor connected clients
7. Press Back to stop

## Bluetooth Attacks

Before using Bluetooth functions, ensure the right switch (Marauder Enable) is in the UP position.

### BLE Scanner
1. Select **Bluetooth Attacks** from the main menu
2. Choose **BLE Scanner**
3. Wait as nearby Bluetooth devices are discovered
4. View details including device name, MAC address, and RSSI
5. Press Back when finished

### BLE Spam
1. Select **Bluetooth Attacks** from the main menu
2. Choose **BLE Spam**
3. Select the type of devices to simulate
4. Start the spam attack
5. Press Back to stop

Additional Bluetooth options include:
- BLE Flood: Mass BLE advertising packet flood
- Apple AirTag Spoof: Simulate Apple AirTags
- Samsung Buds Takeover: Target Samsung earbuds

## SubGHz/RF Operations

The Predator module enhances SubGHz capabilities with its extended range A07 module.

### Frequency Analyzer
1. Select **SubGHz/RF Attacks** from the main menu
2. Choose **Frequency Analyzer**
3. View real-time spectrum analysis
4. Identify active signals
5. Press Back when finished

### Signal Capture
1. Select **SubGHz/RF Attacks** from the main menu
2. Choose **Signal Capture**
3. Set the frequency and modulation type
4. Start capture
5. Save captured signals for later replay
6. Press Back when finished

### Signal Replay
1. Select **SubGHz/RF Attacks** from the main menu
2. Choose **Signal Replay**
3. Select a previously saved signal
4. Transmit the signal
5. Press Back when finished

## Car Attacks

The Car Attacks menu provides specialized functions for automotive systems.

### Car Key Bruteforce
1. Select **Car Attacks** from the main menu
2. Choose **Car Key Bruteforce**
3. Select car make/model
4. Choose frequency (usually 433.92MHz or 315MHz)
5. Start the bruteforce attack
6. Press Back to stop

### Car Jamming
1. Select **Car Attacks** from the main menu
2. Choose **Car Jamming**
3. Select frequency range
4. Start jamming
5. Press Back to stop

### Passive Car Opener
1. Select **Car Attacks** from the main menu
2. Choose **Passive Car Opener**
3. The system will monitor for car key signals
4. When detected, signals will be relayed in real-time
5. Press Back to stop

### Tesla Charge Port
1. Select **Car Attacks** from the main menu
2. Choose **Tesla Charge Port**
3. Point toward Tesla vehicle
4. Send signal to open charge port
5. Press Back to return

## GPS Tracking

For GPS functions, ensure the left switch is in the DOWN position if using Flipper power, or UP for internal battery.

### Live Tracking
1. Select **GPS Tracker** from the main menu
2. Wait for satellite acquisition (may take 1-5 minutes for first fix)
3. View current coordinates, altitude, and satellite count
4. Press right button for advanced debug info
5. Press Back when finished

### Logging & Export
1. Select **GPS Tracker** from the main menu
2. Press right to access debug screen
3. Select logging options
4. Choose GPX export if needed
5. Press Back to return

## RFID/NFC Functions

The Predator enhances the Flipper Zero's RFID capabilities with additional features.

### RFID Clone
1. Select **RFID/NFC Attacks** from the main menu
2. Choose **RFID Clone**
3. Scan the source card/tag
4. Select a compatible blank card/tag
5. Write the data
6. Test the clone
7. Press Back when finished

### RFID Bruteforce
1. Select **RFID/NFC Attacks** from the main menu
2. Choose **RFID Bruteforce**
3. Select card type
4. Start the bruteforce attack
5. Press Back to stop

## Wardriving

The wardriving function combines GPS and WiFi scanning to map networks.

1. Select **Wardriving** from the main menu
2. Ensure both GPS and WiFi modules are enabled
3. Start the wardriving session
4. Drive around target area
5. View real-time discoveries
6. Export data when complete
7. Press Back to stop

## Social Engineering Tools

1. Select **Social Engineering** from the main menu
2. Choose from available attack types
3. Configure parameters
4. Execute the selected attack
5. Press Back when finished

## Settings

The Settings menu allows configuration of the Predator module:

1. Select **Settings** from the main menu
2. Configure options:
   - WiFi Settings: Channel range, scan time, etc.
   - Bluetooth Settings: Scan duration, device filtering
   - SubGHz Settings: External module, power level
   - GPS Settings: Update rate, coordinate format
   - Power Management: Battery saving options
   - Debug Mode: Advanced logging options
3. Press Back to save and return

## Troubleshooting

### Common Issues

#### No WiFi Networks Found
- Ensure right switch (Marauder) is UP
- Verify WiFi antenna is connected to port A
- Try rescanning with longer duration

#### GPS Not Getting Fix
- Ensure left switch is in correct position
- Verify GPS antenna is connected
- Move to area with clear view of sky
- Allow 1-5 minutes for first fix

#### Module Not Powering On
- Check connection to Flipper Zero
- Try reseating the module
- Verify Flipper Zero battery is charged

#### SubGHz Functions Not Working
- Ensure 433MHz antenna is connected
- Check for frequency restrictions in your region
- Verify target is within range

## Legal Disclaimer

The Predator module is designed for authorized security testing, research, and educational purposes only. Using this device for unauthorized network access, disruption of services, or other illegal activities is strictly prohibited and may be subject to legal penalties. 

Users are responsible for:
- Obtaining proper authorization before testing
- Complying with all applicable laws and regulations
- Using these tools responsibly and ethically

This documentation is provided for educational purposes only. The developers and distributors of the Predator module assume no liability for misuse or illegal actions performed with this device.
