/*
 * ButtonB.cpp
 *
 *  Created on: 13 џэт. 2018 у.
 *      Author: Vitaliy
 */

#include "Arduino.h"
#include "PinExternalFunc.h"
#include <FunctionalInterrupt.h>

template <class T>
PinExternalFunc<T>::PinExternalFunc(String _name,uint8_t _pin,std::function<void(void)> _initFunc,std::function<PinEvent(PinEvent)> _funcEvent,uint16_t timeInterval,std::function<T(void)> _getValFunc){

	name=_name;
	pin=_pin;
	interval=timeInterval;

	getValFunc=_getValFunc;
	externalFunction=_funcEvent;
	dispatchState=false;
	changed=false;

	_initFunc();
	oldVal=getVal();

	lastTime=millis();

	Serial.println("...done");
}

template <class T>
String PinExternalFunc<T>::displayDetails(){
	String res="PinExternalFunc("+name+" pin="+pin+")";
	Serial.println(res);

	return res;
}

template <class T>
PinExternalFunc<T>::~PinExternalFunc(){
	Serial.println("PinExternalFunc destructed");
}

template <class T>
boolean PinExternalFunc<T>::hasExternalFunction(){
	return externalFunction!=nullptr;
}

template <class T>
T PinExternalFunc<T>::getVal(){
	val=getValFunc();
	return val;
}

template <class T>
String PinExternalFunc<T>::getName(){
	return name;
}
template <class T>
uint8_t  PinExternalFunc<T>::getPin(){
	return pin;
}
template <class T>
boolean  PinExternalFunc<T>::isChanged(){
	return changed;
}
template <class T>
boolean  PinExternalFunc<T>::isVal(T _val){
	return val==_val;
}

template <class T>
void PinExternalFunc<T>::processInterrupt(){
	T now=getVal();

	if(now!=oldVal){
		Serial.println("...event dispatched");
		changed=true;
	}else{
		changed=false;
		Serial.println("...no event");
	}

	oldVal=now;
}

template <class T>
boolean PinExternalFunc<T>::processEvent(PinEvent event){
	if(event.isValid())
		if(isTargetOfEvent(event)){
			Serial.println(name+" process "+event.getText());
			if(event.getKind().equals(PIN_EVENT_STATE_GET)){
				dispatchState=true;
				return true;
			}
			if(event.getKind().equals(PIN_EVENT_STATE_SET)){
				//setVal(event.getVal());
				return true;
			}
		}
	return false;
}

template <class T>
PinEvent PinExternalFunc<T>::processEventNow(PinEvent event){
	if(event.isValid())
		if(isTargetOfEvent(event)){
			Serial.println(name+" processNow "+event.getText());
			if(event.getKind().equals(PIN_EVENT_STATE_GET)){
				return constructEvent(PIN_EVENT_STATE_CURRENT);
			}
			if(event.getKind().equals(PIN_EVENT_STATE_SET)){
				//setVal(event.getVal());
				return constructEvent(PIN_EVENT_STATE_CURRENT);
			}
		}
	return PinEvent();
}

template <class T>
boolean PinExternalFunc<T>::isDispatcherOfEvent(PinEvent event){

	if(name.equals(event.getDispatcherName()) && event.getPinId()==pin){
		return true;
	}

	return false;
}

template <class T>
boolean PinExternalFunc<T>::isTargetOfEvent(PinEvent event){

	if(name.equals(event.getTargetName()) && event.getPinId()==pin){
		return true;
	}

	return false;
}

template <class T>
boolean PinExternalFunc<T>::loop(){

	boolean result=false;

	if(lastTime+interval>millis()){

		processInterrupt();

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

		lastTime=millis();
	}

	return result;
}

template <class T>
void PinExternalFunc<T>::handleExternalFunction(String str){
	if(hasExternalFunction()){
		Serial.print("Pin "+name+" val="+getVal());
		externalFunction(constructEvent(str));
		Serial.println(" ...completed ");
	}
}

template <class T>
PinEvent PinExternalFunc<T>::constructEvent(String str){
		return PinEvent(str,true,pin,0,0,String(getVal()),name,PIN_EVENT_TARGET_ANY);
}

template <class T>
PinEvent PinExternalFunc<T>::constructPinEventSetState(T _val,String _strVal,String _dispatcherName){
	return PinEvent(PIN_EVENT_STATE_SET,true,pin,0,0,_strVal,_dispatcherName,name);
}

template <class T>
PinEvent PinExternalFunc<T>::constructPinEventSetState(PinEvent parentEvent){
	return PinEvent(PIN_EVENT_STATE_SET,true,pin,0,parentEvent.getVal(),parentEvent.getStrVal(),parentEvent.getDispatcherName(),name);
}


