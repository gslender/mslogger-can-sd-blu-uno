#ifndef megasquirt_h
#define megasquirt_h

#define MS_CELSIUS 1
#define MS_FAHRENHEIT 0

#define MS_ENGINE_READY 1
#define MS_ENGINE_CRANKING 2
#define MS_ENGINE_STARTW 4
#define MS_ENGINE_WARMUP 8
#define MS_ENGINE_TPSACCAEN 16
#define MS_ENGINE_TPSACCDEN 32
#define MS_ENGINE_MAPACCAEN 64
#define MS_ENGINE_MAPACCDEN 128

#include "Arduino.h"
#include "../main.h"
#include <limits.h>

typedef struct {
	uint16_t seconds;
	uint16_t pw1;
	uint16_t pw2;
	uint16_t rpm;
	int16_t advance;
	uint8_t engine;
	uint8_t afrtgt1;
	uint8_t afrtgt2;
	int16_t baro;
	int16_t map;
	int16_t mat;
	int16_t clt;
	int16_t tps;
	int16_t batt;
	int16_t afr1;
	int16_t afr2;
	int16_t ego1;
	int16_t ego2;
	int16_t accel;
	int16_t ve1;
	int16_t ve2;
} MSData;

class MegaSquirt {

public:

	MegaSquirt();
	void process(unsigned long id, unsigned char len, unsigned char buf[]);

	byte getEngine();
	unsigned int getRpm();
	float getMap();
	float getMat(byte celsius);
	float getClt(byte celsius);
	float getTps();
	float getAfr();
	float getTaf();
	float getSpk();
	float getPwp();

private:
	MSData data;
};

#endif

