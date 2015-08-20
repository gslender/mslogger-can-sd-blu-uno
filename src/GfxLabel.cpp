
#include "GfxLabel.h"

// Constructor
GfxLabel::GfxLabel() {}

void GfxLabel::create(Adafruit_TFTLCD* _tft,unsigned int _bgColour, unsigned int _fgColour) {
    tft = _tft;
    bgColour = _bgColour;
    fgColour = _fgColour;
}

void GfxLabel::prepDrawText(unsigned int _x, unsigned int _y, char _fntsize)
{
    tft->setCursor(_x,_y);
    tft->setTextSize(_fntsize);
    tft->setTextColor(fgColour, bgColour);
}

void GfxLabel::drawLabel(unsigned int _x, unsigned int _y, char _fntsize, char* text)
{
    prepDrawText(_x,_y,_fntsize);
    tft->print(text);
}
