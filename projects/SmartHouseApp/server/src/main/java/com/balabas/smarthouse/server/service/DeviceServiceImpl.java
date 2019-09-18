package com.balabas.smarthouse.server.service;

import java.util.ArrayList;
import java.util.List;
import java.util.Optional;

import org.springframework.stereotype.Service;

import com.balabas.smarthouse.server.model.Device;
import com.balabas.smarthouse.server.model.DeviceRegistrationResult;
import com.balabas.smarthouse.server.model.DeviceRegistrationResult.DeviceRegistrationResults;

import lombok.extern.log4j.Log4j2;

@Log4j2
@Service
public class DeviceServiceImpl implements DeviceService {

	private List<Device> devices;

	public DeviceServiceImpl() {
		this.devices = new ArrayList<>();
	}

	@Override
	public List<Device> getDevices() {
		return this.devices;
	}

	@Override
	public Optional<Device> getDeviceByDeviceId(String deviceId) {
		return devices.stream().filter(dev -> (dev.getDeviceId() != null && dev.getDeviceId().equals(deviceId)))
				.findFirst();
	}

	@Override
	public DeviceRegistrationResult registerDevice(Device device) {
		DeviceRegistrationResult result = new DeviceRegistrationResult();

		if (isDeviceAlreadyRegistered(device)) {
			result.setResult(DeviceRegistrationResults.ALREADY_REGISTERED);
		}

		if (!isDeviceRegistrationAllowed(device)) {
			result.setResult(DeviceRegistrationResults.NOT_ALLOWED);
		}

		if (result.isOkOrUnknown()) {
			addDeviceTo(device);
			result.setResult(DeviceRegistrationResults.SUCCESS);
		}
		
		log.info("DeviceServiceImpl Registering device " + device.toString() + " status = "+result.getResult().toString());

		return result;
	}

	private boolean isDeviceAlreadyRegistered(Device device) {
		return devices.stream().anyMatch(dev -> dev.equals(device));
	}

	private boolean isDeviceRegistrationAllowed(Device device) {
		return device != null;
	}

	private void addDeviceTo(Device device) {
		log.info("Device added :" + device.toString());
		devices.add(device);
	}
}
