/*
 * ButtonB.cpp
 *
 *  Created on: 13 џэт. 2018 у.
 *      Author: Vitaliy
 */

#include "Arduino.h"
#include "PinDigital.h"

PinDigital::PinDigital(uint8_t pin){
	construct("PinDigital",pin,INPUT,LOW,LOW);
	//initFunc(nullptr);
}

PinDigital::PinDigital(String _name,uint8_t _pin){
	construct(_name,_pin,INPUT,LOW,LOW);
	//initFunc(nullptr);
}

PinDigital::PinDigital(String _name,uint8_t _pin,std::function<PinEvent(PinEvent)> funcEvent){
	construct(_name,_pin,INPUT,LOW,LOW);
	initFunc(funcEvent,CHANGE,nullptr);
}

PinDigital::PinDigital(String _name,uint8_t _pin,std::function<PinEvent(PinEvent)> funcEvent,std::function<PinEvent(PinEvent)> _isDispatcherPostFunction){
	construct(_name,_pin,INPUT,LOW,LOW);
	initFunc(funcEvent,CHANGE,_isDispatcherPostFunction);
}
PinDigital::PinDigital(String _name,uint8_t _pin,uint8_t _pinMode,std::function<PinEvent(PinEvent)> funcEvent,uint8_t _changeMode,uint8_t _pinVal){
	construct(_name,_pin,_pinMode,_pinVal,LOW);
	initFunc(funcEvent,_changeMode,nullptr);
}

PinDigital::PinDigital(String _name,uint8_t _pin,uint8_t _pinMode,std::function<PinEvent(PinEvent)> funcEvent,uint8_t _changeMode,uint8_t _pinVal,uint8_t _turnOffLevel){
	construct(_name,_pin,_pinMode,_pinVal,_turnOffLevel);
	initFunc(funcEvent,_changeMode,nullptr);
}

PinDigital::PinDigital(String _name,uint8_t _pin,uint8_t _pinMode,std::function<PinEvent(PinEvent)> funcEvent,uint8_t _changeMode,uint8_t _pinVal,std::function<PinEvent(PinEvent)> _isDispatcherPostFunction){
	construct(_name,_pin,_pinMode,_pinVal,LOW);
	initFunc(funcEvent,_changeMode,_isDispatcherPostFunction);
}

PinDigital::PinDigital(String _name,uint8_t _pin,uint8_t _pinMode,std::function<PinEvent(PinEvent)> funcEvent,uint8_t _changeMode,uint8_t _pinVal,uint8_t _turnOffLevel,std::function<PinEvent(PinEvent)> _isDispatcherPostFunction){
	construct(_name,_pin,_pinMode,_pinVal,_turnOffLevel);
	initFunc(funcEvent,_changeMode,_isDispatcherPostFunction);
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

void PinDigital::construct(String _name,uint8_t _pin,uint8_t _pinMode,uint8_t _pinVal,uint8_t _turnOffLevel){

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

void PinDigital::initFunc(std::function<PinEvent(PinEvent)> _externalFunction,uint8_t _buttonMode,std::function<PinEvent(PinEvent)> _isDispatcherPostFunction){
	externalFunction=_externalFunction;
	isDispatcherPostFunction=_isDispatcherPostFunction;
	attachInterrupt(pin, [this](){processInterrupt();}, _buttonMode);
	//attachInterrupt(pin, [this](){onRising();}, RISING);
	//attachInterrupt(pin, [this](){onFalling();}, FALLING);
	Serial.print("..function added");
}

uint16_t PinDigital::getVal(){
	return digitalRead(pin);
}

boolean PinDigital::setVal(uint16_t _val){
	if(pinInOutVal==OUTPUT){
		if(_val!=getVal()){
			digitalWrite(pin, _val);
		}
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

void PinDigital::processInterrupt(){
	uint16_t now=getVal();
	//val=now;
	oldValToDisp=oldVal;
	val=now;

	if(now!=oldVal){
		changed=true;
		#ifdef DIGITAL_PIN_DISPLAY_CHANGE_EVENT
			Serial.print(printState()+" now="+String(now));
			Serial.println("...event dispatched");
		#endif
	}else{
		//changed=false;
		#ifdef DIGITAL_PIN_DISPLAY_NO_CHANGE_EVENT
		Serial.print(printState()+" now="+String(now));
		Serial.println("...no event");
		#endif
	}
	oldVal=now;
}

boolean PinDigital::loop(){
	boolean result=false;
	#ifdef DISPLAY_LOOPS
		Serial.println("Digital pin loop ");
	#endif

	if(changed){
		executeExternalFunction(constructEvent(PIN_EVENT_STATE_UPDATED,true));
		//handleExternalFunction(PIN_EVENT_STATE_UPDATED);
		changed=false;
		result=true;
	}
	if(dispatchState){
		executeExternalFunction(constructEvent(PIN_EVENT_STATE_CURRENT,true));
		//handleExternalFunction(PIN_EVENT_STATE_CURRENT);
		dispatchState=false;
		result=true;
	}

	return result;
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

		//Serial.println("Pin("+name+") processEventNow ->"+event.getText());

		if(isTargetOfEvent(event)){

			//Serial.println("Pin("+name+") isTarget ->"+event.getText());
			if(event.getKind().equals(PIN_EVENT_STATE_GET)){
				return constructEvent(PIN_EVENT_STATE_CURRENT,true);
			}
			if(event.getKind().equals(PIN_EVENT_STATE_SET)){
				setVal(event.getVal());
				return constructEvent(PIN_EVENT_STATE_CURRENT,false);
			}
		}

		if(isDispatcherOfEvent(event)){
			//Serial.println("Pin("+name+") isDIspatcher ->"+event.getText());

			return executeIsDispatcherFunction(event);
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


/*
void PinDigital::handleExternalFunction(String str){
	if(hasExternalFunction()){
		#ifdef DIGITAL_PIN_DISPLAY_HANDLE_EXT_FUNCTION_EVENT
			Serial.println("handleExternalFunction name "+name+" val="+getVal());
		#endif

		executeExternalFunction(constructEvent(str));
	}
}
*/
PinEvent PinDigital::constructEvent(String str,boolean _bubble){
	//Serial.println(printState());
	return PinEvent(str,_bubble,pin,oldValToDisp,val,str,name,PIN_EVENT_TARGET_ANY);
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

void PinDigital::onRising(){
	Serial.println("Rised");
}

void PinDigital::onFalling(){
	Serial.println("fall");
}
