package com.balabas.smarthouse.server.entity.alarmV2;

import java.util.Date;

import com.balabas.smarthouse.server.entity.alarmV2.model.AlarmState;
import com.balabas.smarthouse.server.entity.alarmV2.model.IAlarmStateChangeAction;
import com.balabas.smarthouse.server.entity.alarmV2.model.IAlarmV2;
import com.balabas.smarthouse.server.entity.model.IItemAbstract;

public interface IItemEvent {

	IAlarmV2 getAlarm();
	void setAlarm(IAlarmV2 alarm);
	
	Date getDate();
	
	IItemAbstract getItem();
	
	IAlarmStateChangeAction getChangeAction();
	
	default String getDescription() {
		return getChangeAction().getAlarmDescription(getItem(), true);
	}
	
	default String executeAction() {
		return getChangeAction().executeAction(getItem());
	}
	
	default AlarmState getOldState() {
		AlarmState state = getChangeAction().getOldState();
		if(AlarmState.ANY.equals(state)) {
			state = getAlarm().getPreviousAlarmState();
		}
		return state;
	}
	
	default AlarmState getNewState() {
		AlarmState state = getChangeAction().getNewState();
		if(AlarmState.ANY.equals(state)) {
			state = getAlarm().getAlarmState();
		}
		return state;
	}
	
	default String getOldStateName() {
		return getOldState().getName();
	}
	
	default String getNewStateName() {
		return getNewState().getName();
	}
	
	default boolean isSameState() {
		return getOldState().equals(getNewState());
	}
	
	default String getUid() {
		return getAlarm().getUid();
	}
}
