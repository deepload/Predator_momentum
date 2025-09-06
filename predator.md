Introduction of 4 modules：
1. 433M RF module, using A07 module with a power of 10dBm. When using,
 insert to Flipper Zero and enter Sub Ghz,Click on the advanced settings after 
the menu to see the module being used, and change it to external
 2.ESP32S2 module, default flashing of Marauder firmware, Type-C flashing 
interface. Before flash, press and hold the button on the right side of the back 
(1#), then insert the USB to start burning.
 3. GPS module, powered by Flipper Zero or built-in battery of the 4 in 1 
expansion board, with the switch located on the left side of the front. Turn 
the switch down before insert to Flipper Zero, and use Flipper Zero's power 
supply. When only use the battery of the expansion board, turn the switch to 
the top, and the internal battery of the expansion board is used for power 
supply.
 4. The 2.8-inch Marauder charging interface is located at the top of the 
front, and the indicator light is on during charging and remains on after being 
fully charged. The GPS of the Marauder can be shared with the GPS of Flipper 
Zero, but the power supply is divided into machine power and Flipper Zero 
power, as mentioned above. When using only screen raiders, please turn the 
switch on the left side of the front to the top
 Howto use：
Note: Before plug the expansion board to the flipper zero, please make sure
 the swith on the left is turn to down.
If you want to use Flipper Zero GPS, please turn the front left switch to down
 and insert Flipper Zero. Find Monentum- Protocol Settings- GPIO Pin
 Settings- GPS Pin Settings- Change GPS pin to 13, 14 in the Flipper Zero
 menu, then go to APP-GPIO- [NAME] GPS and wait for a period of time to
 find a satellite for positioning. Our GPS antenna is located on the top left side
 of the front
 If you want to use a 433M RF module, insert Flipper Zero, enter the Sub Ghz
 menu, and click on Advanced Settings to view it.Change the module used to
 an external one
 If you want to use the ESP32S2 module, insert Flipper Zero, find Monentum
Protocol Settings- GPIO Pin Settings- ESP32 Change the pins to 15, 16, then
 go to APP-GPIO-ESP- [ESP32] WiFiMarauder to start using it
 If you want to use Screen Raider, turn on the switch on the right to start using
 it.
Battery capacity: 800mAh
 All pins are gold-plated, and each signal pin is equipped with TVS
 WiFi antenna 3dbi, GPS antenna 20dbi, 433M antenna 3dbi, all antennas are 
SMAinternal pin connectors
 Q&A：
1.Touch screen not working well.
 As this picture,outside the area to up/down, inside the area to select/ok.
 2.Can it work without flipper zero?  Yes
 3.When i use the S2 module, flipper zero shows 0x****** , len******
 For the S2 module is different with the Flipper zero, re-flash the firmware of 
S2 to fix it, reocmmend to re-flash a lower version firmware.
4.How to flash the firmware of screen
 Need to open the case, on the left of the back side, there is a 4pin sh1.0 connector, 
connect as this picture,press the button 2 on the back to flash.
 5.How to flash the firmware of S2
 Press and hold the button 2 on the back,  connect the USB port B to computer, release 
the button after connect to computer, begin to flash.
 6. Interface of the module
 1: Boot button of S2 module
 2: ESP32 marauder Boot Button
 A: Charging port of this device
 B: S2 burning port
 C: GPS power switch
 D: Marauder switch
 E: Marauder burning por