package com.balabas.smarthouse.server.entity.alarm;

import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.view.MessageHolder;

@SuppressWarnings("rawtypes")
public interface IEntityFieldAlarm<T> extends IAlarm<IEntityField> {

	boolean acceptsValueAsWatched(Class<?> candidateClass);
	
	IEntityField getWatchedItem();
	void setWatchedItem(IEntityField watchedItem);
	
	IEntityAlarm getEntityAlarm();
	void setEntityAlarm(IEntityAlarm entityAlarm);
	
	String getCompareSeparator();
	
	String getTriggerDescription();
	
	void setValue(T value);
	T getValue();
	
	void setValueStr(String value);
	String getValueStr();

	MessageHolder getAlarmStartedTextHolder();
}
