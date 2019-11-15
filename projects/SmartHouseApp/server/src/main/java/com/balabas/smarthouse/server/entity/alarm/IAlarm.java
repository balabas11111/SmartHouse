package com.balabas.smarthouse.server.entity.alarm;


import com.balabas.smarthouse.server.entity.model.IItemAbstract;

public interface IAlarm<T extends IItemAbstract>{

	Long getId();
	
	default String getClassSimpleName() {
		return this.getClass().getSimpleName();
	}
	
	boolean check();
	
	T getWatchedItem();
	void setWatchedItem(T watchedItem);
	
	boolean isActivated();
	void setActivated(boolean activated);
	
	boolean isActive();
	boolean isAlarmed();
	
	String getAlarmText();
}
