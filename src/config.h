#ifndef _CONFIG_H_
#define _CONFIG_H_

/* Configuration definitions for MSLOGGER-CAN-SD-BLU-UNO
 * */

#define BT_SER_TX 		2
#define BT_SER_RX 		3
#define LOG_SW_PIN    	5
#define BT_RESET_PIN 	6
#define BT_MODE_PIN 	7
#define GPS_SER_TX 		7
#define GPS_SER_RX 		8
#define CAN_CS_PIN  	9
#define SD_CS_PIN    	10

#define RED_LED 		A0
#define GREEN_LED 		A1
#define RPM_DATA_PIN	A2
#define RPM_LATCH_PIN	A3
#define RPM_CLOCK_PIN	A4

/* The following parameter enables Bluetooth shield and aims to connect
 * to the device address below. Comment out to use the
 * Adafruit Ultimate GPS Logger Shield
 * */
//#define USE_BT_GPS_DEVICE


/* The following parameter is the BT address of the GPS device.
 * This will need to be changed to match the address of your QSTARZ
 * device - mine was reported as being 1c,88,14386e
 * Ensure the format remains as "AT+LINK=xx,yy,zzzzzz"
 * */
#define BT_GPS_DEVICE_ADDR "AT+LINK=1c,88,14386e"


/* Debugging is globally enabled/disabled via this definition.
 * Comment out to remove all Serial print statements
 * */
#define ENABLE_DEBUG


/* Timezone offset from UTC.
 * In Australia that is 10
 * In Venezuela that is -4.3
 * */
#define TZ_OFFSET 10


/* The following parameter, if set to 1 will enable conversion to
 * Celsius for all temperature logged values.
 * Any other value will use Fahrenheit
 * */
#define USE_CELSIUS 1


/* The following parameter is the RPM maximum limit of which all 8
 * LED lights will be showing and flashing. This value is used
 * to set what the remaining LEDs will be lit and using this and
 * the RPM_LIMIT_STEPS value each LED is turned on or off.
 * */
#define RPM_MAX_LIMIT 8000


/* The following parameter is the RPM limit steps that each LED less
 * than 8 will be lit - so in the case of the value being 100
 * (where RPM_MAX_LIMIT is 8000) then when RPM is reporting greater
 * than 7900 (but less than 8000) then 7 LED will be lit.
 * At 7199 RPM or less none of the LEDs will be lit.
 * At 7200 RPM but less than 7299 RPM only the 1st LED will be lit
 * */
#define RPM_LIMIT_STEPS 100

#endif /* _CONFIG_H_ */
