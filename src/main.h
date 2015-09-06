#ifndef _MAIN_H_
#define _MAIN_H_

#define DEBUG // comment out to enable and disable the MegaSquirt code
#include "debug.h"

#include "megasquirt/MegaSquirt.h"
#include "gfx/GfxDataField.h"
#include "gfx/GfxIndicator.h"
#include "gfx/GfxTextButton.h"

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <TinyGPS++.h> // GPS library
#include <SD.h>		// SD Card library
#include <Bluetooth_HC05.h>
#include <SoftwareSerial.h>
#include <SPI.h>

#define DATAFLD_RPM 0
#define DATAFLD_CLT 1
#define DATAFLD_MAT 2
#define DATAFLD_MAP 3
#define DATAFLD_TPS 4
#define DATAFLD_AFR 5
#define DATAFLD_SPK 6
#define MAX_DATAFLDS 7

// Assign human-readable names to some common 16-bit color values:
#define	BLACK   0x0000
#define	DRKGRAY 0x2124
#define	LTGRAY  0xBDD7
#define	BLUE    0x001F
#define	LTBLUE  0x94DF
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define TFT_CS     10
#define TFT_RST    9
#define TFT_DC     8
#define SD_CS 	   4


void dataCaptureLoop();

void drawMainScreen();

void drawLogo();

void drawError(const __FlashStringHelper *ifsh);

void drawWarnRPM();

void drawLimitRPM();

int freeRam();

void display_freeram();


#endif /* _MAIN_H_ */
