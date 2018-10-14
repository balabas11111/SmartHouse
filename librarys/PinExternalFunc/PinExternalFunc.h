/*
 * PinEvent.h
 *
 *  Created on: Oct 9, 2018
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_PINEVENT_PINEXTERNALFUNC_H_
#define LIBRARIES_PINEVENT_PINEXTERNALFUNC_ 1

#include "Arduino.h"
#include <FunctionalInterrupt.h>
#include <Loopable.h>
#include <PinEvent.h>

template <class T>
class PinExternalFunc: public Loopable {

public:
	PinExternalFunc<T>(String name,uint8_t pin,std::function<void(void)> initFunc,std::function<PinEvent(PinEvent)> funcEvent,uint16_t timeInterval,std::function<T(void)> getValFunc);

	virtual ~PinExternalFunc<T>();

	String displayDetails();
	boolean loop();

	String getName();
	uint8_t getPin();
	boolean isChanged();
	boolean isVal(T _val);
	T getVal();
	void processInterrupt();
	boolean hasExternalFunction();

	boolean processEvent(PinEvent event);
	PinEvent processEventNow(PinEvent event);
	boolean isDispatcherOfEvent(PinEvent event);
	boolean isTargetOfEvent(PinEvent event);

	PinEvent constructPinEventSetState(T val,String strVal,String dispatcherName);
	PinEvent constructPinEventSetState(PinEvent parentEvent);

private:
	String name;
	uint8_t pin;
	T oldVal;
	T val;
	boolean changed;
	boolean dispatchState;
	uint16_t interval;
	long lastTime;
	std::function<T(void)> getValFunc;
	std::function<PinEvent(PinEvent)> externalFunction;

	void init(String name,uint8_t pin,std::function<void(void)> initFunc,std::function<T(void)> getValFunc,std::function<PinEvent(PinEvent)> funcEvent);

	void handleExternalFunction(String str);

	PinEvent constructEvent(String str);

};



#endif /* LIBRARIES_PINEVENT_PINEVENT_H_ */
