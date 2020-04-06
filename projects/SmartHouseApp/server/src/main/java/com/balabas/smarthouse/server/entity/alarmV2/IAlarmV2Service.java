package com.balabas.smarthouse.server.entity.alarmV2;

import java.util.Collection;
import java.util.List;
import java.util.Map;
import java.util.Set;

import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.balabas.smarthouse.server.entity.model.ItemAbstractDto;
import com.balabas.smarthouse.server.entity.model.descriptor.ItemType;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityFieldValue;

@SuppressWarnings("rawtypes")
public interface IAlarmV2Service {

	void checkForAlarmsWithParentExecutePostActions(List<IEntityFieldValue> changedValues);
	
	//void checkForAlarmsWithParentExecutePostActionsForFields(Collection<IEntityField> changedValues);
	
	List<IItemEvent> checkForAlarmsExecutePostActions(Collection<IItemAbstract> items);
	List<IItemEvent> checkForAlarmsExecutePostActions(IItemAbstract item);
	
	List<AlarmV2Checker> getEnabledAlarmCheckers(IItemAbstract item);
	
	void saveAlarm(IAlarmV2 alarm);
	
	void deleteAlarm(IAlarmV2 alarm);
	
	List<IAlarmV2> getAllAlarms();

	List<IAlarmV2> getAlarmsForItemClass(Class<?> clazz);
	
	Map<String, List<IAlarmV2>> getAlarmsGrouppedByItemClassName();
	
	List<IAlarmV2> getAlarmsByAlarmState(AlarmState alarmState);
	
	Map<AlarmState, List<IAlarmV2>> getAlarmsGrouppedByAlarmState();

	IAlarmV2 getAlarmOrDefault(Long id, IAlarmV2 def);

	void deleteAlarm(Long id);

	List<AlarmV2Checker> getCheckersByTargetItemClass(Class<?> targetItemClass);

	IAlarmV2 newAlarm(ItemType itemType);

	Set<IItemAbstract> getEnabledAlarmTargets(IAlarmV2 alarm);
	
	IItemAbstract getEnabledAlarmTarget(Long itemId, Class<?> itemClass);
	
	void createOrUpdateAlarm(IAlarmV2 alarm, Long itemId);

	IAlarmV2Repository getRepository(IAlarmV2 alarm);

	Set<IAlarmStateChangeEventProcessor> getAlarmStateChangedEventProcessors(IAlarmV2 alarm);

	void addAlarmStateChangeActionToAlarm(Long alarmId, ItemType it, AlarmStateChangeAction action);

	void deleteAlarmStateChangeActionFromAlarm(Long alarmId, ItemType it, Long actionId);

	IAlarmV2 getAlarm(Long id, ItemType it);

	Map<String, List<AlarmV2>> getAlarmsMap(List<IItemAbstract> items, List<AlarmState> states,
			boolean withStateDescriptionOnly);

	boolean isAlarmInState(IAlarmV2 alarm, List<AlarmState> states);

	boolean attachAlarmDescriptions(IAlarmV2 alarm);

	List<IAlarmV2> getAlarmsForItemClassWithDescriptions(Class<?> clazz);

	Map<String, List<IAlarmV2>> getAlarmsGrouppedByItemClassNameWithAlarmDescriptions();

	AlarmV2Container getAlarmsContainerWithChildren(IItemAbstract parent);

	void checkForAlarmsWithParentExecutePostActionsForFields(List<IEntityField> changedValues);

	List<ItemAbstractDto> getEnabledAlarmAbstractTargets(IAlarmV2 alarm);

	List<IItemEvent> checkForAlarmsExecutePostActions(IAlarmV2 alarm);

}
