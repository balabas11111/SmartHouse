package com.balabas.smarthouse.server.zzz.mock;

import com.balabas.smarthouse.server.model.request.DeviceRequest;

public class RegisterTask implements Runnable {

	private DeviceRequest request;
	private MockedDeviceService service;
	
	public RegisterTask(DeviceRequest request, MockedDeviceService service) {
		this.request = request;
		this.service = service;
	}
	
	@Override
	public void run() {
		service.sendRegistrationRequest(request);
	}

}
