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
#include <PinAbstract.h>
#include <PinEvent.h>
#include <PinEventProcessor.h>

class PinDigital: public Loopable, public PinEventProcessor, public PinAbstract {

public:
	PinDigital(uint8_t pin);
	PinDigital(String _name,uint8_t _pin);
	PinDigital(String name,uint8_t pin,std::function<PinEvent(PinEvent)> funcEvent);
	PinDigital(String name,uint8_t pin,std::function<PinEvent(PinEvent)> funcEvent,std::function<PinEvent(PinEvent)> _isDispatcherPostFunction);
	PinDigital(String name,uint8_t pin,uint8_t _pinMode,std::function<PinEvent(PinEvent)> funcEvent,uint8_t _changeMode,uint8_t _pinVal);
	PinDigital(String name,uint8_t pin,uint8_t _pinMode,std::function<PinEvent(PinEvent)> funcEvent,uint8_t _changeMode,uint8_t _pinVal,std::function<PinEvent(PinEvent)> _isDispatcherPostFunction);
	PinDigital(String name,uint8_t pin,uint8_t _pinMode,std::function<PinEvent(PinEvent)> funcEvent,uint8_t _changeMode,uint8_t _pinVal,uint8_t turnOffLevel);
	PinDigital(String name,uint8_t pin,uint8_t _pinMode,std::function<PinEvent(PinEvent)> funcEvent,uint8_t _changeMode,uint8_t _pinVal,uint8_t turnOffLevel,std::function<PinEvent(PinEvent)> _isDispatcherPostFunction);

	~PinDigital();

	uint8_t change();
	uint8_t changeAndDelay(uint delayTime);
	uint8_t changeAndDelay(uint delayTime,uint _count);
	uint8_t turnOn();
	uint8_t turnOff();
	uint8_t turnOnOff(boolean turnOn);
	boolean isOn();
	void setTurnOffLevel(uint8_t turnOffLevel);

	PinEvent constructPinEventSetState(uint8_t val,String strVal,String dispatcherName);
	PinEvent constructPinEventSetState(PinEvent parentEvent);

	String getName(){
		return PinAbstract::getName();
	}
	//Loopable
	String displayDetails() override;
	boolean loop() override;

	//PinAbstract functions
	uint16_t getVal() override;
	boolean setVal(uint16_t _val) override;
	void processInterrupt() override;

	//PinEventProcessor
	boolean processEvent(PinEvent event) override;
	PinEvent processEventNow(PinEvent event) override;
	boolean isDispatcherOfEvent(PinEvent event) override;
	boolean isTargetOfEvent(PinEvent event) override;

	PinEvent constructEvent(String str,boolean bubble) override;

private:

	void construct(String name,uint8_t pin,uint8_t _pinMode,uint8_t _pinVal,uint8_t _turnOffLevel);
	void initFunc(std::function<PinEvent(PinEvent)> externalFunction,uint8_t _buttonMode,std::function<PinEvent(PinEvent)> _isDispatcherPostFunction);

	uint8_t getOpposite(uint8_t _val);

protected:
	//void handleExternalFunction(String str);
	uint8_t turnOffLevel;
};



#endif /* LIBRARIES_PINEVENT_PINEVENT_H_ */
