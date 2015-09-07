#include "main.h"

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);
SoftwareSerial bt_serial(6, 5);
Bluetooth_HC05 blumod(bt_serial);
TinyGPSPlus gps;
File logfile;

MegaSquirt megaSquirt;

GfxDataField datafields[MAX_DATAFLDS];
GfxIndicator engIndicator;
unsigned long time = millis();
byte rpmWarn;
byte rpmLimit;
byte tempScale = MS_CELSIUS;
byte flashtime;
bool showRpmWarning = false;
bool showRpmLimit = false;
bool showLogo = true;

void setup() {
//	Serial.begin(115200); //115200 pins 0, 1

    D(debugSerial.begin(115200);)
	D(debugSerial.println(F("setup"));)

	tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab

//    tft.reset();
//
//    uint16_t identifier = tft.readID();
//
//    if(identifier != 0x9328)
//    {
//    	D(debugSerial.print(F("Unknown LCD driver chip: "));)
//		D(debugSerial.println(identifier,HEX);)
//    }
//
//    tft.begin(identifier);
//    tft_spi_save = SPCR;

    tft.setRotation(3);

    tft.fillScreen(BLACK);

    drawLogo();


    if (!SD.begin(SD_CS)) {
    	drawError(F("SD CARD INIT FAILED!"));
    }

/*
	// open the file. note that only one file can be open at a time,
	// so you have to close this one before opening another.
	File configFile = SD.open("config.txt");
	// if the file opened okay, write to it:
	if (configFile) {
	} else {
	// no card just display stuff (no logging)
	}
/*
	if (sdcard.begin(SS, SPI_HALF_SPEED)) {

		SdFile configFile;
		// re-open the file for reading:
		if (!configFile.open("test.txt", O_READ)) {
		}
//		Serial.println("test.txt:");

		// read from the file until there's nothing else in it:
		int data;
		while ((data = configFile.read()) > 0) {
			//Serial.write(data);
		}
		// close the file:
		configFile.close();
	}
	*/

	blumod.begin(38400, 7, HC05_MODE_COMMAND);
	blumod.setRole(HC05_ROLE_MASTER);
	blumod.setPassword("0000");
	blumod.initSerialPortProfile();
	blumod.inquire(NULL,10000);
	BluetoothAddress qstarz818 = { 0x00, 0x11, 0x04, 0x29, 0x02, 0x55 };
	blumod.connect(qstarz818);
//	gpsSerial.begin(9600);
    // turn on RMC (recommended minimum) and GGA (fix data) including altitude
    //gps.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);

    // Set the update rate
    //gps.sendCommand(PMTK_SET_NMEA_UPDATE_5HZ);   // 5 Hz update rate
    //gps.sendCommand(PMTK_API_SET_FIX_CTL_5HZ);   // 5 Hz update rate

    // enable interrupts to capture serial reads (for gps / megasquirt etc)
	cli();//stop interrupts
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
    sei();//allow interrupts

//    drawMainScreen();

    flashtime = 0;
}

void loop()
{
//    if (millis() > time) {
//        time = millis()+100;
//    	dataCaptureLoop();
//    }

//    if (gps.newNMEAreceived()) {
//
//       if (gps.parse(gps.lastNMEA())) {
//    	   gps.latitude_fixed;
//    	   gps.longitude_fixed;
//    	   gps.speed;
//    	   gps.angle;
//    	   gps.altitude;
//       }
//    }
}

// Interrupt is called once a millisecond, looks for any new GPS data, and stores it
// at 9600 this is roughly 1 byte or more per interrupt
// at 115200 this is nearly 15 bytes or more per interrupt
SIGNAL(TIMER0_COMPA_vect) {
//	 while (gpsSerial.available() > 0)
//	    gps.encode(gpsSerial.read());
}

void drawLogo()
{
    tft.fillRect(0, 0, TFT_WIDTH, 12, LTBLUE);
    tft.setTextSize(1);
    tft.setCursor(2,2);
    tft.setTextColor(BLACK);
    tft.print(F("Starting..."));

    tft.setCursor(TFT_WIDTH-60,2);
    tft.printf(F("Free:%d"),freeRam());

}

void drawError(const __FlashStringHelper *ifsh)
{
    tft.setTextSize(5);
    tft.setCursor(30,60);
    tft.setTextColor(RED);
    tft.print(ifsh);
}

int freeRam()
{
  extern int __heap_start,*__brkval;
  int v;
  return (int)&v - (__brkval == 0
    ? (int)&__heap_start : (int) __brkval);
}
