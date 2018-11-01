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

class PinDigital:  public AbstractItem {

public:
	PinDigital(uint8_t id,String name,uint8_t pin,std::function<void(void)> onChanged,uint8_t fieldId,String queueName)
		:AbstractItem(id,name,"PinDigital","HIGH/LOW","",0, 0,fieldId,queueName){
		construct(pin, onChanged, INPUT, CHANGE, pinVal, turnOffLevel);
	}

	PinDigital(uint8_t id,String name,uint8_t pin,std::function<void(void)> onChanged,uint8_t pinMode,uint8_t changeMode,uint8_t pinVal,uint8_t turnOffLevel,uint8_t fieldId,String queueName)
		:AbstractItem(id,name,"PinDigital","HIGH/LOW","",pinVal, 0,fieldId,queueName){
		construct(pin, onChanged, pinMode, changeMode, pinVal, turnOffLevel);
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
	virtual bool loop() override{
		bool result=false;
			#ifdef DISPLAY_LOOPS
				Serial.println("Digital pin loop ");
			#endif

			if(changed){
				if(onChanged!=nullptr){
					onChanged();
				}

				changed=false;
				result=true;
			}
			if(dispatchState){
				if(onChanged!=nullptr){
					onChanged();
				}
				dispatchState=false;
				result=true;
			}

			return result;
	}
	virtual void init() override{
		initialized=true;
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
};



#endif /* LIBRARIES_PINEVENT_PINEVENT_H_ */
