package com.balabas.smarthouse.server.entity.alarmV2;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.balabas.smarthouse.server.util.StringUtil;

public interface IItemAlarmStateService {

	// itemAbstract UID --> alarm UID --> list of messages
	Map<String, Map<String, List<String>>> getStateDescriptions();

	default String getKey(IItemEvent event) {
		return event.getItemUid();
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

	default List<String> getStateDescriptions(IItemEvent item) {
		String key = item.getItemUid();
		Map<String, List<String>> stateDescriptions = getStateDescriptions(item.getItem());

		if (!stateDescriptions.containsKey(key)) {
			stateDescriptions.put(key, new ArrayList<>());
		}

		return stateDescriptions.get(key);
	}

	default Map<String, List<String>> getStateDescriptions(IItemAbstract item) {
		String key = getKey(item);
		Map<String, Map<String, List<String>>> stateDescriptions = getStateDescriptions();

		if (!stateDescriptions.containsKey(key)) {
			stateDescriptions.put(key, new HashMap<>());
		}

		return stateDescriptions.get(key);
	}
	
	default void printStateDescription(IItemEvent event) {
		List<String> descr = getStateDescriptions(event);
		StringUtil.printList(descr);
	}
	
	default void printStateDescription(IItemAbstract item) {
		Map<String, List<String>> descr = getStateDescriptions(item);
		StringUtil.printMap(descr);
	}

}
