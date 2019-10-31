package com.balabas.smarthouse.server.entity.alarm;

import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;

@SuppressWarnings("rawtypes")
public interface IEntityFieldAlarm<T> extends IAlarm<IEntityField, T> {

	IEntityField getWatchedItem();
	void setWatchedItem(IEntityField entityField);
	
	IEntityAlarm getEntityAlarm();
	
	void setEntityAlarm(IEntityAlarm entityAlarm);
	
	T getValue();
	String getValueStr();
	void setValue(T value);
	
	String getClassSimpleName();
	String getCompareSeparator();
	
	String getTriggerDescription();
}
