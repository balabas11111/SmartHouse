/*
 * PinEvent.h
 *
 *  Created on: Oct 9, 2018
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_PINEVENT_PINDIGITAL_H_
#define LIBRARIES_PINEVENT_PINDIGITAL_H_

#include "Arduino.h"
#include <FunctionalInterrupt.h>
#include <PinEvent.h>

class PinDigital{

public:
	PinDigital(uint8_t pin);
	PinDigital(String _name,uint8_t _pin);
	PinDigital(String name,uint8_t pin,std::function<PinEvent(PinEvent)> funcEvent);
	PinDigital(String name,uint8_t pin,uint8_t _pinMode,std::function<PinEvent(PinEvent)> funcEvent,uint8_t _changeMode,uint8_t _pinVal);

	boolean loop();

	uint8_t getPin();
	boolean isChanged();
	boolean isVal(uint8_t _val);
	uint8_t getVal();
	boolean setVal(uint8_t);
	void processInterrupt();
	boolean hasExternalFunction();

	boolean processEvent(PinEvent event);
	PinEvent processEventNow(PinEvent event);
	boolean isDispatcherOfEvent(PinEvent event);
	boolean isTargetOfEvent(PinEvent event);

	PinEvent constructPinEventSetState(uint8_t val,String strVal,String dispatcherName);
	PinEvent constructPinEventSetState(PinEvent parentEvent);

private:
	String name;
	uint8_t pin;
	uint8_t pinInOutVal;
	uint8_t oldVal;
	uint8_t val;
	boolean changed;
	boolean dispatchState;
	std::function<PinEvent(PinEvent)> externalFunction;

	void init(String name,uint8_t pin,uint8_t _pinMode,uint8_t _pinVal);
	void initFunc(std::function<PinEvent(PinEvent)> externalFunction,uint8_t _buttonMode);

	void handleExternalFunction(String str);

	PinEvent constructEvent(String str);

};



#endif /* LIBRARIES_PINEVENT_PINEVENT_H_ */
