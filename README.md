# mslogger-can-sd-blu-uno

An Arduino ATmega328P microcontroller application that logs Megasquirt (via CAN) and GPS data (via Bluetooth) to a microSD. Project is built using Platformio's code builder - see http://platformio.org/ and I'm using Eclipse as the IDE

After pulling down the project's repo, run the following platformio command
`platformio init --board uno --ide eclipse`

To build the project (and upload to Arduino) you run `platformio run -t upload`

The Arduino board actually used is the freetronics Eleven (100% Uno) - we've then got the following additions...

* DB9 Serial TTL board wired to digital pins D0-D1 for Hardware Serial for debuging.
* Linksprite Bluetooth Shield for Arduino (for connection to a Qstarz 10Hz GPS logger).
* seeedstudio CAN-BUS Shield wired to an MS3Pro.
* SD Card with a 4-channel I2C-safe Bi-directional Logic Level Converter.

*The project is not yet working or finished, but I will be updating as I progress...*

**Pins used:**
* D0 DB9 Hardware Serial RX - DEBUG 
* D1 DB9 Hardware Serial TX - DEBUG 
* D2 -  
* D3 -
* D4 -
* D5 LOG on/off switch
* D6 - 
* D7 GPS - RX
* D8 GPS - TX
* D9 SPI SS - CAN Shield
* D10 SPI SS - SD Card
* D11 SPI MOSI
* D12 SPI MISO
* D13 SPI SCK
* A0 RED LED
* A1 GREEN LED
* A2 RPM DATA PIN
* A3 RPM LATCH PIN
* A4 RPM CLOCK PIN
* A5 -

* D2 [Bluetooth - RX]  
* D3 [Bluetooth - TX]
* D6 [Bluetooth - RESET (*see below)] 
* D7 [Bluetooth - Command Mode / PIO11 (*see below)]

*Note: The Linksprite Bluetooth Shield for Arduino did not come with support for pins for reset or AT command mode enabling, so I wired these to set pins on the shield. This enables going into and out of AT command mode so that you can control the bluetooth module via software.*
