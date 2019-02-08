/*
 * ButtonB.cpp
 *
 *  Created on: 13 ���. 2018 �.
 *      Author: Vitaliy
 */

#include "Arduino.h"
#include "PinDigital.h"
#include "TimeTrigger.h"
#include <FunctionalInterrupt.h>
#include "AbstractSensor.h"

PinDigital::~PinDigital(){}

void PinDigital::construct(uint8_t pin,std::function<void(void)> onChanged,uint8_t pinModeInOut,uint8_t changeMode,uint8_t pinVal,uint8_t turnOffLevel,long clickMaxTime){
	this->pin=pin;
	this->onChanged=onChanged;
	this->pinModeInOut=pinModeInOut;
	this->changeMode=changeMode;
	this->turnOffLevel=turnOffLevel;
	this->changed=false;
	this->oldVal=getVal();

	setupModes(pinVal);

	this->oldVal=getVal();
	this->turnOffLevel=turnOffLevel;

	boolean setAllowed=(pinModeInOut==OUTPUT);

	float val=isOn();

	items[0]={0,getName(),type,size,descr,val,0,-2,2,setAllowed};

	if((pinModeInOut==INPUT) && clickMaxTime!=0){
		isClick=true;
		clickTrigger=new TimeTrigger(0,clickMaxTime,false, [this](){clickTrigger->stop();});
	}
}

void PinDigital::setupModes(uint8_t pinVal) {
	pinMode(pin, pinModeInOut);

	if(pinModeInOut==OUTPUT){
		setVal(pinVal);
	}else{
		if(onChanged!=nullptr && changeMode!=254){
#ifdef ESP8266
			Serial.print(getName());
			Serial.print(FPSTR(" pin="));
			Serial.print(pin);
			Serial.println(FPSTR(" - interrupt attached"));

			attachInterrupt(pin, [this](){processInterrupt();}, changeMode);

			interruptAttached=true;
#endif
		}
	}
}

bool PinDigital::setVal(uint8_t _val){
	if(pinModeInOut==OUTPUT){

		if(_val!=getVal()){
			digitalWrite(pin, _val);
		}
	}

	items[0].val=isOn();
	return items[0].val;
}

void PinDigital::processClick(boolean fromTimer){
	if(!fromTimer){
		int8_t on=isOn();
		Serial.println(on);

		if(!on && clickTrigger->checkIsBeforeTrigger()){
			Serial.print(name);
			Serial.println(" CLicked");
			clickTrigger->stop();
			changed=true;

			return;
		}

		if(on && !clickTrigger->isActive()){
			Serial.println("CLick started");
			clickTrigger->start();

			return;
		}
	}

	Serial.println("Timer stopped");
	clickTrigger->stop();
}

void PinDigital::processInterrupt(){
	long interval=millis()-lastInterrupt;
	uint16_t now=getVal();
	//Serial.println("interval="+String(interval)+" now="+String(now));

	if(interval!=0 || !isClick){

		items[0].val=turnOffLevel!=now;
		//Serial.print("interrupted ");	Serial.print(name);
		//Serial.println(" old="+String(oldVal)+" now="+String(now));

		//

		if(now!=oldVal){
			changed=true;
			/*
			Serial.print(" name");Serial.print(name);
			Serial.print(" old");Serial.print(oldVal);
			Serial.print(" now");Serial.println(now);
			*/

			#ifdef DIGITAL_PIN_DISPLAY_CHANGE_EVENT
				Serial.print(printState()+" now="+String(now));
				Serial.println("...event dispatched");
			#endif
		}else{
			//Serial.println(" not changed");
			#ifdef DIGITAL_PIN_DISPLAY_NO_CHANGE_EVENT
			Serial.print(printState()+" now="+String(now));
			Serial.println("...no event");
			#endif
		}
		oldVal=now;
	}
	lastInterrupt=millis();
}

uint8_t PinDigital::getPin() {
	return pin;
}

uint8_t PinDigital::getChangeMode() {
	return changeMode;
}

uint8_t PinDigital::getPinModeInOut(){
	return pinModeInOut;
}

uint8_t PinDigital::updateVal(){
	uint8_t now=getVal();

	items[0].val=turnOffLevel!=now;

	if(oldVal!=now){
		setVal(now);
		if(onChanged!=nullptr){
			onChanged();
		}
	}
	oldVal=now;

	return now;
}

boolean PinDigital::handleLoop(){
	yield();
	bool result=false;
	#ifdef DISPLAY_LOOPS
		Serial.println("Digital pin loop ");
	#endif

	if(changed){
		if(!isClick ){
			if(onChanged!=nullptr){
				onChanged();
			}

			changed=false;
			result=true;
		}else{
			int8_t on=isOn();
			if(!on && clickTrigger->checkIsBeforeTrigger()){
				//Serial.print(name);Serial.println(" CLicked");
				if(onChanged!=nullptr){
					onChanged();
				}
				clickTrigger->stop();

				return false;
			}

			if(on && !clickTrigger->isActive()){
				//Serial.println("CLick started");
				clickTrigger->start();

				return true;
			}

			changed=false;
		}
	}

	if(isClick){
		clickTrigger->loop();
	}
	/*
	if(dispatchState){
		if(onChanged!=nullptr){
			onChanged();
		}
		dispatchState=false;
		result=true;
	}
*/
	return result;
}

uint8_t PinDigital::change(){
	setVal(getOpposite(getVal()));

	return getVal();
}
uint8_t PinDigital::changeAndDelay(uint delayTime){
	change();
	delay(delayTime);

	return getVal();
}

uint8_t PinDigital::changeAndDelay(uint delayTime,uint _count){
	if(_count<1){
		return changeAndDelay(delayTime);
	}

	for(uint8_t i=1;i<=_count;i++){
		changeAndDelay(delayTime);
	}

	return getVal();
}

uint8_t PinDigital::turnOn(){
	setVal(getOpposite(turnOffLevel));

	return getVal();
}
uint8_t PinDigital::turnOff(){
	setVal(turnOffLevel);

	return getVal();
}

uint8_t PinDigital::turnOnOff(boolean _turnOn){
	if(_turnOn){
		return turnOn();
	}else{
		return turnOff();
	}
}

boolean PinDigital::isOn(){
	return turnOffLevel!=getVal();
}

void PinDigital::setTurnOffLevel(uint8_t turnOffLevel) {
	this->turnOffLevel=turnOffLevel;
}

uint8_t PinDigital::getOpposite(uint8_t _val){
	return (_val==HIGH)?LOW:HIGH;
}
