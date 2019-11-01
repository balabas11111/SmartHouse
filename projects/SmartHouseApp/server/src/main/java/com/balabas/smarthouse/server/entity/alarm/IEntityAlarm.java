package com.balabas.smarthouse.server.entity.alarm;

import java.util.List;

import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.IUpdateable;

@SuppressWarnings("rawtypes")
public interface IEntityAlarm extends IUpdateable {
	
	Long getId();
	
	IDevice getDevice();

	IEntity getEntity();
	void setEntity(IEntity entity);
	
	List<IEntityFieldAlarm> getAlarms();
	void setAlarms(List<IEntityFieldAlarm> alarms);
	
	void putAlarm(IEntityFieldAlarm entityFieldAlarm);
	
	boolean isActivated();
	void setActivated(boolean activated);
	
	boolean isAlarmed();
	boolean isSound();
	void setSound(boolean sound);
	
	boolean check();
	
	boolean isAlarmStarted();
	boolean isAlarmFinished();
	
	boolean isActive();
	
	void setAlarmStartedSent(boolean notified);
	void setAlarmFinishedSent(boolean notified);
	
	boolean isSendAlarmStartedMessage();
	boolean isSendAlarmFinishedMessage();
	
	void setSendAlarmStartedMessage(boolean sendAlarmStartedMessage);
	void setSendAlarmFinishedMessage(boolean sendAlarmFinishedMessage);
	
	String getAlarmStartedText();
	String getAlarmFinishedText();
	
	void setMessageInterval(Integer messageInterval);
	Integer getMessageInterval();
	
	boolean isNotificationRepeatable();
	void setLogAlarmCheck(boolean logAlarmCheck);
	
}
