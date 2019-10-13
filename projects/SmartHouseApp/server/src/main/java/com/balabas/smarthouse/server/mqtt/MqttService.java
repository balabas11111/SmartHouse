package com.balabas.smarthouse.server.mqtt;

import org.eclipse.paho.client.mqttv3.MqttException;

public interface MqttService {
	
	void publish(String topic, String message) throws MqttException;
	
	void registerSubscriber(IMqttMessageSubscriber subscriber);
	
	String getFromDeviceTopic(String deviceId);
	
	String getToDeviceTopic(String deviceId);
}
