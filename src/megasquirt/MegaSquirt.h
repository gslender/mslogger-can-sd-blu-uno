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
#include "../debug.h"
#include <limits.h>

enum {
	MS_DEFAULT_TIMEOUT = 200,
	MS_SIGNATURE_DEFAULT_TIMEOUT = 1000,
	MS_REALTIME_DATA_DEFAULT_TIMEOUT = 1000,
	MS_REALTIME_DATA_LEN = 112,
};

enum MS_Result {
	MS_OK = 0xFF, MS_FAIL = 0xFE, MS_ERR_TIMEOUT = 0xFD,
};

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
	void process(int incomingByte);

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
	int requestData();
	void requestSignature();

private:
	MSData data;

	unsigned long m_timeout;
	unsigned long m_ticksAtStart;
	MS_Result m_errorCode;
	void startOperation(unsigned long timeout);bool isOperationTimedOut() const;
	unsigned long operationDuration() const;
	int runCommand(byte cmd[], byte cmdLength, byte data[], byte dataLength);
};

#endif

