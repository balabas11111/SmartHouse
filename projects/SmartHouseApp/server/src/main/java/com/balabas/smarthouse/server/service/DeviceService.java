package com.balabas.smarthouse.server.service;

import java.util.List;
import java.util.Map;
import java.util.Optional;

import org.json.JSONObject;

import com.balabas.smarthouse.server.exception.ResourceNotFoundException;
import com.balabas.smarthouse.server.model.Device;
import com.balabas.smarthouse.server.model.Group;
import com.balabas.smarthouse.server.model.request.DeviceRequest;
import com.balabas.smarthouse.server.view.Action;
import com.balabas.smarthouse.server.model.request.DeviceRegistrationResult;

public interface DeviceService {
	
	List<Device> getDevices();
	
	List<Device> getDevicesInitialized();

	Optional<Device> getDeviceByDeviceId(String deviceId);
	
	Device getDevice(String deviceId) throws ResourceNotFoundException;

	DeviceRegistrationResult registerDevice(Device device);
	
	DeviceRegistrationResult registerDevice(DeviceRequest request);
	
	String sendDataToDevice(String deviceId, String groupId, String entityId,
			Map<String,Object> values) throws ResourceNotFoundException;
	
	JSONObject executeGetData(String deviceId, String deviceEntityGroup) throws ResourceNotFoundException;
	
	void markDeviceAsWaitsForDataUpdate(DeviceRequest request) throws ResourceNotFoundException;
	
	void processDeviceDataUpdateDispatched(DeviceRequest request, boolean dataExpected) throws ResourceNotFoundException;
	
	void processDataReceivedFromDevice(Device device, String deviceData, boolean dataExpected) throws ResourceNotFoundException;
	
	void requestDevicesValues(Device device, Group group);
	
    void requestAllDevicesDataWithUpdateRequired();

	void processDeviceAction(Action action) throws Throwable;

}
