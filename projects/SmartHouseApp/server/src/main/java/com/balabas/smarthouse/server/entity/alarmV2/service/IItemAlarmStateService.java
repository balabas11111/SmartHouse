package com.balabas.smarthouse.server.entity.alarmV2.service;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.balabas.smarthouse.server.entity.alarmV2.IItemEvent;
import com.balabas.smarthouse.server.entity.alarmV2.model.IAlarmV2;
import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.balabas.smarthouse.server.util.StringUtil;

public interface IItemAlarmStateService {

	// itemAbstract UID --> alarm UID --> list of messages
	Map<String, Map<String, List<String>>> getStateDescriptions();

	default String getKey(IItemEvent event) {
		return getKey(event.getAlarm());
	}
	
	default String getKey(IAlarmV2 alarm) {
		return alarm.getUid();
	}

	default String getKey(IItemAbstract item) {
		return item.getItemUid();
	}

	default void removeAllStateDescription(IItemEvent event) {
		event.getAlarm().setItem(event.getItem());
		removeAllStateDescription(event.getAlarm());
	}
	
	default void removeAllStateDescription(IAlarmV2 alarm) {
		Map<String, List<String>> stateDescriptions = getStateDescriptions(alarm.getItem());
		String key = getKey(alarm);

		if (!stateDescriptions.containsKey(key)
				|| (stateDescriptions.containsKey(key) && stateDescriptions.get(key).size() > 0)) {
			stateDescriptions.put(key, new ArrayList<>());
		}
	}

	default void addStateDescription(IAlarmV2 alarm) {
		getStateDescriptionsByAlarm(alarm).add(alarm.getCurrentActionEmojiDescription());
	}
	
	default void addStateDescription(IItemEvent event) {
		String message = event.getDescription();
		getStateDescriptions(event).add(message);
	}

	default void putSingleStateDescription(IItemEvent event) {
		removeAllStateDescription(event);
		addStateDescription(event);
	}
	
	default void putSingleStateDescription(IAlarmV2 alarm) {
		removeAllStateDescription(alarm);
		addStateDescription(alarm);
	}

	default List<String> getStateDescriptions(IItemEvent event) {
		return getStateDescriptions(event.getAlarm().getUid(), event.getItem().getItemUid());
	}
	
	default List<String> getStateDescriptionsByAlarm(IAlarmV2 alarm) {
		return getStateDescriptions(alarm.getUid(), alarm.getItem().getItemUid());
	}
	
	default List<String> getStateDescriptions(String alarmUid, String itemUid) {
		Map<String, List<String>> stateDescriptions = getStateDescriptions(itemUid);

		if (!stateDescriptions.containsKey(alarmUid)) {
			stateDescriptions.put(alarmUid, new ArrayList<>());
		}

		return stateDescriptions.get(alarmUid);
	}

	default Map<String, List<String>> getStateDescriptions(IItemAbstract item) {
		return getStateDescriptions(item.getItemUid());
	}
	
	default Map<String, List<String>> getStateDescriptions(String itemUid) {
		Map<String, Map<String, List<String>>> stateDescriptions = getStateDescriptions();

		if (!stateDescriptions.containsKey(itemUid)) {
			stateDescriptions.put(itemUid, new HashMap<>());
		}

		return stateDescriptions.get(itemUid);
	}
	
	default void printStateDescription(IItemEvent event) {
		List<String> descr = getStateDescriptions(event);
		StringUtil.printList(descr);
	}
	
	default void printStateDescription(IItemAbstract item) {
		Map<String, List<String>> descr = getStateDescriptions(item);
		StringUtil.printMap(descr);
	}

	default List<String> getStateDescriptions(IAlarmV2 alarm) {
		List<String> result = getStateDescriptions(alarm.getUid(), alarm.getItem().getItemUid());
		alarm.setStateDescriptions(result);
		return result;
	}

}
