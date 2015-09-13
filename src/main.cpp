#include "main.h"

MCP_CAN CAN(SPI_CS_PIN);
SoftwareSerial bt_serial(3, 2);
Bluetooth_HC05 blumod(bt_serial);
TinyGPSPlus gps;
File logfile;
MegaSquirt megaSquirt;
SdFat SD;

unsigned long time = millis();
byte updatesCnt = 0;

bool sdcard_active = false;
bool bt_gps_active = false;
bool can_active = false;

int last_sentencesWithFix=0;
long time_lap, start_time = millis();

void setup() {
	//setup debug
    D(debugSerial.begin(115200);)
	D(debugSerial.println(F("setup"));)

    if (!setupSd()) {
    	D(debugSerial.println(F("SD CARD FAIL!"));)
		return;
	}

	if (!setupBt()) {
		D(debugSerial.println(F("BT CONNECT FAIL!"));)
    	return;
	}

	if (!setupCan()) {
		D(debugSerial.println(F("CAN COMMS FAIL!"));)
    	return;
	}
}

void loop()
{
	if (bt_gps_active) {
		int n = bt_serial.available();
		while (n > 0){
			n--;
			char c = bt_serial.read();
			gps.encode(c);
//			D(debugSerial.print(c);)
		}
	}

	if (can_active) {
		unsigned char len = 0;
		unsigned char buf[8];
		unsigned long id;

		if (CAN_MSGAVAIL == CAN.checkReceive())       // check if data coming
		{
			CAN.readMsgBufID(&id,&len, buf); // read data,  len: data length, buf: data buf
			megaSquirt.process(id,len, buf);
		}
	}

	if (gps.location.isUpdated()) {
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

		D(debugSerial.printf(F("%d/sec\r\n"),updatesCnt);)
		updatesCnt=0;
	}
}

bool setupSd() {
	D(debugSerial.println(F("Sd"));)

	if (SD.begin(SD_CS, SPI_FULL_SPEED)) {
//    if (SD.begin(SD_CS)) {
		 // Open up the file we're going to log to!
		logfile = SD.open("datalog.msl", FILE_WRITE);
		if (logfile) return true;
	}
	D(debugSerial.println(F("SD CARD INIT FAILED!"));)
	return false;
}

bool setupBt() {
	D(debugSerial.println(F("Bt"));)

	blumod.begin(38400, 6, HC05_RESET_GND, 7, HC05_MODE_COMMAND);

	delay(500);
	while (!blumod.probe()) {
		delay(250);
	}

	blumod.restoreDefaults();
	blumod.hardReset();

	delay(500);
	while (!blumod.probe()) {
		delay(250);
	}

	blumod.setRole(HC05_ROLE_MASTER);
	blumod.setPassword("0000");
	blumod.initSerialPortProfile();
	blumod.inquire(NULL);
	BluetoothAddress qstarz818 = { 0x1c, 0x00, 0x88, 0x6e, 0x38, 0x14 };
	blumod.setSerialMode(9600,1,HC05_NO_PARITY);
	bt_gps_active = blumod.connect(qstarz818,10000);

	delay(500);

	//  ** http://www.hhhh.org/wiml/proj/nmeaxor.html
//	bt_serial.println(F("$PMTK251,9600*17"));
//	bt_serial.println(F("$PMTK314,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*29"));
//	bt_serial.println(F("$PMTK220,100*2F"));
//	bt_serial.println(F("$PMTK300,100*2C"));

	return bt_gps_active;
}

bool setupCan() {
	 return CAN_OK == CAN.begin(CAN_500KBPS);
}

int freeRam() {
  extern int __heap_start,*__brkval;
  int v;
  return (int)&v - (__brkval == 0
    ? (int)&__heap_start : (int) __brkval);
}
