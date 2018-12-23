/*
 * PinEvent.h
 *
 *  Created on: Oct 9, 2018
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_PINEVENT_PINDIGITAL_H_
#define LIBRARIES_PINEVENT_PINDIGITAL_H_

#include "Arduino.h"
#include "TimeTrigger.h"
#include <FunctionalInterrupt.h>
#include <AbstractItem.h>
#include <Loopable.h>

const PROGMEM char PinDigital_name[]="PinDigital";
const PROGMEM char PinDigital_highLow[]="HIGH/LOW";

class PinDigital:  public AbstractItem, public Loopable {

public:

	PinDigital(String _name,uint8_t _pin)
		:AbstractItem(_pin,_name,FPSTR(PinDigital_name),FPSTR(PinDigital_highLow),_name,1){
		construct(_pin, nullptr, OUTPUT, CHANGE, HIGH, HIGH,0);
	}

	PinDigital(String _name,uint8_t _pin, uint8_t pinVal,uint8_t turnOffLevel)
			:AbstractItem(_pin,_name,FPSTR(PinDigital_name),FPSTR(PinDigital_highLow),_name,1){
			construct(_pin, nullptr, OUTPUT, CHANGE, pinVal, turnOffLevel,0);
		}

	PinDigital(String _name,uint8_t _pin,std::function<void(void)> onChanged)
		:AbstractItem(_pin,_name,FPSTR(PinDigital_name),FPSTR(PinDigital_highLow),_name,1){
		construct(_pin, onChanged, INPUT, CHANGE, LOW, LOW,0);
	}

	PinDigital(String _name,uint8_t _pin,std::function<void(void)> onChanged,long clickMaxTime)
		:AbstractItem(_pin,_name,FPSTR(PinDigital_name),FPSTR(PinDigital_highLow),_name,1){
		construct(_pin, onChanged, INPUT, CHANGE, LOW, LOW,clickMaxTime);
	}
/*
 * String _name,uint8_t _pin,std::function<void(void)> _onChanged,uint8_t _pinMode,uint8_t _changeMode,uint8_t _pinVal,uint8_t _turnOffLevel
 */
	PinDigital(String _name,uint8_t _pin,std::function<void(void)> _onChanged,uint8_t _pinMode,uint8_t _changeMode,uint8_t _pinVal,uint8_t _turnOffLevel)
		:AbstractItem(_pin,_name,FPSTR(PinDigital_name),FPSTR(PinDigital_highLow),_name,1){
		construct(_pin, _onChanged, _pinMode, _changeMode, _pinVal, _turnOffLevel,0);
	}

	PinDigital(String _name,uint8_t _pin,std::function<void(void)> _onChanged,uint8_t _pinMode,uint8_t _changeMode,uint8_t _pinVal,uint8_t _turnOffLevel,long clickMaxTime)
		:AbstractItem(_pin,_name,FPSTR(PinDigital_name),FPSTR(PinDigital_highLow),_name,1){
		construct(_pin, _onChanged, _pinMode, _changeMode, _pinVal, _turnOffLevel,clickMaxTime);
	}

	PinDigital(String _name,uint8_t _pin,std::function<void(void)> _onChanged,uint8_t _pinMode,uint8_t _changeMode,uint8_t _pinVal,uint8_t _turnOffLevel,long clickMaxTime,String abstractItemName,String abstractItemSize,String abstractItemDescr)
		:AbstractItem(_pin,_name,abstractItemName,abstractItemSize,abstractItemDescr,1){
		construct(_pin, _onChanged, _pinMode, _changeMode, _pinVal, _turnOffLevel,clickMaxTime);
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

	virtual boolean processMqVal(uint8_t index,String value){
		if(index==0){
			Serial.print(FPSTR("name="));
			Serial.print(name);
			Serial.print(FPSTR(" Val='"));
			Serial.print(value);
			Serial.print(FPSTR("';"));
			setVal(value.toInt());
			Serial.println(FPSTR("...done"));
			return true;
		}

		return false;
	}


	virtual int getVal(){
		return digitalRead(pin);
	}
	virtual bool setVal(uint8_t _val);

	virtual int getValAnalog(){
		return analogRead(pin);
	}

	//PinAbstract functions
	void processClick(boolean fromTimer);
	void processInterrupt();

	virtual uint8_t getPin();
	uint8_t getChangeMode();
	uint8_t getPinModeInOut();

	virtual uint8_t updateVal();

	boolean isInterruptAttached(){
		return interruptAttached;
	}
private:

	bool changed;
	bool dispatchState;

protected:
	uint8_t pin;
	uint8_t oldVal;
	std::function<void(void)> onChanged;
	uint8_t pinModeInOut;
	uint8_t changeMode;
	//uint8_t pinVal;
	uint8_t turnOffLevel;
	boolean isClick;
	boolean interruptAttached;

	TimeTrigger* clickTrigger;

	boolean handleLoop();
	virtual void setupModes(uint8_t pinVal);

	uint8_t getOpposite(uint8_t _val);

	long lastInterrupt;

	void construct(uint8_t pin,std::function<void(void)> onChanged,uint8_t pinMode,uint8_t changeMode,uint8_t pinVal,uint8_t turnOffLevel,long clickMaxTime);

};



#endif /* LIBRARIES_PINEVENT_PINEVENT_H_ */
