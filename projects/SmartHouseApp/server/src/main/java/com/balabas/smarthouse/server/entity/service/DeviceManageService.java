package com.balabas.smarthouse.server.entity.service;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.stream.Collectors;

import javax.transaction.Transactional;

import org.json.JSONObject;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.scheduling.annotation.Scheduled;
import org.springframework.stereotype.Service;

import com.balabas.smarthouse.server.controller.service.DeviceRequestorService;
import com.balabas.smarthouse.server.entity.model.Device;
import com.balabas.smarthouse.server.entity.model.Entity;
import com.balabas.smarthouse.server.entity.model.Group;
import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.IUpdateable;
import com.balabas.smarthouse.server.entity.model.descriptor.State;
import com.balabas.smarthouse.server.entity.repository.IDeviceRepository;
import com.balabas.smarthouse.server.entity.repository.IGroupRepository;
import com.balabas.smarthouse.server.exception.ResourceNotFoundException;
import com.balabas.smarthouse.server.model.request.DeviceRequest;
import com.balabas.smarthouse.server.view.Action;

import lombok.Getter;
import lombok.extern.log4j.Log4j2;

@Log4j2
@Service
public class DeviceManageService implements IDeviceManageService {

	@Autowired
	DeviceRequestorService deviceRequestor;

	@Autowired
	IDeviceStateChangeService stateChanger;

	@Autowired
	SmartHouseItemBuildService itemBuildService;

	@Autowired
	IDeviceRepository deviceRepository;
	
	@Autowired
	IGroupRepository groupRepository;

	@Getter
	private List<Device> devices = new ArrayList<>();

	@Override
	public List<Device> getDevicesInitialized() {
		return devices.stream().filter(Device::isInitialized).collect(Collectors.toList());
	}

	@Override
	public Device getManagedDeviceByName(String deviceName) {
		return this.devices.stream().filter(d -> d.getName().equals(deviceName)).map(d -> (Device) d).findFirst()
				.orElse(null);
	}

	@Override
	public Device getManagedDevice(Device device) {
		return this.devices.stream().filter(d -> d.getName().equals(device.getName())).map(d -> (Device) d).findFirst()
				.orElse(null);
	}

	@Override
	@Transactional
	public void processRegistrationRequest(DeviceRequest request) {
		Device deviceFromRequest = itemBuildService.buildDeviceFromRequest(request);

		Device device = getManagedDevice(deviceFromRequest);

		if (device == null) {
			// register device
			device = deviceFromRequest;
			devices.add(device);
			stateChanger.stateChanged(device, State.REGISTERED);

			log.info("Registered :" + device.getName());
		} else {
			// reregister device
			device.update(deviceFromRequest);
			stateChanger.stateChanged(device, State.REREGISTERED);

			log.info("ReRegistered :" + device.getName());
		}

		//deviceRepository.updateDeviceState(device.getId(), device.getState());
		deviceRepository.save(device);

		device.getTimer().setActionForced(true);
	}

	@Override
	public void processDataReceivedFromDevice(Entity entity, JSONObject data) {
		Device device = entity.getGroup().getDevice();

		processDataReceivedFromDevice(device, data);
	}

	@Override
	public void processDataReceivedFromDevice(Device device, String deviceResponse) {
		try {
			JSONObject deviceJson = new JSONObject(deviceResponse);
			processDataReceivedFromDevice(device, deviceJson);

		} catch (Exception e) {
			log.error(e);
			stateChanger.stateChanged(device, State.BAD_DATA);
		}
	}

	@Override
	@Transactional
	public void processDataReceivedFromDevice(Device device, JSONObject deviceJson) {
		boolean doSave = false;

		try {
			doSave = itemBuildService.processDeviceInfo(device, deviceJson);

			if (!device.isInitialized()) {
				// build new device
				boolean initOk = itemBuildService.buildDeviceFromJson(device, deviceJson);

				if (initOk) {
					stateChanger.stateChanged(device, State.INIT_DATA_RECEIVED);
				} else {
					log.error("Device initialize failed JSON =" + deviceJson.toString());
				}

				doSave = true;
			} else {
				// setDevice values
				boolean ok = itemBuildService.updateDeviceEntityValuesFromJson(device, deviceJson);

				State newState = (ok) ? State.UPDATED : State.BAD_DATA;
				stateChanger.stateChanged(device, newState);
			}
		} catch (Exception e) {
			log.error(e);
			stateChanger.stateChanged(device, State.BAD_DATA);
		}

		if (doSave) {
			deviceRepository.save(device);
			
			
		}
	}

	@Override
	@Scheduled(fixedRate = 5000)
	public void requestAllDevicesDataWithUpdateRequired() {
		if (!getDevices().isEmpty()) {

			Map<String, String> deviceIds = new HashMap<>();

			getDevices().stream().filter(dev -> dev.isRegistered() && checkItemRequiresUpdate(dev, dev))
					.forEach(device -> {
						deviceIds.put(device.getName(), null);
						stateChanger.stateChanged(device, State.TIMED_OUT);
						requestDevicesValues(device, null);
					});

			getDevices().stream()
					.filter(device -> device.isRegistered() && device.getGroups() != null
							&& !device.getGroups().isEmpty() && !deviceIds.containsKey(device.getName()))
					.forEach(device -> device.getGroups().stream()
							.filter(group -> checkItemRequiresUpdate(device, group)).forEach(group -> {
								stateChanger.stateChanged(device, State.TIMED_OUT);
								requestDevicesValues(device, group);
							}));
		}
	}

	@Override
	public void requestDevicesValues(Device device, Group group) {
		State oldState = device.getState();

		try {
			log.debug("Request data " + device.getName());

			String groupId = group != null ? group.getName() : null;
			String result = deviceRequestor.executeGetDataOnDevice(device, groupId);

			processDataReceivedFromDevice(device, result);

		} catch (Exception e) {
			device.getTimer().update(60000, false);

			if (!State.DISCONNECTED.equals(oldState)) {
				log.error("Error request device values", e);
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
	public void processDeviceAction(Action action) throws ResourceNotFoundException {
		log.debug("Action received :" + action.getCallbackData());
		Map<String, Object> params = (new JSONObject(action.getData())).toMap();

		String deviceName = action.getDeviceId();
		String groupName = action.getGroupId();
		String entityName = action.getEntityId();

		if (groupName == null || groupName.isEmpty()) {
			Device device = getManagedDeviceByName(action.getDeviceId());

			try {
				Integer entityRemoteId = Integer.valueOf(entityName);
				IEntity entity = device.getEntity(entityRemoteId);

				groupName = entity.getGroup().getName();
				entityName = entity.getName();

				action.setGroupId(groupName);
				action.setEntityId(entityName);
			} catch (Exception e) {
				groupName = device.getEntity(action.getEntityId()).getGroup().getName();
			}
		}

		sendDataToDevice(deviceName, groupName, entityName, params);
	}

	@Override
	public String sendDataToDevice(String deviceName, String groupName, String entityName, Map<String, Object> values)
			throws ResourceNotFoundException {

		Device device = getManagedDeviceByName(deviceName);

		if (device == null) {
			throw new ResourceNotFoundException(deviceName);
		}

		IEntity entity = device.getGroup(groupName).getEntity(entityName);

		String result = deviceRequestor.executePostDataOnDeviceEntity(device, entity, values);
		processDataReceivedFromDevice(device, result);

		return result;

	}

}
