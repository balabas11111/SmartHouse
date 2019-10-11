package com.balabas.smarthouse.server.mqtt;

import org.eclipse.paho.client.mqttv3.IMqttMessageListener;

public interface MqttMessageSubscriber extends IMqttMessageListener {

	String getTopicName();
	
	boolean onMessageReceived(String message);
}
