package com.balabas.smarthouse.server.service;

import com.balabas.smarthouse.server.model.DeviceEntity;

public interface IDeviceMessageService {

	boolean onRegistrationTopicMessageReceived(String message);

	void initTopicsToFromDevice(String deviceId);
	
	void subscribeFromDeviceTopic(String deviceId);
	
	boolean publishToDeviceTopic(String deviceId, String message);

	void initTopicsToFromDeviceEntity(DeviceEntity entity);
	
	void subscribeFromDeviceEntityTopic(DeviceEntity entity);
	
}
