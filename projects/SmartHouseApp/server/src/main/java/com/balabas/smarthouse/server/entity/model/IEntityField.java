package com.balabas.smarthouse.server.entity.model;

import java.util.Set;

import com.balabas.smarthouse.server.entity.alarm.IEntityFieldAlarm;
import com.balabas.smarthouse.server.exception.BadValueException;

@SuppressWarnings("rawtypes")
public interface IEntityField<T extends Object> extends IEntityAbstract {

	Class getClazz();
	
	T getValue();
	
	String getValueStr();
	
	void setValueStr(String valueStr) throws BadValueException;
	
	void setValue(T value) throws BadValueException;

	Set<IEntityFieldAlarm> getAlarms();
	
	void setAlarms(Set<IEntityFieldAlarm> alarms);
	
	IEntityFieldChangeListener getWatcher();
	
	void setWatcher(IEntityFieldChangeListener watcher);
}
