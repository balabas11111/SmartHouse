package com.balabas.smarthouse.server.entity.alarmV2;

import java.util.Collection;
import java.util.List;
import java.util.Map;

import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.balabas.smarthouse.server.entity.model.entityfields.EntityFieldValue;

public interface IAlarmV2Service {

	void checkForAlarmsWithParentExecutePostActions(List<EntityFieldValue> changedValues);
	
	List<IAlarmStateChangeEvent> checkForAlarmsExecutePostActions(Collection<IItemAbstract> items);
	List<IAlarmStateChangeEvent> checkForAlarmsExecutePostActions(IItemAbstract item);
	
	List<Class<?>> getEnabledAlarms(IItemAbstract item);
	
	void saveAlarm(IAlarmV2 alarm);
	
	void deleteAlarm(IAlarmV2 alarm);
	
	List<IAlarmV2> getAllAlarms();

	List<IAlarmV2> getAlarmsForItemClass(Class<?> clazz);
	
	Map<String, List<IAlarmV2>> getAlarmsGrouppedByItemClassName();
	
	List<IAlarmV2> getAlarmsByAlarmState(AlarmState alarmState);
	
	Map<AlarmState, List<IAlarmV2>> getAlarmsGrouppedByAlarmState();
}
