package com.balabas.smarthouse.server.mqtt;

import org.eclipse.paho.client.mqttv3.MqttException;

import com.balabas.smarthouse.server.model.Device;

public interface MqttService {
	
	void publish(String topic, String message) throws MqttException;
	
	void registerSubscriber(MqttMessageSubscriber subscriber);
	
	String getFromDeviceTopic(Device device);
	
	String getToDeviceTopic(Device device);
}
