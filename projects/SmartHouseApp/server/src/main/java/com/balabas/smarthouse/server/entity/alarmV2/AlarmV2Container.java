package com.balabas.smarthouse.server.entity.alarmV2;

import java.util.Arrays;
import java.util.Collections;
import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.function.Function;
import java.util.stream.Collectors;
import java.util.stream.Stream;

import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.balabas.smarthouse.server.entity.model.ItemAbstractDto;
import com.balabas.smarthouse.server.entity.model.descriptor.Emoji;
import com.google.common.collect.Lists;

@SuppressWarnings({ "rawtypes", "unchecked" })
public class AlarmV2Container {

	private static final AlarmState[] stateOrders = { AlarmState.ALARM, AlarmState.WARNING, AlarmState.OK, AlarmState.NO_DATA };

	private static final List<AlarmState> STATES_ORDERS_LIST = Arrays.asList(stateOrders);
	
	private Map<String, Map<AlarmState, Map<Long, IAlarmV2>>> alarms = new HashMap();

	public void clear() {
		alarms = new HashMap();
	}
	
	public ItemAbstractDto getViewDescriptor() {
		Map<String, List<ItemAbstractDto>> map = sortMap(alarms, adto -> adto.getEmoji().toString());
		Map<String, Integer> counts = getCounts(map);
		
		StringBuffer bufName = new StringBuffer();
		StringBuffer bufDescr = new StringBuffer();
		
		int size = map.keySet().size();
		
		int ind = 0;
		
		for(String key : map.keySet()) {
			int count = counts.get(key);
			
			bufName.append(key);
			if(size > 1) {
				bufName.append(" (");
				bufName.append(count);
				bufName.append(")");
			}
			ind++;
			
			if(ind!=size) {
				bufName.append(";");
			}
			
			bufDescr.append(key);
			
			Map<String, List<ItemAbstractDto>> byNameMap = groupStreamDto(map.get(key).stream(), adto -> adto.getName());
			
			for(String name : byNameMap.keySet()) {
				bufDescr.append(name);
				bufDescr.append(" : ");
				
				List<ItemAbstractDto> list = byNameMap.get(name); 
				
				int countByName = list.size();
				int ind2 = 0;
				
				for(ItemAbstractDto adto : list) {
					ind2++;
					bufDescr.append(adto.getDescription());
					
					if(ind2==countByName) {
						bufDescr.append("; ");
					} else {
						bufDescr.append(", ");
					}
				};
			}
			
		}
		
		return new ItemAbstractDto(Emoji.EMPTY_EMOJI, bufName.toString(), bufDescr.toString());
	}
	
	public List<ItemAbstractDto> getViewDescriptors(IItemAbstract item) {
		List<IAlarmV2> list = getAlarms(item);
		
		if(list==null) {
			return Collections.emptyList();
		}
		
		return getAlarms(item).stream().map(IAlarmV2::getViewDescriptor)
				.collect(Collectors.toList());
	}
	
	private Map<String, List<ItemAbstractDto>> sortMap(Map<String, Map<AlarmState, Map<Long, IAlarmV2>>> al,
			Function <ItemAbstractDto, String> keyProvider) {

		return groupStream(al.values().stream().flatMap(alStateMap -> alStateMap.values().stream())
		.flatMap(longMap -> longMap.values().stream()), keyProvider);
	}
	
	private Map<String, List<ItemAbstractDto>> groupStream(Stream<IAlarmV2> str, Function<ItemAbstractDto, String> keyProvider) {
		Map<String, List<ItemAbstractDto>> result = new LinkedHashMap<String, List<ItemAbstractDto>>();
		
		str.forEach(alarm ->{
			String key = keyProvider.apply(alarm.getViewDescriptor());
			
			if(!result.containsKey(key)) {
				result.put(key, Lists.newArrayList());
			}
			
			result.get(key).add(alarm.getViewDescriptor());
		});
		return result;
	}
	
	private Map<String, List<ItemAbstractDto>> groupStreamDto(Stream<ItemAbstractDto> str, Function<ItemAbstractDto, String> keyProvider) {
		Map<String, List<ItemAbstractDto>> result = new LinkedHashMap<String, List<ItemAbstractDto>>();
		
		str.forEach(al ->{
			String key = keyProvider.apply(al);
			
			if(!result.containsKey(key)) {
				result.put(key, Lists.newArrayList());
			}
			
			result.get(key).add(al);
		});
		return result;
	}
	
	private <T> Map<String, Integer> getCounts(Map<String, List<T>> list ) {
		Map<String, Integer> result = new LinkedHashMap<String, Integer>();
		list.entrySet().stream().forEach(e -> {
			Integer count = e.getValue().size();
			result.put(e.getKey(), count);
		});
		
		return result;
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
