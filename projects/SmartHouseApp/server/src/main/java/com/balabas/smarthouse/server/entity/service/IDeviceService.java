package com.balabas.smarthouse.server.entity.service;

import java.util.List;
import java.util.Map;

import org.json.JSONObject;

import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.IGroup;
import com.balabas.smarthouse.server.exception.ResourceNotFoundException;
import com.balabas.smarthouse.server.model.request.DeviceRequest;
import com.balabas.smarthouse.server.view.Action;

public interface IDeviceService {
	
	IDevice getManagedDeviceByName(String deviceName);
	
	List<IDevice> getDevices();
	
	List<IDevice> getDevicesInitialized();
	
	void processDataReceivedFromDevice(IDevice device, String deviceResponse);
	
	void processDataReceivedFromDevice(IDevice device, JSONObject data);
	
	void processDataReceivedFromDevice(IEntity entity, JSONObject data);

	void requestDevicesValues(IDevice device, IGroup group);

	void processRegistrationRequest(DeviceRequest request);

	void requestAllDevicesDataWithUpdateRequired();

	void processDeviceAction(Action action) throws ResourceNotFoundException;

	String sendDataToDevice(String deviceName, String groupName, String entityName, Map<String, Object> values)
			throws ResourceNotFoundException;

	

	

}
