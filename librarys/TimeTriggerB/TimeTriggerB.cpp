/*
 * TimeTriggerB.cpp
 *
 *  Created on: 13 ���. 2018 �.
 *      Author: Vitaliy
 */

#include "Arduino.h"
#include "TimeTriggerB.h"

TimeTriggerB::TimeTriggerB(unsigned long lastTriggerTime,unsigned long interval,boolean active){
	_lastTriggerTime=lastTriggerTime;
	_interval=interval;
	_active=active;
}
void TimeTriggerB::init(){

}
void TimeTriggerB::setActive(boolean b){
	_active=b;
}

boolean TimeTriggerB::checkTriggerAndSaveTime(){
	boolean result=checkTrigger();
	saveTime();

	return result;
}
boolean TimeTriggerB::checkTrigger(){


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

void TimeTriggerB::saveTime(){
	_lastTriggerTime=millis();
}

unsigned long TimeTriggerB::getInterval(){
	return _interval;
}

unsigned long TimeTriggerB::setInterval(unsigned long interval){
	_interval=interval;
}

unsigned long TimeTriggerB::getLastTriggerTime(){
	return _lastTriggerTime;
}

unsigned long TimeTriggerB::setLastTriggerTime(unsigned long lastTriggerTime){
	_lastTriggerTime=lastTriggerTime;
}


