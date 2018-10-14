/*
 * PinEvent.h
 *
 *  Created on: Oct 9, 2018
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_PINEVENT_PINANALOG_H_
#define LIBRARIES_PINEVENT_PINANALOG_H_

#include "Arduino.h"
#include <FunctionalInterrupt.h>
#include <Loopable.h>
#include <PinEvent.h>

class PinAnalog: public Loopable {

public:
	PinAnalog(uint8_t pin);
	PinAnalog(String _name,uint8_t _pin);
	PinAnalog(String name,uint8_t pin,std::function<PinEvent(PinEvent)> funcEvent);
	PinAnalog(String name,uint8_t pin,uint8_t _pinMode,std::function<PinEvent(PinEvent)> funcEvent,uint8_t _changeMode,uint8_t _pinVal);

	virtual ~PinAnalog();

	String displayDetails();
	boolean loop();

	String getName();
	uint8_t getPin();
	boolean isChanged();
	boolean isVal(uint8_t _val);
	uint16_t getCurrent();
	uint16_t getVal();
	boolean setVal(uint16_t);
	void processInterrupt();
	boolean hasExternalFunction();

	boolean checkIfMeasureCompleted();
	boolean processEvent(PinEvent event);
	PinEvent processEventNow(PinEvent event);
	boolean isDispatcherOfEvent(PinEvent event);
	boolean isTargetOfEvent(PinEvent event);

	PinEvent constructPinEventSetState(uint16_t val,String strVal,String dispatcherName);
	PinEvent constructPinEventSetState(PinEvent parentEvent);

private:
	String name;
	uint8_t pin;
	uint8_t pinInOutVal;
	uint16_t oldVal;
	uint16_t val;
	boolean changed;
	boolean dispatchState;
	std::function<PinEvent(PinEvent)> externalFunction;

	uint16_t warmUpTime;
	uint16_t measurePeriodTime;
	uint16_t maxDiffToDispatchEvent;
	ulong lastTime;

	uint16_t currentIteration;
	uint16_t currentTotal;

	void init(String name,uint8_t pin,uint8_t _pinMode,uint16_t _pinVal,uint8_t _changeMode,uint16_t warmUpTime,uint16_t measurePeriodTime);
	void initFunc(std::function<PinEvent(PinEvent)> externalFunction,uint8_t _changeMode);

	void handleExternalFunction(String str);

	PinEvent constructEvent(String str);

};



#endif /* LIBRARIES_PINEVENT_PINEVENT_H_ */
