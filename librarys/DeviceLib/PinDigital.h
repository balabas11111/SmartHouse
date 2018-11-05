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
#include <AbstractItem.h>
#include <Loopable.h>

class PinDigital:  public AbstractItem, public Loopable {

public:

	PinDigital(String name,uint8_t pin,std::function<void(void)> onChanged)
		:AbstractItem(pin,name,"PinDigital","HIGH/LOW",name,0, 1,-2,2){
		construct(pin, onChanged, INPUT, CHANGE, LOW, LOW);
	}


	PinDigital(String _name,uint8_t _pin,std::function<void(void)> _onChanged,uint8_t _pinMode,uint8_t _changeMode,uint8_t _pinVal,uint8_t _turnOffLevel)
		:AbstractItem(_pin,_name,"PinDigital","HIGH/LOW","",_pinVal, 1,-2,2){
		construct(_pin, _onChanged, _pinMode, _changeMode, _pinVal, _turnOffLevel);
	}

	~PinDigital();

	uint8_t change();
	uint8_t changeAndDelay(uint delayTime);
	uint8_t changeAndDelay(uint delayTime,uint _count);
	uint8_t turnOn();
	uint8_t turnOff();
	uint8_t turnOnOff(boolean turnOn);
	boolean isOn();
	void setTurnOffLevel(uint8_t turnOffLevel);

	//AbstractItem functions
	virtual boolean loop() override{
		return handleLoop();
	}
	virtual String displayDetails(){
		return getJson();
	}

	//PinAbstract functions
	uint16_t getVal();
	bool setVal(uint16_t _val);
	void processInterrupt();

private:

	void construct(uint8_t pin,std::function<void(void)> onChanged,uint8_t pinMode,uint8_t changeMode,uint8_t pinVal,uint8_t turnOffLevel);
	uint8_t getOpposite(uint8_t _val);

	//PinAbstract functions
	uint8_t oldVal;
	bool changed;
	bool dispatchState;

protected:
	uint8_t pin;
	std::function<void(void)> onChanged;
	uint8_t pinModeInOut;
	uint8_t changeMode;
	uint8_t pinVal;
	uint8_t turnOffLevel;

	boolean handleLoop();
};



#endif /* LIBRARIES_PINEVENT_PINEVENT_H_ */
