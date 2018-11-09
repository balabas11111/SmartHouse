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
#include "Loopable.h"

class TimeTrigger:public Loopable{

public:
	TimeTrigger(unsigned long lastTriggerTime,unsigned long interval,boolean active);
	TimeTrigger(unsigned long lastTriggerTime,unsigned long interval,boolean active,std::function<void(void)> funcEvent);
	virtual ~TimeTrigger();
	void init();
	boolean checkTriggerAndSaveTime();
	boolean checkTrigger();
	void saveTime();
	void setActive(boolean b);
	unsigned long getInterval();
	unsigned long setInterval(unsigned long interval);
	unsigned long getLastTriggerTime();
	unsigned long setLastTriggerTime(unsigned long lastTriggerTime);
	void setHandler(std::function<void(void)> funcEvent);

	String getName();
	String displayDetails();
	boolean loop();

private:

	void construct(unsigned long lastTriggerTime,unsigned long interval,boolean active,std::function<void(void)> funcEvent);
	std::function<void(void)> _funcEvent;
	unsigned long _lastTriggerTime;
	unsigned long _interval;
	boolean _active;
};



#endif /* TIMETRIGGER_H_ */
