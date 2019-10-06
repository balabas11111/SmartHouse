package com.balabas.smarthouse.server.alarm;

import java.util.Date;
import java.util.List;

import com.balabas.smarthouse.server.model.SmartHouseItem;
import com.balabas.smarthouse.server.notification.Message;
import com.balabas.smarthouse.server.notification.Notification;

public interface Alarm<T extends SmartHouseItem> {

	Class<T> getClazz();
	
	T getItem();
	
	public void activate(Long alarmIntervalSec, T item, String descriptionHead) ;
	
	Notification<T> checkItemForAlarmAndReschedule();
	
	void setItem(T item) throws IllegalArgumentException;
	
	List<Message> getMessages();
	
	Date getNotifyDate();
	
	boolean isActive();
	
	void setActive(boolean active);
	
	void setNotifyIntervalSec(Long sec);
	
	boolean isAlarmDetected();
	
	boolean checkItemForAlarm();
	
	String getDeviceId();
	
	String getItemName();
	
	boolean sendRequired();
	
	void reschedule();
	
	String getMessageView();
	
}
