package com.balabas.smarthouse.server.entity.alarmV2;

import com.balabas.smarthouse.server.entity.model.IIdentifiable;
import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.balabas.smarthouse.server.view.MessageHolder;

public interface IAlarmV2 extends IIdentifiable {

	Long getItemId();
	
	Class<?> getItemClass();
	
	IItemAbstract getItem();
	void setItem(IItemAbstract item);
	
	boolean isValueValid(String value);
	
	String getValue();
	void setValue(String value);
	
	boolean isAlarmed();
	
	void check();
	
	void check(IItemAbstract item);
	
	void setMessageInterval(Integer messageInterval);
	Integer getMessageInterval();
	
	
	boolean isAlarmStartedSendExpected();
	boolean isAlarmFinishedSendExpected();
	
	MessageHolder getAlarmStartedTextHolder(IItemAbstract item);
	MessageHolder getAlarmFinishedTextHolder(IItemAbstract item);
	
}
