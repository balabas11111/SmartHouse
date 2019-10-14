package com.balabas.smarthouse.server.mqtt;

import org.eclipse.paho.client.mqttv3.IMqttMessageListener;

public interface IMqttMessageSubscribtion extends IMqttMessageListener {

	String getTopicName();
	
	void onMessageReceived(String topic, String message);
}
