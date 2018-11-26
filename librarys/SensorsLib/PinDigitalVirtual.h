/*
 * PinDigitalVirtual.h
 *
 *  Created on: 25 нояб. 2018 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_SENSORSLIB_PINDIGITALVIRTUAL_H_
#define LIBRARIES_SENSORSLIB_PINDIGITALVIRTUAL_H_

#include <PinDigital.h>

#define VIRTUAL_DIGITAL_PIN 255

const PROGMEM char PinDigitalVirtual_name[]="PinDigitalVirtual";
const PROGMEM char PinDigitalVirtual_highLow[]="HIGH/LOW";

class PinDigitalVirtual: public PinDigital {
public:

	PinDigitalVirtual(String name)
		:PinDigital(name,VIRTUAL_DIGITAL_PIN,nullptr,OUTPUT,CHANGE,LOW,LOW,0,FPSTR(PinDigitalVirtual_name),FPSTR(PinDigitalVirtual_highLow),FPSTR(PinDigitalVirtual_name)){
	}
	PinDigitalVirtual(String name,std::function<void(void)> onChanged)
		:PinDigital(name,VIRTUAL_DIGITAL_PIN,onChanged,INPUT,CHANGE,LOW,LOW,0,FPSTR(PinDigitalVirtual_name),FPSTR(PinDigitalVirtual_highLow),FPSTR(PinDigitalVirtual_name)){
	}

	PinDigitalVirtual(String name,std::function<void(void)> onChanged,uint8_t pinModeInOut,uint8_t changeMode,uint8_t pinVal,uint8_t turnOffLevel)
		:PinDigital(name,VIRTUAL_DIGITAL_PIN,onChanged,pinModeInOut,changeMode,pinVal,turnOffLevel,0,FPSTR(PinDigitalVirtual_name),FPSTR(PinDigitalVirtual_highLow),FPSTR(PinDigitalVirtual_name)){
	}

	PinDigitalVirtual(String name,std::function<void(void)> onChanged,uint8_t pinModeInOut,uint8_t changeMode,uint8_t pinVal,uint8_t turnOffLevel,long clickMaxTime,String abstractItemName,String abstractItemSize,String abstractItemDescr)
		:PinDigital(name,VIRTUAL_DIGITAL_PIN,onChanged,pinModeInOut,changeMode,pinVal,turnOffLevel,clickMaxTime,abstractItemName,abstractItemSize,abstractItemDescr){
	}
	virtual ~PinDigitalVirtual(){};

	virtual void setupModes(uint8_t pinVal) override{
		if(pinModeInOut==OUTPUT){
			setVal(pinVal);
		}
	}

	virtual void initialize(uint8_t virtualPin,std::function<int(uint8_t)> externalGet,std::function<void(uint8_t,uint8_t)> externalSet){
		if(!initialized){
			Serial.print(FPSTR(MESSAGE_VIRTUAL_PIN_START_INIT));
			Serial.print(name);
			Serial.print(FPSTR(MESSAGE_VIRTUAL_PIN_ID));
			Serial.print(virtualPin);
			Serial.print(FPSTR(MESSAGE_VIRTUAL_PIN_Mode));
			Serial.print(pinModeInOut);

			this->pin=virtualPin;
			this->externalGet=externalGet;
			this->externalSet=externalSet;

			if(pinModeInOut==OUTPUT){
				uint8_t expValue=items[0].val==1?(getOpposite(turnOffLevel)):turnOffLevel;

				setVal(expValue);
				oldVal=expValue;
			}else{
				oldVal=getVal();
			}

			initialized=externalGet!=nullptr && externalSet!=nullptr;
		}
		if(initialized)
			Serial.println(FPSTR(MESSAGE_DONE));
	}

	virtual int getVal() override{
		uint8_t val=externalGet(pin);
		items[0].val=turnOffLevel!=val;
		return val;
	}

	bool setVal(uint8_t _val) override{
		if(pinModeInOut==OUTPUT){
			if(externalSet!=nullptr){
				externalSet(pin,_val);
			}
			items[0].val=turnOffLevel!=_val;
			return true;
		}
		return false;
	}

private:
	bool initialized=false;
	std::function<int(uint8_t)> externalGet;
	std::function<void(uint8_t,uint8_t)> externalSet;

};

#endif /* LIBRARIES_SENSORSLIB_PINDIGITALVIRTUAL_H_ */
