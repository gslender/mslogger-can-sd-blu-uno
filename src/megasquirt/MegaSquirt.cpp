#include "MegaSquirt.h"

// Constructor
MegaSquirt::MegaSquirt() {
}

void MegaSquirt::process(unsigned long id, unsigned char len, unsigned char buf[]) {
	if (id < 0)
		return;

}

byte MegaSquirt::getEngine() {
	return data.engine;
}

unsigned int MegaSquirt::getRpm() {
	return data.rpm;
}

float MegaSquirt::getMap() {
	return data.map * 0.1f;
}

float MegaSquirt::getMat(byte celsius) {
	if (celsius) {
		return (data.mat - 320)* 0.05555f;
	}
	return data.mat;
}

float MegaSquirt::getClt(byte celsius) {
	if (celsius) {
		return (data.clt - 320)* 0.05555f;
	}
	return data.clt;
}

float MegaSquirt::getTps() {
	return data.tps * 0.1f;
}

float MegaSquirt::getAfr() {
	return data.afr1 * 0.1f;
}

float MegaSquirt::getTaf() {
	return data.afrtgt1 * 0.1f;
}

float MegaSquirt::getSpk() {
	return data.advance * 0.1f;
}

float MegaSquirt::getPwp() {
	return data.pw1 * 0.000666f;
}

