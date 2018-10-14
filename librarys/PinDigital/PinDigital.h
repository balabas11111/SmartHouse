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
#include <Loopable.h>
#include <PinEvent.h>

class PinDigital: public Loopable {

public:
	PinDigital(uint8_t pin);
	PinDigital(String _name,uint8_t _pin);
	PinDigital(String name,uint8_t pin,std::function<PinEvent(PinEvent)> funcEvent);
	PinDigital(String name,uint8_t pin,uint8_t _pinMode,std::function<PinEvent(PinEvent)> funcEvent,uint8_t _changeMode,uint8_t _pinVal);
	PinDigital(String name,uint8_t pin,uint8_t _pinMode,std::function<PinEvent(PinEvent)> funcEvent,uint8_t _changeMode,uint8_t _pinVal,uint8_t turnOffLevel);

	virtual ~PinDigital();

	String displayDetails();
	boolean loop();

	String getName();
	uint8_t getPin();
	boolean isChanged();
	boolean isVal(uint8_t _val);
	uint8_t getVal();
	boolean setVal(uint8_t);
	uint8_t change();
	uint8_t changeAndDelay(uint delayTime);
	uint8_t changeAndDelay(uint delayTime,uint _count);
	uint8_t turnOn();
	uint8_t turnOff();
	uint8_t turnOnOff(boolean turnOn);
	boolean isOn();
	void setTurnOffLevel(uint8_t turnOffLevel);
	void processInterrupt();
	boolean hasExternalFunction();

	uint8_t turnOffLevel;
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

	void init(String name,uint8_t pin,uint8_t _pinMode,uint8_t _pinVal,uint8_t _turnOffLevel);
	void initFunc(std::function<PinEvent(PinEvent)> externalFunction,uint8_t _buttonMode);

	void handleExternalFunction(String str);

	PinEvent constructEvent(String str);

	uint8_t getOpposite(uint8_t _val);

};



#endif /* LIBRARIES_PINEVENT_PINEVENT_H_ */
