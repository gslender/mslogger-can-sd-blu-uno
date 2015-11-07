# mslogger-can-sd-blu-uno

An Arduino ATmega328P microcontroller application that logs Megasquirt (via CAN) and GPS data (via Bluetooth or Adafruit Ultimate GPS) to a microSD. Project is built using Platformio's code builder - see http://platformio.org/ and I'm using Eclipse as the IDE

After pulling down the project's repo, run the following platformio command
`platformio init --board uno --ide eclipse`

To build the project (and upload to Arduino) you run `platformio run -t upload`

*The project is not yet finished, but I will be updating as I progress...*

The Arduino board actually used is the [freetronics Eleven 100% Uno](http://www.freetronics.com.au/products/eleven) and then for CAN-BUS the [seeedstudio CAN-BUS Shield](http://www.seeedstudio.com/depot/CANBUS-Shield-V12-p-2256.html) - and then either a GPS or Blutooth shield...

* Linksprite [Bluetooth Shield for Arduino](http://store.linksprite.com/linksprite-bluetooth-shield-for-arduino/) which required a bit a rewiring to enable the reset and command mode pins (see below). This was then paired to a GPS module which was a Qstarz 10Hz GPS device

*or*

* Adafruit [Ultimate GPS Logger Shield](http://www.adafruit.com/products/1272) with additional external antenna kit. The Ultimate GPS Logger also includes a built in SD card wired to Pin D10 and a batt backed up RTC for the GPS, so the startup time is enhanced.

##Additional Wiring
There is also a few additional wiring for a switch and status LEDs. The switch is used to turn the logging to SD card on/off. There are two (2) status LEDs for SD logging activity and CAN activity. A 74HC595 is used to drive 8 LEDs for an RPM limit status guage.

**Pins used with Adafruit Ultimate GPS Logger Shield:**
* D0 DB9 Hardware Serial RX - DEBUG 
* D1 DB9 Hardware Serial TX - DEBUG
* D5 LOG on/off switch
* D7 Adafruit GPS - RX
* D8 Adafruit GPS - TX
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

**Pins used with Linksprite Bluetooth Shield:**
* D0 DB9 Hardware Serial RX - DEBUG 
* D1 DB9 Hardware Serial TX - DEBUG
* D2 [Bluetooth - RX]
* D3 [Bluetooth - TX]
* D5 LOG on/off switch
* D6 [Bluetooth - RESET (*see below)] 
* D7 [Bluetooth - Command Mode / PIO11 (*see below)]
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

*Note: The Linksprite Bluetooth Shield for Arduino did not come with support for pins for reset or AT command mode enabling, so I wired these to set pins on the shield. This enables going into and out of AT command mode so that you can control the bluetooth module via software.*
