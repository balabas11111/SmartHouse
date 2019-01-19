/*
 * TimeTrigger.h
 *
 *  Created on: 13 ���. 2018 �.
 *      Author: Vitaliy
 */

#ifndef TIMETRIGGER_H_
#define TIMETRIGGER_H_

#include "Arduino.h"
#ifdef ESP8266
	#include "FunctionalInterrupt.h"
#endif
#ifdef ESP32
	#include <functional>
#endif
#include "interfaces/Loopable.h"

class TimeTrigger:public Loopable{

public:
	TimeTrigger(std::function<void(void)> funcEvent);
	TimeTrigger(unsigned long lastTriggerTime,unsigned long interval,boolean active);
	TimeTrigger(unsigned long lastTriggerTime,unsigned long interval,boolean active,std::function<void(void)> funcEvent);

	virtual ~TimeTrigger();
	void init();
	boolean checkTriggerAndSaveTime();
	boolean checkTrigger();
	void saveTime();
	void setActive(boolean b);
	unsigned long getInterval();
	void setInterval(unsigned long interval);
	unsigned long getLastTriggerTime();
	void setLastTriggerTime(unsigned long lastTriggerTime);

	void setHandler(std::function<void(void)> funcEvent);

	String getName();
	boolean loop();

	void start(long interval,boolean active);
	void start(long interval);
	void start();
	void stop();

	boolean isActive();

	boolean checkIsBeforeTrigger();

private:
	void construct(unsigned long lastTriggerTime,unsigned long interval,boolean active,std::function<void(void)> funcEvent);
	std::function<void(void)> _funcEvent;
	unsigned long _lastTriggerTime;
	unsigned long _interval;
	boolean _active;
};



#endif /* TIMETRIGGER_H_ */
