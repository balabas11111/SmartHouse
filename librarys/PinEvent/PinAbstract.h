/*
 * PinAbstract.h
 *
 *  Created on: 15 окт. 2018 г.
 *      Author: Vitaliy
 */

#ifndef LIBRARIES_PINEVENT_PINABSTRACT_H_
#define LIBRARIES_PINEVENT_PINABSTRACT_H_

#include "Arduino.h"

class PinAbstract{

public:

	virtual ~PinAbstract(){};

	String getName(){
		return name;
	}
	uint8_t getPin(){
		return pin;
	}
	boolean isChanged(){
		return changed;
	}
	boolean isVal(uint16_t _val){
		return val==_val;
	}

	String printState(){
		return "name="+name+" oldVal="+String(oldVal)+" val="+String(val)+" changed="+String(changed)+" oldValToDisp="+String(oldValToDisp);
	}

	virtual uint16_t getVal()=0;
	virtual boolean setVal(uint16_t _val)=0;

	virtual void processInterrupt()=0;

protected:
	String name;
	uint8_t pin;
	uint8_t pinInOutVal;
	uint16_t oldVal;
	uint16_t oldValToDisp;
	uint16_t val;
	boolean changed;
	boolean dispatchState;
};


#endif /* LIBRARIES_PINEVENT_PINABSTRACT_H_ */
