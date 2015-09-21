#include "main.h"

MCP_CAN CAN(CAN_CS_PIN);
SoftwareSerial gps_serial(BT_SER_RX,BT_SER_TX);
TinyGPSPlus gps;
File logfile;
MegaSquirt ms;
SdFat SD;

char date_time_filename[30];
char outstr[15];
Metro redLedMetro = Metro(500);
unsigned long lastFixCount = 0;
bool sdcard_active = false;
bool can_active = false;
bool major_fail = true;
unsigned long time_lap, start_time = millis();

void setup() {
	pinMode(RED_LED, OUTPUT);
	pinMode(GREEN_LED, OUTPUT);
	pinMode(LOG_SW_PIN, INPUT);
	digitalWrite(RED_LED, HIGH);
	digitalWrite(GREEN_LED, LOW);
	// setup Timer0 for LED status
	OCR0A = 0xAF;
	TIMSK0 |= _BV(OCIE0A);

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
		redLedMetro.interval(1000);
		return;
	}

    if (!setupSd()) {
    	D(debugSerial.println(F("SD CARD FAIL!"));)
		redLedMetro.interval(2000);
		return;
	}


	if (digitalRead(LOG_SW_PIN) == HIGH) {
		startLogFile();
	}

	if (!setupCan()) {
		D(debugSerial.println(F("CAN FAIL!"));)
	}
    major_fail = false;
}

// Interrupt is called once a millisecond,
SIGNAL(TIMER0_COMPA_vect)
{
	if (!major_fail) return;

	if (redLedMetro.check() == 1) {
		redLedMetro.reset();
		int state = digitalRead(RED_LED);
		if (state)
			state = LOW;
		else
			state = HIGH;
		digitalWrite(RED_LED, state);
	}
}

void loop()
{
	if (major_fail) return;

	if (sdcard_active && digitalRead(LOG_SW_PIN) == LOW) {
		if (logfile && logfile.isOpen()) {
			logfile.close();
		}
		sdcard_active = false;
		D(debugSerial.println(F("LOG OFF!!"));)
		delay(1000);
		return;
	}

	if (!sdcard_active && digitalRead(LOG_SW_PIN) == HIGH) {
		startLogFile();
		delay(1000);
		return;
	}

	grabGPSData(10);

	digitalWrite(GREEN_LED, LOW);

	if (gps.location.isUpdated() && sdcard_active) {
		digitalWrite(GREEN_LED, HIGH);
		logfile.print(millis());
		logfile.print('\t');
		logfile.print(ms.getData().RPM);
		logfile.print('\t');
		dtostrf((float) ms.getData().MAP / 10.0f, 6, 2, outstr);
		logfile.print(outstr);
		logfile.print('\t');
		dtostrf((float) ms.getData().TPS / 10.0f, 6, 2, outstr);
		logfile.print(outstr);
		logfile.print('\t');
		dtostrf((float) ms.getData().AFR1 / 10.0f, 6, 2, outstr);
		logfile.print(outstr);
		logfile.print('\t');
		dtostrf((float) (ms.getData().MAT-320) * 0.05555f, 6, 2, outstr);
		logfile.print(outstr);
		logfile.print('\t');
		dtostrf((float) (ms.getData().CLT-320) * 0.05555f, 6, 2, outstr);
		logfile.print(outstr);
		logfile.print('\t');
		dtostrf((float) ms.getData().BATT / 10.0f, 6, 2, outstr);
		logfile.print(outstr);
		logfile.print('\t');
		dtostrf((float) ms.getData().EGOCOR1 / 10.0f, 6, 2, outstr);
		logfile.print(outstr);
		logfile.print('\t');
		dtostrf((float) ms.getData().PW1 / 1000.0f, 6, 3, outstr);
		logfile.print(outstr);
		logfile.print('\t');
		dtostrf((float) ms.getData().ADV_DEG / 10.0f, 6, 2, outstr);
		logfile.print(outstr);
		logfile.print('\t');
		dtostrf(gps.location.lat(), 12, 6, outstr);
		logfile.print(outstr);
		logfile.print('\t');
		dtostrf(gps.location.lng(), 12, 6, outstr);
		logfile.print(outstr);
		logfile.print('\t');
		dtostrf(gps.speed.kmph(), 6, 1, outstr);
		logfile.println(outstr);

		logfile.flush();
	}

	digitalWrite(RED_LED, LOW);
	if (can_active && CAN_MSGAVAIL == CAN.checkReceive()) {
		unsigned char can_len = 0;
		unsigned char can_buf[8];
		unsigned long can_id;
		CAN.readMsgBufID(&can_id,&can_len, can_buf);
		if (ms.process(can_id, can_buf))
			digitalWrite(RED_LED, HIGH);
	}

	D(debugLoop());
}

void debugLoop() {

	time_lap = millis() - start_time;
	if (time_lap > 1000) {
		start_time = millis();
		int mem = freeMemory();
		int fix = gps.sentencesWithFix()-lastFixCount;
		lastFixCount = gps.sentencesWithFix();
		debugSerial.printf(F("%dB free | %d/s gps fix\r\n"),mem,fix);
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

bool startLogFile() {

	if (logfile && logfile.isOpen()) {
		logfile.close();
	}
	sdcard_active = false;
	if (!buildDateTime()) return false;

	if (SD.exists(date_time_filename)) SD.remove(date_time_filename);

	logfile = SD.open(date_time_filename, FILE_WRITE);
	if (logfile) {
		sdcard_active = true;
		logfile.println(F("Time\tRPM\tMAP\tTPS\tAFR\tMAT\tCLT\tBatt V\tEGO cor1\tPW\tSpark Adv\tGPS Lat\tGPS Lon\tGPS Speed"));
	}
	D(debugSerial.printf(F(">Log:%s"),date_time_filename);)

	return sdcard_active;
}

bool setupSd() {
	D(debugSerial.println(F(">Sd"));)

	return SD.begin(SD_CS_PIN, SPI_FULL_SPEED);
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

	delay(1500);

	while (!doCmdWaitOkRespBt(F("AT"),250)) {
		delay(250);
	}
	doCmdWaitOkRespBt(F("AT+ORGL"),250);
	pinMode(BT_RESET_PIN, OUTPUT);
	digitalWrite(BT_RESET_PIN, LOW);
	delay(6);
	pinMode(BT_RESET_PIN, INPUT);

	delay(1500);

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
