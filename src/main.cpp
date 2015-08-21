
//#include "Arduino.h"

#include "TempSensor.h"
#include "MegaSquirt.h"
#include "GfxDataField.h"
#include "GfxIndicator.h"
#include "GfxTextButton.h"
#include "GfxArrowButton.h"

#include "Adafruit_GFX.h"    // Core graphics library
#include "Adafruit_TFTLCD.h" // Hardware-specific library
#include "TouchScreen.h"
#include <SoftwareSerial.h>
#include <EEPROM.h>

#define DATAFLD_TMP 0
#define DATAFLD_RPM 1
#define DATAFLD_CLT 2
#define DATAFLD_MAT 3
#define DATAFLD_MAP 4
#define DATAFLD_TPS 5
#define DATAFLD_AFR 6
#define DATAFLD_TAF 7
#define DATAFLD_ADV 8
#define DATAFLD_PWP 9

// These are the pins for the TouchScreen
#define YP A1  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 7   // can be a digital pin
#define XP 6   // can be a digital pin

SoftwareSerial* debug;
Adafruit_TFTLCD tft;   // 320 x 240
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
TempSensor tempSensor;
MegaSquirt megaSquirt;
GfxDataField datafields[10];
GfxIndicator engIndicator;
GfxTextButton setupButton;
GfxTextButton exitButton;
unsigned long time = millis();
byte rpmWarn;
byte rpmLimit;
byte tempScale;
byte flashtime;
bool showRpmWarning = false;
bool showRpmLimit = false;
bool showLogo = true;
GfxDataField rpmWarnFld;
GfxDataField rpmLimitFld;
GfxLabel tempScaleLabel;
GfxTextButton celsiusButton;
GfxTextButton fahrenheitButton;
GfxArrowButton rpmWarnUp;
GfxArrowButton rpmWarnDn;
GfxArrowButton rpmLimitUp;
GfxArrowButton rpmLimitDn;
GfxLabel serialLabel;
GfxTextButton ms2Button;
GfxTextButton ms3Button;


void drawMainScreen()
{
    debug->println(F(">> Main"));
    
    //STRIPES
    byte yrow = 30;
    byte c;
    for (c=0; c<5;c++)
    {
        tft.fillRect(0, (yrow+18)+2*c*18, 320, 16, DRKGRAY);
    }
    
    //DATAFIELDS
    datafields[DATAFLD_TMP].create(&tft,230,yrow,2,1,BLACK,LTGRAY);
    datafields[DATAFLD_TMP].drawLabel(10,yrow,2,"Outside Temp:");
    
    yrow +=18;
    datafields[DATAFLD_RPM].create(&tft,230,yrow,2,1,DRKGRAY,WHITE);
    datafields[DATAFLD_RPM].drawLabel(10,yrow,2,"Engine RPM:");
    
    yrow +=18;
    datafields[DATAFLD_CLT].create(&tft,230,yrow,2,1,BLACK,LTGRAY);
    datafields[DATAFLD_CLT].drawLabel(10,yrow,2,"Coolant Temp:");
    
    yrow +=18;
    datafields[DATAFLD_MAT].create(&tft,230,yrow,2,1,DRKGRAY,WHITE);
    datafields[DATAFLD_MAT].drawLabel(10,yrow,2,"Manifold Temp:");
    
    yrow +=18;
    datafields[DATAFLD_MAP].create(&tft,230,yrow,2,1,BLACK,LTGRAY);
    datafields[DATAFLD_MAP].drawLabel(10,yrow,2,"Manifold kPa:");
    
    yrow +=18;
    datafields[DATAFLD_TPS].create(&tft,230,yrow,2,1,DRKGRAY,WHITE);
    datafields[DATAFLD_TPS].drawLabel(10,yrow,2,"Throttle Pos:");
    
    yrow +=18;
    datafields[DATAFLD_AFR].create(&tft,230,yrow,2,1,BLACK,LTGRAY);
    datafields[DATAFLD_AFR].drawLabel(10,yrow,2,"O2 / AFR:");
    
    yrow +=18;
    datafields[DATAFLD_TAF].create(&tft,230,yrow,2,1,DRKGRAY,WHITE);
    datafields[DATAFLD_TAF].drawLabel(10,yrow,2,"Target AFR:");
    
    yrow +=18;
    datafields[DATAFLD_ADV].create(&tft,230,yrow,2,1,BLACK,LTGRAY);
    datafields[DATAFLD_ADV].drawLabel(10,yrow,2,"Spark Adv:");
    
    yrow +=18;
    datafields[DATAFLD_PWP].create(&tft,230,yrow,2,3,DRKGRAY,WHITE);
    datafields[DATAFLD_PWP].drawLabel(10,yrow,2,"Pulse Width:");
    
    //INDICATORS / BUTTONS
    yrow +=20;
    engIndicator.create(&tft, 0, yrow, 140, 24, 2);
    engIndicator.setState(0, "No ECU?", DRKGRAY, LTGRAY);
    
//    setupButton.setDebug(debug);
    setupButton.create(&tft, 240, yrow, 80, 24, 2, "SETUP", BLACK, LTGRAY);
    setupButton.draw();
}

void drawLogo()
{
    tft.fillRect(0, 0, 320, 25, LTBLUE);
    tft.setCursor(75,4);
    tft.setTextSize(2);
    tft.setTextColor(LTGRAY);
    tft.print(F("RednelsRacing"));
    tft.setCursor(76,5);
    tft.setTextColor(BLACK);
    tft.print(F("RednelsRacing"));
}

void drawWarnRPM()
{
    tft.fillRect(0, 0, 320, 25, YELLOW);
    tft.setCursor(85,4);
    tft.setTextSize(2);
    tft.setTextColor(BLACK);
    tft.print(F("RPM WARNING"));
}

void drawLimitRPM()
{
    tft.fillRect(0, 0, 320, 25, RED);
    tft.setCursor(105,4);
    tft.setTextSize(2);
    tft.setTextColor(WHITE);
    tft.print(F("RPM LIMIT"));
}


bool setupLoop()
{
    Point p = ts.getPoint();
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    if (exitButton.isPressed(p))
    {
        if (rpmWarn != EEPROM.read(2)) EEPROM.write(2, rpmWarn);
        if (rpmLimit != EEPROM.read(3)) EEPROM.write(3, rpmLimit);
        if (tempScale != EEPROM.read(4)) EEPROM.write(4, tempScale);
        if (megaSquirt.getNewSerial() != EEPROM.read(5)) EEPROM.write(5, megaSquirt.getNewSerial());
//        if (msOffset != EEPROM.read(5)) EEPROM.write(5, msOffset);

        tft.fillScreen(BLACK);

        drawLogo();

        drawMainScreen();
        return true;
    }


    if (tempScale == MS_CELSIUS)
    {
        if (fahrenheitButton.isPressed(p))
        {
            tempScale = MS_FAHRENHEIT;
            celsiusButton.swapColours();
            fahrenheitButton.swapColours();
            celsiusButton.draw();
            fahrenheitButton.draw();
        }
    }
    else
    {
        if (celsiusButton.isPressed(p))
        {
            tempScale = MS_CELSIUS;
            celsiusButton.swapColours();
            fahrenheitButton.swapColours();
            celsiusButton.draw();
            fahrenheitButton.draw();
        }
    }
    
    
    if (megaSquirt.getNewSerial())
    {
        if (ms2Button.isPressed(p))
        {
            megaSquirt.setNewSerial(false);
            ms2Button.swapColours();
            ms3Button.swapColours();
            ms2Button.draw();
            ms3Button.draw();
        }
    }
    else
    {
        if (ms3Button.isPressed(p))
        {
            megaSquirt.setNewSerial(true);
            ms2Button.swapColours();
            ms3Button.swapColours();
            ms2Button.draw();
            ms3Button.draw();
        }

    }

    
    if (rpmWarnUp.isPressed(p))
    {
        rpmWarn++;
        rpmWarnFld.setValue(rpmWarn*100);
    }

    if (rpmWarnDn.isPressed(p))
    {
        rpmWarn--;
        rpmWarnFld.setValue(rpmWarn*100);
    }
    
    if (rpmLimitUp.isPressed(p))
    {
        rpmLimit++;
        rpmLimitFld.setValue(rpmLimit*100);
    }
    
    if (rpmLimitDn.isPressed(p))
    {
        rpmLimit++;
        rpmLimitFld.setValue(rpmLimit*100);
    }
    
    return false;
}

void setup()
{
    Serial.begin(115200); //115200
    debug = new SoftwareSerial(2, 3); // RX, TX
    debug->begin(9600);
    megaSquirt.setDebug(debug);
    
    debug->println(F("ArdSquirt"));

    tft.reset();
    
    uint16_t identifier = tft.readID();
    
    if(identifier != 0x9328)
    {
        debug->print(F("Unknown LCD driver chip: "));
        debug->println(identifier, HEX);
    }
    
    tft.begin(identifier);
    tft.setRotation(3);
    
    tft.fillScreen(BLACK);
    
    drawLogo();
    
    byte ver1 = EEPROM.read(0);
    byte ver2 = EEPROM.read(1);
    
    if (ver1 != 0xff || ver2 != 0x01)
    {
        EEPROM.write(0, 0xff);
        EEPROM.write(1, 0x01);

        EEPROM.write(2, 60);
        EEPROM.write(3, 65);
        EEPROM.write(4, MS_FAHRENHEIT);
        EEPROM.write(5, 0);
    }
    
    rpmWarn = EEPROM.read(2); // x100
    rpmLimit = EEPROM.read(3); // x100
    tempScale = EEPROM.read(4);
    megaSquirt.setNewSerial(EEPROM.read(5));
    
    drawMainScreen();
    
    flashtime = 0;
}

void loop()
{
    if (millis() > time) {
        time = millis()+100;


        flashtime++;
        if (showRpmWarning)
        {
            if (flashtime % 2)
            {
                drawWarnRPM();
            }
            else
            {
                tft.fillRect(0, 0, 320, 25, BLACK);
            }
        }

        float temp = (tempSensor.getTempInt100()+5)/10/10.0f;

        if (tempScale == MS_FAHRENHEIT) {
            temp = (temp*1.8f) + 32;
        }
//        debug->print(F("Temp: "));
//        debug->println(temp);

        datafields[DATAFLD_TMP].setValue(temp);

        if (megaSquirt.requestData() == 1)
        {
//            debug->print(F("RPM: "));
//            debug->println(megaSquirt.getRpm());

            unsigned int rpm = megaSquirt.getRpm();

            if ((rpm/100) >= rpmWarn && (rpm/100) < rpmLimit)
            {
                showRpmWarning = true;
                showRpmLimit = false;
                showLogo = false;
            }
            else if ((rpm/100) >= rpmLimit)
            {
                if (!showRpmLimit)
                {
                    showRpmLimit = true;
                    showRpmWarning = false;
                    showLogo = false;
                    drawLimitRPM();
                }
            }
            else if (!showLogo)
            {
                drawLogo();
                showLogo = true;
                showRpmWarning = false;
                showRpmLimit = false;
            }

            datafields[DATAFLD_RPM].setValue((int)rpm);
            datafields[DATAFLD_CLT].setValue(megaSquirt.getClt(tempScale));
            datafields[DATAFLD_MAT].setValue(megaSquirt.getMat(tempScale));
            datafields[DATAFLD_MAP].setValue(megaSquirt.getMap());
            datafields[DATAFLD_TPS].setValue(megaSquirt.getTps());
            datafields[DATAFLD_AFR].setValue(megaSquirt.getAfr());
            datafields[DATAFLD_TAF].setValue(megaSquirt.getTaf());
            datafields[DATAFLD_ADV].setValue(megaSquirt.getAdv());
            datafields[DATAFLD_PWP].setValue(megaSquirt.getPwp());

            if (megaSquirt.getEngine() & MS_ENGINE_READY)
            {
                if (megaSquirt.getEngine() & MS_ENGINE_CRANKING)
                    engIndicator.setState(2, "Cranking", BLACK, RED);
                else
                    engIndicator.setState(3, "Running", BLACK, GREEN);
            }
            else
                engIndicator.setState(1, "Not running", DRKGRAY, LTGRAY);
        }
        else
        {
            byte c;
            for (c=1; c<10;c++)
            {
                datafields[c].setUnknown();
            }
            engIndicator.setState(0, "No ECU?", DRKGRAY, LTGRAY);
            if (!showLogo)
            {
                drawLogo();
                showLogo = true;
                showRpmWarning = false;
                showRpmLimit = false;
            }

//            debug->println(F("Megasquirt ??"));
        }
    }

    Point p = ts.getPoint();
    pinMode(XM, OUTPUT);
    pinMode(YP, OUTPUT);
    if (setupButton.isPressed(p))
    {
        debug->println(F(">> Setup"));
        tft.fillScreen(LTGRAY);

        drawLogo();


        rpmWarnDn.create(&tft,190,32, 25, 20, GFXARROW_DOWN, LTGRAY,BLACK);
        rpmWarnDn.draw();

        rpmWarnFld.create(&tft,225,35,2,1,LTGRAY,BLACK);
        rpmWarnFld.drawLabel(10,35,2,"Warning RPM:");
        rpmWarnFld.setValue(rpmWarn*100);

//        rpmWarnUp.setDebug(debug);
//        rpmWarnDn.setDebug(debug);

        rpmWarnUp.create(&tft,280,32, 25, 20, GFXARROW_UP, LTGRAY,BLACK);
        rpmWarnUp.draw();

        rpmLimitDn.create(&tft,190,62, 25, 20, GFXARROW_DOWN, LTGRAY,BLACK);
        rpmLimitDn.draw();

        rpmLimitFld.create(&tft,225,65,2,1,LTGRAY,BLACK);
        rpmLimitFld.drawLabel(10,65,2,"Limit RPM:");
        rpmLimitFld.setValue(rpmLimit*100);

        rpmLimitUp.create(&tft,280,62, 25, 20, GFXARROW_UP, LTGRAY,BLACK);
        rpmLimitUp.draw();

        tempScaleLabel.create(&tft,LTGRAY,BLACK);
        tempScaleLabel.drawLabel(10,95,2,"Temp Scale:");

//        celsiusButton.setDebug(debug);
//        fahrenheitButton.setDebug(debug);
        celsiusButton.create(&tft, 190, 90, 40, 24, 2, "C", LTGRAY, BLACK);
        fahrenheitButton.create(&tft, 260, 90, 40, 24, 2, "F", LTGRAY, BLACK);

        if (tempScale == MS_CELSIUS)
            celsiusButton.swapColours();
        else
            fahrenheitButton.swapColours();

        celsiusButton.draw();
        fahrenheitButton.draw();


        serialLabel.create(&tft,LTGRAY,BLACK);
        serialLabel.drawLabel(10,125,2,"Serial:");

        ms2Button.create(&tft, 190, 120, 50, 24, 2, "MS2", LTGRAY, BLACK);
        ms3Button.create(&tft, 260, 120, 50, 24, 2, "MS3", LTGRAY, BLACK);

        if (EEPROM.read(5) == 0)
            ms2Button.swapColours();
        else
            ms3Button.swapColours();

        ms2Button.draw();
        ms3Button.draw();


//        exitButton.setDebug(debug);
        exitButton.create(&tft, 120, 212, 80, 24, 2, "EXIT", LTGRAY, BLACK);
        exitButton.draw();


        bool exit = false;
        while (!exit)
        {
            exit = setupLoop();
        }
    }
}
