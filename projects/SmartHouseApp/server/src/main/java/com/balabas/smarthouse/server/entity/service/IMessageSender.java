package com.balabas.smarthouse.server.entity.service;

import java.util.List;

import com.balabas.smarthouse.server.entity.model.descriptor.Severity;

public interface IMessageSender {

	boolean sendHtmlMessageToAllUsers(String message);
	
	boolean sendMessageToAllUsers(Severity severity, String message);
	
	boolean sendMessageToAllUsers(Severity severity, String header, List<AlarmMessageHolder> messageHolder);
	
	void sendDeviceRegisteredToAllUsers(String deviceName);
}
