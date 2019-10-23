package com.balabas.smarthouse.server.entity.alarm;

import java.io.IOException;
import java.util.List;

import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.IEntity;

public interface IEntityAlarmService {

	List<IEntityAlarm> getActiveEntityAlarms();
	
	List<IEntityAlarm> getEntityAlarmsWithAlarmDetected(IDevice device);

	IEntityAlarm getAlarm(IDevice device, IEntity entity); 
	
	void registerAlarm(IEntityAlarm alarm);

	void saveAlarms() throws IOException;
	
	void loadAlarms() throws IOException;
	
	default void activateAlarms(List<IDevice> devices){
		devices.stream().forEach(this::activateAlarms);
	}
	
	void activateAlarms(IDevice device);

	void checkAlarms(IDevice device);
	
	void checkAlarmsSendNotifications(IDevice device);

	List<IEntityAlarm> getActiveEntityAlarms(IDevice device);

	List<IEntityAlarm> getAlarmsWithAlarmNotificationRequired(IDevice device);

	List<IEntityAlarm> getAlarmsWithAlarmFinished(IDevice device);

	IEntityAlarm getAlarm(String deviceName, String entityName);

}