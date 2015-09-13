# mslogger-lcd-sd-blu-uno

An Arduino ATmega328P microcontroller application that logs Megasquirt (via Serial) and GPS data (via Bluetooth) with a 1.8" 18-bit Color TFT Shield w/microSD and Joystick to provide limitted displayed information and control of the display, Bluetooth pair and logging functions.

Project is built using Platformio's code builder - see http://platformio.org/ and I'm using Eclipse as the IDE

After pulling down the project's repo, run the following platformio command
`platformio init --board uno --ide eclipse`

To build the project (and upload to Arduino) you run `platformio run -t upload`

The Arduino board actually used is the freetronics Eleven (100% Uno) - we've then got the following additions...

* Adafruit 1.8" 18-bit Color TFT Shield w/microSD and Joystick
* Linksprite Bluetooth Shield for Arduino (for connection to a Qstarz 10Hz GPS logger).
* DB9 Serial TTL board wired to digital pins D0-D1 for Hardware Serial for Megasquirt logging.
* DB9 Serial TTL board wired to digital pins D2-D3 Software Serial for debuging.

*The project is not yet working or finished, but I will be updating as I progress...*

**Pins used:**
* D0 DB9 Hardware Serial RX - DEBUG 
* D1 DB9 Hardware Serial TX - DEBUG 
* D2 Bluetooth - RX  
* D3 Bluetooth - TX
* D4 LCD TFT - SD Card
* D5 
* D6 Bluetooth - RESET (*see below) 
* D7 Bluetooth - Command Mode / PIO11 (*see below)
* D8 LCD TFT - Display
* D9 -
* D10 LCD TFT - Display
* D11 LCD TFT - Display 
* D12 LCD TFT - SD Card
* D13 LCD TFT - Display
* A0 -
* A1 -
* A2 -
* A3 LCD TFT - NAV Switch
* A4 -
* A5 -

*Note: The Linksprite Bluetooth Shield for Arduino did not come with support for pins for reset or AT command mode enabling, so I wired these to set pins on the shield. This enables going into and out of AT command mode so that you can control the bluetooth module via software.*
