package com.balabas.smarthouse.server.entity.alarmV2;

import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.balabas.smarthouse.server.entity.model.ItemAbstractDto;
import com.google.common.collect.Lists;

@SuppressWarnings({ "rawtypes", "unchecked" })
public class AlarmV2Container {

	private static final AlarmState[] stateOrders = { AlarmState.ALARM, AlarmState.WARNING, AlarmState.OK, AlarmState.NO_DATA };

	private static final List<AlarmState> STATES_ORDERS_LIST = Arrays.asList(stateOrders);
	
	private Map<String, Map<AlarmState, Map<Long, IAlarmV2>>> alarms = new HashMap();

	public void clear() {
		alarms = new HashMap();
	}
	
	public String getAlarmStatistic() {
		Map<AlarmState, Integer> states = getStateCounts();
		
		StringBuffer buf = new StringBuffer();
		
		STATES_ORDERS_LIST.forEach(state -> {
			if(states.containsKey(state) && states.get(state)!=0) {
				append(buf, state.emoji.toString(), states.get(state));
			}
		});
		
		return buf.toString();
	}
	
	public String getAlarmStatisticHint() {
		Map<AlarmState, Integer> states = getStateCounts();
		
		StringBuffer buf = new StringBuffer();
		
		STATES_ORDERS_LIST.forEach(state -> {
			if(states.containsKey(state) && states.get(state)!=0) {
				append(buf, state.emoji.toString() + state.description, states.get(state));
			}
		});
		
		return buf.toString();
	}

	private Map<AlarmState, Integer> getStateCounts() {
		Map<AlarmState, Integer> results = new HashMap<>();
		STATES_ORDERS_LIST.forEach(state -> results.put(state, 0));
		
		alarms.values().stream().flatMap(map -> map.keySet().stream())
		.forEach(alarmState -> {
			if(results.containsKey(alarmState)) {
				int staterCount = results.get(alarmState) + 1;
				results.put(alarmState, staterCount);
			}
		});
		return  results;
	}
	
	public List<ItemAbstractDto> getStateHints(IItemAbstract item) {
		List<ItemAbstractDto> result = new ArrayList<>();
		
		Map<AlarmState, List<IAlarmV2>> als = getAlarmsMap(item);
		
		als.keySet().forEach(state -> {
			List<IAlarmV2> list = als.get(state);
			
			if(list!=null && !list.isEmpty()) {
				int size = list.size();
				
				if(size>0) {
					String name = state.emoji.toString();
					
					StringBuffer buf = new StringBuffer();
					buf.append(name);
					buf.append(" ");
					buf.append(state.getDescription());
					buf.append(": ");
					list.stream().forEach(a -> buf.append(a.getStateDescriptionsWithNextLine()));
					
					if(size>1) {
						name += "("+size+")";
					}
					
					result.add(new ItemAbstractDto(0L, null, name, buf.toString()));
				}
				
			}
		});
		
		return result;
	}
		
	private void append(StringBuffer buf, String emoji, int count) {
		buf.append(emoji);
		buf.append(" ");
		buf.append(count);
		buf.append("; ");
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

	public Map<AlarmState, List<IAlarmV2>> getAlarmsMap(IItemAbstract item) {
		Map<AlarmState, List<IAlarmV2>> result = new LinkedHashMap<AlarmState, List<IAlarmV2>>();
		
		String uid = item.getItemUid();
		if (alarms.containsKey(uid)){
			Map<AlarmState,Map<Long,IAlarmV2>> map = alarms.get(uid);
			
			STATES_ORDERS_LIST.forEach(state -> {
				Map<Long,IAlarmV2> mapAlarms = map.get(state);
				if(mapAlarms!=null && !mapAlarms.isEmpty()) {
					result.put(state, mapAlarms.values().stream().collect(Collectors.toList()));
				}
				
			});
			return result;
		}
		return result;
	}
	
	public List<IAlarmV2> getAlarms(IItemAbstract item) {
		if (alarms.containsKey(item.getItemUid())){
			return alarms.get(item.getItemUid()).values().stream().flatMap(map -> map.values().stream()).collect(Collectors.toList());
		}
		return null;
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
	
	public int getAlarmsCount() {
		return alarms.keySet().size();
	}
	
	public boolean hasAlarms(IItemAbstract item) {
		List<IAlarmV2> alarms = getAlarms(item);
		return alarms!=null && !alarms.isEmpty();
	}

	public boolean hasEmoji(IItemAbstract item) {
		String state = getEmoji(item);
		
		return state!=null && !state.isEmpty();
	}
	
	public String getEmoji(IItemAbstract item) {
		return getMaxState(item).emoji.toString();
	}

	public String getHint(IItemAbstract item) {
		StringBuffer result = new StringBuffer();
		for (AlarmState state : STATES_ORDERS_LIST) {
			List<IAlarmV2> alarmList = getAlarms(item, state);
			if (alarmList != null && !alarmList.isEmpty()) {
				result.append(state.getEmojiDescription());
				result.append(" ");
				for(IAlarmV2 alarm: alarmList) {
					String str = alarm.getStateDescriptionsWithNextLine();
					result.append(str);
				}
			}
		}
		return result.toString();
	}

	public AlarmState getMaxState(IItemAbstract item) {
		AlarmState result = null;

		for (AlarmState state : STATES_ORDERS_LIST) {
			List<IAlarmV2> alarmList = getAlarms(item, state);
			if (alarmList != null && !alarmList.isEmpty()) {
				return state;
			}
		}
		
		if(result==null && getAlarmsCount()>0) {
			return AlarmState.NO_DATA;
		}

		return result;
	}

}
