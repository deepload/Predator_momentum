Complete Flipper Zero + Predator/Marauder Expansion Guide
Table of Contents

Hardware Overview
Initial Setup & Installation
Firmware Management
GPIO Configuration
WiFi & Bluetooth Operations
SubGHz RF Module
GPS Navigation
Advanced Usage Scenarios
Troubleshooting
Legal & Ethical Guidelines


Hardware Overview
Predator/Marauder Expansion Board Components

ESP32-S3 main processor
WiFi antenna (2.4 GHz)
SubGHz antenna (433/868/915 MHz)
GPS module with dedicated antenna
OLED display (optional on some models)
MicroSD card slot
USB-C charging port
Internal battery (typically 1000-2000mAh)
GPIO breakout pins

Power Management System

Power Switch Positions:

DOWN: Powered by Flipper Zero
UP: Uses internal battery


Charging: USB-C port (5V, 1-2A recommended)
Battery Life: 4-8 hours depending on usage


Initial Setup & Installation
Step 1: Physical Installation

Power Off both devices completely
Switch Position: Ensure Predator/Marauder switch is DOWN
Alignment: Carefully align GPIO pins with Flipper Zero
Connection: Gently press down until fully seated
Antenna Installation: Screw on all antennas finger-tight

Step 2: First Boot Verification

Power on Flipper Zero
Navigate to Settings → Hardware Info
Verify GPIO module is detected
Check for any error messages

Step 3: Initial Configuration
Menu → Settings → System
- Enable Developer Mode
- USB Mode: VCP + Mass Storage

Firmware Management
Flipper Zero Firmware Requirements

Minimum Version: v0.87.0 or higher
Recommended: Latest stable or dev firmware
Custom Firmware: Unleashed/RogueMaster compatible

ESP32 Marauder Firmware Flashing
Prerequisites

ESP32 Flash Download Tool or esptool.py
Latest Marauder firmware (.bin files)
USB-C cable

Flashing Process

Enter Bootloader Mode:
1. Hold BOOT button on Marauder board
2. Connect USB-C cable to computer
3. Release BOOT button after 3 seconds

Using esptool.py (Command Line):
bashpip install esptool
esptool.py --chip esp32s3 --port COM3 --baud 921600 write_flash 0x0 marauder_firmware.bin

Using ESP32 Flash Tool (GUI):

Select ESP32-S3 chip
Load firmware file at address 0x0
Set baud rate to 921600
Click "START"



Firmware Update Verification

Reboot both devices
Navigate to GPIO → ESP32 Marauder
Check version in "About" section


GPIO Configuration
Pin Mapping (Flipper Zero → Predator/Marauder)
GPIO 0  → ESP32 Boot/Flash
GPIO 1  → UART TX
GPIO 2  → UART RX  
GPIO 3  → Power Control
GPIO 4  → Status LED
GPIO 5  → SPI CLK
GPIO 6  → SPI MISO
GPIO 7  → SPI MOSI
Manual GPIO Setup
c// Example GPIO configuration for custom applications
#include <furi.h>
#include <furi_hal_gpio.h>

// Initialize GPIO pins
void gpio_init_predator() {
    furi_hal_gpio_init(&gpio_ext_pa0, GpioModeOutputPushPull, GpioPullNo, GpioSpeedVeryHigh);
    furi_hal_gpio_init(&gpio_ext_pa1, GpioModeInput, GpioPullUp, GpioSpeedVeryHigh);
}
Common GPIO Conflicts & Solutions

SPI Conflict: Disable internal SPI modules before using external
UART Overlap: Ensure only one UART interface is active
Power Issues: Check 3.3V rail current draw (max 500mA)


WiFi & Bluetooth Operations
ESP32 Marauder Interface
Access through: Menu → Apps → GPIO → ESP32 Marauder
WiFi Reconnaissance

WiFi Scan:

Scans 2.4 GHz networks
Shows SSID, BSSID, Channel, Encryption
Saves results to SD card


Advanced Scanning:
Settings → WiFi:
- Channel Range: 1-14
- Scan Time: 500ms per channel
- Filter Options: Open/WPA/WPA2/WPA3


Deauthentication Attacks
⚠️ WARNING: For authorized testing only

Target Selection:

Select network from scan results
Choose specific client or broadcast


Deauth Parameters:
Deauth → Settings:
- Packet Count: 100 (default)
- Delay: 1ms between packets
- Target: Broadcast or specific MAC


Bluetooth Operations

BLE Scan:

Discovers nearby Bluetooth devices
Shows device names and MAC addresses
Identifies device types (phones, headphones, etc.)


Bluetooth Spam:

Sends advertisement packets
Can simulate various device types
Useful for testing device responses



Packet Analysis
WiFi Sniffing → Monitor Mode:
- Channel: Select specific or scan all
- Filter: Management/Data/Control frames
- Save: PCAP format to SD card

SubGHz RF Module
Configuration Setup

Navigate to SubGHz → Settings
Select External Module → Yes
Module Type: CC1101 or SX1276 (depending on board)
Frequency Range: Verify supported bands

Frequency Ranges

433 MHz: ISM band (worldwide)
868 MHz: European ISM band
915 MHz: North American ISM band
Custom: Check local regulations

Enhanced Range Testing
Transmission Power:
- Internal CC1101: ~10mW
- External Module: Up to 100mW (region dependent)
- Range Improvement: 5-10x typical
Signal Analysis

Frequency Analyzer:

Real-time spectrum view
Peak detection
Signal strength measurement


Protocol Detection:

Automatically identifies common protocols
Decodes ASK/OOK/FSK modulation
Saves captures for analysis




GPS Navigation
GPS Module Setup

Power Mode: Switch UP for battery operation
Antenna: Ensure GPS antenna is connected
Initial Lock: Allow 1-5 minutes for first fix

GPS Interface
Access through: Menu → Apps → GPIO → GPS
Features & Functions

Real-time Position:

Latitude/Longitude (decimal degrees)
Altitude (meters above sea level)
Accuracy estimate (CEP 50%)


Navigation Data:

Speed (km/h or mph)
Heading (degrees true/magnetic)
Satellite count and signal strength


Logging Functions:
GPS Logging:
- Track recording to SD card
- GPX format export
- Waypoint marking
- Distance/time calculations


GPS Configuration
Settings → GPS:
- Update Rate: 1Hz (default) / 5Hz / 10Hz
- Coordinate Format: DD.DDDD° or DMS
- Datum: WGS84 (standard)
- NMEA Output: Enable for external apps

Advanced Usage Scenarios
Security Testing Workflow

Reconnaissance Phase:
1. WiFi scan → Identify targets
2. BLE scan → Discover devices  
3. SubGHz scan → Check for sensors
4. GPS logging → Record locations

Analysis Phase:

Review captured data
Identify vulnerabilities
Document findings



Penetration Testing Use Cases

WiFi Security Assessment
Bluetooth device enumeration
IoT device discovery
Physical security testing
Radio frequency mapping

Research & Development
Custom Firmware Development:
- ESP32 SDK access
- Custom protocol implementation
- Sensor data fusion
- Remote monitoring capabilities

Troubleshooting
Common Issues & Solutions
Power Problems
Symptom: Module not powering on
Solutions:

Check switch position (DOWN for Flipper power)
Verify GPIO connection alignment
Test with USB-C power (switch UP)
Check Flipper battery level

Firmware Issues
Symptom: Apps not loading or crashes
Solutions:
1. Update Flipper firmware first
2. Flash latest Marauder firmware
3. Reset settings: Settings → Reset → Factory Reset
4. Check SD card integrity
GPS Lock Problems
Symptom: No satellite fix after 10+ minutes
Solutions:

Move to open area (away from buildings)
Check antenna connection
Verify GPS module power
Cold start: Power cycle module

WiFi/Bluetooth Not Working
Symptom: No networks detected
Solutions:
1. Check antenna connections
2. Verify ESP32 firmware version
3. Reset WiFi settings
4. Test in different location
SD Card Issues
Symptom: Cannot save data
Solutions:

Format SD card (FAT32)
Check write permissions
Verify card capacity (32GB max recommended)
Test card in computer first

Debug Mode Activation
Developer Options:
1. Settings → System → Debug
2. Enable "Log Level: Debug"
3. USB → Log Output
4. Monitor via serial terminal
Error Codes Reference
Error 001: GPIO initialization failed
Error 002: ESP32 communication timeout
Error 003: SD card mount failed
Error 004: GPS module not detected
Error 005: Antenna VSWR high

Legal & Ethical Guidelines
⚠️ IMPORTANT LEGAL NOTICE
This device and guide are intended for authorized security testing, research, and educational purposes only.
Prohibited Uses

Unauthorized network access
Jamming licensed frequencies
Privacy violations
Malicious interference
Illegal surveillance

Authorized Use Cases

Personal network testing
Authorized penetration testing
Security research
Educational demonstrations
Amateur radio experiments (with license)

Regional Frequency Regulations
Region433 MHz868 MHz915 MHz2.4 GHzUSA✓✗✓✓EU✓✓✗✓Asia✓VariesVaries✓
Best Practices

Written Authorization: Always obtain permission before testing
Scope Limitation: Test only authorized systems
Documentation: Keep detailed logs of activities
Responsible Disclosure: Report vulnerabilities properly
Legal Compliance: Follow local and federal laws


Additional Resources
Official Documentation

Flipper Zero Docs: https://docs.flipperzero.one
ESP32 Marauder GitHub: Search for "ESP32Marauder"
FCC Regulations: Part 97 (Amateur) and Part 15 (Unlicensed)

Community Resources

Flipper Zero Discord/Reddit
Security Research Forums
Amateur Radio Communities
Maker/Hacker Spaces

Recommended Tools

Wireshark: Packet analysis
RF Explorer: Spectrum analysis
SDR Software: Advanced signal processing
GPS Utilities: Track analysis and mapping


Document Version: 2.1
Last Updated: September 2025
Compatibility: Flipper Zero firmware v0.87.0+, ESP32 Marauder v4.0+
This guide is provided for educational and authorized security testing purposes only. Users are responsible for compliance with all applicable laws and regulations.