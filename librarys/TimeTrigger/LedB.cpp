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
	LedB::LedB(String _name,uint8_t pin,uint8_t state,std::function<String(AbstractEvent)> _externalFunctionEvent){
		_pin=pin;
		pinMode(_pin, OUTPUT);
		digitalWrite(_pin,state);

		name=_name;
		externalFunctionEvent=_externalFunctionEvent;

		if(state==HIGH){
			turnedOn=true;
		}
	}

	String LedB::getName(){
		return name;
	}

	void LedB::init(){
		 pinMode(_pin, OUTPUT);
	}
	void LedB::turnOn(){
		digitalWrite(_pin,HIGH);
		turnedOn=true;

		Serial.println("Pin "+String(getPin())+" changed State to HIGH");

		sendCurrentState();
	}
	void LedB::turnOff(){
		digitalWrite(_pin,LOW);
		turnedOn=false;

		Serial.println("Pin "+String(getPin())+" changed State to LOW");

		sendCurrentState();
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
			if(event.isEventOfKind(PIN_STATE_CHANGE_PREFFIX)){
				changeState(event.getIntCommand());
				return true;
			}
			if(event.isEventOfKind(PIN_GET_STATE)){
				sendCurrentState();
			}
		}

		return false;
	}

	boolean LedB::isDispatcherOfEvent(AbstractEvent event){
		return false;
	}

	void LedB::sendCurrentState(){
		String com=(turnedOn)?"ON":"OFF";
		externalFunctionEvent(AbstractEvent(PIN_CURRENT_STATE,_pin,turnedOn,com,name));
	}



