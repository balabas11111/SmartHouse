package com.balabas.smarthouse.server.entity.alarmV2;

import com.balabas.smarthouse.server.entity.model.IIdentifiable;
import com.balabas.smarthouse.server.entity.model.IItemAbstract;

public interface IAlarmV2 extends IIdentifiable {

	Long getItemId();
	
	Class<?> getItemClass();
	
	IItemAbstract getItem();
	void setItem(IItemAbstract item);
	
	boolean isValueValid(String value);
	
	String getValue();
	void setValue(String value);
	
	boolean accepts(IItemAbstract item);
	
	boolean isAlarmed();
	
	void check();
	
	void setMessageInterval(Integer messageInterval);
	Integer getMessageInterval();
	
	
	boolean isAlarmStartedSendExpected();
	boolean isAlarmFinishedSendExpected();
	/*
	MessageHolder getAlarmStartedTextHolder();
	MessageHolder getAlarmFinishedTextHolder();
	*/
}
