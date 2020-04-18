package com.balabas.smarthouse.server.entity.alarmV2;


import com.balabas.smarthouse.server.entity.alarmV2.model.IAlarmV2;
import com.balabas.smarthouse.server.entity.model.IEntity;

public abstract class AlarmV2CheckerAbstractEntity {

	protected abstract boolean checkItemValue(IAlarmV2 alarm);
	
	public Class<?> getItemClass() {
		return IEntity.class;
	}
	
	public void process(IAlarmV2 alarm) {
		alarm.setAlarmStateByBooleanFlagAlarm(checkItemValue(alarm));
	}
	
	protected IEntity getItemAsEntity(IAlarmV2 alarm) {
		return (IEntity) alarm.getItem();
	}
	
}
