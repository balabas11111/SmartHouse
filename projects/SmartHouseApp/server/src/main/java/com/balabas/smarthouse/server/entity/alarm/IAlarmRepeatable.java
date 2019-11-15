package com.balabas.smarthouse.server.entity.alarm;

import com.balabas.smarthouse.server.entity.model.ActionTimer;
import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.balabas.smarthouse.server.view.MessageHolder;

public interface IAlarmRepeatable<T extends IItemAbstract> extends IAlarm<T> {

	IDevice getDevice();
	
	ActionTimer getTimer();
	void setTimer(ActionTimer timer);
	
	boolean isSound();
	void setSound(boolean sound);
	
	boolean isAlarmStarted();
	boolean isAlarmFinished();
	
	void setAlarmStartedSent(boolean notified);
	void setAlarmFinishedSent(boolean notified);
	
	boolean isSendAlarmStartedMessage();
	boolean isSendAlarmFinishedMessage();
	
	void setSendAlarmStartedMessage(boolean sendAlarmStartedMessage);
	void setSendAlarmFinishedMessage(boolean sendAlarmFinishedMessage);
	
	MessageHolder getAlarmStartedTextHolder();
	MessageHolder getAlarmFinishedTextHolder();
	
	void setMessageInterval(Integer messageInterval);
	Integer getMessageInterval();
	
	boolean isNotificationRepeatable();
	void setLogAlarmCheck(boolean logAlarmCheck);
}
