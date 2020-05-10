package com.balabas.smarthouse.server.entity.alarmV2.service;

import java.util.ArrayList;
import java.util.Collection;
import java.util.Date;
import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Map;
import java.util.Optional;
import java.util.Set;
import java.util.function.Function;
import java.util.function.Predicate;
import java.util.stream.Collectors;
import java.util.stream.Stream;

import javax.annotation.PostConstruct;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.scheduling.annotation.Scheduled;
import org.springframework.stereotype.Service;

import com.balabas.smarthouse.server.entity.alarmV2.AlarmStateChangeEntity;
import com.balabas.smarthouse.server.entity.alarmV2.AlarmV2Checker;
import com.balabas.smarthouse.server.entity.alarmV2.AlarmV2Container;
import com.balabas.smarthouse.server.entity.alarmV2.IAlarmStateChangeEventProcessor;
import com.balabas.smarthouse.server.entity.alarmV2.IItemEvent;
import com.balabas.smarthouse.server.entity.alarmV2.ItemChangeEvent;
import com.balabas.smarthouse.server.entity.alarmV2.model.AlarmOfDevice;
import com.balabas.smarthouse.server.entity.alarmV2.model.AlarmOfEntity;
import com.balabas.smarthouse.server.entity.alarmV2.model.AlarmOfEntityField;
import com.balabas.smarthouse.server.entity.alarmV2.model.AlarmState;
import com.balabas.smarthouse.server.entity.alarmV2.model.AlarmStateChangeAction;
import com.balabas.smarthouse.server.entity.alarmV2.model.AlarmV2;
import com.balabas.smarthouse.server.entity.alarmV2.model.IAlarmStateChangeAction;
import com.balabas.smarthouse.server.entity.alarmV2.model.IAlarmV2;
import com.balabas.smarthouse.server.entity.alarmV2.repository.IAlarmStateChangeActionRepository;
import com.balabas.smarthouse.server.entity.alarmV2.repository.IAlarmV2Repository;
import com.balabas.smarthouse.server.entity.alarmV2.repository.IAlarmV2RepositoryDevice;
import com.balabas.smarthouse.server.entity.alarmV2.repository.IAlarmV2RepositoryEntity;
import com.balabas.smarthouse.server.entity.alarmV2.repository.IAlarmV2RepositoryEntityField;
import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.IItemAbstract;
import com.balabas.smarthouse.server.entity.model.ItemAbstract;
import com.balabas.smarthouse.server.entity.model.ItemAbstractDto;
import com.balabas.smarthouse.server.entity.model.descriptor.ItemType;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityFieldValue;
import com.balabas.smarthouse.server.entity.service.IActionTimerService;
import com.balabas.smarthouse.server.entity.service.IDeviceManageService;
import com.balabas.smarthouse.server.view.chart.ChartDataSeries;
import com.google.common.collect.Lists;

import lombok.extern.log4j.Log4j2;

@Log4j2
@Service
@SuppressWarnings("rawtypes")
public class AlarmV2Service implements IAlarmV2Service {

	@Autowired
	IAlarmV2RepositoryDevice alarmRepositoryDevice;

	@Autowired
	IAlarmV2RepositoryEntity alarmRepositoryEntity;

	@Autowired
	IAlarmV2RepositoryEntityField alarmRepositoryEntityField;

	@Autowired
	IAlarmStateChangeActionRepository alarmStateChangeActionRepository;

	@Autowired
	IAlarmv2TypeProvider alarmTypeProvider;

	@Autowired
	IDeviceManageService deviceService;

	@Autowired
	IItemAlarmStateService itemAlarmStateService;

	@Autowired
	IActionTimerService actionTimerService;
	
	@Autowired
	IPostponedExecutorManager postponedManager;

	@Autowired
	IAlarmStateChangeEntityService alarmStateChangeEntityService;

	Map<String, List<IAlarmV2>> alarmMap = new HashMap<String, List<IAlarmV2>>();

	@Override
	public List<AlarmV2Checker> getEnabledAlarmCheckers(IItemAbstract item) {
		return alarmTypeProvider.getEnabledAlarms(item);
	}

	@PostConstruct
	public void loadAlarms() {
		alarmRepositoryDevice.findAll().forEach(this::initAlarm);
		alarmRepositoryEntity.findAll().forEach(this::initAlarm);
		alarmRepositoryEntityField.findAll().forEach(this::initAlarm);
	}

	@Override
	public Map<String, List<AlarmV2>> getAlarmsMap(List<IItemAbstract> items, List<AlarmState> states,
			boolean withStateDescriptionOnly) {
		Map<String, List<AlarmV2>> result = new LinkedHashMap<>();

		List<IAlarmV2> resultList = new ArrayList<>();

		Predicate<IAlarmV2> filter = withStateDescriptionOnly
				? alarm -> isAlarmInState(alarm, states) && attachAlarmDescriptions(alarm)
				: alarm -> isAlarmInState(alarm, states);

		items.stream().forEach(
				item -> getAlarmsByItemUid(item).stream().filter(filter).forEach(alarm -> resultList.add(alarm)));

		return result;
	}

	@Override
	public boolean isAlarmInState(IAlarmV2 alarm, List<AlarmState> states) {
		return states.contains(alarm.getAlarmState());
	}

	@Override
	public boolean attachAlarmDescriptions(IAlarmV2 alarm) {
		List<String> descriptions = itemAlarmStateService.getStateDescriptions(alarm);

		alarm.setStateDescriptions(descriptions);

		return descriptions != null && descriptions.size() > 0;
	}

	@Override
	public void saveAlarm(IAlarmV2 alarm) {
		getRepository(alarm).save(alarm);
		putAlarmToCache(alarm);
		actionTimerService.initAlarmScheduling(alarm);
	}

	@Override
	public void deleteAlarm(IAlarmV2 alarm) {
		if (alarm != null) {
			getRepository(alarm).deleteById(alarm.getId());
			removeAlarmFromCache(alarm);
		}
	}

	@Override
	public void checkForAlarmsWithParentExecutePostActionsForFields(List<IEntityField> changedValues) {
		Map<Long, IItemAbstract> entityFieldsChanged = new HashMap<>();
		Map<Long, IItemAbstract> entitiesChanged = new HashMap<>();
		Map<Long, IItemAbstract> groupsChanged = new HashMap<>();
		Map<Long, IItemAbstract> devicesChanged = new HashMap<>();

		for (IEntityField value : changedValues) {
			putById(entityFieldsChanged, value);
			putById(entitiesChanged, value.getEntity());
			putById(groupsChanged, value.getEntity().getGroup());
			putById(devicesChanged, value.getEntity().getGroup().getDevice());
		}

		checkForAlarmsExecutePostActions(devicesChanged.values());
		checkForAlarmsExecutePostActions(groupsChanged.values());
		checkForAlarmsExecutePostActions(entitiesChanged.values());
		checkForAlarmsExecutePostActions(entityFieldsChanged.values());
	}

	@Override
	public List<IItemEvent> checkForAlarmsExecutePostActions(Collection<IItemAbstract> items) {
		List<IItemEvent> list = new ArrayList<>();

		items.forEach(item -> list.addAll(checkForAlarmsExecutePostActions(item)));

		return list;
	}

	@Override
	public List<IItemEvent> checkForAlarmsExecutePostActions(IAlarmV2 alarm) {

		IItemAbstract item = deviceService.getItemAbstract(alarm.getItem());
		List<IItemEvent> events = new ArrayList<IItemEvent>();

		alarm.setItem(item);
		events.addAll(checkForAlarm(alarm));

		processEvents(events, false);

		return events;
	}

	@Override
	public List<IItemEvent> checkForAlarmsExecutePostActions(IItemAbstract item) {
		List<IItemEvent> events = new ArrayList<IItemEvent>();

		getAlarmsByItemUid(item).forEach(alarm -> {
			alarm.setItem(item);
			events.addAll(checkForAlarm(alarm));
		});
		
		processEvents(events, false);

		return events;
	}

	private List<IItemEvent> checkForAlarm(IAlarmV2 alarm) {
		String checkerName = alarm.getCheckerName();
		AlarmV2Checker checker = alarmTypeProvider.getAlarmV2checker(checkerName);

		if (checker == null) {
			log.error("Null checker for alarm id=" + alarm.getId());
		}

		checker.process(alarm);

		actionTimerService.nextAlarmScheduling(alarm);

		return buildEvent(alarm, false);
	}
	
	private void processEvents(Collection<IItemEvent> events, boolean now) {
		if(events!=null) {
			for(IItemEvent event : events) {
				processEvent(event, now);
			}
		}
	}

	private void processEvent(IItemEvent event, boolean now) {
		
		if(!now) {
			now = !event.getAlarm().isPostponed();
		}
		
		if(!now) {
			postponedManager.postponeExecution(event.getAlarm());
			return;
		}
		
		IAlarmStateChangeEventProcessor processor = alarmTypeProvider.getAlarmStateChangedEventProcessor(event);

		try {
			if (processor != null && processor.isTarget(event)) {
				Long targetFieldId = event.getChangeAction().getTargetFieldId(); 
				if(targetFieldId!=null) {
					event.getChangeAction().setTargetField(deviceService.getEntityFieldById(targetFieldId));
				}
				
				boolean doProcess = false;
				boolean ssd = false;
				boolean sameState = false;
				
				if(event.getChangeAction()!=null) {
					
					ssd = Boolean.TRUE.equals(event.getChangeAction().isDisabledIfSameState());
					sameState = event.isSameState();
					
					doProcess = ssd ? !sameState : true;
				}
				
				if(doProcess) {
					processor.process(event);
				}
			}
		}catch(Exception e) {
			e.printStackTrace();
		}

		alarmStateChangeEntityService.saveAlarmStateChange(event);
	}

	private List<IItemEvent> buildEvent(IAlarmV2 alarm, boolean now) {

		List<IItemEvent> events = new ArrayList<IItemEvent>();
		List<IAlarmStateChangeAction> action = alarm.getCurrentActions();

		action.stream().forEach(a -> {
			if (alarm.getMessageInterval() != null && alarm.getMessageInterval() > 0) {
				a.setInterval(alarm.getMessageInterval());
			}
			events.add(new ItemChangeEvent(alarm.getItem(), a, alarm, now));
		});
		return events;
	}

	private void initAlarm(IAlarmV2 alarm) {
		putAlarmToCache(alarm);
		actionTimerService.initAlarmScheduling(alarm);
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

	private List<IAlarmV2> getAlarmsRequiredCheckBySchedule() {
		return alarmMap.values().stream().flatMap(list -> list.stream())
				.filter(actionTimerService::requireScheduledCheck).collect(Collectors.toList());
	}

	@Scheduled(fixedRateString = "${smarthouse.server.alarm.request.interval:10000}")
	public void checkScheduledAlarms() {
		getAlarmsRequiredCheckBySchedule().stream().forEach(this::checkForAlarmsExecutePostActions);
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
	public List<IAlarmV2> getAlarmsForItemClassWithDescriptions(Class<?> clazz) {
		List<IAlarmV2> result = getAlarmsForItemClass(clazz);
		result.forEach(this::attachAlarmDescriptions);

		return result;
	}

	@Override
	public Map<String, List<IAlarmV2>> getAlarmsGrouppedByItemClassName() {
		return getAlarmsGrouppedBy(alarm -> alarm.getTargetItemClass().getSimpleName());
	}

	@Override
	public Map<String, List<IAlarmV2>> getAlarmsGrouppedByItemClassNameWithAlarmDescriptions() {
		Map<String, List<IAlarmV2>> result = getAlarmsGrouppedBy(alarm -> alarm.getTargetItemClass().getSimpleName());

		result.values().stream().forEach(list -> list.forEach(this::attachAlarmDescriptions));

		return result;
	}
	
	@Override
	public Map<String, List<IAlarmV2>> getAlarmsGrouppedByItemTypeWithAlarmDescriptions() {
		Map<String, List<IAlarmV2>> result = getAlarmsGrouppedBy(alarm -> alarm.getItemType().name());

		result.values().stream().forEach(list -> list.forEach(this::attachAlarmDescriptions));

		return result;
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

	public List<IAlarmV2> getAlarmsByFilterNotSorted(Predicate<? super IAlarmV2> predicate) {
		return getAllAlarms().stream().filter(predicate).collect(Collectors.toList());
	}

	public List<IAlarmV2> getAlarmsByFilterWithDescriptions(Predicate<? super IAlarmV2> predicate) {
		List<IAlarmV2> result = getAllAlarms().stream().filter(predicate)
				.sorted(AlarmV2Service::compareByItemDescriptionField).collect(Collectors.toList());

		result.forEach(this::attachAlarmDescriptions);

		return result;
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
	public IAlarmV2 getAlarmOrDefault(Long id, IAlarmV2 def) {
		return getAlarmByFilter(alarm -> id.equals(alarm.getId())).orElse(def);
	}

	@Override
	public List<AlarmV2Checker> getCheckersByTargetItemClass(Class<?> targetItemClass) {
		return alarmTypeProvider.getCheckersByTargetItemClass(targetItemClass);
	}

	@Override
	public IAlarmV2Repository getRepository(IAlarmV2 alarm) {
		switch (alarm.getItemType()) {
		case DEVICE:
			return alarmRepositoryDevice;
		case ENTITY:
			return alarmRepositoryEntity;
		case ENTITY_FIELD:
			return alarmRepositoryEntityField;
		default:
			return null;
		}
	}

	@Override
	public IAlarmV2 newAlarm(ItemType itemType) {
		switch (itemType) {
		case DEVICE:
			return new AlarmOfDevice();
		case ENTITY:
			return new AlarmOfEntity();
		case ENTITY_FIELD:
			return new AlarmOfEntityField();
		default:
			return null;
		}
	}

	@Override
	public Set<IItemAbstract> getEnabledAlarmTargets(IAlarmV2 alarm) {

		Stream<? extends IItemAbstract> stream = Stream.empty();

		switch (alarm.getItemType()) {
		case DEVICE:
			stream = deviceService.getDevices().stream();
			break;
		case ENTITY:
			stream = deviceService.getEntities().stream();
			break;
		case ENTITY_FIELD:
			stream = deviceService.getEntityFields().stream().filter(IEntityField::isNotStringAndSensors);
			break;
		default:
			stream = Stream.empty();
		}

		return stream.collect(Collectors.toSet());
	}
	
	@Override
	public Set<IItemAbstract> getEnabledAlarmTargets(ItemType itemType) {

		Stream<? extends IItemAbstract> stream = Stream.empty();

		switch (itemType) {
		case DEVICE:
			stream = deviceService.getDevices().stream();
			break;
		case ENTITY:
			stream = deviceService.getEntities().stream();
			break;
		case ENTITY_FIELD:
			stream = deviceService.getEntityFields().stream().filter(IEntityField::isNotStringAndSensors);
			break;
		default:
			stream = Stream.empty();
		}

		return stream.sorted((i1,i2) -> i1.getFullName().compareToIgnoreCase(i2.getFullName()))
				.collect(Collectors.toCollection( LinkedHashSet::new ));
	}

	@Override
	public List<ItemAbstractDto> getEnabledAlarmAbstractTargets(IAlarmV2 alarm) {
		return getEnabledAlarmTargets(alarm).stream().sorted(ItemAbstract::compareByParentNameChain)
				.map(ItemAbstractDto::fromItemByPath).collect(Collectors.toList());
	}

	@Override
	public IItemAbstract getEnabledAlarmTarget(Long itemId, Class<?> itemClass) {
		if (IDevice.class.isAssignableFrom(itemClass)) {
			return deviceService.getDeviceById(itemId);
		}
		if (IEntity.class.isAssignableFrom(itemClass)) {
			return deviceService.getEntityById(itemId);
		}
		if (IEntityField.class.isAssignableFrom(itemClass)) {
			return deviceService.getEntityFieldById(itemId);
		}

		return null;
	}

	@Override
	public void createOrUpdateAlarm(IAlarmV2 alarm, Long targetId) {
		IItemAbstract item = getEnabledAlarmTarget(targetId, alarm.getTargetItemClass());
		alarm.setItem(item);

		if (!alarm.isNew()) {
			IAlarmV2 existAlarm = getAlarm(alarm.getId(), alarm.getItemType());
			if (existAlarm != null) {
				alarm.setActions(existAlarm.getActions());
			}
		}

		saveAlarm(alarm);
		log.debug("Alarm saved " + alarm);
	}

	@Override
	public Set<IAlarmStateChangeEventProcessor> getAlarmStateChangedEventProcessors(IAlarmV2 alarm) {
		return alarmTypeProvider.getAlarmStateChangedEventProcessors();
	}

	@SuppressWarnings({ "unchecked" })
	@Override
	public void addAlarmStateChangeActionToAlarm(Long alarmId, ItemType it, AlarmStateChangeAction action) {
		IAlarmV2 alarm = getAlarm(alarmId, it);

		Set<IAlarmStateChangeAction> set = Optional.ofNullable(alarm.getActions()).orElse(new LinkedHashSet());
		set.add(action);
		alarm.setActions(set);

		saveAlarm(alarm);
	}

	@SuppressWarnings({ "unchecked" })
	@Override
	public void deleteAlarmStateChangeActionFromAlarm(Long alarmId, ItemType itemType, Long actionId) {
		IAlarmV2 alarm = getAlarm(alarmId, itemType);

		Set<IAlarmStateChangeAction> set = Optional.ofNullable(alarm.getActions()).orElse(new LinkedHashSet());

		IAlarmStateChangeAction action = set.stream().filter(a -> actionId.equals(a.getId())).findFirst().orElse(null);

		Optional.ofNullable(action).ifPresent(set::remove);

		alarm.setActions(set);
		saveAlarm(alarm);
		try {
			alarmStateChangeActionRepository.deleteById(actionId);
		} catch (Exception e) {
			log.error("Error delete actionId=" + actionId);
		}
	}

	@Override
	public IAlarmV2 getAlarm(Long id, ItemType it) {
		if (id != null && id > 0) {
			IAlarmV2 al = getAlarmByFilter(alarm -> alarm.getItemType().equals(it) && id.equals(alarm.getId()))
					.orElse(null);
			if (al != null) {
				return al;
			}

		}
		return getAlarmOrDefault(id, newAlarm(it));
	}
	
	@Override
	public Map<ItemType, String> getItemMapNames() {
		Map<ItemType, String> result =new LinkedHashMap<ItemType, String>();
		
		for (ItemType type : ItemType.ITEM_TYPES_ORDERED) {
			result.put(type, type.getDescription());
		}
		
		return result;
	}

	@Override
	public Map<ItemType, Map<AlarmState, List<IAlarmV2>>> getAlarmsAsMap() {
		return getAlarmsAsMap(null, false);
	}

	@Override
	public AlarmV2Container getAlarmsContainerWithChildren(IItemAbstract parent) {
		AlarmV2Container result = new AlarmV2Container();

		getAlarmsWithItemChildren(parent).forEach(alarm -> {
			itemAlarmStateService.getStateDescriptions(alarm);
			result.putAlarm(alarm);
		});

		return result;
	}
	
	@Override
	public List<IAlarmV2> getAlarmsWithItemChildren(IItemAbstract item) {
		return getAlarms(item.getItemAndAllChildren());
	}
	
	@Override
	public List<IAlarmV2> getAlarms(List<IItemAbstract> items) {
		List<IAlarmV2> alarms = new ArrayList<>();

		items.stream().forEach(i -> alarms.addAll(getAlarmsByItemUid(i)));

		return alarms.stream().sorted(IAlarmV2::compareByDescription).collect(Collectors.toList());
	}

	@Override
	public void checkForAlarmsWithParentExecutePostActions(List<IEntityFieldValue> changedValues) {
		List<IEntityField> entityFields = changedValues.stream().map(value -> value.getEntityField())
				.collect(Collectors.toList());
		checkForAlarmsWithParentExecutePostActionsForFields(entityFields);
	}

	@Override
	public Map<ItemType, Map<AlarmState, List<IAlarmV2>>> getAlarmsAsMap(AlarmState maxState, boolean only) {
		return getAlarmsAsMap(getAllAlarms(), maxState, only);
	}
	
	@Override
	public Map<ItemType, Map<AlarmState, List<IAlarmV2>>> getAlarmsAsMap(IItemAbstract item, AlarmState maxState, boolean only) {
		return getAlarmsAsMap(getAlarmsWithItemChildren(item), maxState, only);
	}
	
	@Override
	public Map<ItemType, Map<AlarmState, List<IAlarmV2>>> getAlarmsAsMap(Long itemId, ItemType itemType, AlarmState maxState, boolean only) {
		return getAlarmsAsMap(getAlarmsWithItemChildren(itemId,  itemType), maxState, only);
	}
	
	@Override
	public Map<ItemType, Map<AlarmState, List<IAlarmV2>>> getAlarmsAsMap(List<IAlarmV2> alarmList, AlarmState maxState, boolean only) {
		Map<ItemType, Map<AlarmState, List<IAlarmV2>>> result = new LinkedHashMap<ItemType, Map<AlarmState, List<IAlarmV2>>>();

		for (ItemType type : ItemType.ITEM_TYPES_ORDERED) {
			result.put(type, new LinkedHashMap<AlarmState, List<IAlarmV2>>());
			Map<AlarmState, List<IAlarmV2>> stateMap = result.get(type);

			for (AlarmState state : AlarmState.ALARM_STATE_ORDER) {
				if(only) {
					if(!state.equals(maxState)) {
						continue;
					}
				}
				stateMap.put(state, Lists.newArrayList());
				if(maxState!=null && state.equals(maxState)) {
					break;
				}
			}
		}

		for(IAlarmV2 alarm : alarmList) {
			if(result.containsKey(alarm.getItemType())) {
				Map<AlarmState, List<IAlarmV2>> stateMap = result.get(alarm.getItemType());
				if(stateMap.containsKey(alarm.getAlarmState())) {
					result.get(alarm.getItemType()).get(alarm.getAlarmState()).add(alarm);
				}
			}
		}

		for (ItemType type : ItemType.ITEM_TYPES_ORDERED) {
			for (AlarmState state : AlarmState.ALARM_STATE_ORDER) {
				if(only) {
					if(!state.equals(maxState)) {
						continue;
					}
				}
				Map<AlarmState, List<IAlarmV2>> stateMap =result.get(type);
				
				List<IAlarmV2> list = stateMap.get(state).stream().sorted((al1, al2)->al1.getDescription().compareTo(al2.getDescription())).collect(Collectors.toList());
				if(list.size()>0) {
					stateMap.put(state, list);
				} else {
					stateMap.remove(state);
				}
				if(maxState!=null && state.equals(maxState)) {
					break;
				}
			}
		}
		return result;
	}

	@Override
	public String getAlarmsIconsString(AlarmState state, boolean only) {
		if(state==null) {
			state = AlarmState.NO_DATA;
		}
		if(only) {
			return state.getEmojiDescription();
		}
		StringBuffer buf = new StringBuffer();

		for(AlarmState st : AlarmState.ALARM_STATE_ORDER) {
			buf.append(st.getEmojiDescription());
			buf.append(";  ");
			if(state!=null && st.equals(state)) {
				break;
			}
		}
		
		return buf.toString();
	}

	@Override
	public List<IAlarmV2> getAlarmsWithItemChildren(Long itemId, ItemType itemItemType) {
		return getAlarmsWithItemChildren(deviceService.getItemAbstract(itemId, itemItemType));
	}

	@Override
	public void executePostponed(IAlarmV2 alarm) {
		processEvents(buildEvent(alarm, true), true);
	}

	@Override
	public IAlarmV2 getAlarm(Long id) {
		return getAlarmsByFilter(al -> al.getId().equals(id)).stream().findFirst().orElse(null);
		
	}

	@Override
	public List<ChartDataSeries> getAlarmStates(IAlarmV2 alarm, Date date1, Date date2) {
		
		List<ChartDataSeries> result = new ArrayList<ChartDataSeries>();
		Map<AlarmState, List<AlarmStateChangeEntity>> map = alarmStateChangeEntityService.getAlarmStateChangeEntitiesForPeriod(alarm.getId(), date1, date2);
		
		for(AlarmState state : AlarmState.getList()) {
			if(!AlarmState.ANY.equals(state)) {
				ChartDataSeries ser = new ChartDataSeries(state.getEmojiDescription(), state.getColor());
				List<AlarmStateChangeEntity> list = map.get(state);
				
				for(AlarmStateChangeEntity entity : list) {
					ser.addDataPoint(entity.getDateTime().getTime(), state.getValueInt());
				}
				
				result.add(ser);
			}
		}
				
		
		return result;
	}
	
	@Override
	public List<ChartDataSeries> getAlarmStatesOrdered(IAlarmV2 alarm, Date date1, Date date2) {
		
		List<ChartDataSeries> result = new ArrayList<ChartDataSeries>();
		
		List<AlarmStateChangeEntity> list = alarmStateChangeEntityService.getAlarmStateChangeEntitiesForPeriodList(alarm.getId(), date1, date2);

		AlarmState prevState = null;
		AlarmState state = null;
		
		ChartDataSeries ser = null;
		
		for(AlarmStateChangeEntity entity : list) {
			state = entity.getAlarmState();
			
			if(!state.equals(prevState) && !AlarmState.ANY.equals(state)) {
				ser = new ChartDataSeries(state.getEmojiDescription(), state.getColor());
				if(prevState!=null) {
					ser.addDataPoint(entity.getDateTime().getTime(), prevState.getValueInt());
				}
				ser.addDataPoint(entity.getDateTime().getTime(), state.getValueInt());
				result.add(ser);
			} else {
				if(ser!=null) {
					ser.addDataPoint(entity.getDateTime().getTime(), state.getValueInt());
				}
			}
			
			prevState = state;
		}
				
		
		return result;
	}

}
