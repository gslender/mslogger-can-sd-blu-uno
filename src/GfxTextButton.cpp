
#include "GfxTextButton.h"


// Constructor
GfxTextButton::GfxTextButton() {}

void GfxTextButton::create(Adafruit_TFTLCD* _tft,unsigned int _x, unsigned int _y, unsigned int _w, unsigned int _h, byte _fntsize, char* _text, unsigned int _fgColour, unsigned int _bgColour) {

    GfxButton::create(_tft,_x,_y,_w,_h,_fgColour,_bgColour);
    fntsize = _fntsize;
    text = _text;
}

void GfxTextButton::drawText(unsigned int col)
{    
    unsigned int x1 = x+(w-(strlen(text)*6*fntsize))/2;
    unsigned int y1 = y+(h-8*fntsize)/2;
    tft->setCursor(x1,y1);
    tft->setTextSize(fntsize);
    tft->setTextColor(col);
    tft->print(text);
}

void GfxTextButton::draw()
{
    GfxButton::draw();
    drawText(bgColour);
}


void GfxTextButton::drawPressed()
{
    GfxButton::drawPressed();
    drawText(fgColour);
}
