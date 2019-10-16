package com.balabas.smarthouse.server.entity.service;

import com.balabas.smarthouse.server.entity.model.IEntity;

public interface IDeviceMqService {

	boolean onRegistrationTopicMessageReceived(String message);

	void initTopicsToFromDevice(String deviceId);
	
	void subscribeFromDeviceTopic(String deviceId);
	
	boolean publishToDeviceTopic(String deviceId, String message);

	void initTopicsToFromDeviceEntity(IEntity entity);
	
	void subscribeFromDeviceEntityTopic(IEntity entity);
	
	void onDeviceEntityDataReceived(IEntity entity, String message);
}
