package com.balabas.smarthouse.server.entity.service;

import java.util.List;

import org.json.JSONObject;

import com.balabas.smarthouse.server.entity.model.IDevice;
import com.balabas.smarthouse.server.entity.model.IEntity;
import com.balabas.smarthouse.server.entity.model.IGroup;
import com.balabas.smarthouse.server.model.request.DeviceRequest;
import com.balabas.smarthouse.server.view.Action;

public interface IDeviceService {

	IDevice getDevice(String deviceName);
	
	IDevice getDevice(IDevice device);
	
	void processDataReceivedFromDevice(IDevice device, String deviceResponse);
	
	void processDataReceivedFromDevice(IDevice device, JSONObject data);
	
	void processDataReceivedFromDevice(IEntity entity, JSONObject data);

	void requestDevicesValues(IDevice device, IGroup group);

	void processRegistrationRequest(DeviceRequest request);

	List<IDevice> getDevicesInitialized();

	void requestAllDevicesDataWithUpdateRequired();

	void processDeviceAction(Action action);

	

}
