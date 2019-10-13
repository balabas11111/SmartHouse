package com.balabas.smarthouse.server.service;

import org.eclipse.paho.client.mqttv3.MqttException;

public interface IDeviceMessageService {

	boolean registerDevice(String message);

	boolean processData(String topicName, String message);

	boolean sendRegisteredToDevice(String deviceId) throws MqttException;

	boolean sendRegisteredToDevice(String deviceId, String token) throws MqttException;
	
}
