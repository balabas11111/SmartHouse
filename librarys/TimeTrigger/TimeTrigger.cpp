/*
 * TimeTrigger.cpp
 *
 *  Created on: 13 ���. 2018 �.
 *      Author: Vitaliy
 */

#include "Arduino.h"
#include "TimeTrigger.h"
#include "FunctionalInterrupt.h"

TimeTrigger::TimeTrigger(unsigned long lastTriggerTime,unsigned long interval,boolean active){
	construct(lastTriggerTime, interval, active, nullptr);
}

TimeTrigger::TimeTrigger(unsigned long lastTriggerTime,unsigned long interval,boolean active,std::function<void(void)> funcEvent){
	construct(lastTriggerTime, interval, active, funcEvent);
}

TimeTrigger::~TimeTrigger(){
Serial.println("TimeTrigger destructed");
}

void TimeTrigger::construct(unsigned long lastTriggerTime,unsigned long interval,boolean active,std::function<void(void)> funcEvent){
	_lastTriggerTime=lastTriggerTime;
	_interval=interval;
	_active=active;

	_funcEvent=funcEvent;
}

void TimeTrigger::init(){
	setActive(true);
}
void TimeTrigger::setActive(boolean b){
	//Serial.println("TimeTrigger active="+String(b));
	_active=b;
}

boolean TimeTrigger::checkTriggerAndSaveTime(){
	boolean result=checkTrigger();
	saveTime();

	return result;
}
boolean TimeTrigger::checkTrigger(){
	//Serial.print("checkTrigger  --- ");
	if(!_active){
		return false;
	}

	boolean result=false;

	unsigned long now=millis();
	unsigned long expected=getLastTriggerTime()+getInterval();

	if(expected<now){
		result=true;
	}

	return result;
}

String TimeTrigger::getName(){
	return "TimeTrigger(_interval="+String(_interval)+" _active="+String(_active)+")";
}

String TimeTrigger::displayDetails(){
	String res=getName();
	Serial.println(res);

	return res;
}

boolean TimeTrigger::loop(){
	if(_active){
		boolean result=checkTrigger();

		if(result && _funcEvent!=nullptr){
			_funcEvent();
		}

		if(result){
			saveTime();
		}

		return result;
	}

	return false;
}

void TimeTrigger::saveTime(){
	_lastTriggerTime=millis();
}

unsigned long TimeTrigger::getInterval(){
	return _interval;
}

unsigned long TimeTrigger::setInterval(unsigned long interval){
	_interval=interval;
}

unsigned long TimeTrigger::getLastTriggerTime(){
	return _lastTriggerTime;
}

unsigned long TimeTrigger::setLastTriggerTime(unsigned long lastTriggerTime){
	_lastTriggerTime=lastTriggerTime;
}


