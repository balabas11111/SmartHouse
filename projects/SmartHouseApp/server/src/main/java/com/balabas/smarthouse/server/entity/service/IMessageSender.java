package com.balabas.smarthouse.server.entity.service;

import com.balabas.smarthouse.server.entity.model.descriptor.Severity;

public interface IMessageSender {

	boolean sendHtmlMessageToAllUsers(String message);
	
	boolean sendMessageToAllUsers(Severity severity, String message);
	
	void sendDeviceRegisteredToAllUsers(String deviceName);
}
