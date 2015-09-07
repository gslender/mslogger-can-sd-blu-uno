#include "MegaSquirt.h"

#define offset 3 // new serial protocol is offset by 3

// Constructor
MegaSquirt::MegaSquirt() :
		m_timeout(MS_DEFAULT_TIMEOUT), m_ticksAtStart(millis()), m_errorCode(
				MS_OK) {
}

void MegaSquirt::process(int incomingByte) {
	if (incomingByte < 0)
		return;

	// check timeout
	if (isOperationTimedOut())
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
// { 'a', 0x0, 0x6 }  sends 112 bytes from the outpc
int MegaSquirt::requestData() {
//	return runCommand((byte*) (const byte[] ) { 0x0, 0x1, 0x41, 0xD3, 0xD9,
//					0x9E, 0x8B }, 7, data, 170);
//        return runCommand((byte*)(const byte []){0x41},1,data,170);

}

void MegaSquirt::requestSignature() {
//	runCommand((byte*) (const byte[] ) { 'S' }, 1, data, 59);
}

//private
void MegaSquirt::startOperation(unsigned long timeout) {
	m_ticksAtStart = millis();
	m_timeout = timeout;
	m_errorCode = MS_OK;
}

bool MegaSquirt::isOperationTimedOut() const {
	return operationDuration() >= m_timeout;
}

unsigned long MegaSquirt::operationDuration() const {
	unsigned long current_ticks = millis(), elapsed_ticks;

	if (current_ticks >= m_ticksAtStart)
		elapsed_ticks = current_ticks - m_ticksAtStart;
	else
		elapsed_ticks = (ULONG_MAX - m_ticksAtStart) + current_ticks;

	return elapsed_ticks;
}

int MegaSquirt::runCommand(byte cmd[], byte cmdLength, byte data[],
		byte dataLength) {
	// Flush any leftover data from the last command.
	// Read any leftover dat on the line.
	Serial.flush();
	while (Serial.available())
		Serial.read();

	// Send the specified command to the controller.
	byte i;
	for (i = 0; i < cmdLength; i++) {
		Serial.write(cmd[i]);
	}

	unsigned long timeOut;
	int counter;

	// The Serial buffer is 64 bytes, this is less than the largest payload
	// from the controller, as such don't wait, start reading the data right
	// away and don't stop until either the requested number of bytes has
	// been read or the timeout is reached.
	//
	counter = 0;
	timeOut = millis() + 300; // MS WAIT TIMEOUT
	while (counter < dataLength && millis() < timeOut) {
		while (counter < dataLength && Serial.available()) {
			data[counter] = Serial.read();
			++counter;
		}
	}

	// If there is still data pending to be read, raise OVERFLOW error.
	if (Serial.available() > 0 && counter >= dataLength) {
		D(debugSerial->println(F("MS_ERR_COMM_OVERFLOW"));)
		return counter; //MS_ERR_COMM_OVERFLOW
	}

	// If the wrong amount of data was read, return timeout.
	if (counter != dataLength) {
		D(debugSerial->println(F("MS_ERR_COMM_TIMEOUT"));)
		return counter; //MS_ERR_COMM_TIMEOUT
	}

	// Otherwise return success.
	return 1; //MS_COMM_SUCCESS
}
