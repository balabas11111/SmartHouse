package com.balabas.smarthouse.server.entity.service;

import com.balabas.smarthouse.server.entity.model.Entity;
import com.balabas.smarthouse.server.entity.model.IDevice;

public interface IDeviceMqService {

	boolean onRegistrationTopicMessageReceived(String message);

	void initTopicsToFromDevice(String deviceId);
	
	void subscribeFromDeviceTopic(IDevice device);
	
	boolean publishToDeviceTopic(String deviceId, String message);

	void initTopicsToFromDeviceEntity(Entity entity);
	
	void subscribeFromDeviceEntityTopic(Entity entity);
	
	void onDeviceEntityDataReceived(Entity entity, String message);
}
