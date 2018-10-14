/*
 * ButtonB.cpp
 *
 *  Created on: 13 џэт. 2018 у.
 *      Author: Vitaliy
 */

#include "Arduino.h"
#include "PinDigital.h"

PinDigital::PinDigital(uint8_t pin){
	init("PinDigital",pin,INPUT,LOW,LOW);
	//initFunc(nullptr);
}

PinDigital::PinDigital(String _name,uint8_t _pin){
	init(_name,_pin,INPUT,LOW,LOW);
	//initFunc(nullptr);
}

PinDigital::PinDigital(String _name,uint8_t _pin,std::function<PinEvent(PinEvent)> funcEvent){
	init(_name,_pin,INPUT,LOW,LOW);
	initFunc(funcEvent,CHANGE);
}

PinDigital::PinDigital(String _name,uint8_t _pin,uint8_t _pinMode,std::function<PinEvent(PinEvent)> funcEvent,uint8_t _changeMode,uint8_t _pinVal){
	init(_name,_pin,_pinMode,_pinVal,LOW);
	initFunc(funcEvent,_changeMode);
}

PinDigital::PinDigital(String _name,uint8_t _pin,uint8_t _pinMode,std::function<PinEvent(PinEvent)> funcEvent,uint8_t _changeMode,uint8_t _pinVal,uint8_t _turnOffLevel){
	init(_name,_pin,_pinMode,_pinVal,_turnOffLevel);
	initFunc(funcEvent,_changeMode);
}

PinDigital::~PinDigital(){
	Serial.println("destructed");
}

String PinDigital::displayDetails(){
	String pinModeStr="OTHER";

	if(pinInOutVal==INPUT){pinModeStr="INPUT";}
	if(pinInOutVal==OUTPUT){pinModeStr="OUTPUT";}

	String pinValStr=(val==HIGH)?"HIGH":"LOW";

	String result=("PinDigital(name="+name+" pin="+String(pin)+" _pinMode="+pinModeStr+" _pinVal="+pinValStr+")");

	Serial.println(result);
	return result;
}

void PinDigital::init(String _name,uint8_t _pin,uint8_t _pinMode,uint8_t _pinVal,uint8_t _turnOffLevel){

	name=_name;
	pin=_pin;
	pinInOutVal=_pinMode;
	changed=false;

	pinMode(pin, _pinMode);

	if(pinInOutVal==OUTPUT)
		digitalWrite(pin, _pinVal);

	oldVal=getVal();
	turnOffLevel=_turnOffLevel;
	Serial.print("...done");
}

void PinDigital::initFunc(std::function<PinEvent(PinEvent)> _externalFunction,uint8_t _buttonMode){
	externalFunction=_externalFunction;
	attachInterrupt(pin, [this](){processInterrupt();}, _buttonMode);
	Serial.print("..function added");
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
			digitalWrite(pin, val);
			//changed=true;
		}
		//dispatchState=true;

		return val==digitalRead(pin);
	}
	return false;
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

String PinDigital::getName(){
	return name;
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

	if(now!=oldVal){
		Serial.print("Interrupt "+name+" pin="+pin+" state="+now);
		Serial.println("...event dispatched");
		changed=true;
	}else{
		#ifdef DIGITAL_PIN_DISPLAY_NO_CHANGE_EVENT
			Serial.print("Interrupt "+name+" pin="+pin+" state="+now);
			changed=false;
			Serial.println("...no event");
		#endif
	}

	oldVal=now;
}

boolean PinDigital::processEvent(PinEvent event){
	if(event.isValid())
		if(isTargetOfEvent(event)){
			Serial.println(name+" process "+event.getText());
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

PinEvent PinDigital::processEventNow(PinEvent event){
	if(event.isValid())
		if(isTargetOfEvent(event)){
			Serial.println(name+" processNow "+event.getText());
			if(event.getKind().equals(PIN_EVENT_STATE_GET)){
				return constructEvent(PIN_EVENT_STATE_CURRENT);
			}
			if(event.getKind().equals(PIN_EVENT_STATE_SET)){
				setVal(event.getVal());
				return constructEvent(PIN_EVENT_STATE_CURRENT);
			}
		}
	return PinEvent();
}

boolean PinDigital::isDispatcherOfEvent(PinEvent event){

	if(name.equals(event.getDispatcherName()) && event.getPinId()==pin){
		return true;
	}

	return false;
}

boolean PinDigital::isTargetOfEvent(PinEvent event){

	if(name.equals(event.getTargetName()) && event.getPinId()==pin){
		return true;
	}

	return false;
}

boolean PinDigital::loop(){
	boolean result=false;
	#ifdef DISPLAY_LOOPS
		Serial.println("Digital pin loop ");
	#endif

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
		Serial.println("handleExternalFunction name "+name+" val="+getVal());
		externalFunction(constructEvent(str));
		//Serial.println(" ...completed ");
	}
}

PinEvent PinDigital::constructEvent(String str){
		return PinEvent(str,true,pin,oldVal,val,str,name,PIN_EVENT_TARGET_ANY);
}

PinEvent PinDigital::constructPinEventSetState(uint8_t _val,String _strVal,String _dispatcherName){
	return PinEvent(PIN_EVENT_STATE_SET,true,pin,0,_val,_strVal,_dispatcherName,name);
}

PinEvent PinDigital::constructPinEventSetState(PinEvent parentEvent){
	return PinEvent(PIN_EVENT_STATE_SET,true,pin,0,parentEvent.getVal(),parentEvent.getStrVal(),parentEvent.getDispatcherName(),name);
}

uint8_t PinDigital::getOpposite(uint8_t _val){
	return (_val==HIGH)?LOW:HIGH;
}


