/**
 * A simple DEBUG wrapper to simply add/remove debug code in an app.
 *
	#define DEBUG // comment out to en/disable
	#include "debug.h"

	#define DEBUG // comment out to en/disable
	#define DEBUG_USE_SOFTSERIAL
	#include "debug.h"
	D(SoftwareSerial debugSerial;)

    // in setup
    D(debugSerial = SoftwareSerial(2, 3);)
    D(debugSerial.begin(9600);)
 */

#ifdef DEBUG_USE_SOFTSERIAL
	//// the following must be in an impl file  D(SoftwareSerial* debugSerial;)
	#include <SoftwareSerial.h>
	extern SoftwareSerial debugSerial;
#else
	#define debugSerial Serial
#endif
//#define ENABLE_DEBUG // comment out this line to disable

#ifdef ENABLE_DEBUG
	#define D(x) x
	#define UN_D(y)
#else
	#define D(x)
	#define UN_D(y) y
#endif
