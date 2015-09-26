#include "MegaSquirt.h"

// Constructor
MegaSquirt::MegaSquirt() {
}

inline void cpyFast(uint8_t *from,uint8_t *to) {
	to[0] = from[1];
	to[1] = from[0];
	to[2] = from[3];
	to[3] = from[2];
	to[4] = from[5];
	to[5] = from[4];
	to[6] = from[7];
	to[7] = from[6];
}

MSVars MegaSquirt::getData() {
	return ms_variables;
}

void MegaSquirt::zeroData() {
	byte buf[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
	cpyFast(buf, ms_variables.bytes1512);
	cpyFast(buf, ms_variables.bytes1513);
	cpyFast(buf, ms_variables.bytes1514);
	cpyFast(buf, ms_variables.bytes1515);
	cpyFast(buf, ms_variables.bytes1516);
}

bool MegaSquirt::process(unsigned long id, unsigned char buf[]) {
	switch (id) {
	case 1512:
		cpyFast(buf, ms_variables.bytes1512);
		return true;
		break;
	case 1513:
		cpyFast(buf, ms_variables.bytes1513);
		break;
	case 1514:
		cpyFast(buf, ms_variables.bytes1514);
		break;
	case 1515:
		cpyFast(buf, ms_variables.bytes1515);
		break;
	case 1516:
		cpyFast(buf, ms_variables.bytes1516);
		break;
	}
	return false;
}

