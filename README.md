[![BR-M5 | Open-Source Bluetooth Remote for Canon DSLRs](./.github/cover.png)](https://youtu.be/Gh5uEc2dNJM "BR-M5 | Open-Source Bluetooth Remote for Canon DSLRs")

# From an https://github.com/ArthurFDLR idea

# BR-M5: An open-source Bluetooth Remote for Canon DSLRs

This Bluetooth remote aims to replicate and empower Canon's BR-E1 behavior while adding key features like fully programable time-lapses, star-trails and bulb modes that most Canon cameras lack! Fortunately, *M5Stack* proposes a nice little inexpensive development board that perfectly fits our needs, the [M5Stick-Cplus2](https://shop.m5stack.com/products/m5stickc-plus2-esp32-mini-iot-development-kit).

The BR-M5 has been primarily developed for the Canon M50 Mark I, which misses a descent timelapse mode that can do long exposures. The shutter speed option in M50's timelapse mode is indeed limited to 1/30s. Furthermore, the M50 doesn't have any shutter release port to plug classic intervalometers. No official Bluetooth, WiFi, or tethering tools currently offer time-lapse capabilities.

## Installation

* Install [Visual Studio Code](https://code.visualstudio.com/download) and the [PlatformIO IDE](https://marketplace.visualstudio.com/items?itemName=platformio.platformio-ide) extension.
* Clone this repository - ```git clone https://github.com/MattiaTinfena/BR-M5-v2``` -, open it as project in PlatformIO and upload - ```Ctrl+Alt+u``` - to a M5Stick-Cplus2 connected to your PC.

## How-To-Use
 ![M5StickCPlus2](https://github.com/MattiaTinfena/BR-M5-v2/assets/101633758/f5ef207a-4a7d-424d-9c77-bb4c58053798)
 
* Power on by pressing the C button and power off with a 6 seconds press on the same button.
* To pair a new camera press A button while M5StickPlus2 is starting up
* You can connect to your Canon DSLR as soon as the remote is powered on and displays *Pairing in progress*.
* Use B button and C button to navigate the main menu.
* Use A button to choose the mode
* To change the interval, press the B button to increase and the button C to decrease the interval/shutter speed. Once set, press the A button to shoot.
* Hold B button for a second to return to main menu

## To-do

- [] Optimize and clean the code
- [] Remove warnings.

Feel free to contribute!
