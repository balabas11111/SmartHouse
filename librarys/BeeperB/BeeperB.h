/*
 * ButtonB.h
 *
 *  Created on: 13 ���. 2018 �.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_TIMETRIGGER_BEEPERB_H_
#define LIBRARIES_TIMETRIGGER_BEEPERB_H_

#include "Arduino.h"

class BeeperB{

public:
	BeeperB(uint8_t pin,uint8_t NO_PLAY_FLAG,uint8_t PLAY_FLAG,boolean sendNoPlayFLag);
	void init();
	void shortBeep();
	void longBeep();
	void play(unsigned int frequency, unsigned long duration);
	void noPlay();
	void doBeep(unsigned long duration);

	void playGenerator();
	void noPlayGenerator();

private:
	uint8_t _pin;
	boolean useWriteMethodInsteadPlay;
	uint8_t _NO_PLAY_FLAG;
	uint8_t _PLAY_FLAG;
};


#endif /* LIBRARIES_TIMETRIGGER_BUTTONB_H_ */
