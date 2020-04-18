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
		return event.getUid();
	}

	default String getKey(IItemAbstract item) {
		return item.getItemUid();
	}

	default void removeAllStateDescription(IItemEvent event) {
		Map<String, List<String>> stateDescriptions = getStateDescriptions(event.getItem());
		String key = getKey(event);

		if (!stateDescriptions.containsKey(key)
				|| (stateDescriptions.containsKey(key) && stateDescriptions.get(key).size() > 0)) {
			stateDescriptions.put(key, new ArrayList<>());
		}
	}

	default void addStateDescription(IItemEvent event) {
		String message = event.getDescription();
		getStateDescriptions(event).add(message);
	}

	default void putSingleStateDescription(IItemEvent event) {
		removeAllStateDescription(event);
		addStateDescription(event);
	}

	default List<String> getStateDescriptions(IItemEvent event) {
		return getStateDescriptions(event.getAlarm().getUid(), event.getItem().getItemUid());
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
