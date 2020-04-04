package com.balabas.smarthouse.server.entity.alarmV2;

import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.google.common.collect.Lists;

@SuppressWarnings({ "rawtypes", "unchecked" })
public class AlarmV2Container {

	private static final AlarmState[] stateOrders = { AlarmState.ALARM, AlarmState.WARNING, AlarmState.OK };

	private Map<String, Map<AlarmState, Map<Long, IAlarmV2>>> alarms = new HashMap();

	public void clear() {
		alarms = new HashMap();
	}

	public void putAlarm(IAlarmV2 alarm) {
		String itemUid = alarm.getItem().getItemUid();
		AlarmState alarmState = alarm.getAlarmState();

		if (!alarms.containsKey(itemUid)) {
			alarms.put(itemUid, new HashMap<AlarmState, Map<Long, IAlarmV2>>());
		}

		Map<AlarmState, Map<Long, IAlarmV2>> byStateMap = alarms.get(itemUid);

		if (!byStateMap.containsKey(alarmState)) {
			byStateMap.put(alarmState, new LinkedHashMap<Long, IAlarmV2>());
		}

		Map<Long, IAlarmV2> alarmMap = byStateMap.get(alarmState);

		alarmMap.put(alarm.getId(), alarm);
	}

	public List<IAlarmV2> getAlarms(IItemAbstract item, AlarmState state) {
		return getAlarms(item.getItemUid(), state);
	}

	public List<IAlarmV2> getAlarms(String itemUid, AlarmState state) {
		if (alarms.containsKey(itemUid) && alarms.get(itemUid).containsKey(state)) {
			return alarms.get(itemUid).get(state).values().stream().collect(Collectors.toList());
		}

		return Lists.newArrayList();
	}

	public boolean hasEmojiToDisplay(IItemAbstract item) {
		String state = getEmojiToDisplay(item);
		
		return state!=null && state.isEmpty();
	}
	
	public String getEmojiToDisplay(IItemAbstract item) {
		AlarmState state = getMaxState(item);

		if (state == null) {
			return "";
		}
		return state.emoji.toString();
	}

	public String getHintToDisplay(IItemAbstract item) {
		StringBuffer result = new StringBuffer();
		for (AlarmState state : stateOrders) {
			List<IAlarmV2> alarmList = getAlarms(item, state);
			if (alarmList != null && !alarmList.isEmpty()) {
				result.append(state.getEmojiDescription());
				result.append(" ");
				for(IAlarmV2 alarm: alarmList) {
					result.append(alarm.getStateDescriptionsWithNextLine());
				}
			}
		}
		return result.toString();
	}

	public AlarmState getMaxState(IItemAbstract item) {
		AlarmState result = null;

		for (AlarmState state : stateOrders) {
			List<IAlarmV2> alarmList = getAlarms(item, state);
			if (alarmList != null && !alarmList.isEmpty()) {
				return state;
			}
		}

		return result;
	}

}
