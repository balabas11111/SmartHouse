package com.balabas.smarthouse.server.entity.alarmV2;

import java.util.Set;

import com.balabas.smarthouse.server.entity.model.IIdentifiable;
import com.balabas.smarthouse.server.entity.model.IItemAbstract;

public interface IAlarmV2 extends IIdentifiable {

	Long getItemId();
	
	Class<?> getItemClass();
	
	IItemAbstract getItem();
	void setItem(IItemAbstract item);
	
	AlarmState getAlarmState();
	void setAlarmState(AlarmState alarmState);
	
	AlarmState getPreviousAlarmState();
	void setPreviousAlarmState(AlarmState previousAlarmState);
	
	boolean isValueValid(String value);
	
	String getValue();
	void setValue(String value);
	
	boolean isInBadState();
	boolean isRepeatable();
	
	boolean check(IItemAbstract item);
	
	void setMessageInterval(Integer messageInterval);
	Integer getMessageInterval();
	
	Set<IAlarmStateChangeAction> getActions();
	void setActions(Set<IAlarmStateChangeAction> actions);
	
	IAlarmStateChangeAction getCurrentAction();

	default boolean check() {
		return check(getItem());
	};
	
}
