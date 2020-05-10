package com.balabas.smarthouse.server.entity.alarmV2.service;

import java.util.Collection;
import java.util.Date;
import java.util.List;
import java.util.Map;
import java.util.Set;

import com.balabas.smarthouse.server.entity.alarmV2.AlarmV2Checker;
import com.balabas.smarthouse.server.entity.alarmV2.AlarmV2Container;
import com.balabas.smarthouse.server.entity.alarmV2.IAlarmStateChangeEventProcessor;
import com.balabas.smarthouse.server.entity.alarmV2.IItemEvent;
import com.balabas.smarthouse.server.entity.alarmV2.model.AlarmState;
import com.balabas.smarthouse.server.entity.alarmV2.model.AlarmStateChangeAction;
import com.balabas.smarthouse.server.entity.alarmV2.model.AlarmV2;
import com.balabas.smarthouse.server.entity.alarmV2.model.IAlarmV2;
import com.balabas.smarthouse.server.entity.alarmV2.repository.IAlarmV2Repository;
import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.balabas.smarthouse.server.entity.model.ItemAbstractDto;
import com.balabas.smarthouse.server.entity.model.descriptor.ItemType;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityFieldValue;
import com.balabas.smarthouse.server.view.chart.ChartDataSeries;

@SuppressWarnings("rawtypes")
public interface IAlarmV2Service {

	void checkForAlarmsWithParentExecutePostActions(List<IEntityFieldValue> changedValues);
	
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

	List<AlarmV2Checker> getCheckersByTargetItemClass(Class<?> targetItemClass);

	IAlarmV2 newAlarm(ItemType itemType);

	Set<IItemAbstract> getEnabledAlarmTargets(IAlarmV2 alarm);
	
	IItemAbstract getEnabledAlarmTarget(Long itemId, Class<?> itemClass);
	
	void createOrUpdateAlarm(IAlarmV2 alarm, Long targetId);

	IAlarmV2Repository getRepository(IAlarmV2 alarm);

	Set<IAlarmStateChangeEventProcessor> getAlarmStateChangedEventProcessors(IAlarmV2 alarm);

	void addAlarmStateChangeActionToAlarm(Long alarmId, ItemType it, AlarmStateChangeAction action);

	void deleteAlarmStateChangeActionFromAlarm(Long alarmId, ItemType itemType, Long actionId);

	IAlarmV2 getAlarm(Long id, ItemType it);

	Map<String, List<AlarmV2>> getAlarmsMap(List<IItemAbstract> items, List<AlarmState> states,
			boolean withStateDescriptionOnly);

	boolean isAlarmInState(IAlarmV2 alarm, List<AlarmState> states);

	boolean attachAlarmDescriptions(IAlarmV2 alarm);

	List<IAlarmV2> getAlarmsForItemClassWithDescriptions(Class<?> clazz);

	Map<String, List<IAlarmV2>> getAlarmsGrouppedByItemClassNameWithAlarmDescriptions();
	
	Map<String, List<IAlarmV2>> getAlarmsGrouppedByItemTypeWithAlarmDescriptions();

	AlarmV2Container getAlarmsContainerWithChildren(IItemAbstract parent);

	void checkForAlarmsWithParentExecutePostActionsForFields(List<IEntityField> changedValues);

	List<IItemEvent> checkForAlarmsExecutePostActions(IAlarmV2 alarm);

	Map<ItemType, Map<AlarmState, List<IAlarmV2>>> getAlarmsAsMap();

	Map<ItemType, String> getItemMapNames();

	Map<ItemType, Map<AlarmState, List<IAlarmV2>>> getAlarmsAsMap(AlarmState maxState, boolean only);

	String getAlarmsIconsString(AlarmState state, boolean only);

	Map<ItemType, Map<AlarmState, List<IAlarmV2>>> getAlarmsAsMap(List<IAlarmV2> alarmList, AlarmState maxState,
			boolean only);

	Map<ItemType, Map<AlarmState, List<IAlarmV2>>> getAlarmsAsMap(IItemAbstract item, AlarmState maxState,
			boolean only);

	List<IAlarmV2> getAlarmsWithItemChildren(IItemAbstract item);
	
	List<IAlarmV2> getAlarmsWithItemChildren(Long itemId, ItemType itemItemType);

	List<IAlarmV2> getAlarms(List<IItemAbstract> items);
	
	default IAlarmV2 getAlarm(Long id, String itemTypeName) {
		ItemType it = ItemType.getItemTypeByName(itemTypeName);
		return getAlarm(id, it);
	}

	Map<ItemType, Map<AlarmState, List<IAlarmV2>>> getAlarmsAsMap(Long itemId, ItemType itemType, AlarmState maxState,
			boolean only);

	List<ItemAbstractDto> getEnabledAlarmAbstractTargets(IAlarmV2 alarm);
	
	Set<IItemAbstract> getEnabledAlarmTargets(ItemType itemType);

	void executePostponed(IAlarmV2 alarm);

	IAlarmV2 getAlarm(Long id);

	List<ChartDataSeries> getAlarmStates(IAlarmV2 alarm, Date date1, Date date2);

	List<ChartDataSeries> getAlarmStatesOrdered(IAlarmV2 alarm, Date date1, Date date2);

}
