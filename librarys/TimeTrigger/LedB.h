/*
 * ButtonB.h
 *
 *  Created on: 13 ���. 2018 �.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_TIMETRIGGER_LEDB_H_
#define LIBRARIES_TIMETRIGGER_LEDB_H_

#define PIN_STATE_CHANGE_PREFFIX "PIN_SC"

#include "Arduino.h"
#include "AbstractEvent.h"

class LedB{

public:
	LedB(uint8_t pin);
	LedB(uint8_t pin,uint8_t state);
	void init();
	void turnOn();
	void turnOff();
	void changeStateForDefaultInterval();
	void changeStateForInterval(int interval);
	void changeState();
	boolean getTurnedOn();
	uint8_t getPin();
	void changeState(uint8_t state);
	boolean processEvent(AbstractEvent event);
	boolean isDispatcherOfEvent(AbstractEvent event);


private:
	uint8_t _pin;
	boolean turnedOn;
};


#endif /* LIBRARIES_TIMETRIGGER_BUTTONB_H_ */
