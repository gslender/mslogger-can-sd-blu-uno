#ifndef _MAIN_H_
#define _MAIN_H_

#define DEBUG // comment out this line to disable
#include "debug.h"

#include "megasquirt/MegaSquirt.h"

#include <SPI.h>

#include <TinyGPS++.h> // GPS library
#include <SdFat.h>
//#include <SD.h>		// SD Card library
#include <Bluetooth_HC05.h>
#include <SoftwareSerial.h>
#include <mcp_can.h>


#define SPI_CS_PIN  9
#define SD_CS 	    4


bool setupSd();
bool setupBt();
bool setupCan();
int freeRam();


#endif /* _MAIN_H_ */
