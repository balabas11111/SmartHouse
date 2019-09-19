package com.balabas.smarthouse.server.service;

import java.io.UnsupportedEncodingException;
import java.util.List;
import java.util.Optional;

import org.springframework.http.ResponseEntity;

import com.balabas.smarthouse.server.model.Device;
import com.balabas.smarthouse.server.model.DeviceRegistrationResult;

public interface DeviceService {
	
	List<Device> getDevices();

	Optional<Device> getDeviceByDeviceId(String deviceId);

	DeviceRegistrationResult registerDevice(Device device);
	
	boolean activateDevice(String deviceId);
	
	ResponseEntity<String> executeGetDataOnDevice(String deviceId, String deviceEntityGroup) throws UnsupportedEncodingException;
}
