package com.balabas.smarthouse.server.service;

import java.io.UnsupportedEncodingException;
import java.net.UnknownHostException;
import java.util.List;
import java.util.Optional;

import org.json.JSONObject;

import com.balabas.smarthouse.server.exception.ResourceNotFoundException;
import com.balabas.smarthouse.server.model.Device;
import com.balabas.smarthouse.server.model.Group;
import com.balabas.smarthouse.server.model.request.DeviceOnDataUpdatedRequest;
import com.balabas.smarthouse.server.model.request.DeviceRegistrationRequest;
import com.balabas.smarthouse.server.model.request.DeviceRegistrationResult;

public interface DeviceService {
	
	List<Device> getDevices();

	Optional<Device> getDeviceByDeviceId(String deviceId);
	
	Device getDevice(String deviceId) throws ResourceNotFoundException;

	DeviceRegistrationResult registerDevice(Device device);
	
	DeviceRegistrationResult registerDevice(DeviceRegistrationRequest request) throws UnknownHostException;
	
	String executeGetDataOnDevice(String deviceId, String deviceEntityGroup) throws UnsupportedEncodingException;
	
	JSONObject executeGetData(String deviceId, String deviceEntityGroup) throws ResourceNotFoundException;
	
	void markDeviceAsWaitsForDataUpdate(DeviceOnDataUpdatedRequest request) throws ResourceNotFoundException;
	
	void processDeviceDataUpdateDispatched(DeviceOnDataUpdatedRequest request) throws ResourceNotFoundException;
	
	void processDeviceDataUpdateDispatched(String deviceId, String deviceData) throws ResourceNotFoundException;
	
	void processDeviceDataUpdate(Device device, String deviceData) throws ResourceNotFoundException;
	
	boolean validateDeviceData(String data);
	
	void requestDevicesValues(Device device);
	
	void requestGroupValues(Device device, Group group);

    void requestAllDevicesData();
}
