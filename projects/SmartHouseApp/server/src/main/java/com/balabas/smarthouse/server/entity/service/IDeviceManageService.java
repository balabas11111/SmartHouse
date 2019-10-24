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
	
	Device getManagedDeviceByName(String deviceName);
	
	List<Device> getDevices();
	
	List<Device> getDevicesInitialized();
	
	void processDataReceivedFromDevice(Device device, String deviceResponse);
	
	void processDataReceivedFromDevice(Device device, JSONObject data);
	
	void processDataReceivedFromDevice(Entity entity, JSONObject data);

	void requestDevicesValues(Device device, Group group);

	void processRegistrationRequest(DeviceRequest request);

	void requestAllDevicesDataWithUpdateRequired();

	String sendDataToDevice(String deviceName, String groupName, String entityName, Map<String, Object> values)
			throws ResourceNotFoundException;

	Device getDeviceById(Long id);

	Group getGroupById(Long id);

	Entity getEntityById(Long id);

	IEntityField getEntityFieldById(Long id);

	IEntityFieldEnabledValue getEntityFieldEnabledValueById(Long id);

	Device save(Device device);

}
