package com.balabas.smarthouse.server.entity.alarmV2;

import java.util.ArrayList;
import java.util.Collection;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Optional;
import java.util.function.Function;
import java.util.function.Predicate;
import java.util.stream.Collectors;

import javax.annotation.PostConstruct;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;

import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.balabas.smarthouse.server.entity.model.ItemAbstract;
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
		alarmRepository.save((AlarmV2) alarm);
		putAlarmToCache(alarm);
	}

	@Override
	public void deleteAlarm(IAlarmV2 alarm) {
		if(alarm!=null) {
			alarmRepository.deleteById(alarm.getId());
			removeAlarmFromCache(alarm);
		}
	}

	@Override
	public void checkForAlarmsWithParentExecutePostActions(List<EntityFieldValue> changedValues) {
		Map<Long, IItemAbstract> entityFieldsChanged = new HashMap<>();
		Map<Long, IItemAbstract> entitiesChanged = new HashMap<>();
		Map<Long, IItemAbstract> groupsChanged = new HashMap<>();
		Map<Long, IItemAbstract> devicesChanged = new HashMap<>();

		for (IEntityFieldValue value : changedValues) {
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

		getAlarmsByItemUid(item).forEach(alarm -> checkForAlarm(alarm).ifPresent(events::add));
		events.forEach(this::processEvent);

		return events;
	}

	private Optional<IAlarmStateChangeEvent> checkForAlarm(IAlarmV2 alarm) {
		alarm.check();
		return Optional.ofNullable(buildEvent(alarm));
	}

	private void processEvent(IAlarmStateChangeEvent event) {
		IAlarmStateChangeEventProcessor processor = alarmTypeProvider.getAlarmStateChangedEventProcessor(event);

		if (processor != null && processor.isTarget(event)) {
			processor.processEvent(event);
		}
	}

	private IAlarmStateChangeEvent buildEvent(IAlarmV2 alarm) {

		IAlarmStateChangeAction action = alarm.getCurrentAction();

		if (action != null) {
			IAlarmStateChangeEvent result = new AlarmStateChangeEvent(alarm, action);

			return result;
		}
		return null;
	}

	private void putAlarmToCache(IAlarmV2 alarm) {
		List<IAlarmV2> alarms = getAlarmsByItemUid(alarm.getItem());

		int ind = getAlarmIndex(alarms, alarm);

		if (ind > -1) {
			alarms.set(ind, alarm);
		} else {
			alarms.add(alarm);
		}
	}

	private void removeAlarmFromCache(IAlarmV2 alarm) {
		List<IAlarmV2> alarms = getAlarmsByItemUid(alarm.getItem());

		int ind = getAlarmIndex(alarms, alarm);

		if (ind > -1) {
			alarms.remove(ind);
		}
	}

	private List<IAlarmV2> getAlarmsByItemUid(IItemAbstract item) {
		String uid = item.getItemUid();

		if (!alarmMap.containsKey(uid)) {
			alarmMap.put(uid, new ArrayList<>());
		}

		List<IAlarmV2> result = alarmMap.get(uid);

		result.stream().forEach(alarm -> alarm.setItem(item));
		return result;
	}

	private int getAlarmIndex(List<IAlarmV2> alarms, IAlarmV2 source) {
		int ind = -1;
		for (IAlarmV2 al : alarms) {
			ind++;
			if (al.getId().equals(source.getId())) {
				return ind;
			}
		}

		return -1;
	}

	private void putById(Map<Long, IItemAbstract> map, IItemAbstract item) {
		map.put(item.getId(), item);
	}

	@Override
	public List<IAlarmV2> getAllAlarms() {
		return alarmMap.values().stream().flatMap(list -> list.stream()).filter(alarm -> alarm.getItem() != null)
				.collect(Collectors.toList());
	}

	@Override
	public List<IAlarmV2> getAlarmsForItemClass(Class<?> clazz) {
		return getAlarmsByFilter(
				alarm -> alarm.getItem() != null && clazz.isAssignableFrom(alarm.getTargetItemClass()));
	}

	@Override
	public Map<String, List<IAlarmV2>> getAlarmsGrouppedByItemClassName() {
		return getAlarmsGrouppedBy(alarm -> alarm.getTargetItemClass().getSimpleName());
	}

	@Override
	public List<IAlarmV2> getAlarmsByAlarmState(AlarmState alarmState) {
		return getAlarmsByFilter(alarm -> alarm.getItem() != null && alarmState.equals(alarm.getAlarmState()));
	}

	@Override
	public Map<AlarmState, List<IAlarmV2>> getAlarmsGrouppedByAlarmState() {
		return getAlarmsGrouppedBy(alarm -> alarm.getAlarmState());
	}

	public Optional<IAlarmV2> getAlarmByFilter(Predicate<? super IAlarmV2> predicate) {
		return getAllAlarms().stream().filter(predicate).findFirst();
	}
	
	public List<IAlarmV2> getAlarmsByFilter(Predicate<? super IAlarmV2> predicate) {
		return getAllAlarms().stream().filter(predicate).sorted(AlarmV2Service::compareByItemDescriptionField)
				.collect(Collectors.toList());
	}

	public <T> Map<T, List<IAlarmV2>> getAlarmsGrouppedBy(Function<IAlarmV2, T> mapKeyFunc) {
		Map<T, List<IAlarmV2>> result = new HashMap<>();

		getAllAlarms().forEach(alarm -> {
			T key = mapKeyFunc.apply(alarm);

			if (!result.containsKey(key)) {
				result.put(key, new ArrayList<IAlarmV2>());
			}

			result.get(key).add(alarm);

		});

		for (T key : result.keySet()) {
			List<IAlarmV2> list = result.get(key).stream().sorted(AlarmV2Service::compareByItemDescriptionField)
					.collect(Collectors.toList());
			result.put(key, list);
		}

		return result;
	}

	public static int compareByItemDescriptionField(IAlarmV2 a1, IAlarmV2 a2) {
		return ItemAbstract.compareByDescriptionField(a1.getItem(), a2.getItem());
	}

	@Override
	public IAlarmV2 getAlarm(Long id) {
		return getAlarmByFilter(alarm -> id.equals(alarm.getId())).orElse(null);
	}

	@Override
	public void deleteAlarm(Long id) {
		deleteAlarm(getAlarm(id));
	}

}
