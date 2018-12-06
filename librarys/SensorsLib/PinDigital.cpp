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

	this->processValueFromMqtt=(pinModeInOut==OUTPUT);

	float val=isOn();
	items[0]={0,name,type,size,descr,val,0,-2,2,"",processValueFromMqtt};

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
		if(onChanged!=nullptr && changeMode!=NULL){
#ifdef ESP8266
			attachInterrupt(pin, [this](){processInterrupt();}, changeMode);
#endif
		}
	}
}

int PinDigital::getVal(){
	return digitalRead(pin);
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
/*
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
*/
void PinDigital::processInterrupt(){
	long interval=millis()-lastInterrupt;
	uint16_t now=getVal();
	//Serial.println("interval="+String(interval)+" now="+String(now));

	if(interval!=0 || !isClick){

		items[0].val=turnOffLevel!=now;
		//Serial.print("interrupted ");	Serial.print(name);
		//Serial.println(" old="+String(oldVal)+" now="+String(now));

		if(now!=oldVal){
			changed=true;
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


uint8_t PinDigital::getPin() {
	return pin;
}

uint8_t PinDigital::getChangeMode() {
	return changeMode;
}

uint8_t PinDigital::getPinModeInOut(){
	return pinModeInOut;
}
