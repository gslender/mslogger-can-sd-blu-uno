#ifndef _MAIN_H_
#define _MAIN_H_

//#define ENABLE_DEBUG // comment out this line to disable
#include "debug.h"
#include "Arduino.h"

#include "megasquirt/MegaSquirt.h"

#include <SPI.h>
#include <Metro.h>
#include <MemoryFree.h>
#include <TinyGPS++.h>
#include <SdFat.h>
#include <SoftwareSerial.h>
#include <mcp_can.h>

#define BT_SER_TX 		2
#define BT_SER_RX 		3
#define SD_CS_PIN    	4
#define LOG_SW_PIN    	5
#define BT_RESET_PIN 	6
#define BT_MODE_PIN 	7
#define CAN_CS_PIN  	9

#define RED_LED 		A0
#define GREEN_LED 		A1


void grabGPSData(unsigned long duration);
bool setupGps();
bool buildDateTime();
bool startLogFile();
bool setupSd();
bool setupBt();
bool setupCan();
void debugLoop();


#endif /* _MAIN_H_ */
