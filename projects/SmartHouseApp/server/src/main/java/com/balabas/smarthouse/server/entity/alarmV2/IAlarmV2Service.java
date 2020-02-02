package com.balabas.smarthouse.server.entity.alarmV2;

import java.util.Collection;
import java.util.List;

import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.balabas.smarthouse.server.entity.model.entityfields.EntityFieldValue;

public interface IAlarmV2Service {

	void checkForAlarmsWithParent(List<EntityFieldValue> changedValues);
	
	List<IAlarmStateChangeEvent> checkForAlarmsExecutePostActions(Collection<IItemAbstract> items);
	List<IAlarmStateChangeEvent> checkForAlarmsExecutePostActions(IItemAbstract item);
	
	List<Class<?>> getEnabledAlarms(IItemAbstract item);
	
	void saveAlarm(IAlarmV2 alarm);
	
	void deleteAlarm(IAlarmV2 alarm);

}
