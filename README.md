# megasquirt-lcd-duino

An Arduino AVR microcontroller application that logs Megasquirt and GPS data. Also uses LCD shield to provide displayed information along with a menu to select RPM limits etc.

Current project is based on Platformio's code builder - see http://platformio.org/

After pulling down the project's repo, run the following platformio command
`platformio init --board megaatmega2560 --ide eclipse`

The Arduino board actually used is the Seeeduino Mega (which is a Uno sized board but with a ATmega2560) - we've then got the following shields...

1) Adafruit's ILI9328 TFT LCD Display and Touch screen shield - uses digital pins 5-13 and analog 0-3. 
2) Adafruit's Ultimate GPS Logger shield - using pins 18(TX1) and 19(RX1) by running wires from GPS.
3) Linksprite Bluetooth Shield for Arduino on pins D2 and D3

In order to get Adafruit's Ultimate GPS Logger working on a MEGA, you need to wire the RX/TX to one of the other Serial ports (Serial1).

.. more details to follow soon ..