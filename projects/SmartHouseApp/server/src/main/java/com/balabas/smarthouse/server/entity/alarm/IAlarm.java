package com.balabas.smarthouse.server.entity.alarm;


import com.balabas.smarthouse.server.entity.model.IItemAbstract;

public interface IAlarm<T extends IItemAbstract, O> extends IItemAbstract{

	boolean acceptsAsWatched(T entityField);
	
	boolean check();
	
	void setValue(O value);
	O getValue();
	
	T getWatchedItem();
	void setWatchedItem(T watchedItem);
	
	boolean isActive();
	boolean isAlarmed();
	
	String getAlarmText();
}
