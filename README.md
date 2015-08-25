# mslogger-lcd-sd-blu-uno

An Arduino ATmega328P microcontroller application that logs Megasquirt (via Serial) and GPS data (via Bluetooth). Also uses LCD shield to provide limitted displayed information.

Current project is based on Platformio's code builder - see http://platformio.org/

After pulling down the project's repo, run the following platformio command
`platformio init --board uno --ide eclipse`

The Arduino board actually used is the freetronics Eleven (100% Uno) - we've then got the following additions...

1) Adafruit's ILI9328 TFT LCD Display and Touch screen shield - uses digital pins D5-D13 and analog A0-A3. 
2) Linksprite Bluetooth Shield for Arduino on digital pins D3 and D4 (for connection to a Qstarz 10Hz GPS logger).
3) DB9 Serial TTL board wired to digital pins D0-D1 for Hardware Serial for Megasquirt logging.
3) DB9 Serial TTL board wired to digital pins D2-D3 Software Serial for debuging.

.. more details to follow soon ..