
#include "GfxButton.h"


// Constructor
GfxButton::GfxButton() {}

//void GfxButton::setDebug(SoftwareSerial* _debug)
//{
//    debug = _debug;
//}


void GfxButton::create(Adafruit_TFTLCD* _tft,unsigned int _x, unsigned int _y, unsigned int _w, unsigned int _h, unsigned int _fgColour, unsigned int _bgColour) {
    tft = _tft;
    x = _x;
    y = _y;
    w = _w;
    h = _h;
    fgColour = _fgColour;
    bgColour = _bgColour;
}

void GfxButton::swapColours()
{
    int c = fgColour;
    fgColour = bgColour;
    bgColour = c;
}

void GfxButton::draw()
{
    tft->fillRoundRect(x, y, w, h, 4, fgColour);
    tft->drawRoundRect(x, y, w, h, 4, bgColour);
}

void GfxButton::drawPressed()
{
    tft->fillRoundRect(x, y, w, h, 4, bgColour);
}

bool GfxButton::isPressed(Point p)
{    
    // TouchScreen doesn't follow TFT orientation nor dimensions
    int p_x = p.x;
    p.x = map(p.y, TS_MINY, TS_MAXY, 0, tft->width());
    p.y = map(p_x, TS_MINX, TS_MAXX, tft->height(), 0);
    
    if (armed && p.z < MINPRESSURE)
    {
        armed = false;
        delay(100);
        draw();
        return true;
    }
    
    if (p.z < MINPRESSURE) return false;
        
    if (p.x > x && p.x < x+w && p.y > y && p.y < y+h)
    {
//        debug->print(p.x);
//        debug->print(F(","));
//        debug->print(p.y);
//        debug->print(F(","));
//        debug->print(p.z);
//        debug->println(F(" good"));
        
        if (p.z > MINPRESSURE && p.z < MAXPRESSURE && !armed)
        {
            armed = true;
            drawPressed();
        }
        
    }
    else if (armed)
    {
//        debug->print(p.x);
//        debug->print(F(","));
//        debug->print(p.y);
//        debug->print(F(","));
//        debug->print(p.z);
//        debug->println(F(" bad"));

        armed = false;
        draw();
    }
    return false;

    /*
    Point p = touchScreen().getPoint();
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    
    if (p.z > MINPRESSURE && p.z < MAXPRESSURE)
    {
        
        debug->print(p.x);
        debug->print(F(","));
        debug->print(p.y);
        debug->println(F(" - before map"));

        // TouchScreen doesn't follow TFT orientation nor dimensions
        int p_x = p.x;
        p.x = map(p.y, TS_MINY, TS_MAXY, 0, tft->width());
        p.y = map(p_x, TS_MINX, TS_MAXX, tft->height(), 0);
        
        debug->print(p.x);
        debug->print(F(","));
        debug->print(p.y);
        debug->println(F(""));
        
        tft->drawPixel(p.x, p.y, WHITE);
        
        if (p.x > x && p.x < x+w && p.y > y && p.y < y+h)
        {
            drawPressed();
            delay(500);
            while (p.z > MINPRESSURE)
            {
                p.z = touchScreen().getPoint().z;
                pinMode(XM, OUTPUT);
                pinMode(YP, OUTPUT);
                delay(50);

            }
            draw();
            return true;
        }
    }
    return false;
     */
}