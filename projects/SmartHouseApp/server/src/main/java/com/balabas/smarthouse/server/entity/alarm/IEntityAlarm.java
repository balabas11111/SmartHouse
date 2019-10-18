package com.balabas.smarthouse.server.entity.alarm;

import java.util.List;

import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.IUpdateable;

@SuppressWarnings("rawtypes")
public interface IEntityAlarm extends IUpdateable {
	
	void setDeviceName(String deviceName);
	String getDeviceName();
	
	void setEntityName(String entityName);
	String getEntityName();
	
	IDevice getDevice();
	void setDevice(IDevice device);

	IEntity getEntity();
	void setEntity(IEntity entity);
	
	List<IAlarm> getAlarms();
	void setAlarms(List<IAlarm> alarms);
	
	void putAlarm(IAlarm entityFieldAlarm);
	
	boolean isAlarmed();
	
	boolean check();
	
	boolean isAlarmStarted();
	boolean isAlarmFinished();
	
	boolean isActive();
	
	void setAlarmStartedSent(boolean notified);
	void setAlarmFinishedSent(boolean notified);
	
	String getAlarmStartedText();
	String getAlarmFinishedText();
	
	void setSingleAlarmMessage(boolean singleAlarmMessage);
	 
	
	
	
}
