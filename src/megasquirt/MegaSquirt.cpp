#include "MegaSquirt.h"

// Constructor
MegaSquirt::MegaSquirt() {
}

MSVars MegaSquirt::getData() {
	return ms_variables;
}

void MegaSquirt::process(unsigned long id, unsigned char buf[]) {
	if (id < 0)
		return;

	switch (id) { // ID's 1520+ are Megasquirt CAN broadcast frames

	// typical compiler optimizations will convert memcpy to multiple LDM/STM instructions
	case 1512:
		//ms_variables.RPM = (uint16_t) (word(buf[2], buf[3]));
		memcpy(ms_variables.bytes1512, buf, 8);
		break;
	case 1513:
		memcpy(ms_variables.bytes1513, buf, 8);
		break;
	case 1514:
		memcpy(ms_variables.bytes1514, buf, 8);
		break;
	case 1515:
		memcpy(ms_variables.bytes1515, buf, 8);
		break;
	case 1516:
		memcpy(ms_variables.bytes1516, buf, 8);
		break;
	default: // not a broadcast packet

		Serial.write("ID: ");
		Serial.println(id);

	}
}

