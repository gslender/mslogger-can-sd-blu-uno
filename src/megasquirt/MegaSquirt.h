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
#include <limits.h>

typedef struct {
	// 1512
	union {
		uint8_t bytes1512[8];
		struct {
			int16_t MAP; // * 0.1
			uint16_t RPM;
			int16_t CLT; // * 0.1
			int16_t TPS; // * 0.1
		};
	};
	// 1513
	union {
		uint8_t bytes1513[8];
		struct {
			uint16_t PW1; // * 0.001
			uint16_t PW2; // * 0.001
			int16_t MAT; // * 0.1
			int16_t ADV_DEG; // * 0.1
		};
	};

	// 1514
	union {
		uint8_t bytes1514[8];
		struct {
			uint8_t AFRTGT1; // * 0.1
			uint8_t AFR1; // * 0.1
			int16_t EGOCOR1; // * 0.1
			int16_t EGT1; // * 0.1
			int16_t PWSEQ1; // * 0.1
		};
	};

	// 1515
	union {
		uint8_t bytes1515[8];
		struct {
			int16_t BATT; // * 0.1
			int16_t SENSORS1; // * 0.1
			int16_t SENSORS2; // * 0.1
			int16_t KNK_RTD; // * 0.1
		};
	};

	// 1516
	union {
		uint8_t bytes1516[8];
		struct {
			uint16_t VSS; // * 0.1
			int16_t TC_RTD; // * 0.1
			int16_t LAUNCH_TIMING; // * 0.1
			uint16_t unused;
		};
	};
} MSVars;

class MegaSquirt {

public:
	MegaSquirt();
	void process(unsigned long id, unsigned char buf[]);
	MSVars getData();

private:
	MSVars ms_variables;
};

#endif

