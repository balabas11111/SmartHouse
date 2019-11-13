package com.balabas.smarthouse.server.entity.service;

import com.balabas.smarthouse.server.entity.model.Entity;

public interface IDeviceMqService {

	void initTopicsToFromDevice(String deviceId);
	
	boolean publishToDeviceTopic(String deviceId, String message);

	void subscribeFromDeviceEntityTopic(Entity entity);
	
	void onDeviceEntityDataReceived(Entity entity, String message);
}
