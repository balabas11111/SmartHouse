package com.balabas.smarthouse.server.alarm;

import java.io.IOException;
import java.util.List;

import com.balabas.smarthouse.server.model.Device;
import com.balabas.smarthouse.server.model.SmartHouseItem;

public interface AlarmRepository<T extends SmartHouseItem, S extends Alarm<T>> {

	void init() throws IOException;
	
	List<S> getAlarms();
	
	List<S> getActiveAlarms();
	
	List<S> getActiveAlarms(String deviceId);
	
	List<S> getActiveAlarms(String deviceId, String itemName);
	
	List<S> getAlarmsForItem(T item);
	
	void putAlarm(S alarm);
	
	void saveAlarms(List<S> alarms) throws IOException;
	
	void activateAlarmsForDevice(Device device);

	void activateAlarmsForItem(Device device, T item);
}
