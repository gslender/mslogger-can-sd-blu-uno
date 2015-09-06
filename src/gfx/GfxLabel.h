#ifndef _GFXLABEL_H_
#define _GFXLABEL_H_

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif
#include "Adafruit_GFX.h"

class GfxLabel {
    
public:
    
    GfxLabel();
    void create(Adafruit_GFX* _tft, unsigned int _bgColour, unsigned int _fgColour);
    void drawLabel(unsigned int _x, unsigned int _y, char _fntsize, const __FlashStringHelper *ifsh);
    
protected:
    void prepDrawText(unsigned int _x, unsigned int _y, char _fntsize);
    Adafruit_GFX* tft;
    unsigned int bgColour;
    unsigned int fgColour;
};

#endif

