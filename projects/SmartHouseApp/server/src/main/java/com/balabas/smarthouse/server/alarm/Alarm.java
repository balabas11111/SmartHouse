package com.balabas.smarthouse.server.alarm;

import java.util.Date;
import java.util.List;

import com.balabas.smarthouse.server.model.NameAble;

public interface Alarm<T extends NameAble> {

	T getItem();
	
	List<String> getMessages();
	
	Date getCreateDate();
	
	Date getLastNotifiedDate();
	
	Date getNextNotifyDateTime();
	
	boolean isActive();
	
	boolean check(T item);
}
