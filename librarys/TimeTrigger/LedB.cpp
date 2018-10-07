/*
 * ButtonB.cpp
 *
 *  Created on: 13 џэт. 2018 у.
 *      Author: Vitaliy
 */

#include "Arduino.h"
#include "LedB.h"

const int defaultDelay=100;

	LedB::LedB(uint8_t pin){
		_pin=pin;
		turnOff();
	}
	LedB::LedB(uint8_t pin,uint8_t state){
		_pin=pin;
		pinMode(_pin, OUTPUT);
		digitalWrite(_pin,state);

		if(state==HIGH){
			turnedOn=true;
		}
	}
	void LedB::init(){
		 pinMode(_pin, OUTPUT);
	}
	void LedB::turnOn(){
		digitalWrite(_pin,HIGH);
		turnedOn=true;

		Serial.println("Pin "+String(getPin())+" changed State to HIGH");
	}
	void LedB::turnOff(){
		digitalWrite(_pin,LOW);
		turnedOn=false;

		Serial.println("Pin "+String(getPin())+" changed State to LOW");
	}
	void LedB::changeState(){
		if(turnedOn){
			turnOff();
		}else{
			turnOn();
		}
	}
	void LedB::changeStateForDefaultInterval(){
		changeStateForInterval(0);
	}

	void LedB::changeStateForInterval(int interval){
		changeState();

		if(interval<=0){
			interval=defaultDelay;
		}
		delay(interval);

		changeState();
	}

	uint8_t LedB::getPin(){
		return _pin;
	}

	void LedB::changeState(uint8_t state){
		if(state==0){
			turnOff();
			return;
		}
		if(state==1){
			turnOn();
		}

	}

	boolean LedB::processEvent(AbstractEvent event){
		if(getPin()==event.getPin()){
			changeState(event.getIntCommand());
			return true;
		}

		return false;
	}

	boolean LedB::isDispatcherOfEvent(AbstractEvent event){
		return false;
	}



