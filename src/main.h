#ifndef _MAIN_H_
#define _MAIN_H_

#define DEBUG // comment out this line to disable
#include "debug.h"

#include "megasquirt/MegaSquirt.h"

#include <SPI.h>

#include <TinyGPS++.h> // GPS library
#include <SdFat.h>
//#include <SD.h>		// SD Card library
//#include <Bluetooth_HC05.h>
#include <SoftwareSerial.h>
#include <mcp_can.h>

#define BT_SER_TX 		2
#define BT_SER_RX 		3
#define SD_CS_PIN    	4
#define BT_RESET_PIN 	6
#define BT_MODE_PIN 	7
#define CAN_CS_PIN  	9



void grabGPSData(unsigned long duration);
bool setupGps();
bool obtainDateTime();
bool setupSd();
bool setupBt();
bool setupCan();
int freeRam();


#endif /* _MAIN_H_ */
