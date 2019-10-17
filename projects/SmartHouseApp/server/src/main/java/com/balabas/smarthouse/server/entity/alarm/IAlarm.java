package com.balabas.smarthouse.server.entity.alarm;


import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.balabas.smarthouse.server.entity.model.IUpdateable;

public interface IAlarm<T extends IItemAbstract, O> extends IItemAbstract, IUpdateable {

	boolean acceptsAsWatched(T entityField);
	
	boolean check();
	
	void setValue(O value);
	O getValue();
	
	boolean isActive();
	boolean isAlarmed();
	
	String getAlarmText();
}
