package com.balabas.smarthouse.server.entity.alarm;


import com.balabas.smarthouse.server.entity.model.IItemAbstract;

public interface IAlarm<T extends IItemAbstract, O>{

	Long getId();
	
	boolean acceptsAsWatched(T entityField);
	
	boolean check();
	
	void setValue(O value);
	O getValue();
	
	T getWatchedItem();
	void setWatchedItem(T watchedItem);
	
	boolean isActivated();
	void setActivated(boolean activated);
	
	boolean isActive();
	boolean isAlarmed();
	
	String getAlarmText();
}
