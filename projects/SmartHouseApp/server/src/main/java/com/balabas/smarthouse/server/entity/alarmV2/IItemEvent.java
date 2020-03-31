package com.balabas.smarthouse.server.entity.alarmV2;

import java.util.Date;

import com.balabas.smarthouse.server.entity.model.IItemAbstract;

public interface IItemEvent {

	Long getAlarmId();
	void setAlarmId(Long alarmId);
	
	Date getDate();
	
	IItemAbstract getItem();
	
	IAlarmStateChangeAction getChangeAction();
	
	default String getDescription() {
		return getChangeAction().getAlarmDescription(getItem());
	}
	
	default String executeAction() {
		return getChangeAction().executeAction(getItem());
	}
	
	default AlarmState getOldState() {
		return getChangeAction().getOldState();
	}
	
	default AlarmState getNewState() {
		return getChangeAction().getNewState();
	}
	
	default String getOldStateName() {
		return getChangeAction().getOldState().name;
	}
	
	default String getNewStateName() {
		return getChangeAction().getNewState().name;
	}
	
	default boolean isSameState() {
		return getOldState().equals(getNewState());
	}
	
	default String getItemUid() {
		return "IItemEvent"+ getAlarmId(); 
	}
}
