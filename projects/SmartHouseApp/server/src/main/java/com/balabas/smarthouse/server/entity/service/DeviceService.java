package com.balabas.smarthouse.server.entity.service;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.stream.Collectors;

import org.json.JSONObject;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.scheduling.annotation.Scheduled;
import org.springframework.stereotype.Service;

import com.balabas.smarthouse.server.controller.service.DeviceRequestorService;
import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.IGroup;
import com.balabas.smarthouse.server.entity.model.IUpdateable;
import com.balabas.smarthouse.server.entity.model.SmartHouseEntityBuilder;
import com.balabas.smarthouse.server.entity.model.State;
import com.balabas.smarthouse.server.exception.ResourceNotFoundException;
import com.balabas.smarthouse.server.model.request.DeviceRequest;
import com.balabas.smarthouse.server.view.Action;

import lombok.Getter;
import lombok.extern.log4j.Log4j2;

@Log4j2
@Service
public class DeviceService implements IDeviceService {

	@Autowired
	DeviceRequestorService deviceRequestor;
	
	@Autowired
	IDeviceStateChangeService stateChanger;
	
	@Getter
	private List<IDevice> devices = new ArrayList<>();

	@Override
	public List<IDevice> getDevicesInitialized() {
		return devices.stream().filter(IDevice::isInitialized).collect(Collectors.toList());
	}
	
	@Override
	public IDevice getDevice(String deviceName) {
		return this.devices.stream().filter(d -> d.getName().equals(deviceName)).findFirst().orElse(null);
	}

	@Override
	public IDevice getDevice(IDevice device) {
		return this.devices.stream().filter(d -> d.getName().equals(device.getName())).findFirst().orElse(null);
	}

	@Override
	public void processRegistrationRequest(DeviceRequest request) {
		IDevice deviceFromRequest = SmartHouseEntityBuilder.buildDeviceFromRequest(request);

		IDevice device = getDevice(deviceFromRequest);

		if (device == null) {
			// register device
			device = deviceFromRequest;
			devices.add(device);
			
			stateChanger.stateChanged(device, State.REGISTERED);

			log.info("Registered :" + device.getName());
		} else {
			// reregister device
			log.info("ReRegistered :" + device.getName());
			stateChanger.stateChanged(device, State.REGISTERED);
		}

		device.getTimer().setActionForced(true);
	}

	@Override
	public void requestDevicesValues(IDevice device, IGroup group) {
		State oldState = device.getState();

		try {
			log.debug("Request data " + device.getDeviceName());

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
	

	@Override
	public void processDataReceivedFromDevice(IEntity entity, JSONObject data) {
		IDevice device = getDevice(entity.getDeviceName());
		
		processDataReceivedFromDevice(device, data);
	}

	@Override
	public void processDataReceivedFromDevice(IDevice device, String deviceResponse) {
		try {
			JSONObject deviceJson = new JSONObject(deviceResponse);
			processDataReceivedFromDevice(device, deviceJson);
			
		} catch (Exception e) {
			log.error(e);
			stateChanger.stateChanged(device, State.BAD_DATA);
		}
	}
	
	@Override
	public void processDataReceivedFromDevice(IDevice device, JSONObject deviceJson) {
		try {
			SmartHouseEntityBuilder.processDeviceInfo(device, deviceJson);

			if (!device.isInitialized()) {
				// build new device
				Set<IGroup> groups = SmartHouseEntityBuilder.buildGroupsFromJson(device.getName(), deviceJson);
				boolean initOk = !groups.isEmpty();

				if (initOk) {
					for (IGroup group : groups) {
						Set<IEntity> entities = SmartHouseEntityBuilder.buildEntitiesForGroup(group, deviceJson);

						group.setChildren(entities);
						initOk = initOk && !entities.isEmpty();
					}
				}

				device.setChildren(groups);

				if (initOk) {
					stateChanger.stateChanged(device, State.INIT_DATA_RECEIVED);
				}

			} else {
				// setDevice values
				boolean ok = SmartHouseEntityBuilder.updateDeviceEntityValuesFromJson(device, deviceJson);

				State newState = (ok) ? State.UPDATED : State.BAD_DATA;
				stateChanger.stateChanged(device, newState);
			}
		} catch (Exception e) {
			log.error(e);
			stateChanger.stateChanged(device, State.BAD_DATA);
		}
	}
	
	@Override
	@Scheduled(fixedRate = 5000)
    public void requestAllDevicesDataWithUpdateRequired() {
        if(!getDevices().isEmpty()){
            
            Map<String,String> deviceIds = new HashMap<>();
            
            getDevices().stream()
                .filter(dev->dev.isRegistered() && checkItemRequiresUpdate(dev, dev))
                .forEach(device->{
                        deviceIds.put(device.getName(), null);
                        stateChanger.stateChanged(device, State.TIMED_OUT);
                        requestDevicesValues(device, null);
                    });
            
            getDevices().stream()
            .filter(device->device.isRegistered() && device.getGroups()!=null && !device.getGroups().isEmpty()
                        && !deviceIds.containsKey(device.getName()))
            .forEach(device->
                device.getGroups().stream()
                        .filter(group->checkItemRequiresUpdate(device, group))
                        .forEach(group->{
                        	stateChanger.stateChanged(device, State.TIMED_OUT);
                        	requestDevicesValues(device,group);
                        })
            );
        }
    }
	
	protected boolean checkItemRequiresUpdate(IDevice target, IUpdateable source){
    	boolean waits = source.getTimer().isActionForced();
		boolean dataTooOld = source.getTimer().isTimeToExecuteAction();
		
		if(dataTooOld && !target.getState().equals(State.TIMED_OUT) 
				&& !target.getState().equals(State.DISCONNECTED)){
			
			stateChanger.stateChanged(target, State.TIMED_OUT);
		}
		
		return waits || dataTooOld;
    }

	@Override
	public void processDeviceAction(Action action) throws ResourceNotFoundException {
		log.debug("Action received :"+action.getCallbackData());
    	Map<String,Object> params = (new JSONObject(action.getData())).toMap(); 
    	
		sendDataToDevice(action.getDeviceId(), action.getGroupId(), action.getEntityId(), params);
	}
	
	@Override
	public String sendDataToDevice(String deviceName, String groupName, String entityName, Map<String, Object> values) throws ResourceNotFoundException {
		
		IDevice device = getDevice(deviceName);
		
		if(device == null) {
			throw new ResourceNotFoundException(deviceName);
		}
		
		IEntity entity = device.getGroup(groupName).getEntity(entityName);
		
		String result = deviceRequestor.executePostDataOnDeviceEntity(device, entity, values);
		processDataReceivedFromDevice(device, result);
		
		return result;
		
	}

}
