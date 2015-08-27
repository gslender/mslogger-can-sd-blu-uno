/**
 * A simple DEBUG wrapper to simply add/remove debug code in an app.
 *
	#define DEBUG // comment out to en/disable
	#include "debug.h"
	D(Serial debugSerial;)

	#define DEBUG // comment out to en/disable
	#define DEBUG_USE_SOFTSERIAL
	#include "debug.h"
	D(SoftwareSerial debugSerial;)

    // in setup
    D(debugSerial = SoftwareSerial(2, 3);)
    D(debugSerial.begin(9600);)
 */


//#define DEBUG // comment out this line to disable
#ifdef DEBUG
	#ifdef DEBUG_USE_SOFTSERIAL
		#include <SoftwareSerial.h>
		extern SoftwareSerial debugSerial;
	#else
		#define debugSerial Serial
	#endif
	//// the following must be in an impl file  D(SoftwareSerial* debugSerial;)
	#define D(x) x
#else
	#define D(x)
#endif
