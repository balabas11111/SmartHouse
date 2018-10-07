/*
 * ButtonB.h
 *
 *  Created on: 13 џэт. 2018 у.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_TIMETRIGGER_LEDB_H_
#define LIBRARIES_TIMETRIGGER_LEDB_H_

#define PIN_STATE_CHANGE_PREFFIX "PIN_SC"
#define PIN_CURRENT_STATE "PIN_STATE"
#define PIN_GET_STATE "PIN_GET_STATE"

#include "Arduino.h"
#include "AbstractEvent.h"
#include <FunctionalInterrupt.h>

class LedB{

public:
	LedB(uint8_t pin);
	LedB(String name,uint8_t pin,uint8_t state,std::function<String(AbstractEvent)> externalFunctionEvent);
	void init();
	void turnOn();
	void turnOff();
	void changeStateForDefaultInterval();
	void changeStateForInterval(int interval);
	void changeState();
	boolean getTurnedOn();
	uint8_t getPin();
	String getName();
	void changeState(uint8_t state);
	boolean processEvent(AbstractEvent event);
	boolean isDispatcherOfEvent(AbstractEvent event);

	void sendCurrentState();

private:
	uint8_t _pin;
	boolean turnedOn;
	String name;

	std::function<String(AbstractEvent)> externalFunctionEvent;
};


#endif /* LIBRARIES_TIMETRIGGER_BUTTONB_H_ */
