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
#define	BLACK   ST7735_BLACK
#define	BLUE    ST7735_BLUE
#define	RED     ST7735_RED
#define	GREEN   ST7735_GREEN
#define CYAN    ST7735_CYAN
#define MAGENTA ST7735_MAGENTA
#define YELLOW  ST7735_YELLOW
#define WHITE   ST7735_WHITE

#define	DRKGRAY 0x2124
#define	LTGRAY  0xBDD7
#define	LTBLUE  0x94DF

#define TFT_CS     10
#define TFT_RST    9
#define TFT_DC     8
#define SD_CS 	   4

#define TFT_WIDTH 160
#define TFT_HEIGHT 128


void drawLogo();

void drawError(const __FlashStringHelper *ifsh);

int freeRam();


#endif /* _MAIN_H_ */
