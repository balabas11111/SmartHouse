/*
 * TimeTrigger.h
 *
 *  Created on: 13 ���. 2018 �.
 *      Author: Vitaliy
 */

#ifndef TIMETRIGGER_H_
#define TIMETRIGGER_H_

#include "Arduino.h"
#include "FunctionalInterrupt.h"

class TimeTrigger{

public:
	TimeTrigger(unsigned long lastTriggerTime,unsigned long interval,boolean active);
	TimeTrigger(unsigned long lastTriggerTime,unsigned long interval,boolean active,std::function<void(void)> funcEvent);
	void init();
	boolean checkTriggerAndSaveTime();
	boolean checkTrigger();
	void saveTime();
	void setActive(boolean b);
	unsigned long getInterval();
	unsigned long setInterval(unsigned long interval);
	unsigned long getLastTriggerTime();
	unsigned long setLastTriggerTime(unsigned long lastTriggerTime);

	boolean loop();

private:

	void construct(unsigned long lastTriggerTime,unsigned long interval,boolean active,std::function<void(void)> funcEvent);
	std::function<void(void)> _funcEvent;
	unsigned long _lastTriggerTime;
	unsigned long _interval;
	boolean _active;
};



#endif /* TIMETRIGGER_H_ */
