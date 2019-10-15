package com.balabas.smarthouse.server.entity.alarm;


import com.balabas.smarthouse.server.entity.model.IItemAbstract;

public interface IAlarm<T extends IItemAbstract> extends IItemAbstract {

	boolean accepts(T entityField);
	
	void setEntity(T entity);
	T getEntity();
	
	boolean isActive();
	boolean isAlarmed();
	
}
