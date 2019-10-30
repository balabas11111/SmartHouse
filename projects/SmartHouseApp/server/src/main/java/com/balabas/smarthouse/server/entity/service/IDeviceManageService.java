package com.balabas.smarthouse.server.entity.service;

import java.util.List;
import java.util.Map;

import org.json.JSONObject;

import com.balabas.smarthouse.server.entity.model.Device;
import com.balabas.smarthouse.server.entity.model.Entity;
import com.balabas.smarthouse.server.entity.model.Group;
import com.balabas.smarthouse.server.entity.model.enabledvalue.IEntityFieldEnabledValue;
import com.balabas.smarthouse.server.entity.model.entityfields.IEntityField;
import com.balabas.smarthouse.server.exception.ResourceNotFoundException;
import com.balabas.smarthouse.server.model.request.DeviceRequest;

@SuppressWarnings({ "rawtypes" })
public interface IDeviceManageService {
	
	Device getManagedDevice(Device device);
	
	
	List<Device> getDevices();
	
	List<Device> getDevicesInitialized();
	
	void processDataReceivedFromDevice(Device device, String deviceResponse, boolean updateDeviceTimer, boolean updateGroupTimer);
	
	void processDataReceivedFromDevice(Device device, JSONObject data, boolean updateDeviceTimer, boolean updateGroupTimer);
	
	void processDataReceivedFromEntity(Entity entity, JSONObject data);

	void requestDevicesValues(Device device, Group group);

	void processRegistrationRequest(DeviceRequest request);

	void requestAllDevicesDataWithUpdateRequired();

	String sendDataToDevice(String deviceName, String groupName, String entityName, Map<String, Object> values)
			throws ResourceNotFoundException;

	Device getDeviceById(Long id);
	
	Device getDeviceByName(String name);

	Group getGroupById(Long id);

	Entity getEntityById(Long id);

	IEntityField getEntityFieldById(Long id);

	IEntityFieldEnabledValue getEntityFieldEnabledValueById(Long id);

	Device save(Device device);

	List<Device> getDevicesRequireUpdate();

	List<Group> getGroupsRequireUpdate();

	void createNewEntityAlarm(Long entityId);

}
