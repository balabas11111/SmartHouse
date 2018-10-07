/*
 * TimeTrigger.cpp
 *
 *  Created on: 13 ���. 2018 �.
 *      Author: Vitaliy
 */

#include "Arduino.h"
#include "TimeTrigger.h"

TimeTrigger::TimeTrigger(unsigned long lastTriggerTime,unsigned long interval,boolean active){
	_lastTriggerTime=lastTriggerTime;
	_interval=interval;
	_active=active;
}
void TimeTrigger::init(){

}
void TimeTrigger::setActive(boolean b){
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


