package com.balabas.smarthouse.server.entity.alarmV2;

import java.util.Collection;
import java.util.List;
import java.util.Map;
import java.util.Set;

import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.balabas.smarthouse.server.entity.model.descriptor.ItemType;
import com.balabas.smarthouse.server.entity.model.entityfields.EntityFieldValue;

public interface IAlarmV2Service {

	void checkForAlarmsWithParentExecutePostActions(List<EntityFieldValue> changedValues);
	
	List<IAlarmStateChangeEvent> checkForAlarmsExecutePostActions(Collection<IItemAbstract> items);
	List<IAlarmStateChangeEvent> checkForAlarmsExecutePostActions(IItemAbstract item);
	
	List<AlarmV2Checker> getEnabledAlarmCheckers(IItemAbstract item);
	
	void saveAlarm(IAlarmV2 alarm);
	
	void deleteAlarm(IAlarmV2 alarm);
	
	List<IAlarmV2> getAllAlarms();

	List<IAlarmV2> getAlarmsForItemClass(Class<?> clazz);
	
	Map<String, List<IAlarmV2>> getAlarmsGrouppedByItemClassName();
	
	List<IAlarmV2> getAlarmsByAlarmState(AlarmState alarmState);
	
	Map<AlarmState, List<IAlarmV2>> getAlarmsGrouppedByAlarmState();

	IAlarmV2 getAlarm(Long id);

	void deleteAlarm(Long id);

	List<AlarmV2Checker> getCheckersByTargetItemClass(Class<?> targetItemClass);

	IAlarmV2 newAlarm(ItemType itemType);

	Set<IItemAbstract> getEnabledAlarmTargets(IAlarmV2 alarm);
}
