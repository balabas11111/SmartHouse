package com.balabas.smarthouse.server.entity.alarm;


import com.balabas.smarthouse.server.entity.model.IEntityAbstract;

public interface IAlarm<T extends IEntityAbstract> extends IEntityAbstract {

	boolean accepts(T entityField);
	
	void setEntity(T entity);
	T getEntity();
	
	boolean isActive();
	boolean isAlarmed();
	
	IAlarmListener getListener();
	void setListener(IAlarmListener listener);
}
