package com.balabas.smarthouse.server.alarm;

import java.util.List;

import com.balabas.smarthouse.server.model.Device;
import com.balabas.smarthouse.server.model.ValueContainer;

public interface AlarmProcessService<T extends ValueContainer>{
	
	List<AlarmRepositoryBaseValueContainer<T, ?>> getAlarmRepositories();
	
	void activateAlarms(Device device, T entity);
	
	List<Alarm<T>> getALarms(T entity);

	List<Alarm<T>> getActiveAlarms(Device device);
}
