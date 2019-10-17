package com.balabas.smarthouse.server.entity.alarm;

import java.util.List;

import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.IUpdateable;

@SuppressWarnings("rawtypes")
public interface IEntityAlarm extends IUpdateable {
	
	IDevice getDevice();
	void setDevice(IDevice device);

	IEntity getEntity();
	void setEntity(IEntity entity);
	
	List<IAlarm> getAlarms();
	void setAlarms(List<IAlarm> alarms);
	
	void putEntityFieldAlarm(IAlarm entityFieldAlarm);
	
	boolean isAlarmed();
	
	boolean check();
	
	boolean isAlarmStarted();
	boolean isAlarmFinished();
	
	String getAlarmText();
	
	boolean isActive();
	
	void setNotified(boolean notified);
	
}
