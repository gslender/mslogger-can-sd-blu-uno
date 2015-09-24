#ifndef _MAIN_H_
#define _MAIN_H_

#include "config.h"
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

#define RPM_ACTIVE_VALUE RPM_MAX_LIMIT-(RPM_LIMIT_STEPS*8)
#define RPM_FLASH_VALUE RPM_MAX_LIMIT-(RPM_LIMIT_STEPS*2)

void grabGPSData(unsigned long duration);
bool setupGps();
bool buildDateTime();
bool startLogFile();
bool setupSd();
#ifdef USE_BT_GPS_DEVICE
bool setupBt();
#endif
bool setupCan();
void debugLoop();
void checkRPMLimits();
void writeRPMLedShiftBits();
void enableRPMLed(int led, bool enable);

#endif /* _MAIN_H_ */
