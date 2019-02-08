/*
 * PinEvent.h
 *
 *  Created on: Oct 9, 2018
 *      Author: Vitaliy_Vlasov
 */

#ifndef LIBRARIES_PINEVENT_PINDIGITALNOINT_H_
#define LIBRARIES_PINEVENT_PINDIGITALNOINT_H_

#include "Arduino.h"
#include "TimeTrigger.h"
#include <FunctionalInterrupt.h>
#include <AbstractSensor.h>
#include <Loopable.h>
#include "PinDigital.h"

const PROGMEM char PinDigitalNoInt_name[]="PinDigital";
const PROGMEM char PinDigitalNoInt_highLow[]="HIGH/LOW";

class PinDigitalNoInt:public PinDigital {

public:

	PinDigitalNoInt(String _name,uint8_t _pin,std::function<void(void)> _onChanged)
		:PinDigital(_name,_pin,_onChanged,INPUT,254,LOW,LOW,0,FPSTR(PinDigitalNoInt_name),_name,FPSTR(PinDigitalNoInt_highLow)){

		//construct(_pin, onChanged, INPUT, NULL, LOW, LOW,0);
	}

	PinDigitalNoInt(String _name,uint8_t _pin,std::function<void(void)> _onChanged,long clickMaxTime)
		:PinDigital(_name,_pin,_onChanged,INPUT,254,LOW,LOW,clickMaxTime,FPSTR(PinDigitalNoInt_name),_name,FPSTR(PinDigitalNoInt_highLow)){
		//construct(_pin, onChanged, INPUT, NULL, LOW, LOW,clickMaxTime);
	}

	PinDigitalNoInt(String _name,uint8_t _pin,std::function<void(void)> _onChanged,uint8_t _pinMode,uint8_t _changeMode,uint8_t _pinVal,uint8_t _turnOffLevel)
	:PinDigital(_name,_pin,_onChanged,INPUT,254,_pinVal,_turnOffLevel,0,FPSTR(PinDigitalNoInt_name),_name,FPSTR(PinDigitalNoInt_highLow)){
		//construct(_pin, _onChanged, _pinMode, NULL, _pinVal, _turnOffLevel,0);
	}

	PinDigitalNoInt(String _name,uint8_t _pin,std::function<void(void)> _onChanged,uint8_t _pinMode,uint8_t _changeMode,uint8_t _pinVal,uint8_t _turnOffLevel,long clickMaxTime)
	:PinDigital(_name,_pin,_onChanged,_pinMode,254,_pinVal,_turnOffLevel,clickMaxTime,FPSTR(PinDigitalNoInt_name),_name,FPSTR(PinDigitalNoInt_highLow)){
		//construct(_pin, _onChanged, _pinMode, NULL, _pinVal, _turnOffLevel,clickMaxTime);
	}

	PinDigitalNoInt(String _name,uint8_t _pin,std::function<void(void)> _onChanged,uint8_t _pinMode,uint8_t _changeMode,uint8_t _pinVal,uint8_t _turnOffLevel,long clickMaxTime,String abstractItemName,String abstractItemSize,String abstractItemDescr)
	:PinDigital(_name,_pin,_onChanged,_pinMode,254,_pinVal,_turnOffLevel,clickMaxTime,abstractItemName,abstractItemSize,abstractItemDescr){
		//construct(_pin, _onChanged, _pinMode, NULL, _pinVal, _turnOffLevel,clickMaxTime);
	}

	virtual ~PinDigitalNoInt(){};

	//AbstractSensor functions
	virtual boolean loop() override{
		return updateVal();
	}

};



#endif /* LIBRARIES_PINEVENT_PINEVENT_H_ */
