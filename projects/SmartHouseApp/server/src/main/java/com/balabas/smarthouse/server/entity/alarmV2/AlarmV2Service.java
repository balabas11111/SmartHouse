package com.balabas.smarthouse.server.entity.alarmV2;

import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Optional;

import javax.annotation.PostConstruct;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.balabas.smarthouse.server.entity.model.entityfields.EntityFieldValue;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityFieldValue;
import com.balabas.smarthouse.server.entity.service.IDeviceManageService;

@Service
public class AlarmV2Service implements IAlarmV2Service {

	@Autowired
	IAlarmV2Repository alarmRepository;
	
	@Autowired
	IAlarmv2TypeProvider alarmTypeProvider;
	
	@Autowired
	IDeviceManageService deviceService;
	
	Map<String, List<IAlarmV2>> alarmMap = new HashMap<String, List<IAlarmV2>>();

	@Override
	public List<Class<?>> getEnabledAlarms(IItemAbstract item) {
		return alarmTypeProvider.getEnabledAlarms(item);
	}

	@PostConstruct
	public void loadAlarms() {
		alarmRepository.findAll().forEach(this::putAlarmToCache);
	}
	
	@Override
	public void saveAlarm(IAlarmV2 alarm) {
		alarmRepository.save((AlarmV2)alarm);
		putAlarmToCache(alarm);
	}

	@Override
	public void deleteAlarm(IAlarmV2 alarm) {
		alarmRepository.deleteById(alarm.getId());
		removeAlarmFromCache(alarm);
	}
	
	@Override
	public void checkForAlarmsWithParent(List<EntityFieldValue> changedValues) {
		Map<Long, IItemAbstract> entityFieldsChanged = new HashMap<>();
		Map<Long, IItemAbstract> entitiesChanged = new HashMap<>();
		Map<Long, IItemAbstract> groupsChanged = new HashMap<>();
		Map<Long, IItemAbstract> devicesChanged = new HashMap<>();
		
		for(IEntityFieldValue value : changedValues) {
			putById(entityFieldsChanged, value.getEntityField());
			putById(entitiesChanged, value.getEntityField().getEntity());
			putById(groupsChanged, value.getEntityField().getEntity().getGroup());
			putById(devicesChanged, value.getEntityField().getEntity().getGroup().getDevice());
		}
		
		checkForAlarmsExecutePostActions(devicesChanged.values());
		checkForAlarmsExecutePostActions(groupsChanged.values());
		checkForAlarmsExecutePostActions(entitiesChanged.values());
		checkForAlarmsExecutePostActions(entityFieldsChanged.values());
	}
	
	@Override
	public List<IAlarmStateChangeEvent> checkForAlarmsExecutePostActions(Collection<IItemAbstract> items) {
		List<IAlarmStateChangeEvent> list = new ArrayList<>();
		
		items.forEach(item -> list.addAll(checkForAlarmsExecutePostActions(item)));
		
		return list;
	}

	@Override
	public List<IAlarmStateChangeEvent> checkForAlarmsExecutePostActions(IItemAbstract item) {
		List<IAlarmStateChangeEvent> events = new ArrayList<IAlarmStateChangeEvent>();
		
		getAlarmsByItemUid(item).forEach(alarm ->	checkForAlarm(alarm).ifPresent(events::add));
		events.forEach(this::processEvent);
		
		return events;
	}
	
	private Optional<IAlarmStateChangeEvent> checkForAlarm(IAlarmV2 alarm) {
		alarm.check();
		return Optional.ofNullable(buildEvent(alarm));
	}
	
	private void processEvent(IAlarmStateChangeEvent event) {
		IAlarmStateChangeEventProcessor processor = alarmTypeProvider.getAlarmStateChangedEventProcessor(event);
		
		if(processor!=null && processor.isTarget(event)) {
			processor.processEvent(event);
		}
	}
	
	private IAlarmStateChangeEvent buildEvent(IAlarmV2 alarm) {
		
		IAlarmStateChangeAction action = alarm.getCurrentAction();
		
		if(action!=null) {
			IAlarmStateChangeEvent result = new AlarmStateChangeEvent(alarm, action);
		
			return result;
		}
		return null;
	}
	
	private void putAlarmToCache(IAlarmV2 alarm) {
		List<IAlarmV2> alarms = getAlarmsByItemUid(alarm.getItem());
		
		int ind = getAlarmIndex(alarms, alarm);
		
		if(ind>-1) {
			alarms.set(ind, alarm);
		} else {
			alarms.add(alarm);
		}
	}
	
	private void removeAlarmFromCache(IAlarmV2 alarm) {
		List<IAlarmV2> alarms = getAlarmsByItemUid(alarm.getItem());
		
		int ind = getAlarmIndex(alarms, alarm);
		
		if(ind>-1) {
			alarms.remove(ind);
		}
	}
	
	private List<IAlarmV2> getAlarmsByItemUid(IItemAbstract item) {
		String uid = item.getItemUid();
		
		if(!alarmMap.containsKey(uid)) {
			alarmMap.put(uid, new ArrayList<>());
		}
		
		List<IAlarmV2> result = alarmMap.get(uid);
		
		result.stream().forEach(alarm -> alarm.setItem(item));
		return result;
	}
	
	private int getAlarmIndex(List<IAlarmV2> alarms, IAlarmV2 source) {
		int ind = -1;
		for(IAlarmV2 al : alarms) {
			ind++;
			if(al.getId().equals(source.getId())) {
				return ind;
			}
		}
		
		return -1;
	}
	
	private void putById(Map<Long, IItemAbstract> map, IItemAbstract item) {
		map.put(item.getId(), item);
	}
	
}
