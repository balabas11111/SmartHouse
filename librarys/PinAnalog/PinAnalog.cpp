/*
 * ButtonB.cpp
 *
 *  Created on: 13 џэт. 2018 у.
 *      Author: Vitaliy
 */

#include "Arduino.h"
#include "PinAnalog.h"

PinAnalog::PinAnalog(uint8_t pin){
	init("PinAnalog",pin,INPUT,LOW,CHANGE,0,0);
	//initFunc(nullptr);
}

PinAnalog::PinAnalog(String _name,uint8_t _pin){
	init(_name,_pin,INPUT,LOW,CHANGE,0,0);
	//initFunc(nullptr);
}

PinAnalog::PinAnalog(String _name,uint8_t _pin,std::function<PinEvent(PinEvent)> funcEvent){
	init(_name,_pin,INPUT,LOW,CHANGE,0,0);
	initFunc(funcEvent,CHANGE);
}

PinAnalog::PinAnalog(String _name,uint8_t _pin,uint8_t _pinMode,std::function<PinEvent(PinEvent)> funcEvent,uint8_t _changeMode,uint8_t _pinVal){
	init(_name,_pin,_pinMode,_pinVal,_changeMode,0,0);
	initFunc(funcEvent,_changeMode);
}

PinAnalog::~PinAnalog(){
	Serial.println("destructed");
}

void PinAnalog::init(String _name,uint8_t _pin,uint8_t _pinMode,uint16_t _pinVal,uint8_t _changeMode,uint16_t _warmUpTime,uint16_t _measurePeriodTime){

	String pinModeStr="OTHER";

	if(_pinMode==INPUT){pinModeStr="INPUT";}
	if(_pinMode==OUTPUT){pinModeStr="OUTPUT";}

	String pinValStr=(_pinVal==HIGH)?"HIGH":"LOW";

	Serial.print("PinAnalog(name="+_name+" pin="+String(_pin)+" _pinMode="+pinModeStr+" _pinVal="+pinValStr+")");
	name=_name;
	pin=_pin;
	pinInOutVal=_pinMode;
	changed=false;
	warmUpTime=_warmUpTime;
	measurePeriodTime=_measurePeriodTime;

	lastTime=millis();

	pinMode(pin, _pinMode);

	if(pinInOutVal==OUTPUT)
		analogWrite(pin, _pinVal);

	currentTotal=getCurrent();
	currentIteration=1;

	oldVal=currentTotal;

	attachInterrupt(pin, [this](){processInterrupt();}, _changeMode);
	Serial.print("...done");
}

String PinAnalog::displayDetails(){
	String pinModeStr="OTHER";

		if(pinInOutVal==INPUT){pinModeStr="INPUT";}
		if(pinInOutVal==OUTPUT){pinModeStr="OUTPUT";}

		String pinValStr=(val==HIGH)?"HIGH":"LOW";

		String res=("PinDigital(name="+name+" pin="+String(pin)+" _pinMode="+pinModeStr+" _pinVal="+pinValStr+")");

		Serial.println(res);

		return res;
}

void PinAnalog::initFunc(std::function<PinEvent(PinEvent)> _externalFunction,uint8_t _buttonMode){
	externalFunction=_externalFunction;
	Serial.print("bm="+String(_buttonMode));
	Serial.print("..function added");
}

boolean PinAnalog::hasExternalFunction(){
	return externalFunction!=nullptr;
}

uint16_t PinAnalog::getVal(){
	val =currentTotal/currentIteration;
	return val;
}

uint16_t PinAnalog::getCurrent(){
	return analogRead(pin);
}

boolean PinAnalog::setVal(uint16_t val){
	if(pinInOutVal==OUTPUT){
		if(val!=getVal()){
			analogWrite(pin, val);
			//changed=true;
		}
		//dispatchState=true;

		return val==analogRead(pin);
	}
	return false;
}

void PinAnalog::processInterrupt(){

	uint16_t curVal=getCurrent();
	Serial.print("Interrupt "+name+" pin="+pin+" curVal="+curVal);

	if(warmUpTime!=0){
		//need to warm up sensor
		currentIteration++;
		currentTotal+=curVal;
		oldVal=currentTotal/currentIteration;

		if(lastTime+warmUpTime>millis()){
			//still not warmed
			Serial.println("Warming iter="+String(currentIteration)+" currTotal"+String(currentTotal)+" oldVal="+String(oldVal));
		}else{
			warmUpTime=0;
			Serial.println("Warmed iter="+String(currentIteration)+" currTotal"+String(currentTotal)+" oldVal="+String(oldVal));

			currentTotal=curVal;
			currentIteration=1;

			lastTime=millis();
		}

		return;
	}

	boolean complete=checkIfMeasureCompleted();

	if(complete){
		return;
	}

	Serial.println("Measure still not completed");
	currentIteration++;
	currentTotal+=curVal;

}

boolean PinAnalog::checkIfMeasureCompleted(){
	boolean completed=false;

	if(lastTime+measurePeriodTime>millis()){
		Serial.println("Measure complete");

		uint16_t nowVal=getVal();

		currentIteration=1;
		currentTotal=getCurrent();

		lastTime=millis();

		uint16_t currDiff=(nowVal>oldVal)?(nowVal-oldVal):(oldVal-nowVal);

		if(currDiff>maxDiffToDispatchEvent){
			Serial.println("...event dispatched");
			changed=true;
		}else{
			changed=false;
			Serial.println("...no event");
		}

		oldVal=nowVal;
		completed=true;
	}

	return completed;
}

boolean PinAnalog::processEvent(PinEvent event){
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

PinEvent PinAnalog::processEventNow(PinEvent event){
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

boolean PinAnalog::isDispatcherOfEvent(PinEvent event){

	if(name.equals(event.getDispatcherName()) && event.getPinId()==pin){
		return true;
	}

	return false;
}

boolean PinAnalog::isTargetOfEvent(PinEvent event){

	if(name.equals(event.getTargetName()) && event.getPinId()==pin){
		return true;
	}

	return false;
}

boolean PinAnalog::loop(){
	boolean result=false;

	if(!changed){
		checkIfMeasureCompleted();
	}

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

void PinAnalog::handleExternalFunction(String str){
	if(hasExternalFunction()){
		Serial.print("Pin "+name+" val="+getVal());
		externalFunction(constructEvent(str));
		Serial.println(" ...completed ");
	}
}

PinEvent PinAnalog::constructEvent(String str){
		return PinEvent(str,true,pin,oldVal,val,str,name,PIN_EVENT_TARGET_ANY);
}

PinEvent PinAnalog::constructPinEventSetState(uint16_t _val,String _strVal,String _dispatcherName){
	return PinEvent(PIN_EVENT_STATE_SET,true,pin,0,_val,_strVal,_dispatcherName,name);
}

PinEvent PinAnalog::constructPinEventSetState(PinEvent parentEvent){
	return PinEvent(PIN_EVENT_STATE_SET,true,pin,0,parentEvent.getVal(),parentEvent.getStrVal(),parentEvent.getDispatcherName(),name);
}


