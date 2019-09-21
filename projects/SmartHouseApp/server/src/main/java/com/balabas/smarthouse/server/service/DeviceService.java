package com.balabas.smarthouse.server.service;

import java.io.UnsupportedEncodingException;
import java.net.UnknownHostException;
import java.util.List;
import java.util.Optional;

import org.springframework.http.ResponseEntity;

import com.balabas.smarthouse.server.model.Device;
import com.balabas.smarthouse.server.model.DeviceOnDataUpdatedRequest;
import com.balabas.smarthouse.server.model.DeviceRegistrationRequest;
import com.balabas.smarthouse.server.model.DeviceRegistrationResult;

public interface DeviceService {
	
	List<Device> getDevices();

	Optional<Device> getDeviceByDeviceId(String deviceId);
	
	Device getDevice(String deviceId);

	DeviceRegistrationResult registerDevice(Device device);
	
	DeviceRegistrationResult registerDevice(DeviceRegistrationRequest request) throws UnknownHostException;
	
	boolean activateDevice(String deviceId);
	
	ResponseEntity<String> executeGetDataOnDevice(String deviceId, String deviceEntityGroup) throws UnsupportedEncodingException;
	
	void markDeviceAsWaitsForRequest(DeviceOnDataUpdatedRequest request);
	
	void setDeviceDataRequestCompleted(String deviceId, String data);
	
	void setDeviceDataRequestCompleted(Device device, String data);
	
	void processDevicesDataChangedDispatched();
	
	void requestDevicesValues(List<Device> devices);
}
