package com.balabas.smarthouse.server.entity.service;

import static com.balabas.smarthouse.server.DeviceConstants.DEVICE_URL_DATA;
import static com.balabas.smarthouse.server.DeviceConstants.HTTP_PREFFIX;
import static com.balabas.smarthouse.server.view.Action.ACTION_TYPE_SEND_DATA_TO_DEVICE;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Optional;
import java.util.Set;
import java.util.stream.Collectors;

import javax.annotation.PostConstruct;

import org.json.JSONObject;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.scheduling.annotation.Scheduled;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import com.balabas.smarthouse.server.controller.ControllerConstants;
import com.balabas.smarthouse.server.controller.service.DeviceRequestorService;
import com.balabas.smarthouse.server.entity.alarm.IEntityAlarmService;
import com.balabas.smarthouse.server.entity.alarmV2.IAlarmV2Service;
import com.balabas.smarthouse.server.entity.model.ActionTimer;
import com.balabas.smarthouse.server.entity.model.Device;
import com.balabas.smarthouse.server.entity.model.Entity;
import com.balabas.smarthouse.server.entity.model.Group;
import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.IGroup;
import com.balabas.smarthouse.server.entity.model.ItemAbstract;
import com.balabas.smarthouse.server.entity.model.descriptor.ItemType;
import com.balabas.smarthouse.server.entity.model.descriptor.State;
import com.balabas.smarthouse.server.entity.model.enabledvalue.IEntityFieldEnabledValue;
import com.balabas.smarthouse.server.entity.model.entityfields.EntityField;
import com.balabas.smarthouse.server.entity.model.entityfields.EntityFieldValue;
import com.balabas.smarthouse.server.entity.model.entityfields.EntityFieldValueBoolean;
import com.balabas.smarthouse.server.entity.model.entityfields.EntityFieldValueNumber;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.entity.repository.IDeviceRepository;
import com.balabas.smarthouse.server.entity.repository.IEntityFieldRepository;
import com.balabas.smarthouse.server.entity.repository.IEntityRepository;
import com.balabas.smarthouse.server.entity.repository.IGroupRepository;
import com.balabas.smarthouse.server.view.Action;
import com.balabas.smarthouse.server.view.DeviceEntityFieldActionHolder;
import com.google.common.base.Joiner;
import com.google.common.collect.Lists;
import com.google.common.collect.Maps;

import lombok.Getter;
import lombok.extern.log4j.Log4j2;

@Log4j2
@Service
@SuppressWarnings({ "rawtypes", "unchecked" })
public class DeviceManageService implements IDeviceManageService {

	@Value("${smarthouse.server.mock}")
	private boolean mock;

	@Value("${smarthouse.server.log.device.requests:false}")
	private boolean logDeviceRequests;

	@Value("${smarthouse.server.log.device.values.change:true}")
	private boolean logDeviceValuesChange;

	@Value("${smarthouse.server.fields.data.save.before.post:true}")
	private boolean saveDataBeforeSendToDevice;
	@Value("${smarthouse.server.device.data.request.after.post:false}")
	private boolean requestDataAfterSendToDevice;

	@Autowired
	IActionTimerService actionTimerService;

	@Autowired
	DeviceRequestorService deviceRequestor;

	@Autowired
	IDeviceStateChangeService stateChanger;

	@Autowired
	SmartHouseItemBuildService itemBuildService;

	@Autowired
	IEntityAlarmService alarmService;

	@Autowired
	IAlarmV2Service alarmV2Service;

	@Autowired
	IDeviceRepository deviceRepository;

	@Autowired
	IGroupRepository groupRepository;

	@Autowired
	IEntityRepository entityRepository;

	@Autowired
	IEntityFieldRepository entityFieldRepository;

	@Autowired
	IGroupService groupService;

	@Autowired
	IEntityService entityService;

	@Autowired
	IEntityFieldService entityFieldService;

	@Autowired
	IDeviceMqService deviceMqService;

	boolean isNotFirstIteration = false;

	@Getter
	private List<Device> devices = Collections.synchronizedList(new ArrayList<>());

	@Getter
	private Date lastDeviceDRUcheck;

	@PostConstruct
	@Transactional
	public void afterPropertiesSet() throws Exception {

		deviceRepository.findAll().forEach(d -> {
			State state = State.CONSTRUCTED;

			if (d.isVirtualized()) {
				state = State.CONNECTED;
			}
			d.setState(state);
			save(d);
		});

		log.info("---Loaded persisted devices---");
	}

	@Override
	public List<Device> getDevicesInitialized() {
		return devices.stream().filter(Device::isInitialized).collect(Collectors.toList());
	}

	@Override
	public Device getManagedDevice(Device dev) {
		for (Device device : devices) {
			if (device.getName().equals(dev.getName())) {
				return device;
			}
		}

		return null;
	}

	@Override
	public Device getDeviceById(Long id) {
		if (id == null || id == 0) {
			return null;
		}
		for (Device device : devices) {
			if (id.equals(device.getId())) {
				return device;
			}
		}

		return null;
	}

	@Override
	public Device getDeviceByName(String deviceName) {
		if (deviceName == null) {
			return null;
		}
		return this.devices.stream().filter(d -> deviceName.equals(d.getName())).map(d -> (Device) d).findFirst()
				.orElse(null);
	}

	@Override
	public Group getGroupById(Long id) {
		return this.devices.stream().flatMap(d -> d.getGroups().stream()).filter(g -> id.equals(g.getId())).findFirst()
				.orElse(null);
	}

	@Override
	public Entity getEntityById(Long id) {
		return this.devices.stream().flatMap(d -> d.getGroups().stream()).flatMap(g -> g.getEntities().stream())
				.filter(e -> id.equals(e.getId())).findFirst().orElse(null);
	}

	@Override
	public IEntityField getEntityFieldById(Long id) {
		return this.devices.stream().flatMap(d -> d.getGroups().stream()).flatMap(g -> g.getEntities().stream())
				.flatMap(e -> e.getEntityFields().stream()).filter(e -> id.equals(e.getId())).findFirst().orElse(null);
	}

	@Override
	public IEntityFieldEnabledValue getEntityFieldEnabledValueById(Long id) {
		return (IEntityFieldEnabledValue) this.devices.stream().flatMap(d -> d.getGroups().stream())
				.flatMap(g -> g.getEntities().stream()).flatMap(e -> e.getEntityFields().stream())
				.flatMap(ef -> ef.getEnabledValues().stream())
				.filter(e -> id.equals(((IEntityFieldEnabledValue) e).getId())).findFirst().orElse(null);
	}

	private int getDeviceIndex(Long id) {
		if (id != null) {
			for (int i = 0; i < devices.size(); i++) {
				if (id.equals(devices.get(i).getId())) {
					return i;
				}
			}
		}

		return -1;
	}

	@Override
	@Transactional
	public boolean processRegistrationRequest(Device device) {
		boolean exists = true;

		Device exDevice = getDeviceByName(device.getName());

		if (exDevice == null) {
			exDevice = Optional.ofNullable(deviceRepository.findByName(device.getName())).orElse(new Device());

			if (exDevice.getId() == null || exDevice.getId() == 0L) {
				exDevice.setName(device.getName());
				exDevice.setState(State.CONSTRUCTED);
				exDevice.setRegistrationDate(new Date());
			}

			exists = false;
		}

		exDevice.setIp(device.getIp());
		exDevice.setDataUrl(getDeviceUrl(device));

		device = exDevice;

		if (!device.isInitialized()) {
			// register device
			stateChanger.stateChanged(device, State.REGISTERED);

			save(device);
			actionTimerService.setActionForced(device);

			log.info("Registered :" + device.getName());
		}

		if (device.isInBadState()) {
			stateChanger.stateChanged(device, State.REREGISTERED);

			save(device);
			actionTimerService.setActionForced(device);

			deviceMqService.initTopicsToFromDevice(device.getName());

			log.info("ReREgistered :" + device.getName());
		}

		log.debug("register process complete");

		return exists;
	}

	@Override
	public void processDataReceivedFromEntity(Entity entity, JSONObject data) {
		Device device = getDeviceById(entity.getGroup().getDevice().getId());

		processDataReceivedFromDevice(device, data, false, false);
	}

	@Override
	public void processDataReceivedFromDevice(Device device, String deviceResponse, boolean updateDeviceTimer,
			boolean updateGroupTimer) {
		try {
			JSONObject deviceJson = new JSONObject(deviceResponse);
			processDataReceivedFromDevice(device, deviceJson, updateDeviceTimer, updateGroupTimer);

		} catch (Exception e) {
			log.error(e);
			stateChanger.stateChanged(device, State.BAD_DATA);
		}
	}

	@Override
	public IEntityFieldService getEntityFieldService() {
		return entityFieldService;
	}

	@Override
	public void saveEntityFieldValues(List<IEntityField> fields) {
		List<EntityFieldValue> changedValues = Lists.newArrayList();

		fields.stream().filter(SmartHouseItemBuildService::isFieldValueSaveAble)
				.forEach(entityField -> itemBuildService.processValueChange(entityField, null, changedValues));

		entityFieldService.saveAll(changedValues);
	}

	@Override
	@Transactional
	public void processDataReceivedFromDevice(Device device, JSONObject deviceJson, boolean updateDeviceTimer,
			boolean updateGroupTimer) {
		boolean doSave = false;

		try {
			doSave = itemBuildService.processDeviceInfo(device, deviceJson);

			if (!device.isInitialized()) {
				// build new device
				boolean initOk = itemBuildService.buildDeviceFromJson(device, deviceJson);

				if (initOk) {
					stateChanger.stateChanged(device, State.INIT_DATA_RECEIVED);

					List<EntityFieldValue> changedValues = Lists.newArrayList();

					device.getEntities().stream().flatMap(entity -> entity.getEntityFields().stream())
							.filter(SmartHouseItemBuildService::isFieldValueSaveAble)
							.forEach(entityField -> itemBuildService.processValueChange(entityField, null,
									changedValues));

					entityFieldService.saveAll(changedValues);

					alarmService.loadAlarmsForDevice(device);

					deviceMqService.initTopicsToFromDevice(device.getName());

					log.info("Device initialized " + device.getName());

				} else {
					log.error("Device initialize failed JSON =" + deviceJson.toString());
				}

				doSave = true;
			} else {
				List<EntityFieldValue> changedValues = Lists.newArrayList();
				// setDevice values
				boolean ok = itemBuildService.updateDeviceEntityValuesFromJson(device, deviceJson, updateDeviceTimer,
						updateGroupTimer, changedValues);

				State newState = (ok) ? State.UPDATED : State.BAD_DATA;
				stateChanger.stateChanged(device, newState);

				if ((mock || logDeviceValuesChange) && changedValues.size() > 0) {

					Map<String, String> changedValuesStrMap = Maps.newHashMap();
					String tmp = "";

					for (EntityFieldValue value : changedValues) {
						String entityName = value.getEntityField().getEntity().getName();
						if (!changedValuesStrMap.containsKey(entityName)) {
							changedValuesStrMap.put(entityName, new String(""));
						}
						changedValuesStrMap.put(entityName, changedValuesStrMap.get(entityName)
								+ value.getEntityField().getName() + "=" + value.getValueStr() + "; ");
					}
					tmp = Joiner.on(") ").withKeyValueSeparator("(").join(changedValuesStrMap) + ")";

					log.info("Values changed = " + changedValues.size() + " device=" + device.getDescription()
							+ " VALUES " + tmp);
				}

				entityFieldService.saveAll(changedValues);

				doSave = false;
			}
		} catch (Exception e) {
			log.error(e);
			stateChanger.stateChanged(device, State.BAD_DATA);
		}

		if (doSave) {
			device = save(device);
			log.debug("device saved");
		}

	}

	@Override
	@Scheduled(fixedRateString = "${smarthouse.server.fields.data.request.interval.notRegistered:120000}")
	public void requestAllNotRegisteredDevice() {

		try {

			if (isNotFirstIteration) {
				List<Device> notRegisteredDevices = getNotRegisteredDevices();

				if (notRegisteredDevices != null && notRegisteredDevices.size() > 0) {

					log.warn("Try to init not registered devices count = " + notRegisteredDevices.size());

					notRegisteredDevices.forEach(device -> {
						try {
							requestDevicesValues(device, null);
						} catch (Exception e) {
							log.error("FAILED request notRegistered devices");
						}
					});

				}
			} else {
				isNotFirstIteration = true;
			}
		} catch (Exception e) {
			log.error(e);
		}
	}

	@Override
	@Scheduled(fixedRateString = "${smarthouse.server.fields.data.request.interval.registered:5000}")
	public void requestAllDevicesDataWithUpdateRequired() {
		if (!getDevices().isEmpty()) {

			lastDeviceDRUcheck = new Date();

			Map<String, String> deviceIds = new HashMap<>();

			List<Device> devicesRu = getDevicesRequireUpdate();

			List<Group> groupsRu = getGroupsRequireUpdate();

			devicesRu.stream().filter(device -> !device.isVirtualized()).forEach(device -> {
				deviceIds.put(device.getName(), null);
				stateChanger.stateChanged(device, State.TIMED_OUT);
				requestDevicesValues(device, null);
			});
			groupsRu.stream().filter(
					group -> !group.getDevice().isVirtualized() && !deviceIds.containsKey(group.getDevice().getName()))
					.forEach(group -> {
						stateChanger.stateChanged(group.getDevice(), State.TIMED_OUT);
						requestDevicesValues(group.getDevice(), group);
					});
		}
	}

	@Override
	public List<Device> getNotRegisteredDevices() {
		return getDevices().stream().filter(dev -> !dev.isRegistered()).collect(Collectors.toList());
	}

	@Override
	public List<Device> getDevicesRequireUpdate() {
		return getDevices()
				.stream().filter(dev -> dev.isRegistered() && (!dev.isInitialized() || actionTimerService
						.getActionTimer(dev).isActionForced())/* && checkItemRequiresUpdate(dev, dev) */)
				.collect(Collectors.toList());
	}

	@Override
	public List<Group> getGroupsRequireUpdate() {
		return getDevices().stream()
				.filter(dev -> dev.isRegistered() && !dev.isVirtualized() /* && !dev.isInitialized() */
						&& dev.getGroups() != null && !dev.getGroups().isEmpty())
				.flatMap(dev -> dev.getGroups().stream())
				.filter(group -> checkItemRequiresUpdate(group.getDevice(), actionTimerService.getActionTimer(group)))
				.collect(Collectors.toList());
	}

	@Override
	public void requestDevicesValues(String deviceName) {
		Device device = getDeviceByName(deviceName);

		if (device != null) {
			requestDevicesValues(device, null);
		}
	}

	@Override
	public void requestDevicesValues(Device device, Group group) {
		State oldState = device.getState();

		boolean groupRequest = group != null;
		boolean logDR = mock || logDeviceRequests;

		try {
			if (logDR) {
				String message = "Request d=" + device.getName();

				if (group != null) {
					message += " g=" + group.getName();
				}
				log.info(message);

			} else {
				log.debug("Request data " + device.getName());
			}

			String groupId = groupRequest ? group.getName() : null;
			String result = deviceRequestor.executeGetDataOnDevice(device, groupId);

			processDataReceivedFromDevice(device, result, !groupRequest, groupRequest);

		} catch (Exception e) {
			actionTimerService.getActionTimer(device).update(60000, false);

			if (logDR || !State.DISCONNECTED.equals(oldState)) {
				log.error("Error request device values", e.getMessage());
			}

			stateChanger.stateChanged(device, State.DISCONNECTED);
		}
	}

	protected boolean checkItemRequiresUpdate(IDevice target, ActionTimer timer) {
		if (target.isVirtualized()) {
			return false;
		}
		boolean waits = timer.isActionForced();
		boolean dataTooOld = timer.isTimeToExecuteAction();

		if (dataTooOld && !target.getState().equals(State.TIMED_OUT) && !target.getState().equals(State.DISCONNECTED)) {
			stateChanger.stateChanged(target, State.TIMED_OUT);
		}

		return waits || dataTooOld;
	}

	@Override
	public String sendDataToDevice(Long deviceId, Long entityId, String actionData) {
		Device device = getDeviceById(deviceId);

		if (device == null) {
			throw new IllegalArgumentException("device id not found" + deviceId);
		}

		IEntity entity = device.getEntity(entityId);

		JSONObject json = new JSONObject(actionData);

		return sendDataToDevice(device, entity, json.toMap());

	}

	@Override
	public String sendDataToDevice(String deviceName, String entityName, Map<String, Object> values) {

		Device device = getDeviceByName(deviceName);

		if (device == null) {
			throw new IllegalArgumentException("device name not found" + deviceName);
		}

		IEntity entity = device.getEntity(entityName);

		return sendDataToDevice(device, entity, values);
	}

	@Override
	public String sendDataToDevice(IDevice device, IEntity entity, Map<String, Object> values) {
		if (!device.isVirtualized()) {
			try {
				preprocessSendDataToDevice(device, entity, values);
				String result = deviceRequestor.executePostDataOnDeviceEntity(device, entity, values);
				processDataReceivedFromDevice((Device) device, result, false, false);

				if (requestDataAfterSendToDevice) {
					actionTimerService.setActionForced(device);
				}

				return result;
			} catch (Exception e) {
				log.error(e);
				throw e;
			}
		}
		return "";
	}

	private void preprocessSendDataToDevice(IDevice device, IEntity entity, Map<String, Object> values) {
		if (saveDataBeforeSendToDevice) {

			List<EntityFieldValue> changedValues = new ArrayList<>();

			for (String entityFieldName : values.keySet()) {
				IEntityField entityField = entity.getEntityField(entityFieldName);

				if (entityField != null) {
					if (Number.class.isAssignableFrom(entityField.getClazz())) {
						changedValues.add(new EntityFieldValueNumber(entityField));
					} else if (Boolean.class.isAssignableFrom(entityField.getClazz())) {
						changedValues.add(new EntityFieldValueBoolean(entityField));
					}
				}
			}
			log.info("Saved before post =" + changedValues.size());
			entityFieldService.saveAll(changedValues);

		}
	}

	@Override
	public IDevice save(IDevice device) {
		return save((Device) device);
	}

	@Transactional
	@Override
	public Device save(Device device) {

		boolean initialized = device.isInitialized();
		Map<String, Object> fieldValues = new HashMap<>();

		if (device.getGroups() != null) {
			for (Group group : device.getGroups()) {
				putFieldValuesToMap(group, fieldValues);
			}
		}

		Long id = deviceRepository.save(device).getId();
		device = deviceRepository.findById(id).orElse(null);

		int index = getDeviceIndex(id);

		if (index > -1) {
			devices.set(index, device);
		} else {
			log.info("added Device ");
			devices.add(device);
		}

		device.setInitialized(initialized);

		if (device.getGroups() != null) {
			for (Group group : device.getGroups()) {
				putFieldValuesFromMap(group, fieldValues);
			}
		}

		alarmService.reattachAlarms(device);
		alarmV2Service.reattachAlarms(device);

		log.debug("saved Device" + device.getName());

		return device;
	}

	@Override
	public IGroup save(IGroup group) {
		Map<String, Object> fieldValues = new HashMap<>();
		Long oldDeviceId = null;
		Group oldGroup = null;

		if (ItemAbstract.existsAndIsNotNew(group)) {
			oldGroup = getGroupById(group.getId());
			if(oldGroup!=null && ItemAbstract.existsAndIsNotNew(oldGroup.getDevice())) {
				oldDeviceId = oldGroup.getDevice().getId();
			}
		}

		putFieldValuesToMap(group, fieldValues);

		Long id = groupRepository.save((Group) group).getId();
		group = groupRepository.findById(id).orElse(null);

		Group exists = getGroupById(id);

		if (exists != null) {
			if (oldDeviceId != null) {
				Device oldDevice = getDeviceById(oldDeviceId);
				if (oldDevice != null && oldDevice.getGroups() != null && oldGroup != null) {
					oldDevice.getGroups().remove(oldGroup);
				}
			}

			if (exists.getDevice() != null && exists.getDevice().getId() != null) {
				Device newDevice = getDeviceById(exists.getDevice().getId());
				if (newDevice.getGroups() != null) {
					newDevice.getGroups().add((Group) group);
				}
			}

		} else if (group.getDevice() != null && group.getDevice().getId() != null) {
			log.info("added Group");
			Device realDevice = getDeviceById(group.getDevice().getId());
			realDevice.getGroups().add((Group) group);
		}

		putFieldValuesFromMap(group, fieldValues);

		alarmV2Service.reattachAlarms(group);

		log.debug("saved Group" + group.getName());

		return group;
	}

	@Override
	public IEntity save(IEntity entity) {
		Map<String, Object> fieldValues = new HashMap<>();

		putFieldValuesToMap(entity, fieldValues);

		Long id = entityRepository.save((Entity) entity).getId();
		entity = entityRepository.findById(id).orElse(null);

		Entity exists = getEntityById(id);

		if (exists != null) {
			Group realGroup = getGroupById(exists.getGroup().getId());
			Set<Entity> entities = realGroup.getEntities();

			entities.remove(exists);
			entities.add((Entity) entity);

		} else if (entity.getGroup() != null && entity.getGroup().getId() != null) {
			log.info("added Entity");
			Group realGroup = getGroupById(entity.getGroup().getId());
			realGroup.getEntities().add((Entity) entity);
		}

		putFieldValuesFromMap(entity, fieldValues);

		alarmV2Service.reattachAlarms(entity);

		log.debug("saved Entity" + entity.getName());

		return entity;
	}

	@Override
	public IEntityField save(IEntityField entityField) {
		Map<String, Object> fieldValues = new HashMap<>();

		putFieldValuesToMap(entityField, fieldValues);

		Long id = entityFieldRepository.save((EntityField) entityField).getId();
		entityField = entityFieldRepository.findById(id).orElse(null);

		IEntityField exists = getEntityFieldById(id);

		if (exists != null) {
			Entity realEntity = getEntityById(exists.getEntity().getId());
			Set<IEntityField> entityFields = realEntity.getEntityFields();

			entityFields.remove(exists);
			entityFields.add((EntityField) entityField);

		} else if (entityField.getEntity() != null && entityField.getEntity().getId() != null) {
			log.info("added Entityfield");
			Entity realEntity = getEntityById(entityField.getEntity().getId());
			realEntity.getEntityFields().add(entityField);
		}

		putFieldValuesFromMap(entityField, fieldValues);

		alarmV2Service.reattachAlarms(entityField);

		log.debug("saved EntityField" + entityField.getName());

		return entityField;
	}

	@Override
	public void reattachAlarmsForDevice(IDevice device) {
		alarmService.reattachAlarms(device);
	}

	@Override
	public List<Entity> getEntitiesForDevice(Long deviceId) {
		return getDeviceById(deviceId).getEntities().stream()
				.filter(e -> ItemType.SENSORS.equals(e.getGroup().getType())).sorted(ItemAbstract::compareByName)
				.collect(Collectors.toList());
	}

	@Override
	public DeviceEntityFieldActionHolder getValueActionHolder(Long deviceId) {
		DeviceEntityFieldActionHolder holder = new DeviceEntityFieldActionHolder();
		List<IEntityField> entityFields = getCurrentEntityFieldsForDevice(deviceId);

		for (IEntityField entityField : entityFields) {
			if (entityField.isButton() && !entityField.isReadOnly()) {
				List<Action> actions = entityFieldService.getActionsForEntityField(ACTION_TYPE_SEND_DATA_TO_DEVICE,
						entityField);
				holder.addFieldAction(entityField.getEntity().getName(), actions);
			} else {
				holder.addFieldValue(entityField);
			}
		}

		return holder;
	}

	@Override
	public List<IEntityField> getCurrentEntityFieldsForDevice(Long deviceId) {

		return getDeviceById(deviceId).getEntities().stream().flatMap(entity -> entity.getEntityFields().stream())
				.filter(IEntityField::isNotCalculated).filter(SmartHouseItemBuildService::isFieldValueSaveAble)
				.sorted(ItemAbstract::compareByName).collect(Collectors.toList());

	}

	@Override
	public List<EntityFieldValue> getLastEntityFieldValuesForDevice(Long deviceId) {
		return entityFieldService.getLastEntityFieldValuesForDevice(deviceId);
	}

	@Override
	public List<EntityFieldValue> getLastEntityFieldValuesForEntity(Long entityId) {
		return entityFieldService.getLastEntityFieldValuesForEntity(entityId);
	}

	@Transactional
	@Override
	public void deleteDeviceById(Long deviceId) {
		int index = getDeviceIndex(deviceId);
		devices.remove(index);

		alarmService.deleteAlarmsByDeviceId(deviceId);

		entityFieldService.deleteEntityFieldsForDevice(deviceId);
		entityService.deleteEntitiesForDevice(deviceId);
		groupService.deleteGroupsForDevice(deviceId);
		deviceRepository.deleteByDeviceId(deviceId);
	}

	private String getDeviceUrl(Device device) {
		if (device.getName().startsWith("MockedDeviceId")) {
			return HTTP_PREFFIX + device.getIp() + ControllerConstants.MOCK + "/mock_" + device.getName();
		}

		return HTTP_PREFFIX + device.getIp() + DEVICE_URL_DATA;
	}

	private void putFieldValuesToMap(IGroup group, Map<String, Object> fieldValues) {
		if (group.getEntities() != null && !group.getEntities().isEmpty()) {
			for (Entity entity : group.getEntities()) {
				putFieldValuesToMap(entity, fieldValues);
			}
		}
	}

	private void putFieldValuesToMap(IEntity entity, Map<String, Object> fieldValues) {
		if (entity.getEntityFields() != null && !entity.getEntityFields().isEmpty()) {
			for (IEntityField entityField : entity.getEntityFields()) {
				putFieldValuesToMap(entityField, fieldValues);
			}
		}
	}

	private void putFieldValuesToMap(IEntityField entityField, Map<String, Object> fieldValues) {
		String key = entityField.getEntity().getGroup().getName() + entityField.getEntity().getName()
				+ entityField.getName();
		fieldValues.put(key, entityField.getValue());
	}

	private void putFieldValuesFromMap(IGroup group, Map<String, Object> fieldValues) {
		if (group.getEntities() != null && !group.getEntities().isEmpty()) {
			for (Entity entity : group.getEntities()) {
				putFieldValuesFromMap(entity, fieldValues);
			}
		}
	}

	private void putFieldValuesFromMap(IEntity entity, Map<String, Object> fieldValues) {
		if (entity.getEntityFields() != null && !entity.getEntityFields().isEmpty()) {
			for (IEntityField entityField : entity.getEntityFields()) {
				putFieldValuesFromMap(entityField, fieldValues);
			}
		}
	}

	private void putFieldValuesFromMap(IEntityField entityField, Map<String, Object> fieldValues) {
		String key = entityField.getEntity().getGroup().getName() + entityField.getEntity().getName()
				+ entityField.getName();

		if (entityField.getValueObj() == null && fieldValues.containsKey(key)) {
			entityField.setValueWithNoCheck(fieldValues.get(key));
		}
	}

}
