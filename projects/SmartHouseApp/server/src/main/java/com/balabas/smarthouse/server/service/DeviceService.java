package com.balabas.smarthouse.server.service;

import java.util.List;
import java.util.Optional;

import com.balabas.smarthouse.server.model.Device;
import com.balabas.smarthouse.server.model.DeviceRegistrationResult;

public interface DeviceService {

	public List<Device> getDevices();

	public Optional<Device> getDeviceByDeviceId(String deviceId);

	public DeviceRegistrationResult registerDevice(Device device);
}
