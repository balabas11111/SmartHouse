package com.balabas.smarthouse.server.entity.alarm;

@SuppressWarnings("rawtypes")
public interface IAlarmListener {

	void processAlarm(IAlarm alarm);
	
}
