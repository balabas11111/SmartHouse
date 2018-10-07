/*
 * TimeTrigger.h
 *
 *  Created on: 13 ���. 2018 �.
 *      Author: Vitaliy
 */

#ifndef TIMETRIGGERB_H_
#define TIMETRIGGERB_H_

#include "Arduino.h"

class TimeTriggerB{

public:
	TimeTriggerB(unsigned long lastTriggerTime,unsigned long interval,boolean active);
	void init();
	boolean checkTriggerAndSaveTime();
	boolean checkTrigger();
	void saveTime();
	void setActive(boolean b);
	unsigned long getInterval();
	unsigned long setInterval(unsigned long interval);
	unsigned long getLastTriggerTime();
	unsigned long setLastTriggerTime(unsigned long lastTriggerTime);

private:
	unsigned long _lastTriggerTime;
	unsigned long _interval;
	boolean _active;
};



#endif /* TIMETRIGGERB_H_ */
