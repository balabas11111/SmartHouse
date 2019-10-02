package com.balabas.smarthouse.server.alarm;

import java.util.List;

import com.balabas.smarthouse.server.model.SmartHouseItem;

public interface AlarmRepository<T extends SmartHouseItem> {

	void init();
	
	List<Alarm<T>> getAlarms();
	
	List<Alarm<T>> getAlarms(String deviceId);
	
	void putAlarm(T alarm);
	
	void saveAlarms(List<Alarm<T>> alarms);
}
