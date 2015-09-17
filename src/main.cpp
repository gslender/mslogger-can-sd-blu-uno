#include "main.h"

MCP_CAN CAN(CAN_CS_PIN);
SoftwareSerial gps_serial(BT_SER_RX,BT_SER_TX);
TinyGPSPlus gps;
File logfile;
MegaSquirt megaSquirt;
SdFat SD;

char date_time_filename[30];
unsigned long time = millis();
byte updatesCnt = 0;

bool sdcard_active = false;
bool can_active = false;

int last_sentencesWithFix=0;
long time_lap, start_time = millis();

void setup() {
	//setup debug
    D(debugSerial.begin(115200);)
	D(debugSerial.println(F("setup"));)

	if (setupBt()) {
		if (!setupGps()) {
			D(debugSerial.println(F("GPS FAIL!"));)
		}

		if (!buildDateTime()) {
			D(debugSerial.println(F("TIME FAIL!"));)
		}
	} else {
		D(debugSerial.println(F("BT FAIL!"));)
	}

    if (!setupSd()) {
    	D(debugSerial.println(F("SD CARD FAIL!"));)
	}

	if (!setupCan()) {
		D(debugSerial.println(F("CAN FAIL!"));)
	}
}

void loop()
{
	grabGPSData(5);

	if (can_active) {
		unsigned char len = 0;
		unsigned char buf[8];
		unsigned long id;

		if (CAN_MSGAVAIL == CAN.checkReceive())       // check if data coming
		{
			CAN.readMsgBufID(&id,&len, buf); // read data,  len: data length, buf: data buf
			megaSquirt.process(id, buf);
		}
	}

	if (gps.location.isUpdated() && sdcard_active) {
		updatesCnt++;

		logfile.print(gps.location.lat());
		logfile.print(',');
		logfile.print(gps.location.lng());
		logfile.print(',');
		logfile.print(gps.speed.kmph());
		logfile.print(',');
		logfile.print(gps.time.hour());
		logfile.print(',');
		logfile.print(gps.time.minute());
		logfile.print(',');
		logfile.print(gps.time.second());
		logfile.print(',');
		logfile.print(gps.time.centisecond());
		logfile.println();
	}

	time_lap = millis() - start_time;
	if (time_lap > 1000) {
		start_time = millis();
		logfile.flush();

		D(debugSerial.printf(F("%d/sec - %d sram\r\n"),updatesCnt,freeRam());)
		updatesCnt=0;
	}
}

void grabGPSData(unsigned long duration) {
	unsigned long end_time = millis() + duration;
	while (millis() < end_time ) {
		int n = gps_serial.available();
		while (n > 0){
			n--;
			char c = gps_serial.read();
			gps.encode(c);
		}
	}
}

bool setupSd() {
	D(debugSerial.println(F(">Sd"));)

	if (SD.begin(SD_CS_PIN, SPI_FULL_SPEED)) {
		logfile = SD.open(date_time_filename, FILE_WRITE);
		if (logfile) sdcard_active = true;
	}
	return sdcard_active;
}

bool doCmdWaitOkRespBt(const __FlashStringHelper *cmd, unsigned int waitforMS) {
	while (gps_serial.available()) gps_serial.read();
	D(debugSerial.println(cmd);)
	gps_serial.println(cmd);
	byte state=0;
	unsigned long end_time = millis() + waitforMS;
	while (end_time > millis()) {
		if (gps_serial.available()) {
			char c = gps_serial.read();
			D(debugSerial.print(c);)
			switch (state) {
			case 0:
				if (c == 'O' || c == 'o')
					state = 1;
				break;
			case 1:
				if (c == 'K' || c == 'k')
					state = 2;
				break;
			case 2:
				if (c == '\n')
					return true;
				break;
			}
		} else
			delay(5);
	}
	D(debugSerial.println("**timeout");)

	return false;
}

bool setupBt() {
	D(debugSerial.println(F(">Bt"));)

	gps_serial.begin(38400);
	pinMode(BT_MODE_PIN, OUTPUT);
	digitalWrite(BT_MODE_PIN, HIGH);

	pinMode(BT_RESET_PIN, OUTPUT);
	digitalWrite(BT_RESET_PIN, LOW);
	delay(6);
	pinMode(BT_RESET_PIN, INPUT);

	delay(700);

	while (!doCmdWaitOkRespBt(F("AT"),250)) {
		delay(250);
	}
	doCmdWaitOkRespBt(F("AT+ORGL"),250);
	pinMode(BT_RESET_PIN, OUTPUT);
	digitalWrite(BT_RESET_PIN, LOW);
	delay(6);
	pinMode(BT_RESET_PIN, INPUT);

	delay(700);

	while (!doCmdWaitOkRespBt(F("AT"),250)) {
		delay(250);
	}

	doCmdWaitOkRespBt(F("AT+ROLE=1"),250);
	doCmdWaitOkRespBt(F("AT+PSWD=0000"),250);
	doCmdWaitOkRespBt(F("AT+INQM=1,1,20"),250);
	doCmdWaitOkRespBt(F("AT+INIT"),250);
	doCmdWaitOkRespBt(F("AT+INQ"),30000); // wait 30 seconds
	doCmdWaitOkRespBt(F("AT+UART=9600,0,0"),250);
	bool result = doCmdWaitOkRespBt(F("AT+LINK=1c,88,14386e"),10000); // wait ten seconds

	delay(500);

	return result;
}

bool setupGps() {
	D(debugSerial.println(F(">Gps"));)

	gps_serial.begin(38400);

	/***use to calc checksum >> http://www.hhhh.org/wiml/proj/nmeaxor.html    **/

	// switch to 9600 baud
	gps_serial.println(F("$PMTK251,9600*17"));
	/*
	// switch to 38400 baud
	gps_serial.println(F("$PMTK251,38400*27"));
	// switch to 14400 baud
	gps_serial.println(F("$PMTK251,14400*29"));
	*/
	// turn on DGPS with WAAS mode
	gps_serial.println(F("$PMTK301,2*2E"));
	// turn off all but $GPRMC and $GPGGA
	gps_serial.println(F("$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28"));
	// set update rate at 100ms = 10Hz
	gps_serial.println(F("$PMTK220,100*2F"));
	// set pos fix rate at 100ms = 10Hz
	gps_serial.println(F("$PMTK300,100*2C"));


	return true;
}

bool buildDateTime() {
	D(debugSerial.println(F(">DateTime"));)
	//try to gather date&time for 10 seconds
	for (int i = 0; i < 10; i++) {
		grabGPSData(1000);

		if (gps.time.isUpdated()) {
			// 2015.09.17-20.41.11
			snprintf(date_time_filename,30,"log-%d.%02d.%02d-%02d.%02d.%02d.msl",gps.date.year(),gps.date.month(),gps.date.day(),
					gps.time.hour(),gps.time.minute(),gps.time.second());

			D(debugSerial.println(date_time_filename);)
			return true;
		}
	}
	return false;
}

bool setupCan() {
	D(debugSerial.println(F(">Can"));)
	can_active = (CAN_OK == CAN.begin(CAN_500KBPS));
	return can_active;
}

int freeRam() {
  extern int __heap_start,*__brkval;
  int v;
  return (int)&v - (__brkval == 0
    ? (int)&__heap_start : (int) __brkval);
}
