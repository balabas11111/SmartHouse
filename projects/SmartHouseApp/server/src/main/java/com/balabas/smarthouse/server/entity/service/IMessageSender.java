package com.balabas.smarthouse.server.entity.service;

import com.balabas.smarthouse.server.entity.model.Severity;

public interface IMessageSender {

	void sendMessageToAllUsers(Severity severity, String message);
	
	void sendDeviceRegisteredToAllUsers(String deviceName);
}
