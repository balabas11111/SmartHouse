/*
 * ButtonB.cpp
 *
 *  Created on: 13 џэт. 2018 у.
 *      Author: Vitaliy
 */

#include "Arduino.h"
#include "PinDigital.h"

PinDigital::PinDigital(uint8_t pin){
	init("PinDigital",pin,INPUT,LOW);
	//initFunc(nullptr);
}

PinDigital::PinDigital(String _name,uint8_t _pin){
	init(_name,_pin,INPUT,LOW);
	//initFunc(nullptr);
}

PinDigital::PinDigital(String _name,uint8_t _pin,std::function<PinEvent(PinEvent)> funcEvent){
	init(_name,_pin,INPUT,LOW);
	initFunc(funcEvent,CHANGE);
}

PinDigital::PinDigital(String _name,uint8_t _pin,uint8_t _pinMode,std::function<PinEvent(PinEvent)> funcEvent,uint8_t _buttonMode,uint8_t _pinVal){
	init(_name,_pin,_pinMode,_pinVal);
	initFunc(funcEvent,_buttonMode);
}

void PinDigital::init(String _name,uint8_t _pin,uint8_t _pinMode,uint8_t _pinVal){
	Serial.println("Start init pin "+_name);
	name=_name;
	pin=_pin;
	pinInOutVal=_pinMode;
	changed=false;

	pinMode(pin, _pinMode);

	if(pinInOutVal==OUTPUT)
		digitalWrite(pin, _pinVal);

	oldVal=getVal();
}

void PinDigital::initFunc(std::function<PinEvent(PinEvent)> _externalFunction,uint8_t _buttonMode){
	externalFunction=_externalFunction;
	attachInterrupt(pin, [this](){processInterrupt();}, _buttonMode);
}

boolean PinDigital::hasExternalFunction(){
	return externalFunction!=nullptr;
}

uint8_t PinDigital::getVal(){
	val=digitalRead(pin);
	return val;
}

boolean PinDigital::setVal(uint8_t val){
	if(pinInOutVal==OUTPUT){
		if(val!=getVal()){
			changed=true;
		}
		dispatchState=true;
		digitalWrite(pin, val);

		return val==digitalRead(pin);
	}
	return false;
}

uint8_t  PinDigital::getPin(){
	return pin;
}
boolean  PinDigital::isChanged(){
	return changed;
}
boolean  PinDigital::isVal(uint8_t _val){
	return val==_val;
}

void PinDigital::processInterrupt(){
	uint8_t now=getVal();

	Serial.print("Interrupt "+name+" pin="+pin+" state="+now);

	if(now!=oldVal){
		Serial.println("...event dispatched");
		changed=true;
	}else{
		changed=false;
		Serial.println("...no event");
	}

	oldVal=now;
}

boolean PinDigital::processEvent(PinEvent event){
	if(event.isValid())
		if(name.equals(event.getTargetName())){
			Serial.println(event.getText());
			if(event.getKind().equals(PIN_EVENT_STATE_GET)){
				dispatchState=true;
				return true;
			}
			if(event.getKind().equals(PIN_EVENT_STATE_SET)){
				setVal(event.getVal());
				return true;
			}
		}
	return false;
}

boolean PinDigital::isDispatcherOfEvent(PinEvent event){

	if(name.equals(event.getDispatcherName())){
		return true;
	}

	return false;
}

boolean PinDigital::isTargetOfEvent(PinEvent event){

	if(name.equals(event.getTargetName())){
		return true;
	}

	return false;
}

boolean PinDigital::loop(){
	boolean result=false;

	if(changed){
		handleExternalFunction(PIN_EVENT_STATE_UPDATED);
		changed=false;
		result=true;
	}
	if(dispatchState){
		handleExternalFunction(PIN_EVENT_STATE_CURRENT);
		dispatchState=false;
		result=true;
	}

	return result;
}

void PinDigital::handleExternalFunction(String str){
	if(hasExternalFunction()){
		Serial.print("Pin "+name+" val="+getVal());
		externalFunction(constructEvent(str));
		Serial.println(" ...completed ");
	}
}

PinEvent PinDigital::constructEvent(String str){
		return PinEvent(str,true,pin,oldVal,val,str,name);
}


