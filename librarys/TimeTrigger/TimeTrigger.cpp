/*
 * TimeTrigger.cpp
 *
 *  Created on: 13 ���. 2018 �.
 *      Author: Vitaliy
 */

#include "Arduino.h"
#include "TimeTrigger.h"
#ifdef ESP8266
	#include "FunctionalInterrupt.h"
#endif
#ifdef ESP32
	#include <functional>
#endif

TimeTrigger::TimeTrigger(std::function<void(void)> funcEvent) {
	construct(0,0,false,funcEvent);
}

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
	//setActive(true);
	start();
}
void TimeTrigger::setActive(boolean b){
	//Serial.println("TimeTrigger active="+String(b)+" interval="+String(_interval));
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

	if(getInterval()==0){
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

void TimeTrigger::start(long interval,boolean active){
	setInterval(interval);
	saveTime();
	setActive(true);
}

void TimeTrigger::start(long interval) {
	setInterval(interval);
	start();
}

void TimeTrigger::start(){
	saveTime();
	setActive(true);
}

void TimeTrigger::stop(){
	setActive(false);
}

boolean TimeTrigger::isActive(){
	return _active;
}

boolean TimeTrigger::checkIsBeforeTrigger(){
	if(!_active){
		return false;
	}

	if(getInterval()==0){
		return false;
	}

	boolean result=false;

	unsigned long now=millis();
	unsigned long expected=getLastTriggerTime()+getInterval();

	if(expected>=now){
		result=true;
	}

	return result;
}

void TimeTrigger::saveTime(){
	_lastTriggerTime=millis();
}

unsigned long TimeTrigger::getInterval(){
	return _interval;
}

void TimeTrigger::setInterval(unsigned long interval){
	_interval=interval;
}

unsigned long TimeTrigger::getLastTriggerTime(){
	return _lastTriggerTime;
}

void TimeTrigger::setLastTriggerTime(unsigned long lastTriggerTime){
	_lastTriggerTime=lastTriggerTime;
}

void TimeTrigger::setHandler(std::function<void(void)> funcEvent){
	_funcEvent=funcEvent;
}


