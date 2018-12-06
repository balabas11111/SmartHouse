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
#include "PinDigital.h"

const PROGMEM char PinDigital_name[]="PinDigital";
const PROGMEM char PinDigital_highLow[]="HIGH/LOW";

class PinDigitalNoInt:  public PinDigital {

public:

	PinDigitalNoInt(String _name,uint8_t _pin,std::function<void(void)> onChanged)
		:AbstractItem(_pin,_name,FPSTR(PinDigital_name),FPSTR(PinDigital_highLow),_name,1){
		construct(_pin, onChanged, INPUT, NULL, LOW, LOW,0);
	}

	PinDigitalNoInt(String _name,uint8_t _pin,std::function<void(void)> onChanged,long clickMaxTime)
		:AbstractItem(_pin,_name,FPSTR(PinDigital_name),FPSTR(PinDigital_highLow),_name,1){
		construct(_pin, onChanged, INPUT, NULL, LOW, LOW,clickMaxTime);
	}

	PinDigitalNoInt(String _name,uint8_t _pin,std::function<void(void)> _onChanged,uint8_t _pinMode,uint8_t _changeMode,uint8_t _pinVal,uint8_t _turnOffLevel)
		:AbstractItem(_pin,_name,FPSTR(PinDigital_name),FPSTR(PinDigital_highLow),_name,1){
		construct(_pin, _onChanged, _pinMode, NULL, _pinVal, _turnOffLevel,0);
	}

	PinDigitalNoInt(String _name,uint8_t _pin,std::function<void(void)> _onChanged,uint8_t _pinMode,uint8_t _changeMode,uint8_t _pinVal,uint8_t _turnOffLevel,long clickMaxTime)
		:AbstractItem(_pin,_name,FPSTR(PinDigital_name),FPSTR(PinDigital_highLow),_name,1){
		construct(_pin, _onChanged, _pinMode, NULL, _pinVal, _turnOffLevel,clickMaxTime);
	}

	PinDigitalNoInt(String _name,uint8_t _pin,std::function<void(void)> _onChanged,uint8_t _pinMode,uint8_t _changeMode,uint8_t _pinVal,uint8_t _turnOffLevel,long clickMaxTime,String abstractItemName,String abstractItemSize,String abstractItemDescr)
		:AbstractItem(_pin,_name,abstractItemName,abstractItemSize,abstractItemDescr,1){
		construct(_pin, _onChanged, _pinMode, NULL, _pinVal, _turnOffLevel,clickMaxTime);
	}

	~PinDigitalNoInt();

	//AbstractItem functions
	virtual boolean loop() override{
		processInterrupt();
		return handleLoop();
	}

};



#endif /* LIBRARIES_PINEVENT_PINEVENT_H_ */
