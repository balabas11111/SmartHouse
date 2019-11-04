package com.balabas.smarthouse.server.entity.service;

import static com.balabas.smarthouse.server.DeviceConstants.DEVICE_URL_DATA;
import static com.balabas.smarthouse.server.DeviceConstants.HTTP_PREFFIX;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Date;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Optional;
import java.util.stream.Collectors;

import javax.annotation.PostConstruct;

import org.json.JSONObject;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Value;
import org.springframework.scheduling.annotation.Scheduled;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import com.balabas.smarthouse.server.controller.service.DeviceRequestorService;
import com.balabas.smarthouse.server.entity.alarm.IEntityAlarmService;
import com.balabas.smarthouse.server.entity.model.ActionTimer;
import com.balabas.smarthouse.server.entity.model.Device;
import com.balabas.smarthouse.server.entity.model.Entity;
import com.balabas.smarthouse.server.entity.model.EntityFieldValue;
import com.balabas.smarthouse.server.entity.model.Group;
import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.IUpdateable;
import com.balabas.smarthouse.server.entity.model.descriptor.State;
import com.balabas.smarthouse.server.entity.model.enabledvalue.IEntityFieldEnabledValue;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.entity.repository.IDeviceRepository;
import com.balabas.smarthouse.server.entity.repository.IEntityFieldValueRepository;
import com.balabas.smarthouse.server.exception.ResourceNotFoundException;
import com.google.common.collect.Lists;

import lombok.Getter;
import lombok.extern.log4j.Log4j2;

@Log4j2
@Service
@SuppressWarnings({ "rawtypes", "unchecked" })
public class DeviceManageService implements IDeviceManageService {

	@Value("${smarthouse.server.mock}")
	private boolean mock;

	@Value("${smarthouse.server.log.devicerequests}")
	private boolean logDeviceRequests;

	@Autowired
	DeviceRequestorService deviceRequestor;

	@Autowired
	IDeviceStateChangeService stateChanger;

	@Autowired
	SmartHouseItemBuildService itemBuildService;

	@Autowired
	IEntityAlarmService alarmService;

	@Autowired
	IDeviceRepository deviceRepository;

	@Autowired
	IEntityFieldValueRepository entityFieldValueRepository;

	@Getter
	private List<Device> devices = Collections.synchronizedList(new ArrayList<>());

	@Getter
	private Date lastDeviceDRUcheck;

	@PostConstruct
	@Transactional
	public void afterPropertiesSet() throws Exception {

		deviceRepository.findAll().forEach(d -> {
			d.setState(State.CONSTRUCTED);
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
		for (Device device : devices) {
			if (id.equals(device.getId())) {
				return device;
			}
		}

		return null;
	}

	@Override
	public Device getDeviceByName(String deviceName) {
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

		Device exDevice = getDeviceByName(device.getName());

		if (exDevice == null) {
			exDevice = Optional.ofNullable(deviceRepository.findByName(device.getName())).orElse(new Device());

			if (exDevice.getId() == null || exDevice.getId() == 0L) {
				exDevice.setName(device.getName());
				exDevice.setState(State.CONSTRUCTED);
				exDevice.setDataUrl(HTTP_PREFFIX + device.getIp() + DEVICE_URL_DATA);
				exDevice.setRegistrationDate(new Date());
			}
		}

		exDevice.setIp(device.getIp());
		device = exDevice;

		if (!device.isInitialized()) {
			// register device
			stateChanger.stateChanged(device, State.REGISTERED);

			save(device).getTimer().setActionForced(true);

			log.info("Registered :" + device.getName());
		}

		log.debug("register process complete");

		return true;
	}

	@Override
	public void processDataReceivedFromEntity(Entity entity, JSONObject data) {
		Device device = entity.getGroup().getDevice();

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
					.forEach( entityField -> {
							if(SmartHouseItemBuildService.isFieldValueSaveAble(entityField)) {
								SmartHouseItemBuildService.processValueChange(entityField, null, changedValues);
							}
						});
					
					entityFieldValueRepository.saveAll(changedValues);
					
					alarmService.loadAlarmsForDevice(device);
					
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

				if (mock || logDeviceRequests) {
					log.info("Values changed = " + changedValues.size());
				}
				
				entityFieldValueRepository.saveAll(changedValues);

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
	@Scheduled(fixedRate = 5000)
	public void requestAllDevicesDataWithUpdateRequired() {
		if (!getDevices().isEmpty()) {

			lastDeviceDRUcheck = new Date();

			Map<String, String> deviceIds = new HashMap<>();

			List<Device> devicesRu = getDevicesRequireUpdate();

			List<Group> groupsRu = getGroupsRequireUpdate();

			devicesRu.forEach(device -> {
				deviceIds.put(device.getName(), null);
				stateChanger.stateChanged(device, State.TIMED_OUT);
				requestDevicesValues(device, null);
			});
			groupsRu.stream().filter(group -> !deviceIds.containsKey(group.getDevice().getName())).forEach(group -> {
				stateChanger.stateChanged(group.getDevice(), State.TIMED_OUT);
				requestDevicesValues(group.getDevice(), group);
			});
		}
	}

	@Override
	public List<Device> getDevicesRequireUpdate() {
		return getDevices().stream().filter(dev -> dev.isRegistered() && checkItemRequiresUpdate(dev, dev))
				.collect(Collectors.toList());
	}

	@Override
	public List<Group> getGroupsRequireUpdate() {
		return getDevices().stream()
				.filter(device -> device.isRegistered() && device.getGroups() != null && !device.getGroups().isEmpty())
				.flatMap(device -> device.getGroups().stream())
				.filter(group -> checkItemRequiresUpdate(group.getDevice(), group)).collect(Collectors.toList());
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
			device.getTimer().update(60000, false);

			if (logDR || !State.DISCONNECTED.equals(oldState)) {
				log.error("Error request device values", e.getMessage());
			}

			stateChanger.stateChanged(device, State.DISCONNECTED);
		}
	}

	protected boolean checkItemRequiresUpdate(Device target, IUpdateable source) {
		boolean waits = source.getTimer().isActionForced();
		boolean dataTooOld = source.getTimer().isTimeToExecuteAction();

		if (dataTooOld && !target.getState().equals(State.TIMED_OUT) && !target.getState().equals(State.DISCONNECTED)) {

			stateChanger.stateChanged(target, State.TIMED_OUT);
		}

		return waits || dataTooOld;
	}

	@Override
	public String sendDataToDevice(String deviceName, String groupName, String entityName, Map<String, Object> values)
			throws ResourceNotFoundException {

		Device device = getDeviceByName(deviceName);

		if (device == null) {
			throw new IllegalArgumentException("device name not found" + deviceName);
		}

		IEntity entity = device.getGroup(groupName).getEntity(entityName);

		try {
			String result = deviceRequestor.executePostDataOnDeviceEntity(device, entity, values);
			processDataReceivedFromDevice(device, result, false, false);
			return result;
		} catch (Exception e) {
			log.error(e);
			throw e;
		}
	}

	@Transactional
	@Override
	public Device save(Device device) {

		boolean initialized = device.isInitialized();
		ActionTimer timer = device.getTimer();
		Map<String, ActionTimer> groupTimers = new HashMap<>();
		// Map<String, Set<IEntityField>> generatedFields = new HashMap<>();
		Map<String, Object> fieldValues = new HashMap<>();

		if (device.getGroups() != null) {

			for (Group group : device.getGroups()) {
				if (group.getTimer() == null) {

				}
				groupTimers.put(group.getName(), group.getTimer());

				if (group.getEntities() != null && !group.getEntities().isEmpty()) {
					for (Entity entity : group.getEntities()) {
						if (entity.getEntityFields() != null && !entity.getEntityFields().isEmpty()) {
							for (IEntityField entityField : entity.getEntityFields()) {
								String key = group.getName() + entity.getName() + entityField.getName();

								fieldValues.put(key, entityField.getValue());
							}
						}
					}
				}
			}
		}

		Long id = deviceRepository.save(device).getId();
		device = deviceRepository.findById(id).orElse(null);

		int index = getDeviceIndex(id);

		if (index > -1) {
			devices.set(index, device);
		} else {
			devices.add(device);
		}

		device.setInitialized(initialized);
		device.setTimer(timer);

		if (device.getGroups() != null) {
			for (Group group : device.getGroups()) {
				group.setTimer(groupTimers.get(group.getName()));

				if (group.getEntities() != null && !group.getEntities().isEmpty()) {
					for (Entity entity : group.getEntities()) {
						if (entity.getEntityFields() != null && !entity.getEntityFields().isEmpty()) {
							for (IEntityField entityField : entity.getEntityFields()) {
								String keyEntityField = group.getName() + entity.getName() + entityField.getName();

								if (entityField.getValueObj() == null && fieldValues.containsKey(keyEntityField)) {
									entityField.setValueWithNoCheck(fieldValues.get(keyEntityField));
								}
							}
						}
					}
				}
			}
		}

		alarmService.reattachAlarms(device);

		log.info("saved " + device.getName());

		return device;
	}

	@Override
	public void reattachAlarmsForDevice(IDevice device) {
		alarmService.reattachAlarms(device);
	}

	@Override
	public void reattachAlarmsForDevice(String deviceName) {
		IDevice device = getDeviceByName(deviceName);
		reattachAlarmsForDevice(device);
	}

	@Override
	public void reattachAlarmsForEntity(IEntity entity) {
		Device device = entity.getGroup().getDevice();
		reattachAlarmsForDevice(device);
	}

	@Override
	public void reattachAlarmsForEntityField(IEntityField entityField) {
		reattachAlarmsForEntity(entityField.getEntity());
	}

	@Override
	public void reattachAlarmsForEntity(Long entityId) {
		reattachAlarmsForEntity(getEntityById(entityId));
	}

	@Override
	public void reattachAlarmsForEntityAlarm(Long entityAlarmId) {
		IEntity entity = alarmService.getAlarmById(entityAlarmId).getEntity();
		reattachAlarmsForEntity(entity);
	}

	@Override
	public void reattachAlarmsForEntityFieldAlarm(Long entityFieldAlarmId) {
		IEntityField entityField = getEntityFieldById(entityFieldAlarmId);
		reattachAlarmsForEntity(entityField.getEntity());
	}

}
